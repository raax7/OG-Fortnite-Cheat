#pragma once
#include "../SDK/SDK.h"
#include "../SDK/Classes/Engine_classes.h"

class Input {
public:
	/* Enum for easy key usage */
	enum class KeyName {
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
		Gamepad_DPad_Right,
		Gamepad_DPad_Left,
		Gamepad_LeftStick_Up,
		Gamepad_LeftStick_Down,
		Gamepad_LeftStick_Right,
		Gamepad_LeftStick_Left,
		Gamepad_RightStick_Up,
		Gamepad_RightStick_Down,
		Gamepad_RightStick_Right,
		Gamepad_RightStick_Left
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
		SDK::FName FName;
		std::string Name;

		KeyInfoCache IsDown{};
		KeyInfoCache WasJustReleased{};
		KeyInfoCache WasJustPressed{};

		KeyName KeyName;
	};

	/* Cache info on all keys */
	struct KeysCache {
		KeyData AnyKey;

		KeyData MouseX;
		KeyData MouseY;
		KeyData MouseScrollUp;
		KeyData MouseScrollDown;
		KeyData LeftMouseButton;
		KeyData RightMouseButton;
		KeyData MiddleMouseButton;
		KeyData ThumbMouseButton;
		KeyData ThumbMouseButton2;

		KeyData BackSpace;
		KeyData Tab;
		KeyData Enter;
		KeyData Pause;
		KeyData CapsLock;
		KeyData Escape;
		KeyData SpaceBar;
		KeyData PageUp;
		KeyData PageDown;
		KeyData End;
		KeyData Home;

		KeyData Left;
		KeyData Up;
		KeyData Right;
		KeyData Down;

		KeyData Insert;
		KeyData Delete;

		KeyData Zero;
		KeyData One;
		KeyData Two;
		KeyData Three;
		KeyData Four;
		KeyData Five;
		KeyData Six;
		KeyData Seven;
		KeyData Eight;
		KeyData Nine;

		KeyData A;
		KeyData B;
		KeyData C;
		KeyData D;
		KeyData E;
		KeyData F;
		KeyData G;
		KeyData H;
		KeyData I;
		KeyData J;
		KeyData K;
		KeyData L;
		KeyData M;
		KeyData N;
		KeyData O;
		KeyData P;
		KeyData Q;
		KeyData R;
		KeyData S;
		KeyData T;
		KeyData U;
		KeyData V;
		KeyData W;
		KeyData X;
		KeyData Y;
		KeyData Z;

		KeyData NumPadZero;
		KeyData NumPadOne;
		KeyData NumPadTwo;
		KeyData NumPadThree;
		KeyData NumPadFour;
		KeyData NumPadFive;
		KeyData NumPadSix;
		KeyData NumPadSeven;
		KeyData NumPadEight;
		KeyData NumPadNine;

		KeyData Multiply;
		KeyData Add;
		KeyData Subtract;
		KeyData Decimal;
		KeyData Divide;

		KeyData F1;
		KeyData F2;
		KeyData F3;
		KeyData F4;
		KeyData F5;
		KeyData F6;
		KeyData F7;
		KeyData F8;
		KeyData F9;
		KeyData F10;
		KeyData F11;
		KeyData F12;

		KeyData NumLock;
		KeyData ScrollLock;
		KeyData LeftShift;
		KeyData RightShift;
		KeyData LeftControl;
		KeyData RightControl;
		KeyData LeftAlt;
		KeyData RightAlt;
		KeyData LeftCommand;
		KeyData RightCommand;
		KeyData Semicolon;
		KeyData Equals;
		KeyData Comma;
		KeyData Underscore;
		KeyData Period;
		KeyData Slash;
		KeyData Tilde;
		KeyData LeftBracket;
		KeyData Backslash;
		KeyData RightBracket;
		KeyData Quote;
		KeyData Asterix;
		KeyData Ampersand;
		KeyData Caret;
		KeyData Dollar;
		KeyData Exclamation;
		KeyData Colon;

		KeyData A_AccentGrave;
		KeyData E_AccentGrave;
		KeyData E_AccentAigu;
		KeyData C_Cedille;

		KeyData Section;

		KeyData Gamepad_LeftX;
		KeyData Gamepad_LeftY;
		KeyData Gamepad_RightX;
		KeyData Gamepad_RightY;
		KeyData Gamepad_LeftTriggerAxis;
		KeyData Gamepad_RightTriggerAxis;
		KeyData Gamepad_LeftThumbstick;
		KeyData Gamepad_RightThumbstick;
		KeyData Gamepad_Special_Left;
		KeyData Gamepad_Special_Left_X;
		KeyData Gamepad_Special_Left_Y;
		KeyData Gamepad_Special_Right;
		KeyData Gamepad_FaceButton_Bottom;
		KeyData Gamepad_FaceButton_Right;
		KeyData Gamepad_FaceButton_Left;
		KeyData Gamepad_FaceButton_Top;
		KeyData Gamepad_LeftShoulder;
		KeyData Gamepad_RightShoulder;
		KeyData Gamepad_LeftTrigger;
		KeyData Gamepad_RightTrigger;
		KeyData Gamepad_DPad_Up;
		KeyData Gamepad_DPad_Down;
		KeyData Gamepad_DPad_Right;
		KeyData Gamepad_DPad_Left;
		KeyData Gamepad_LeftStick_Up;
		KeyData Gamepad_LeftStick_Down;
		KeyData Gamepad_LeftStick_Right;
		KeyData Gamepad_LeftStick_Left;
		KeyData Gamepad_RightStick_Up;
		KeyData Gamepad_RightStick_Down;
		KeyData Gamepad_RightStick_Right;
		KeyData Gamepad_RightStick_Left;

		KeyData None;
	};



	static KeysCache Keys;
	static MouseCache Mouse;
private:
	/*
	* @brief Get the key data for a specific key
	* 
	* @param Key - The key to get the data for
	*/
	static KeyData& GetKeyData(KeyName Key);
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

	/* Init the input cache system */
	static void Init();
};