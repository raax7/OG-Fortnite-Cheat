#pragma once
#include <unordered_map>

#include "../SDK/Classes/Basic.h"

namespace Input
{
    /* Enum for easy key usage */
    enum class KeyName
    {
        NONE,

        AnyKey,
        MouseX,
        MouseY,
        MouseScrollUp,
        MouseScrollDown,
        LeftMouseButton,
        RightMouseButton,
        MiddleMouseButton,
        ThumbMouseButton,
        ThumbMouseButton2,
        BackSpace,
        Tab,
        Enter,
        Pause,
        CapsLock,
        Escape,
        SpaceBar,
        PageUp,
        PageDown,
        End,
        Home,
        Left,
        Up,
        Right,
        Down,
        Insert,
        Delete,
        Zero,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        NumPadZero,
        NumPadOne,
        NumPadTwo,
        NumPadThree,
        NumPadFour,
        NumPadFive,
        NumPadSix,
        NumPadSeven,
        NumPadEight,
        NumPadNine,
        Multiply,
        Add,
        Subtract,
        Decimal,
        Divide,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        NumLock,
        ScrollLock,
        LeftShift,
        RightShift,
        LeftControl,
        RightControl,
        LeftAlt,
        RightAlt,
        LeftCommand,
        RightCommand,
        Semicolon,
        Equals,
        Comma,
        Underscore,
        Period,
        Slash,
        Tilde,
        LeftBracket,
        Backslash,
        RightBracket,
        Quote,
        Asterix,
        Ampersand,
        Caret,
        Dollar,
        Exclamation,
        Colon,
        A_AccentGrave,
        E_AccentGrave,
        E_AccentAigu,
        C_Cedille,
        Section,
        Gamepad_LeftX,
        Gamepad_LeftY,
        Gamepad_RightX,
        Gamepad_RightY,
        Gamepad_LeftTriggerAxis,
        Gamepad_RightTriggerAxis,
        Gamepad_LeftThumbstick,
        Gamepad_RightThumbstick,
        Gamepad_Special_Left,
        Gamepad_Special_Left_X,
        Gamepad_Special_Left_Y,
        Gamepad_Special_Right,
        Gamepad_FaceButton_Bottom,
        Gamepad_FaceButton_Right,
        Gamepad_FaceButton_Left,
        Gamepad_FaceButton_Top,
        Gamepad_LeftShoulder,
        Gamepad_RightShoulder,
        Gamepad_LeftTrigger,
        Gamepad_RightTrigger,
        Gamepad_DPad_Up,
        Gamepad_DPad_Down,
        Gamepad_DPad_Left,
        Gamepad_DPad_Right,
        Gamepad_LeftStick_Up,
        Gamepad_LeftStick_Down,
        Gamepad_LeftStick_Left,
        Gamepad_LeftStick_Right,
        Gamepad_RightStick_Up,
        Gamepad_RightStick_Down,
        Gamepad_RightStick_Left,
        Gamepad_RightStick_Right,

        KEYNAME_MAX
    };

    /* Info on key names */
    struct KeyMapInfo
    {
        std::string Name;
        SDK::FName FName;
        Input::KeyName KeyName;
    };

    /* Cache info on keystates to avoid multiple checks per frame */
    struct KeyInfoCache
    {
        bool Value = false;
        uint64 FrameUpdated = 0;
    };

    /* Cache info on mouse position to avoid multiple checks per frame */
    struct MouseCache
    {
        SDK::FVector2D Position;
        uint64 FrameUpdated = 0;
    };

    /* Cache info on key name, states etc */
    struct KeyData
    {
        int VKCode;
        KeyName KeyName;

        SDK::FName FName;
        std::string Name;

        KeyInfoCache IsDown{};
        KeyInfoCache WasJustReleased{};
        KeyInfoCache WasJustPressed{};



        // Default constructor
        KeyData() : VKCode(0), KeyName(Input::KeyName::NONE), FName(), Name() {}

        // Constructor with SDK::FName and std::string arguments
        KeyData(const int VKCode, const Input::KeyName KeyName, const SDK::FName& FName, const const char* Name) : VKCode(VKCode), KeyName(KeyName), FName(FName), Name(Name) {}
    };

    inline std::unordered_map<KeyName, KeyData> Keys = {
        { Input::KeyName::NONE, {0x0, Input::KeyName::NONE, SDK::FName(), skCrypt("None") } },

        { Input::KeyName::AnyKey, { 0x0, Input::KeyName::AnyKey, SDK::FName(), skCrypt("Any Key") } },

        { Input::KeyName::MouseX, { 0x0, Input::KeyName::MouseX, SDK::FName(), skCrypt("Mouse X") } },
        { Input::KeyName::MouseY, { 0x0, Input::KeyName::MouseY, SDK::FName(), skCrypt("Mouse Y") } },
        { Input::KeyName::MouseScrollUp, { 0x0, Input::KeyName::MouseScrollUp, SDK::FName(), skCrypt("Scroll Up") } },
        { Input::KeyName::MouseScrollDown, { 0x0, Input::KeyName::MouseScrollDown, SDK::FName(), skCrypt("Scroll Down") } },
        { Input::KeyName::LeftMouseButton, { VK_LBUTTON, Input::KeyName::LeftMouseButton, SDK::FName(), skCrypt("LMB") } },
        { Input::KeyName::RightMouseButton, { VK_RBUTTON, Input::KeyName::RightMouseButton, SDK::FName(), skCrypt("RMB") } },
        { Input::KeyName::MiddleMouseButton, { VK_MBUTTON, Input::KeyName::RightMouseButton,SDK::FName(), skCrypt("MMB") } },
        { Input::KeyName::ThumbMouseButton, { VK_XBUTTON1, Input::KeyName::ThumbMouseButton, SDK::FName(), skCrypt("Thumb MB") } },
        { Input::KeyName::ThumbMouseButton2, { VK_XBUTTON2, Input::KeyName::ThumbMouseButton, SDK::FName(), skCrypt("Thumb MB2") } },

        { Input::KeyName::BackSpace, { VK_BACK, Input::KeyName::BackSpace, SDK::FName(), skCrypt("BackSpace") } },
        { Input::KeyName::Tab, { VK_TAB, Input::KeyName::BackSpace, SDK::FName(), skCrypt("Tab") } },
        { Input::KeyName::Enter, { VK_RETURN, Input::KeyName::Enter, SDK::FName(), skCrypt("Enter") } },
        { Input::KeyName::Pause, { VK_PAUSE, Input::KeyName::Pause, SDK::FName(), skCrypt("Pause") } },
        { Input::KeyName::CapsLock, { VK_CAPITAL, Input::KeyName::CapsLock, SDK::FName(), skCrypt("CapsLock") } },
        { Input::KeyName::Escape, { VK_ESCAPE, Input::KeyName::Escape, SDK::FName(), skCrypt("Escape") } },
        { Input::KeyName::SpaceBar, { VK_SPACE, Input::KeyName::SpaceBar, SDK::FName(), skCrypt("Space") } },
        { Input::KeyName::PageUp, { VK_PRIOR, Input::KeyName::SpaceBar, SDK::FName(), skCrypt("PageUp") } },
        { Input::KeyName::PageDown, { VK_NEXT, Input::KeyName::PageDown, SDK::FName(), skCrypt("PageDown") } },
        { Input::KeyName::End, { VK_END, Input::KeyName::End, SDK::FName(), skCrypt("End") } },
        { Input::KeyName::Home, { VK_HOME, Input::KeyName::Home, SDK::FName(), skCrypt("Home") } },

        { Input::KeyName::Left, { VK_LEFT, Input::KeyName::Left, SDK::FName(), skCrypt("Left Arrow") } },
        { Input::KeyName::Up, { VK_UP, Input::KeyName::Up, SDK::FName(), skCrypt("Up Arrow") } },
        { Input::KeyName::Right, { VK_RIGHT, Input::KeyName::Right, SDK::FName(), skCrypt("Right Arrow") } },
        { Input::KeyName::Down, { VK_DOWN, Input::KeyName::Down, SDK::FName(), skCrypt("Down Arrow") } },

        { Input::KeyName::Insert, { VK_INSERT, Input::KeyName::Insert, SDK::FName(), skCrypt("Insert") } },
        { Input::KeyName::Delete, { VK_DELETE, Input::KeyName::Delete, SDK::FName(), skCrypt("Delete") } },

        { Input::KeyName::Zero, { '0', Input::KeyName::Zero, SDK::FName(), skCrypt("0") } },
        { Input::KeyName::One, { '1', Input::KeyName::One, SDK::FName(), skCrypt("1") } },
        { Input::KeyName::Two, { '2', Input::KeyName::Two, SDK::FName(), skCrypt("2") } },
        { Input::KeyName::Three, { '3', Input::KeyName::Three, SDK::FName(), skCrypt("3") } },
        { Input::KeyName::Four, { '4', Input::KeyName::Four, SDK::FName(), skCrypt("4") } },
        { Input::KeyName::Five, { '5', Input::KeyName::Five, SDK::FName(), skCrypt("5") } },
        { Input::KeyName::Six, { '6', Input::KeyName::Six, SDK::FName(), skCrypt("6") } },
        { Input::KeyName::Seven, { '7', Input::KeyName::Seven, SDK::FName(), skCrypt("7") } },
        { Input::KeyName::Eight, { '8', Input::KeyName::Eight, SDK::FName(), skCrypt("8") } },
        { Input::KeyName::Nine, { '9', Input::KeyName::Nine, SDK::FName(), skCrypt("9") } },

        { Input::KeyName::A, { 'A', Input::KeyName::A, SDK::FName(), skCrypt("A") } },
        { Input::KeyName::B, { 'B', Input::KeyName::B, SDK::FName(), skCrypt("B") } },
        { Input::KeyName::C, { 'C', Input::KeyName::C, SDK::FName(), skCrypt("C") } },
        { Input::KeyName::D, { 'D', Input::KeyName::D, SDK::FName(), skCrypt("D") } },
        { Input::KeyName::E, { 'E', Input::KeyName::E, SDK::FName(), skCrypt("E") } },
        { Input::KeyName::F, { 'F', Input::KeyName::F, SDK::FName(), skCrypt("F") } },
        { Input::KeyName::G, { 'G', Input::KeyName::G, SDK::FName(), skCrypt("G") } },
        { Input::KeyName::H, { 'H', Input::KeyName::H, SDK::FName(), skCrypt("H") } },
        { Input::KeyName::I, { 'I', Input::KeyName::I, SDK::FName(), skCrypt("I") } },
        { Input::KeyName::J, { 'J', Input::KeyName::J, SDK::FName(), skCrypt("J") } },
        { Input::KeyName::K, { 'K', Input::KeyName::K, SDK::FName(), skCrypt("K") } },
        { Input::KeyName::L, { 'L', Input::KeyName::L, SDK::FName(), skCrypt("L") } },
        { Input::KeyName::M, { 'M', Input::KeyName::M, SDK::FName(), skCrypt("M") } },
        { Input::KeyName::N, { 'N', Input::KeyName::N, SDK::FName(), skCrypt("N") } },
        { Input::KeyName::O, { 'O', Input::KeyName::O, SDK::FName(), skCrypt("O") } },
        { Input::KeyName::P, { 'P', Input::KeyName::P, SDK::FName(), skCrypt("P") } },
        { Input::KeyName::Q, { 'Q', Input::KeyName::Q, SDK::FName(), skCrypt("Q") } },
        { Input::KeyName::R, { 'R', Input::KeyName::R, SDK::FName(), skCrypt("R") } },
        { Input::KeyName::S, { 'S', Input::KeyName::S, SDK::FName(), skCrypt("S") } },
        { Input::KeyName::T, { 'T', Input::KeyName::T, SDK::FName(), skCrypt("T") } },
        { Input::KeyName::U, { 'U', Input::KeyName::U, SDK::FName(), skCrypt("U") } },
        { Input::KeyName::V, { 'V', Input::KeyName::V, SDK::FName(), skCrypt("V") } },
        { Input::KeyName::W, { 'W', Input::KeyName::W, SDK::FName(), skCrypt("W") } },
        { Input::KeyName::X, { 'X', Input::KeyName::X, SDK::FName(), skCrypt("X") } },
        { Input::KeyName::Y, { 'Y', Input::KeyName::Y, SDK::FName(), skCrypt("Y") } },
        { Input::KeyName::Z, { 'Z', Input::KeyName::Z, SDK::FName(), skCrypt("Z") } },

        { Input::KeyName::NumPadZero, { VK_NUMPAD0, Input::KeyName::NumPadZero, SDK::FName(), skCrypt("NumPad 0") } },
        { Input::KeyName::NumPadOne, { VK_NUMPAD1, Input::KeyName::NumPadOne, SDK::FName(), skCrypt("NumPad 1") } },
        { Input::KeyName::NumPadTwo, { VK_NUMPAD2, Input::KeyName::NumPadTwo, SDK::FName(), skCrypt("NumPad 2") } },
        { Input::KeyName::NumPadThree, { VK_NUMPAD3, Input::KeyName::NumPadThree, SDK::FName(), skCrypt("NumPad 3") } },
        { Input::KeyName::NumPadFour, { VK_NUMPAD4, Input::KeyName::NumPadFour, SDK::FName(), skCrypt("NumPad 4") } },
        { Input::KeyName::NumPadFive, { VK_NUMPAD5, Input::KeyName::NumPadFive, SDK::FName(), skCrypt("NumPad 5") } },
        { Input::KeyName::NumPadSix, { VK_NUMPAD6, Input::KeyName::NumPadSix, SDK::FName(), skCrypt("NumPad 6") } },
        { Input::KeyName::NumPadSeven, { VK_NUMPAD7, Input::KeyName::NumPadSeven, SDK::FName(), skCrypt("NumPad 7") } },
        { Input::KeyName::NumPadEight, { VK_NUMPAD8, Input::KeyName::NumPadEight, SDK::FName(), skCrypt("NumPad 8") } },
        { Input::KeyName::NumPadNine, { VK_NUMPAD9, Input::KeyName::NumPadNine, SDK::FName(), skCrypt("NumPad 9") } },

        { Input::KeyName::Multiply, { VK_MULTIPLY, Input::KeyName::Multiply, SDK::FName(), skCrypt("Multiply") } },
        { Input::KeyName::Add, { VK_ADD, Input::KeyName::Add, SDK::FName(), skCrypt("Add") } },
        { Input::KeyName::Subtract, { VK_SUBTRACT, Input::KeyName::Subtract, SDK::FName(), skCrypt("Subtract") } },
        { Input::KeyName::Decimal, { VK_DECIMAL, Input::KeyName::Decimal, SDK::FName(), skCrypt("Decimal") } },
        { Input::KeyName::Divide, { VK_DIVIDE, Input::KeyName::Divide, SDK::FName(), skCrypt("Divide") } },

        { Input::KeyName::F1, { VK_F1, Input::KeyName::F1, SDK::FName(), skCrypt("F1") } },
        { Input::KeyName::F2, { VK_F2, Input::KeyName::F2, SDK::FName(), skCrypt("F2") } },
        { Input::KeyName::F3, { VK_F3, Input::KeyName::F3, SDK::FName(), skCrypt("F3") } },
        { Input::KeyName::F4, { VK_F4, Input::KeyName::F4, SDK::FName(), skCrypt("F4") } },
        { Input::KeyName::F5, { VK_F5, Input::KeyName::F5, SDK::FName(), skCrypt("F5") } },
        { Input::KeyName::F6, { VK_F6, Input::KeyName::F6, SDK::FName(), skCrypt("F6") } },
        { Input::KeyName::F7, { VK_F7, Input::KeyName::F7, SDK::FName(), skCrypt("F7") } },
        { Input::KeyName::F8, { VK_F8, Input::KeyName::F8, SDK::FName(), skCrypt("F8") } },
        { Input::KeyName::F9, { VK_F9, Input::KeyName::F9, SDK::FName(), skCrypt("F9") } },
        { Input::KeyName::F10, { VK_F10, Input::KeyName::F10, SDK::FName(), skCrypt("F10") } },
        { Input::KeyName::F11, { VK_F11, Input::KeyName::F11, SDK::FName(), skCrypt("F11") } },
        { Input::KeyName::F12, { VK_F12, Input::KeyName::F12, SDK::FName(), skCrypt("F12") } },

        { Input::KeyName::NumLock, { 0x0, Input::KeyName::NumLock, SDK::FName(), skCrypt("NumLock") } },
        { Input::KeyName::ScrollLock, { 0x0, Input::KeyName::ScrollLock, SDK::FName(), skCrypt("ScrollLock") } },
        { Input::KeyName::LeftShift, { 0x0, Input::KeyName::LeftShift, SDK::FName(), skCrypt("Left Shift") } },
        { Input::KeyName::RightShift, { 0x0, Input::KeyName::RightShift, SDK::FName(), skCrypt("Right Shift") } },
        { Input::KeyName::LeftControl, { 0x0, Input::KeyName::LeftControl, SDK::FName(), skCrypt("Left Control") } },
        { Input::KeyName::RightControl, { 0x0, Input::KeyName::RightControl, SDK::FName(), skCrypt("Right Control") } },
        { Input::KeyName::LeftAlt, { 0x0, Input::KeyName::LeftAlt, SDK::FName(), skCrypt("Left Alt") } },
        { Input::KeyName::RightAlt, { 0x0, Input::KeyName::RightAlt, SDK::FName(), skCrypt("Right Alt") } },
        { Input::KeyName::LeftCommand, { 0x0, Input::KeyName::LeftCommand, SDK::FName(), skCrypt("Left Command") } },
        { Input::KeyName::RightCommand, { 0x0, Input::KeyName::RightCommand, SDK::FName(), skCrypt("Right Command") } },

        { Input::KeyName::Semicolon, { 0x0, Input::KeyName::Semicolon, SDK::FName(), skCrypt("Semicolon") } },
        { Input::KeyName::Equals, { 0x0, Input::KeyName::Equals, SDK::FName(), skCrypt("Equals") } },
        { Input::KeyName::Comma, { 0x0, Input::KeyName::Comma, SDK::FName(), skCrypt("Comma") } },
        { Input::KeyName::Underscore, { 0x0, Input::KeyName::Underscore, SDK::FName(), skCrypt("Underscore") } },
        { Input::KeyName::Period, { 0x0, Input::KeyName::Period, SDK::FName(), skCrypt("Period") } },
        { Input::KeyName::Slash, { 0x0, Input::KeyName::Slash, SDK::FName(), skCrypt("Slash") } },
        { Input::KeyName::Tilde, { 0x0, Input::KeyName::Tilde, SDK::FName(), skCrypt("Tilde") } },
        { Input::KeyName::LeftBracket, { 0x0, Input::KeyName::LeftBracket, SDK::FName(), skCrypt("Left Bracket") } },
        { Input::KeyName::Backslash, { 0x0, Input::KeyName::Backslash, SDK::FName(), skCrypt("Backslash") } },
        { Input::KeyName::RightBracket, { 0x0, Input::KeyName::RightBracket, SDK::FName(), skCrypt("Right Bracket") } },
        { Input::KeyName::Quote, { 0x0, Input::KeyName::Quote, SDK::FName(), skCrypt("Quote") } },
        { Input::KeyName::Asterix, { 0x0, Input::KeyName::Asterix, SDK::FName(), skCrypt("Asterix") } },
        { Input::KeyName::Ampersand, { 0x0, Input::KeyName::Ampersand, SDK::FName(), skCrypt("Ampersand") } },
        { Input::KeyName::Caret, { 0x0, Input::KeyName::Caret, SDK::FName(), skCrypt("Caret") } },
        { Input::KeyName::Dollar, { 0x0, Input::KeyName::Dollar, SDK::FName(), skCrypt("Dollar") } },
        { Input::KeyName::Exclamation, { 0x0, Input::KeyName::Exclamation, SDK::FName(), skCrypt("Exclamation") } },
        { Input::KeyName::Colon, { 0x0, Input::KeyName::Colon, SDK::FName(), skCrypt("Colon") } },

        { Input::KeyName::A_AccentGrave, { 0x0, Input::KeyName::A_AccentGrave, SDK::FName(), skCrypt("A Accent Grave") } },
        { Input::KeyName::E_AccentGrave, { 0x0, Input::KeyName::E_AccentGrave, SDK::FName(), skCrypt("E Accent Grave") } },
        { Input::KeyName::E_AccentAigu, { 0x0, Input::KeyName::E_AccentAigu, SDK::FName(), skCrypt("E Accent Aigu") } },
        { Input::KeyName::C_Cedille, { 0x0, Input::KeyName::C_Cedille, SDK::FName(), skCrypt("C Cedille") } },

        { Input::KeyName::Section, { 0x0, Input::KeyName::Section, SDK::FName(), skCrypt("Section") } },

        { Input::KeyName::Gamepad_LeftX, { 0x0, Input::KeyName::Gamepad_LeftX, SDK::FName(), skCrypt("Gamepad Left X") } },
        { Input::KeyName::Gamepad_LeftY, { 0x0, Input::KeyName::Gamepad_LeftY, SDK::FName(), skCrypt("Gamepad Left Y") } },
        { Input::KeyName::Gamepad_RightX, { 0x0, Input::KeyName::Gamepad_RightX, SDK::FName(), skCrypt("Gamepad Right X") } },
        { Input::KeyName::Gamepad_RightY, { 0x0, Input::KeyName::Gamepad_RightY, SDK::FName(), skCrypt("Gamepad Right Y") } },
        { Input::KeyName::Gamepad_LeftTriggerAxis, { 0x0, Input::KeyName::Gamepad_LeftTriggerAxis, SDK::FName(), skCrypt("Gamepad Left Trigger Axis") } },
        { Input::KeyName::Gamepad_RightTriggerAxis, { 0x0, Input::KeyName::Gamepad_RightTriggerAxis, SDK::FName(), skCrypt("Gamepad Right Trigger Axis") } },
        { Input::KeyName::Gamepad_LeftThumbstick, { 0x0, Input::KeyName::Gamepad_LeftThumbstick, SDK::FName(), skCrypt("Gamepad Left Thumbstick") } },
        { Input::KeyName::Gamepad_RightThumbstick, { 0x0, Input::KeyName::Gamepad_RightThumbstick, SDK::FName(), skCrypt("Gamepad Right Thumbstick") } },
        { Input::KeyName::Gamepad_Special_Left, { 0x0, Input::KeyName::Gamepad_Special_Left, SDK::FName(), skCrypt("Gamepad Special Left") } },
        { Input::KeyName::Gamepad_Special_Left_X, { 0x0, Input::KeyName::Gamepad_Special_Left_X, SDK::FName(), skCrypt("Gamepad Special Left X") } },
        { Input::KeyName::Gamepad_Special_Left_Y, { 0x0, Input::KeyName::Gamepad_Special_Left_Y, SDK::FName(), skCrypt("Gamepad Special Left Y") } },
        { Input::KeyName::Gamepad_Special_Right, { 0x0, Input::KeyName::Gamepad_Special_Right, SDK::FName(), skCrypt("Gamepad Special Right") } },
        { Input::KeyName::Gamepad_FaceButton_Bottom, { 0x0, Input::KeyName::Gamepad_FaceButton_Bottom, SDK::FName(), skCrypt("Gamepad Face Button Bottom") } },
        { Input::KeyName::Gamepad_FaceButton_Right, { 0x0, Input::KeyName::Gamepad_FaceButton_Right, SDK::FName(), skCrypt("Gamepad Face Button Right") } },
        { Input::KeyName::Gamepad_FaceButton_Left, { 0x0, Input::KeyName::Gamepad_FaceButton_Left, SDK::FName(), skCrypt("Gamepad Face Button Left") } },
        { Input::KeyName::Gamepad_FaceButton_Top, { 0x0, Input::KeyName::Gamepad_FaceButton_Top, SDK::FName(), skCrypt("Gamepad Face Button Top") } },
        { Input::KeyName::Gamepad_LeftShoulder, { 0x0, Input::KeyName::Gamepad_LeftShoulder, SDK::FName(), skCrypt("Gamepad Left Shoulder") } },
        { Input::KeyName::Gamepad_RightShoulder, { 0x0, Input::KeyName::Gamepad_RightShoulder, SDK::FName(), skCrypt("Gamepad Right Shoulder") } },
        { Input::KeyName::Gamepad_LeftTrigger, { 0x0, Input::KeyName::Gamepad_LeftTrigger, SDK::FName(), skCrypt("Gamepad Left Trigger") } },
        { Input::KeyName::Gamepad_RightTrigger, { 0x0, Input::KeyName::Gamepad_RightTrigger, SDK::FName(), skCrypt("Gamepad Right Trigger") } },
        { Input::KeyName::Gamepad_DPad_Up, { 0x0, Input::KeyName::Gamepad_DPad_Up, SDK::FName(), skCrypt("Gamepad DPad Up") } },
        { Input::KeyName::Gamepad_DPad_Down, { 0x0, Input::KeyName::Gamepad_DPad_Down, SDK::FName(), skCrypt("Gamepad DPad Down") } },
        { Input::KeyName::Gamepad_DPad_Left, { 0x0, Input::KeyName::Gamepad_DPad_Left, SDK::FName(), skCrypt("Gamepad DPad Left") } },
        { Input::KeyName::Gamepad_DPad_Right, { 0x0, Input::KeyName::Gamepad_DPad_Right, SDK::FName(), skCrypt("Gamepad DPad Right") } },
        { Input::KeyName::Gamepad_LeftStick_Up, { 0x0, Input::KeyName::Gamepad_LeftStick_Up, SDK::FName(), skCrypt("Gamepad Leftstick Up") } },
        { Input::KeyName::Gamepad_LeftStick_Down, { 0x0, Input::KeyName::Gamepad_LeftStick_Down, SDK::FName(), skCrypt("Gamepad Leftstick Down") } },
        { Input::KeyName::Gamepad_LeftStick_Left, { 0x0, Input::KeyName::Gamepad_LeftStick_Left, SDK::FName(), skCrypt("Gamepad Leftstick Left") } },
        { Input::KeyName::Gamepad_LeftStick_Right, { 0x0, Input::KeyName::Gamepad_LeftStick_Right, SDK::FName(), skCrypt("Gamepad Leftstick Right") } },
        { Input::KeyName::Gamepad_RightStick_Up, { 0x0, Input::KeyName::Gamepad_RightStick_Up, SDK::FName(), skCrypt("Gamepad Rightstick Up") } },
        { Input::KeyName::Gamepad_RightStick_Down, { 0x0, Input::KeyName::Gamepad_RightStick_Down, SDK::FName(), skCrypt("Gamepad Rightstick Down") } },
        { Input::KeyName::Gamepad_RightStick_Left, { 0x0, Input::KeyName::Gamepad_RightStick_Left, SDK::FName(), skCrypt("Gamepad Rightstick Left") } },
        { Input::KeyName::Gamepad_RightStick_Right, { 0x0, Input::KeyName::Gamepad_RightStick_Right, SDK::FName(), skCrypt("Gamepad Rightstick Right") } },
    };
    inline MouseCache Mouse;

    /*
    * @brief Get the mouse position
    *
    * @return Returns - The mouse position
    */
    SDK::FVector2D GetMousePosition();

    /*
    * @brief Get the mouse position
    *
    * @param MousePosition - The mouse position
    *
    * @return Returns - The mouse position
    */
    bool IsKeyDown(const KeyName Key);
    /*
    * @brief Get the mouse position
    *
    * @param MousePosition - The mouse position
    *
    * @return Returns - The mouse position
    */
    bool WasKeyJustReleased(const KeyName Key);
    /*
    * @brief Get the mouse position
    *
    * @param MousePosition - The mouse position
    *
    * @return Returns - The mouse position
    */
    bool WasKeyJustPressed(const KeyName Key);
    /*
    * @brief Get all the keys that are currently down
    *
    * @return A vector of all the keys that are currently down
    */
    std::vector<Input::KeyName> GetAllDownKeys();
    /*
    * @brief Get all the keys that were just released
    *
    * @return A vector of all the keys that were just released
    */
    std::vector<Input::KeyName> GetAllJustReleasedKeys();
    /*
    * @brief Get all the keys that were just pressed
    *
    * @return A vector of all the keys that were just pressed
    */
    std::vector<Input::KeyName> GetAllJustPressedKeys();

    /*
    * @brief Get the key name as a string from a key
    *
    * @param Key - The key to get the name of
    *
    * @return The name of the key
    */
    std::string GetKeyNameString(const Input::KeyName Key);

    /* Init the input cache system */
    void Init();
};