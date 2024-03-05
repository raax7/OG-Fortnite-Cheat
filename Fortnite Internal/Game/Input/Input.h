#pragma once
#include <unordered_map>

#include "../../Globals.h"

#include "../SDK/SDK.h"
#include "../SDK/Classes/Engine_classes.h"

class Input {
public:
	/* Enum for easy key usage */
	enum class KeyName {
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
	struct KeyMapInfo {
		std::string Name;
		SDK::FName FName;
		Input::KeyName KeyName;
	};
private:
	/* Cache info on keystates to avoid multiple checks per frame */
	struct KeyInfoCache {
		bool Value = false;
		uint64 FrameUpdated = 0;
	};

	/* Cache info on mouse position to avoid multiple checks per frame */
	struct MouseCache {
		SDK::FVector2D Position;
		uint64 FrameUpdated = 0;
	};

	/* Cache info on key name, states etc */
	struct KeyData {
		int VKCode;
		KeyName KeyName;

		SDK::FName FName;
		std::string Name;

		KeyInfoCache IsDown{};
		KeyInfoCache WasJustReleased{};
		KeyInfoCache WasJustPressed{};



		// Default constructor
		KeyData() : VKCode(0), KeyName(Input::KeyName::KEYNAME_MAX), FName(), Name() {}

		// Constructor with SDK::FName and std::string arguments
		KeyData(int VKCode, Input::KeyName KeyName, SDK::FName FName, std::string Name) : VKCode(VKCode), KeyName(KeyName), FName(FName), Name(Name) {}
	};

	static std::unordered_map<KeyName, KeyData> Keys;
	static MouseCache Mouse;
public:
	/*
	* @brief Get the mouse position
	* 	
	* @return Returns - The mouse position
	*/
	static SDK::FVector2D GetMousePosition();

	/*
	* @brief Get the mouse position
	* 
	* @param MousePosition - The mouse position
	* 
	* @return Returns - The mouse position
	*/
	static bool IsKeyDown(KeyName Key);
	/*
	* @brief Get the mouse position
	* 
	* @param MousePosition - The mouse position
	* 
	* @return Returns - The mouse position
	*/
	static bool WasKeyJustReleased(KeyName Key);
	/*
	* @brief Get the mouse position
	* 
	* @param MousePosition - The mouse position
	* 
	* @return Returns - The mouse position
	*/
	static bool WasKeyJustPressed(KeyName Key);
	/*
	* @brief Get all the keys that are currently down
	* 
	* @return A vector of all the keys that are currently down
	*/
	static std::vector<Input::KeyName> GetAllDownKeys();
	/*
	* @brief Get all the keys that were just released
	* 
	* @return A vector of all the keys that were just released
	*/
	static std::vector<Input::KeyName> GetAllJustReleasedKeys();
	/*
	* @brief Get all the keys that were just pressed
	* 
	* @return A vector of all the keys that were just pressed
	*/
	static std::vector<Input::KeyName> GetAllJustPressedKeys();

	/*
	* @brief Get the key name as a string from a key
	* 
	* @param Key - The key to get the name of
	* 
	* @return The name of the key
	*/
	static std::string GetKeyNameString(Input::KeyName Key);

	/* Init the input cache system */
	static void Init();
};