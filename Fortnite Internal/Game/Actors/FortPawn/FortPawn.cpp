#include "../Actors.h"
#include "../../SDK/SDK.h"
#include "../../SDK/Classes/Engine_classes.h"
#include "../../Features/ESP/FortPawn.h"
#include "../../Hooks/PostRender/Drawing.h"

void Actors::FortPawn::Tick(uintptr_t Canvas) {
	if (!SDK::IsValidPointer(Canvas)) return;
	SDK::UCanvas* Canvas_ = reinterpret_cast<SDK::UCanvas*>(Canvas);

	// Player Cache (to avoid calling GetAllActorsOfClass every tick)
	static SDK::TArray<SDK::AActor*> CachedPlayers;
	{
		auto currentTime = std::chrono::steady_clock::now();
		double elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - lastTime).count();

		if (elapsedTime >= intervalSeconds) {
			lastTime = currentTime;

			CachedPlayers = SDK::UGameplayStatics::StaticClass()->GetAllActorsOfClass(SDK::GetWorld(), SDK::AFortPawn::StaticClass());
		}
	}

	for (int i = 0; i < CachedPlayers.Num(); i++) {
		SDK::AActor*					Actor				= CachedPlayers[i];										if (!Actor) continue;
		SDK::AFortPawn*					FortPawn			= reinterpret_cast<SDK::AFortPawn*>(Actor);				if (!FortPawn) continue;
		SDK::AFortPlayerState*			FortPlayerState		= FortPawn->PlayerState();								//if (!FortPlayerState) continue;
		SDK::ACharacter*				Character			= static_cast<SDK::ACharacter*>((SDK::APawn*)FortPawn);	if (!Character) continue;
		SDK::USkeletalMeshComponent*	Mesh				= Character->Mesh();									if (!Mesh) continue;
		
		if (FortPawn == SDK::GetLocalController()->AcknowledgedPawn()) {
			//FVector Bone = Mesh->GetBonePosition(0);
			//Objects::local.Position = Bone;
			//Objects::local.TeamIndex = FortPlayerState->TeamIndex();

			/*std::cout << "Found Local" << std::endl;
			SDK::UInputComponent* inp = SDK::GetLocalController()->EditInputComponent();
			if (inp) {
				std::cout << "Edit Input Component Valid" << std::endl;
				SDK::TArray<SDK::FInputKeyBinding> inpbinds = inp->KeyBindings();
				std::cout << "Input Size " << inpbinds.Num() << std::endl;

				if (inpbinds.Num()) {
					for (int jj = 0; jj < inpbinds.Num(); jj++) {
						SDK::FInputKeyBinding inputbind = inpbinds[jj];
						std::cout << "Input Bind Found " << inputbind.KeyEvent().Value << std::endl;
					}
				}
			}*/

			continue;
		}

		//if (FortPlayerState->TeamIndex() == Objects::local.TeamIndex) {
		//	continue;
		//}



		std::vector<SDK::FVector> BoneRegister(100);
		std::vector<SDK::FVector2D> BoneRegister2D(100);
		std::vector<bool> BoneVisibilities(100);

		if (!Player::PopulateBones(Canvas_, Mesh, BoneRegister, BoneRegister2D)) continue;
		Player::PopulateVisibilitys(FortPawn, BoneRegister, BoneVisibilities);

		bool Visible = SDK::UKismetSystemLibrary::IsPositionVisible(FortPawn, SDK::GetLocalController()->PlayerCameraManager()->GetCameraLocation(), BoneRegister[Bones::Head], FortPawn);

		SDK::FLinearColor Colour = SDK::FLinearColor(1.f, 1.f, 1.f, 1.f);
		if (Visible)
			Colour = SDK::FLinearColor(1.f, 0.f, 0.f, 1.f);

		//if (FortPawn == Objects::target.Actor) {
		//	Colour = SDK::FLinearColor(0.9f, 0.5f, 0.1f, 1.f);
		//}

		float Top = FLT_MAX;
		float Bottom = FLT_MIN;
		float Left = FLT_MAX;
		float Right = FLT_MIN;

		for (int i2 = 0; i2 < BoneRegister2D.size(); i2++) {
			if (i2 == Bones::Bottom) continue;

			if (BoneRegister2D[i2].X && BoneRegister2D[i2].Y) {
				SDK::FVector2D BonePos = BoneRegister2D[i2];

				// REMOVE HARDCODED SIZE!!
				if (!BonePos.X > 0 && !BonePos.X < Canvas_->SizeX()) continue;
				if (!BonePos.Y > 0 && !BonePos.Y < Canvas_->SizeY()) continue;

				if (BonePos.Y < Top)
					Top = BonePos.Y;
				if (BonePos.Y > Bottom)
					Bottom = BonePos.Y;
				if (BonePos.X < Left)
					Left = BonePos.X;
				if (BonePos.X > Right)
					Right = BonePos.X;
			}
		}

		float LeftRightOffset = (Right - Left) * 0.3;
		float TopBottomOffset = (Bottom - Top) * 0.22;

		SDK::FVector2D bottomLeft(Left - LeftRightOffset, Bottom + (TopBottomOffset * 0.75));
		SDK::FVector2D topRight(Right + LeftRightOffset, Top - TopBottomOffset);

		SDK::FVector2D Chest(-1, -1);

		//float DistanceFromLocal = Objects::local.Position.Distance(BoneRegister[Bones::Bottom]) / 100.f;
		float DistanceFromLocal = 50.f;
		float maxDistance = 150.0f;
		float minFontSize = 10.0f;
		float maxFontSize = 20.0f;
		float fontSize = maxFontSize - (maxFontSize - minFontSize) * (DistanceFromLocal / maxDistance);
		fontSize = (fontSize < minFontSize) ? minFontSize : ((fontSize > maxFontSize) ? maxFontSize : fontSize);

		bool boneVisible = false;

		for (const auto& pair : bonePairs) {
			int boneID1 = pair.first;
			int boneID2 = pair.second;

			bool aboneisvisibletoplayer = false;

			if (boneID1 < -1 || boneID1 >= 99 || boneID2 < -1 || boneID2 >= 99)
				continue;

			SDK::FVector2D ScreenPos1, ScreenPos2;

			if (boneID1 == Bones::Chest) {
				if (Chest.X == -1 && Chest.Y == -1) {
					SDK::FVector LeftChest = BoneRegister[Bones::ChestLeft];
					SDK::FVector RightChest = BoneRegister[Bones::ChestRight];

					SDK::FVector Midpoint;
					Midpoint.X = (LeftChest.X + RightChest.X) / 2.0f;
					Midpoint.Y = (LeftChest.Y + RightChest.Y) / 2.0f;
					Midpoint.Z = (LeftChest.Z + RightChest.Z) / 2.0f;

					Chest = Canvas_->K2_Project(Midpoint);
				}

				ScreenPos1 = Chest;
			}
			else
			{
				ScreenPos1 = BoneRegister2D[boneID1];

				if (BoneVisibilities[boneID1]) {
					aboneisvisibletoplayer = true;
				}
			}

			if (boneID2 == Bones::Chest) {
				if (Chest.X == -1 && Chest.Y == -1) {
					SDK::FVector LeftChest = BoneRegister[Bones::ChestLeft];
					SDK::FVector RightChest = BoneRegister[Bones::ChestRight];

					SDK::FVector Midpoint;
					Midpoint.X = (LeftChest.X + RightChest.X) / 2.0f;
					Midpoint.Y = (LeftChest.Y + RightChest.Y) / 2.0f;
					Midpoint.Z = (LeftChest.Z + RightChest.Z) / 2.0f;

					Chest = Canvas_->K2_Project(Midpoint);
				}

				ScreenPos2 = Chest;
			}
			else
			{
				ScreenPos2 = BoneRegister2D[boneID2];

				if (BoneVisibilities[boneID2]) {
					aboneisvisibletoplayer = true;
				}
			}

			// REMOVE HARDCODED SIZE!!
			// AND IMPROVE GENERALLY
			if (!(ScreenPos1.X > 0 && ScreenPos1.X < Canvas_->SizeX())) continue;
			if (!(ScreenPos2.X > 0 && ScreenPos2.X < Canvas_->SizeX())) continue;
			if (!(ScreenPos1.Y > 0 && ScreenPos1.Y < Canvas_->SizeY())) continue;
			if (!(ScreenPos2.Y > 0 && ScreenPos2.Y < Canvas_->SizeY())) continue;

			boneVisible = true;

			if (aboneisvisibletoplayer) {
				Canvas_->K2_DrawLine(
					SDK::FVector2D(ScreenPos1.X, ScreenPos1.Y),
					SDK::FVector2D(ScreenPos2.X, ScreenPos2.Y),
					1,
					SDK::FLinearColor(0.0f, 1.f, 1.f, 1.0f)
				);
			}
			else {
				Canvas_->K2_DrawLine(
					SDK::FVector2D(ScreenPos1.X, ScreenPos1.Y),
					SDK::FVector2D(ScreenPos2.X, ScreenPos2.Y),
					1,
					SDK::FLinearColor(1.0f, 0.f, 0.f, 1.0f)
				);
			}
		}



		if (boneVisible) {
			//if (Settings::PlayerESP && Settings::BoxESP) {
				if (DistanceFromLocal > 50) {
					Drawing::DrawOutlinedCornerBox(bottomLeft.X, topRight.Y, (topRight.X - bottomLeft.X), (bottomLeft.Y - topRight.Y), Colour, 1);
				}
				else {
					Drawing::DrawOutlinedCornerBox(bottomLeft.X, topRight.Y, (topRight.X - bottomLeft.X), (bottomLeft.Y - topRight.Y), Colour, 2);
				}
			//}
		}
	}
}