#include "Chams.h"

#include "../../../Configs/Config.h"
#include "../../Game.h"
#include "../../../Utilities/Logger.h"

bool Features::FortPawnHelper::Chams::ShouldApplyChams(SDK::AFortPawn* FortPawn) {
	if (SDK::IsValidPointer(FortPawn) == false) return false;
	if (Config::Visuals::Players::Chams == false || Config::Visuals::Players::Enabled == false) return false;

	if ((FortPawn != SDK::GetLocalPawn()) || (FortPawn == SDK::GetLocalPawn() && Config::Visuals::Players::SelfChams)) {
		return true;
	}

	return false;
}

bool Features::FortPawnHelper::Chams::ApplyChams(SDK::AFortPawn* FortPawn, SDK::UMaterialInstanceDynamic* Material, bool SendingNullptrMaterial) {
	// You can send a nullptr material to revert materials
	if (SendingNullptrMaterial == false) {
		if (ShouldApplyChams(FortPawn) == false) return false;
	}
	
	std::array<SDK::USkeletalMeshComponentBudgeted*, 8> CharacterParts = SDK::Cast<SDK::AFortPlayerPawn>(FortPawn)->GetCharacterPartSkeletalMeshComponents();

	for (auto Mesh : CharacterParts) {
		if (SDK::IsValidPointer(Mesh) == false) continue;
		SDK::TArray<SDK::UMaterialInterface*> Materials = Mesh->GetMaterials();

		for (int i = 0; i < Materials.Num(); i++) {
			if (Materials.IsValidIndex(i) == false) continue;

			// VERY IMPORTANT CHECKS! Do not remove them!
			if ((SDK::IsValidPointer(Material) && SDK::IsValidPointer(Material->Class) && Material->IsA(SDK::UMaterialInstanceDynamic::StaticClass())) 
				|| SendingNullptrMaterial == true) {
				Mesh->SetMaterial(i, Material);
			}
		}
	}

	return true;
}

void Features::FortPawnHelper::Chams::RemoveChams(SDK::AFortPawn* FortPawn) {
	ApplyChams(FortPawn, nullptr, true);
}

void Features::FortPawnHelper::Chams::RevertAll() {
	for (auto& ChammedPawn : ChammedPawns) {
		if (SDK::IsValidPointer(ChammedPawn->FortPawn)) {
			RemoveChams(ChammedPawn->FortPawn);
		}
	}

	ChammedPawns.clear();
}

void Features::FortPawnHelper::Chams::Tick(SDK::AFortPawn* FortPawn) {
	// Update the material settings
	UpdateMaterialSettings();

	auto it = std::find_if(ChammedPawns.begin(), ChammedPawns.end(), [FortPawn](const auto& ChammedPawn) {
		return ChammedPawn->FortPawn == FortPawn;
	});

	bool PawnInList = it != ChammedPawns.end();

	if (SDK::IsValidPointer(FortPawn)) {
		if (PawnInList == false) {
			// Apply the chams
			if (ApplyChams(FortPawn, SDK::GetChamsMaterialDynamic())) {
				ChammedPawns.push_back(std::make_unique<ChammedPawn>(FortPawn, Game::CurrentTime));
			}
		}
		else {
			// Update the chams every second
			auto& ChammedPawn = *it;
			if (Game::CurrentTime - ChammedPawn->LastUpdateTime > std::chrono::seconds(1)) {
				if (ApplyChams(FortPawn, SDK::GetChamsMaterialDynamic())) {
					ChammedPawn->LastUpdateTime = Game::CurrentTime;
				}
			}
		}
	}
	else if (PawnInList) {
		// If the pawn is not valid, remove it and the chams
		ChammedPawns.erase(it);
		RemoveChams(FortPawn);
	}
}

void Features::FortPawnHelper::Chams::UpdateDynamicMaterialSettings() {
	// Set the colors
	SDK::GetChamsMaterialDynamic()->SetVectorParameterValue(SColor1, SDK::FLinearColor(1.f, 0.f, 0.75f, 1.f));
	SDK::GetChamsMaterialDynamic()->SetVectorParameterValue(SColor2, SDK::FLinearColor(1.f, 0.f, 0.75f, 1.f));

	// Set the emissive brightness
	SDK::GetChamsMaterialDynamic()->SetScalarParameterValue(DissolvePatternEmissiveBrightness, Config::Visuals::Players::GlowAmount);
	SDK::GetChamsMaterialDynamic()->SetScalarParameterValue(GradientPassEmissiveA, Config::Visuals::Players::GlowAmount);
	SDK::GetChamsMaterialDynamic()->SetScalarParameterValue(GradientPassEmissiveB, Config::Visuals::Players::GlowAmount);
}

void Features::FortPawnHelper::Chams::UpdateMaterialSettings() {
	SDK::GetChamsMaterial()->SetbDisableDepthTest(true, &Config::Visuals::Players::Chams);
	SDK::GetChamsMaterial()->SetBlendMode(SDK::EBlendMode::BLEND_AlphaComposite, &Config::Visuals::Players::Chams);
	SDK::GetChamsMaterial()->SetWireFrame(Config::Visuals::Players::Wireframe, &Config::Visuals::Players::Chams);
}

void Features::FortPawnHelper::Chams::Init() {
	// Initialize the FName's
	SColor1 = SDK::FName(skCrypt(L"S Color1"));
	SColor2 = SDK::FName(skCrypt(L"S Color2"));
	DissolvePatternEmissiveBrightness = SDK::FName(skCrypt(L"Dissolve Pattern Emissive Brightness"));
	GradientPassEmissiveA = SDK::FName(skCrypt(L"Gradient Pass Emissive A"));
	GradientPassEmissiveB = SDK::FName(skCrypt(L"Gradient Pass Emissive B"));
}