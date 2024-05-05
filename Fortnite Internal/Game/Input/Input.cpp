#include "Input.h"

#include "../SDK/Classes/Engine_Classes.h"

#include "../../Utilities/Logger.h"

#include "../Game.h"

#ifdef _IMGUI
#include "../../External-Libs/ImGui/imgui.h"
#include "../../Hooks/Hooks.h"
#endif // _IMGUI

SDK::FVector2D Input::GetMousePosition()
{
    if (Mouse.FrameUpdated != Game::CurrentFrame)
    {
        Mouse.FrameUpdated = Game::CurrentFrame;

        float LocationX = 0.f;
        float LocationY = 0.f;

        if (SDK::GetLocalController()->GetMousePosition(&LocationX, &LocationY))
        {
            Mouse.Position = SDK::FVector2D(LocationX, LocationY);
        }
    }

    return Mouse.Position;
}

bool Input::IsKeyDown(const KeyName Key)
{
    auto& KeyData = Keys[Key];

    if (KeyData.IsDown.FrameUpdated != Game::CurrentFrame)
    {
        KeyData.IsDown.FrameUpdated = Game::CurrentFrame;

        SDK::FKey FKey{};
        FKey.KeyName = KeyData.FName;

        KeyData.IsDown.Value = SDK::GetLocalController()->IsInputKeyDown(FKey);
    }

    return KeyData.IsDown.Value;
}
bool Input::WasKeyJustReleased(const KeyName Key)
{
    auto& KeyData = Keys[Key];

    if (KeyData.WasJustReleased.FrameUpdated != Game::CurrentFrame)
    {
        KeyData.WasJustReleased.FrameUpdated = Game::CurrentFrame;

        SDK::FKey FKey{};
        FKey.KeyName = KeyData.FName;

        KeyData.WasJustReleased.Value = SDK::GetLocalController()->WasInputKeyJustReleased(FKey);
    }

    return KeyData.WasJustReleased.Value;
}
bool Input::WasKeyJustPressed(const KeyName Key)
{
    auto& KeyData = Keys[Key];

    if (KeyData.WasJustPressed.FrameUpdated != Game::CurrentFrame)
    {
        KeyData.WasJustPressed.FrameUpdated = Game::CurrentFrame;

        SDK::FKey FKey{};
        FKey.KeyName = KeyData.FName;

        KeyData.WasJustPressed.Value = SDK::GetLocalController()->WasInputKeyJustPressed(FKey);
    }

    return KeyData.WasJustPressed.Value;
}

std::vector<Input::KeyName> Input::GetAllDownKeys()
{
    std::vector<Input::KeyName> KeysDown{};

    for (auto& Key : Keys)
    {
        if (IsKeyDown(Key.first))
        {
            KeysDown.push_back(Key.first);
        }
    }

    return KeysDown;
}
std::vector<Input::KeyName> Input::GetAllJustReleasedKeys()
{
    std::vector<Input::KeyName> KeysJustReleased{};

    for (auto& Key : Keys)
    {
        if (WasKeyJustReleased(Key.first))
        {
            KeysJustReleased.push_back(Key.first);
        }
    }

    return KeysJustReleased;
}
std::vector<Input::KeyName> Input::GetAllJustPressedKeys()
{
    std::vector<Input::KeyName> KeysJustPressed{};

    for (auto& Key : Keys)
    {
        if (WasKeyJustPressed(Key.first))
        {
            KeysJustPressed.push_back(Key.first);
        }
    }

    return KeysJustPressed;
}

std::string Input::GetKeyNameString(const Input::KeyName Key)
{
    return Keys[Key].Name;
}

void Input::Init()
{
    DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("Initializing input system...")));

    // Init KeyNames
    {
        Keys[KeyName::NONE].FName = SDK::FName(skCrypt(L"NONE"));

        Keys[KeyName::AnyKey].FName = SDK::FName(skCrypt(L"AnyKey"));

        Keys[KeyName::MouseX].FName = SDK::FName(skCrypt(L"MouseX"));
        Keys[KeyName::MouseY].FName = SDK::FName(skCrypt(L"MouseY"));
        Keys[KeyName::MouseScrollUp].FName = SDK::FName(skCrypt(L"MouseScrollUp"));
        Keys[KeyName::MouseScrollDown].FName = SDK::FName(skCrypt(L"MouseScrollDown"));
        Keys[KeyName::LeftMouseButton].FName = SDK::FName(skCrypt(L"LeftMouseButton"));
        Keys[KeyName::RightMouseButton].FName = SDK::FName(skCrypt(L"RightMouseButton"));
        Keys[KeyName::MiddleMouseButton].FName = SDK::FName(skCrypt(L"MiddleMouseButton"));
        Keys[KeyName::ThumbMouseButton].FName = SDK::FName(skCrypt(L"ThumbMouseButton"));
        Keys[KeyName::ThumbMouseButton2].FName = SDK::FName(skCrypt(L"ThumbMouseButton2"));

        Keys[KeyName::BackSpace].FName = SDK::FName(skCrypt(L"BackSpace"));
        Keys[KeyName::Tab].FName = SDK::FName(skCrypt(L"Tab"));
        Keys[KeyName::Enter].FName = SDK::FName(skCrypt(L"Enter"));
        Keys[KeyName::Pause].FName = SDK::FName(skCrypt(L"Pause"));
        Keys[KeyName::CapsLock].FName = SDK::FName(skCrypt(L"CapsLock"));
        Keys[KeyName::Escape].FName = SDK::FName(skCrypt(L"Escape"));
        Keys[KeyName::SpaceBar].FName = SDK::FName(skCrypt(L"SpaceBar"));
        Keys[KeyName::PageUp].FName = SDK::FName(skCrypt(L"PageUp"));
        Keys[KeyName::PageDown].FName = SDK::FName(skCrypt(L"PageDown"));
        Keys[KeyName::End].FName = SDK::FName(skCrypt(L"End"));
        Keys[KeyName::Home].FName = SDK::FName(skCrypt(L"Home"));

        Keys[KeyName::Left].FName = SDK::FName(skCrypt(L"Left"));
        Keys[KeyName::Up].FName = SDK::FName(skCrypt(L"Up"));
        Keys[KeyName::Right].FName = SDK::FName(skCrypt(L"Right"));
        Keys[KeyName::Down].FName = SDK::FName(skCrypt(L"Down"));

        Keys[KeyName::Insert].FName = SDK::FName(skCrypt(L"Insert"));
        Keys[KeyName::Delete].FName = SDK::FName(skCrypt(L"Delete"));

        Keys[KeyName::Zero].FName = SDK::FName(skCrypt(L"Zero"));
        Keys[KeyName::One].FName = SDK::FName(skCrypt(L"One"));
        Keys[KeyName::Two].FName = SDK::FName(skCrypt(L"Two"));
        Keys[KeyName::Three].FName = SDK::FName(skCrypt(L"Three"));
        Keys[KeyName::Four].FName = SDK::FName(skCrypt(L"Four"));
        Keys[KeyName::Five].FName = SDK::FName(skCrypt(L"Five"));
        Keys[KeyName::Six].FName = SDK::FName(skCrypt(L"Six"));
        Keys[KeyName::Seven].FName = SDK::FName(skCrypt(L"Seven"));
        Keys[KeyName::Eight].FName = SDK::FName(skCrypt(L"Eight"));
        Keys[KeyName::Nine].FName = SDK::FName(skCrypt(L"Nine"));

        Keys[KeyName::A].FName = SDK::FName(skCrypt(L"A"));
        Keys[KeyName::B].FName = SDK::FName(skCrypt(L"B"));
        Keys[KeyName::C].FName = SDK::FName(skCrypt(L"C"));
        Keys[KeyName::D].FName = SDK::FName(skCrypt(L"D"));
        Keys[KeyName::E].FName = SDK::FName(skCrypt(L"E"));
        Keys[KeyName::F].FName = SDK::FName(skCrypt(L"F"));
        Keys[KeyName::G].FName = SDK::FName(skCrypt(L"G"));
        Keys[KeyName::H].FName = SDK::FName(skCrypt(L"H"));
        Keys[KeyName::I].FName = SDK::FName(skCrypt(L"I"));
        Keys[KeyName::J].FName = SDK::FName(skCrypt(L"J"));
        Keys[KeyName::K].FName = SDK::FName(skCrypt(L"K"));
        Keys[KeyName::L].FName = SDK::FName(skCrypt(L"L"));
        Keys[KeyName::M].FName = SDK::FName(skCrypt(L"M"));
        Keys[KeyName::N].FName = SDK::FName(skCrypt(L"N"));
        Keys[KeyName::O].FName = SDK::FName(skCrypt(L"O"));
        Keys[KeyName::P].FName = SDK::FName(skCrypt(L"P"));
        Keys[KeyName::Q].FName = SDK::FName(skCrypt(L"Q"));
        Keys[KeyName::R].FName = SDK::FName(skCrypt(L"R"));
        Keys[KeyName::S].FName = SDK::FName(skCrypt(L"S"));
        Keys[KeyName::T].FName = SDK::FName(skCrypt(L"T"));
        Keys[KeyName::U].FName = SDK::FName(skCrypt(L"U"));
        Keys[KeyName::V].FName = SDK::FName(skCrypt(L"V"));
        Keys[KeyName::W].FName = SDK::FName(skCrypt(L"W"));
        Keys[KeyName::X].FName = SDK::FName(skCrypt(L"X"));
        Keys[KeyName::Y].FName = SDK::FName(skCrypt(L"Y"));
        Keys[KeyName::Z].FName = SDK::FName(skCrypt(L"Z"));

        Keys[KeyName::NumPadZero].FName = SDK::FName(skCrypt(L"NumPadZero"));
        Keys[KeyName::NumPadOne].FName = SDK::FName(skCrypt(L"NumPadOne"));
        Keys[KeyName::NumPadTwo].FName = SDK::FName(skCrypt(L"NumPadTwo"));
        Keys[KeyName::NumPadThree].FName = SDK::FName(skCrypt(L"NumPadThree"));
        Keys[KeyName::NumPadFour].FName = SDK::FName(skCrypt(L"NumPadFour"));
        Keys[KeyName::NumPadFive].FName = SDK::FName(skCrypt(L"NumPadFive"));
        Keys[KeyName::NumPadSix].FName = SDK::FName(skCrypt(L"NumPadSix"));
        Keys[KeyName::NumPadSeven].FName = SDK::FName(skCrypt(L"NumPadSeven"));
        Keys[KeyName::NumPadEight].FName = SDK::FName(skCrypt(L"NumPadEight"));
        Keys[KeyName::NumPadNine].FName = SDK::FName(skCrypt(L"NumPadNine"));

        Keys[KeyName::Multiply].FName = SDK::FName(skCrypt(L"Multiply"));
        Keys[KeyName::Add].FName = SDK::FName(skCrypt(L"Add"));
        Keys[KeyName::Subtract].FName = SDK::FName(skCrypt(L"Subtract"));
        Keys[KeyName::Decimal].FName = SDK::FName(skCrypt(L"Decimal"));
        Keys[KeyName::Divide].FName = SDK::FName(skCrypt(L"Divide"));

        Keys[KeyName::F1].FName = SDK::FName(skCrypt(L"F1"));
        Keys[KeyName::F2].FName = SDK::FName(skCrypt(L"F2"));
        Keys[KeyName::F3].FName = SDK::FName(skCrypt(L"F3"));
        Keys[KeyName::F4].FName = SDK::FName(skCrypt(L"F4"));
        Keys[KeyName::F5].FName = SDK::FName(skCrypt(L"F5"));
        Keys[KeyName::F6].FName = SDK::FName(skCrypt(L"F6"));
        Keys[KeyName::F7].FName = SDK::FName(skCrypt(L"F7"));
        Keys[KeyName::F8].FName = SDK::FName(skCrypt(L"F8"));
        Keys[KeyName::F9].FName = SDK::FName(skCrypt(L"F9"));
        Keys[KeyName::F10].FName = SDK::FName(skCrypt(L"F10"));
        Keys[KeyName::F11].FName = SDK::FName(skCrypt(L"F11"));
        Keys[KeyName::F12].FName = SDK::FName(skCrypt(L"F12"));

        Keys[KeyName::NumLock].FName = SDK::FName(skCrypt(L"NumLock"));
        Keys[KeyName::ScrollLock].FName = SDK::FName(skCrypt(L"ScrollLock"));
        Keys[KeyName::LeftShift].FName = SDK::FName(skCrypt(L"LeftShift"));
        Keys[KeyName::RightShift].FName = SDK::FName(skCrypt(L"RightShift"));
        Keys[KeyName::LeftControl].FName = SDK::FName(skCrypt(L"LeftControl"));
        Keys[KeyName::RightControl].FName = SDK::FName(skCrypt(L"RightControl"));
        Keys[KeyName::LeftAlt].FName = SDK::FName(skCrypt(L"LeftAlt"));
        Keys[KeyName::RightAlt].FName = SDK::FName(skCrypt(L"RightAlt"));
        Keys[KeyName::LeftCommand].FName = SDK::FName(skCrypt(L"LeftCommand"));
        Keys[KeyName::RightCommand].FName = SDK::FName(skCrypt(L"RightCommand"));
        Keys[KeyName::Semicolon].FName = SDK::FName(skCrypt(L"Semicolon"));
        Keys[KeyName::Equals].FName = SDK::FName(skCrypt(L"Equals"));
        Keys[KeyName::Comma].FName = SDK::FName(skCrypt(L"Comma"));
        Keys[KeyName::Underscore].FName = SDK::FName(skCrypt(L"Underscore"));
        Keys[KeyName::Period].FName = SDK::FName(skCrypt(L"Period"));
        Keys[KeyName::Slash].FName = SDK::FName(skCrypt(L"Slash"));
        Keys[KeyName::Tilde].FName = SDK::FName(skCrypt(L"Tilde"));
        Keys[KeyName::LeftBracket].FName = SDK::FName(skCrypt(L"LeftBracket"));
        Keys[KeyName::Backslash].FName = SDK::FName(skCrypt(L"Backslash"));
        Keys[KeyName::RightBracket].FName = SDK::FName(skCrypt(L"RightBracket"));
        Keys[KeyName::Quote].FName = SDK::FName(skCrypt(L"Quote"));
        Keys[KeyName::Asterix].FName = SDK::FName(skCrypt(L"Asterix"));
        Keys[KeyName::Ampersand].FName = SDK::FName(skCrypt(L"Ampersand"));
        Keys[KeyName::Caret].FName = SDK::FName(skCrypt(L"Caret"));
        Keys[KeyName::Dollar].FName = SDK::FName(skCrypt(L"Dollar"));
        Keys[KeyName::Exclamation].FName = SDK::FName(skCrypt(L"Exclamation"));
        Keys[KeyName::Colon].FName = SDK::FName(skCrypt(L"Colon"));

        Keys[KeyName::A_AccentGrave].FName = SDK::FName(skCrypt(L"A_AccentGrave"));
        Keys[KeyName::E_AccentGrave].FName = SDK::FName(skCrypt(L"E_AccentGrave"));
        Keys[KeyName::E_AccentAigu].FName = SDK::FName(skCrypt(L"E_AccentAigu"));
        Keys[KeyName::C_Cedille].FName = SDK::FName(skCrypt(L"C_Cedille"));

        Keys[KeyName::Section].FName = SDK::FName(skCrypt(L"Section"));

        Keys[KeyName::Gamepad_LeftX].FName = SDK::FName(skCrypt(L"Gamepad_LeftX"));
        Keys[KeyName::Gamepad_LeftY].FName = SDK::FName(skCrypt(L"Gamepad_LeftY"));
        Keys[KeyName::Gamepad_RightX].FName = SDK::FName(skCrypt(L"Gamepad_RightX"));
        Keys[KeyName::Gamepad_RightY].FName = SDK::FName(skCrypt(L"Gamepad_RightY"));
        Keys[KeyName::Gamepad_LeftTriggerAxis].FName = SDK::FName(skCrypt(L"Gamepad_LeftTriggerAxis"));
        Keys[KeyName::Gamepad_RightTriggerAxis].FName = SDK::FName(skCrypt(L"Gamepad_RightTriggerAxis"));
        Keys[KeyName::Gamepad_LeftThumbstick].FName = SDK::FName(skCrypt(L"Gamepad_LeftThumbstick"));
        Keys[KeyName::Gamepad_RightThumbstick].FName = SDK::FName(skCrypt(L"Gamepad_RightThumbstick"));
        Keys[KeyName::Gamepad_Special_Left].FName = SDK::FName(skCrypt(L"Gamepad_Special_Left"));
        Keys[KeyName::Gamepad_Special_Left_X].FName = SDK::FName(skCrypt(L"Gamepad_Special_Left_X"));
        Keys[KeyName::Gamepad_Special_Left_Y].FName = SDK::FName(skCrypt(L"Gamepad_Special_Left_Y"));
        Keys[KeyName::Gamepad_Special_Right].FName = SDK::FName(skCrypt(L"Gamepad_Special_Right"));

        Keys[KeyName::Gamepad_FaceButton_Bottom].FName = SDK::FName(skCrypt(L"Gamepad_FaceButton_Bottom"));
        Keys[KeyName::Gamepad_FaceButton_Right].FName = SDK::FName(skCrypt(L"Gamepad_FaceButton_Right"));
        Keys[KeyName::Gamepad_FaceButton_Left].FName = SDK::FName(skCrypt(L"Gamepad_FaceButton_Left"));
        Keys[KeyName::Gamepad_FaceButton_Top].FName = SDK::FName(skCrypt(L"Gamepad_FaceButton_Top"));
        Keys[KeyName::Gamepad_LeftShoulder].FName = SDK::FName(skCrypt(L"Gamepad_LeftShoulder"));
        Keys[KeyName::Gamepad_RightShoulder].FName = SDK::FName(skCrypt(L"Gamepad_RightShoulder"));
        Keys[KeyName::Gamepad_LeftTrigger].FName = SDK::FName(skCrypt(L"Gamepad_LeftTrigger"));
        Keys[KeyName::Gamepad_RightTrigger].FName = SDK::FName(skCrypt(L"Gamepad_RightTrigger"));
        Keys[KeyName::Gamepad_DPad_Up].FName = SDK::FName(skCrypt(L"Gamepad_DPad_Up"));
        Keys[KeyName::Gamepad_DPad_Down].FName = SDK::FName(skCrypt(L"Gamepad_DPad_Down"));
        Keys[KeyName::Gamepad_DPad_Right].FName = SDK::FName(skCrypt(L"Gamepad_DPad_Right"));
        Keys[KeyName::Gamepad_DPad_Left].FName = SDK::FName(skCrypt(L"Gamepad_DPad_Left"));
        Keys[KeyName::Gamepad_LeftStick_Up].FName = SDK::FName(skCrypt(L"Gamepad_LeftStick_Up"));
        Keys[KeyName::Gamepad_LeftStick_Down].FName = SDK::FName(skCrypt(L"Gamepad_LeftStick_Down"));
        Keys[KeyName::Gamepad_LeftStick_Right].FName = SDK::FName(skCrypt(L"Gamepad_LeftStick_Right"));
        Keys[KeyName::Gamepad_LeftStick_Left].FName = SDK::FName(skCrypt(L"Gamepad_LeftStick_Left"));
        Keys[KeyName::Gamepad_RightStick_Up].FName = SDK::FName(skCrypt(L"Gamepad_RightStick_Up"));
        Keys[KeyName::Gamepad_RightStick_Down].FName = SDK::FName(skCrypt(L"Gamepad_RightStick_Down"));
        Keys[KeyName::Gamepad_RightStick_Right].FName = SDK::FName(skCrypt(L"Gamepad_RightStick_Right"));
        Keys[KeyName::Gamepad_RightStick_Left].FName = SDK::FName(skCrypt(L"Gamepad_RightStick_Left"));
    }

    DEBUG_LOG(LOG_OFFSET, skCrypt("Input system initialized!"));
}