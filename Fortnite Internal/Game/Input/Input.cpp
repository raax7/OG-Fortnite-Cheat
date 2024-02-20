#include "Input.h"

inline Input::KeysCache Input::Keys;
inline Input::MouseCache Input::Mouse;

Input::KeyData& Input::GetKeyData(KeyName Key) {
	switch (Key) {
	case Input::KeyName::AnyKey:
		return Input::Keys.AnyKey;

	case Input::KeyName::MouseX:
		return Input::Keys.MouseX;
	case Input::KeyName::MouseY:
		return Input::Keys.MouseY;
	case Input::KeyName::MouseScrollUp:
		return Input::Keys.MouseScrollUp;
	case Input::KeyName::MouseScrollDown:
		return Input::Keys.MouseScrollDown;
	case Input::KeyName::LeftMouseButton:
		return Input::Keys.LeftMouseButton;
	case Input::KeyName::RightMouseButton:
		return Input::Keys.RightMouseButton;
	case Input::KeyName::MiddleMouseButton:
		return Input::Keys.MiddleMouseButton;
	case Input::KeyName::ThumbMouseButton:
		return Input::Keys.ThumbMouseButton;
	case Input::KeyName::ThumbMouseButton2:
		return Input::Keys.ThumbMouseButton2;

	case Input::KeyName::BackSpace:
		return Input::Keys.BackSpace;
	case Input::KeyName::Tab:
		return Input::Keys.Tab;
	case Input::KeyName::Enter:
		return Input::Keys.Enter;
	case Input::KeyName::Pause:
		return Input::Keys.Pause;
	case Input::KeyName::CapsLock:
		return Input::Keys.CapsLock;
	case Input::KeyName::Escape:
		return Input::Keys.Escape;
	case Input::KeyName::SpaceBar:
		return Input::Keys.SpaceBar;
	case Input::KeyName::PageUp:
		return Input::Keys.PageUp;
	case Input::KeyName::PageDown:
		return Input::Keys.PageDown;
	case Input::KeyName::End:
		return Input::Keys.End;
	case Input::KeyName::Home:
		return Input::Keys.Home;

	case Input::KeyName::Left:
		return Input::Keys.Left;
	case Input::KeyName::Up:
		return Input::Keys.Up;
	case Input::KeyName::Right:
		return Input::Keys.Right;
	case Input::KeyName::Down:
		return Input::Keys.Down;

	case Input::KeyName::Insert:
		return Input::Keys.Insert;
	case Input::KeyName::Delete:
		return Input::Keys.Delete;

	case Input::KeyName::Zero:
		return Input::Keys.Zero;
	case Input::KeyName::One:
		return Input::Keys.One;
	case Input::KeyName::Two:
		return Input::Keys.Two;
	case Input::KeyName::Three:
		return Input::Keys.Three;
	case Input::KeyName::Four:
		return Input::Keys.Four;
	case Input::KeyName::Five:
		return Input::Keys.Five;
	case Input::KeyName::Six:
		return Input::Keys.Six;
	case Input::KeyName::Seven:
		return Input::Keys.Seven;
	case Input::KeyName::Eight:
		return Input::Keys.Eight;
	case Input::KeyName::Nine:
		return Input::Keys.Nine;

	case Input::KeyName::A:
		return Input::Keys.A;
	case Input::KeyName::B:
		return Input::Keys.B;
	case Input::KeyName::C:
		return Input::Keys.C;
	case Input::KeyName::D:
		return Input::Keys.D;
	case Input::KeyName::E:
		return Input::Keys.E;
	case Input::KeyName::F:
		return Input::Keys.F;
	case Input::KeyName::G:
		return Input::Keys.G;
	case Input::KeyName::H:
		return Input::Keys.H;
	case Input::KeyName::I:
		return Input::Keys.I;
	case Input::KeyName::J:
		return Input::Keys.J;
	case Input::KeyName::K:
		return Input::Keys.K;
	case Input::KeyName::L:
		return Input::Keys.L;
	case Input::KeyName::M:
		return Input::Keys.M;
	case Input::KeyName::N:
		return Input::Keys.N;
	case Input::KeyName::O:
		return Input::Keys.O;
	case Input::KeyName::P:
		return Input::Keys.P;
	case Input::KeyName::Q:
		return Input::Keys.Q;
	case Input::KeyName::R:
		return Input::Keys.R;
	case Input::KeyName::S:
		return Input::Keys.S;
	case Input::KeyName::T:
		return Input::Keys.T;
	case Input::KeyName::U:
		return Input::Keys.U;
	case Input::KeyName::V:
		return Input::Keys.V;
	case Input::KeyName::W:
		return Input::Keys.W;
	case Input::KeyName::X:
		return Input::Keys.X;
	case Input::KeyName::Y:
		return Input::Keys.Y;
	case Input::KeyName::Z:
		return Input::Keys.Z;

	case Input::KeyName::NumPadZero:
		return Input::Keys.NumPadZero;
	case Input::KeyName::NumPadOne:
		return Input::Keys.NumPadOne;
	case Input::KeyName::NumPadTwo:
		return Input::Keys.NumPadTwo;
	case Input::KeyName::NumPadThree:
		return Input::Keys.NumPadThree;
	case Input::KeyName::NumPadFour:
		return Input::Keys.NumPadFour;
	case Input::KeyName::NumPadFive:
		return Input::Keys.NumPadFive;
	case Input::KeyName::NumPadSix:
		return Input::Keys.NumPadSix;
	case Input::KeyName::NumPadSeven:
		return Input::Keys.NumPadSeven;
	case Input::KeyName::NumPadEight:
		return Input::Keys.NumPadEight;
	case Input::KeyName::NumPadNine:
		return Input::Keys.NumPadNine;

	case Input::KeyName::Multiply:
		return Input::Keys.Multiply;
	case Input::KeyName::Add:
		return Input::Keys.Add;
	case Input::KeyName::Subtract:
		return Input::Keys.Subtract;
	case Input::KeyName::Decimal:
		return Input::Keys.Decimal;
	case Input::KeyName::Divide:
		return Input::Keys.Divide;

	case Input::KeyName::F1:
		return Input::Keys.F1;
	case Input::KeyName::F2:
		return Input::Keys.F2;
	case Input::KeyName::F3:
		return Input::Keys.F3;
	case Input::KeyName::F4:
		return Input::Keys.F4;
	case Input::KeyName::F5:
		return Input::Keys.F5;
	case Input::KeyName::F6:
		return Input::Keys.F6;
	case Input::KeyName::F7:
		return Input::Keys.F7;
	case Input::KeyName::F8:
		return Input::Keys.F8;
	case Input::KeyName::F9:
		return Input::Keys.F9;
	case Input::KeyName::F10:
		return Input::Keys.F10;
	case Input::KeyName::F11:
		return Input::Keys.F11;
	case Input::KeyName::F12:
		return Input::Keys.F12;

	case Input::KeyName::NumLock:
		return Input::Keys.NumLock;
	case Input::KeyName::ScrollLock:
		return Input::Keys.ScrollLock;
	case Input::KeyName::LeftShift:
		return Input::Keys.LeftShift;
	case Input::KeyName::RightShift:
		return Input::Keys.RightShift;
	case Input::KeyName::LeftControl:
		return Input::Keys.LeftControl;
	case Input::KeyName::RightControl:
		return Input::Keys.RightControl;
	case Input::KeyName::LeftAlt:
		return Input::Keys.LeftAlt;
	case Input::KeyName::RightAlt:
		return Input::Keys.RightAlt;
	case Input::KeyName::LeftCommand:
		return Input::Keys.LeftCommand;
	case Input::KeyName::RightCommand:
		return Input::Keys.RightCommand;
	case Input::KeyName::Semicolon:
		return Input::Keys.Semicolon;
	case Input::KeyName::Equals:
		return Input::Keys.Equals;
	case Input::KeyName::Comma:
		return Input::Keys.Comma;
	case Input::KeyName::Underscore:
		return Input::Keys.Underscore;
	case Input::KeyName::Period:
		return Input::Keys.Period;
	case Input::KeyName::Slash:
		return Input::Keys.Slash;
	case Input::KeyName::Tilde:
		return Input::Keys.Tilde;
	case Input::KeyName::LeftBracket:
		return Input::Keys.LeftBracket;
	case Input::KeyName::Backslash:
		return Input::Keys.Backslash;
	case Input::KeyName::RightBracket:
		return Input::Keys.RightBracket;
	case Input::KeyName::Quote:
		return Input::Keys.Quote;
	case Input::KeyName::Asterix:
		return Input::Keys.Asterix;
	case Input::KeyName::Ampersand:
		return Input::Keys.Ampersand;
	case Input::KeyName::Caret:
		return Input::Keys.Caret;
	case Input::KeyName::Dollar:
		return Input::Keys.Dollar;
	case Input::KeyName::Exclamation:
		return Input::Keys.Exclamation;
	case Input::KeyName::Colon:
		return Input::Keys.Colon;

	case Input::KeyName::A_AccentGrave:
		return Input::Keys.A_AccentGrave;
	case Input::KeyName::E_AccentGrave:
		return Input::Keys.E_AccentGrave;
	case Input::KeyName::E_AccentAigu:
		return Input::Keys.E_AccentAigu;
	case Input::KeyName::C_Cedille:
		return Input::Keys.C_Cedille;

	case Input::KeyName::Section:
		return Input::Keys.Section;

	case Input::KeyName::Gamepad_LeftX:
		return Input::Keys.Gamepad_LeftX;
	case Input::KeyName::Gamepad_LeftY:
		return Input::Keys.Gamepad_LeftY;
	case Input::KeyName::Gamepad_RightX:
		return Input::Keys.Gamepad_RightX;
	case Input::KeyName::Gamepad_RightY:
		return Input::Keys.Gamepad_RightY;
	case Input::KeyName::Gamepad_LeftTriggerAxis:
		return Input::Keys.Gamepad_LeftTriggerAxis;
	case Input::KeyName::Gamepad_RightTriggerAxis:
		return Input::Keys.Gamepad_RightTriggerAxis;
	case Input::KeyName::Gamepad_LeftThumbstick:
		return Input::Keys.Gamepad_LeftThumbstick;
	case Input::KeyName::Gamepad_RightThumbstick:
		return Input::Keys.Gamepad_RightThumbstick;
	case Input::KeyName::Gamepad_Special_Left:
		return Input::Keys.Gamepad_Special_Left;
	case Input::KeyName::Gamepad_Special_Left_X:
		return Input::Keys.Gamepad_Special_Left_X;
	case Input::KeyName::Gamepad_Special_Left_Y:
		return Input::Keys.Gamepad_Special_Left_Y;
	case Input::KeyName::Gamepad_Special_Right:
		return Input::Keys.Gamepad_Special_Right;
	case Input::KeyName::Gamepad_FaceButton_Bottom:
		return Input::Keys.Gamepad_FaceButton_Bottom;
	case Input::KeyName::Gamepad_FaceButton_Right:
		return Input::Keys.Gamepad_FaceButton_Right;
	case Input::KeyName::Gamepad_FaceButton_Left:
		return Input::Keys.Gamepad_FaceButton_Left;
	case Input::KeyName::Gamepad_FaceButton_Top:
		return Input::Keys.Gamepad_FaceButton_Top;
	case Input::KeyName::Gamepad_LeftShoulder:
		return Input::Keys.Gamepad_LeftShoulder;
	case Input::KeyName::Gamepad_RightShoulder:
		return Input::Keys.Gamepad_RightShoulder;
	case Input::KeyName::Gamepad_LeftTrigger:
		return Input::Keys.Gamepad_LeftTrigger;
	case Input::KeyName::Gamepad_RightTrigger:
		return Input::Keys.Gamepad_RightTrigger;
	case Input::KeyName::Gamepad_DPad_Up:
		return Input::Keys.Gamepad_DPad_Up;
	case Input::KeyName::Gamepad_DPad_Down:
		return Input::Keys.Gamepad_DPad_Down;
	case Input::KeyName::Gamepad_DPad_Right:
		return Input::Keys.Gamepad_DPad_Right;
	case Input::KeyName::Gamepad_DPad_Left:
		return Input::Keys.Gamepad_DPad_Left;
	case Input::KeyName::Gamepad_LeftStick_Up:
		return Input::Keys.Gamepad_LeftStick_Up;
	case Input::KeyName::Gamepad_LeftStick_Down:
		return Input::Keys.Gamepad_LeftStick_Down;
	case Input::KeyName::Gamepad_LeftStick_Right:
		return Input::Keys.Gamepad_LeftStick_Right;
	case Input::KeyName::Gamepad_LeftStick_Left:
		return Input::Keys.Gamepad_LeftStick_Left;
	case Input::KeyName::Gamepad_RightStick_Up:
		return Input::Keys.Gamepad_RightStick_Up;
	case Input::KeyName::Gamepad_RightStick_Down:
		return Input::Keys.Gamepad_RightStick_Down;
	case Input::KeyName::Gamepad_RightStick_Right:
		return Input::Keys.Gamepad_RightStick_Right;
	case Input::KeyName::Gamepad_RightStick_Left:
		return Input::Keys.Gamepad_RightStick_Left;

	default:
		return Input::Keys.None;
	}
}

SDK::FVector2D Input::GetMousePosition() {
	if (Mouse.FrameUpdated != Game::CurrentFrame) {
		Mouse.FrameUpdated = Game::CurrentFrame;

		float LocationX = 0.f;
		float LocationY = 0.f;

		if (SDK::GetLocalController()->GetMousePosition(&LocationX, &LocationY)) {
			Mouse.Position = SDK::FVector2D(LocationX, LocationY);
		}
	}

	return Mouse.Position;
}

bool Input::IsKeyDown(KeyName Key) {
	KeyData& KeyData = GetKeyData(Key);

	if (KeyData.IsDown.FrameUpdated != Game::CurrentFrame) {
		KeyData.IsDown.FrameUpdated = Game::CurrentFrame;

		SDK::FKey Key{};
		Key.KeyName = KeyData.FName;

		KeyData.IsDown.Value = SDK::GetLocalController()->IsInputKeyDown(Key);
	}

	return KeyData.IsDown.Value;
}
bool Input::WasKeyJustReleased(KeyName Key) {
	KeyData& KeyData = GetKeyData(Key);

	if (KeyData.WasJustReleased.FrameUpdated != Game::CurrentFrame) {
		KeyData.WasJustReleased.FrameUpdated = Game::CurrentFrame;

		SDK::FKey Key{};
		Key.KeyName = KeyData.FName;

		KeyData.WasJustReleased.Value = SDK::GetLocalController()->WasInputKeyJustReleased(Key);
	}

	return KeyData.WasJustReleased.Value;
}
bool Input::WasKeyJustPressed(KeyName Key) {
	KeyData& KeyData = GetKeyData(Key);

	if (KeyData.WasJustPressed.FrameUpdated != Game::CurrentFrame) {
		KeyData.WasJustPressed.FrameUpdated = Game::CurrentFrame;

		SDK::FKey Key{};
		Key.KeyName = KeyData.FName;

		KeyData.WasJustPressed.Value = SDK::GetLocalController()->WasInputKeyJustPressed(Key);
	}

	return KeyData.WasJustPressed.Value;
}

void Input::Init() {
	DEBUG_LOG(skCrypt("Initializing input system").decrypt());

	// Init KeyNames
	{
		Keys.AnyKey.FName = SDK::FName(skCrypt(L"AnyKey").decrypt());
		Keys.AnyKey.Name = skCrypt("Any Key").decrypt();

		Keys.MouseX.FName = SDK::FName(skCrypt(L"MouseX").decrypt());
		Keys.MouseX.Name = skCrypt("Mouse X").decrypt();
		Keys.MouseY.FName = SDK::FName(skCrypt(L"MouseY").decrypt());
		Keys.MouseY.Name = skCrypt("Mouse Y").decrypt();
		Keys.MouseScrollUp.FName = SDK::FName(skCrypt(L"MouseScrollUp").decrypt());
		Keys.MouseScrollUp.Name = skCrypt("Scroll Up").decrypt();
		Keys.MouseScrollDown.FName = SDK::FName(skCrypt(L"MouseScrollDown").decrypt());
		Keys.MouseScrollDown.Name = skCrypt("Scroll Down").decrypt();
		Keys.LeftMouseButton.FName = SDK::FName(skCrypt(L"LeftMouseButton").decrypt());
		Keys.LeftMouseButton.Name = skCrypt("LMB").decrypt();
		Keys.RightMouseButton.FName = SDK::FName(skCrypt(L"RightMouseButton").decrypt());
		Keys.RightMouseButton.Name = skCrypt("RMB").decrypt();
		Keys.MiddleMouseButton.FName = SDK::FName(skCrypt(L"MiddleMouseButton").decrypt());
		Keys.MiddleMouseButton.Name = skCrypt("MMB").decrypt();
		Keys.ThumbMouseButton.FName = SDK::FName(skCrypt(L"ThumbMouseButton").decrypt());
		Keys.ThumbMouseButton.Name = skCrypt("Thumb MB").decrypt();
		Keys.ThumbMouseButton2.FName = SDK::FName(skCrypt(L"ThumbMouseButton2").decrypt());
		Keys.ThumbMouseButton2.Name = skCrypt("Thumb MB2").decrypt();

		Keys.BackSpace.FName = SDK::FName(skCrypt(L"BackSpace").decrypt());
		Keys.BackSpace.Name = skCrypt("BackSpace").decrypt();
		Keys.Tab.FName = SDK::FName(skCrypt(L"Tab").decrypt());
		Keys.Tab.Name = skCrypt("Tab").decrypt();
		Keys.Enter.FName = SDK::FName(skCrypt(L"Enter").decrypt());
		Keys.Enter.Name = skCrypt("Enter").decrypt();
		Keys.Pause.FName = SDK::FName(skCrypt(L"Pause").decrypt());
		Keys.Pause.Name = skCrypt("Pause").decrypt();
		Keys.CapsLock.FName = SDK::FName(skCrypt(L"CapsLock").decrypt());
		Keys.CapsLock.Name = skCrypt("CapsLock").decrypt();
		Keys.Escape.FName = SDK::FName(skCrypt(L"Escape").decrypt());
		Keys.Escape.Name = skCrypt("Escape").decrypt();
		Keys.SpaceBar.FName = SDK::FName(skCrypt(L"SpaceBar").decrypt());
		Keys.SpaceBar.Name = skCrypt("Space").decrypt();
		Keys.PageUp.FName = SDK::FName(skCrypt(L"PageUp").decrypt());
		Keys.PageUp.Name = skCrypt("PageUp").decrypt();
		Keys.PageDown.FName = SDK::FName(skCrypt(L"PageDown").decrypt());
		Keys.PageDown.Name = skCrypt("PageDown").decrypt();
		Keys.End.FName = SDK::FName(skCrypt(L"End").decrypt());
		Keys.End.Name = skCrypt("End").decrypt();
		Keys.Home.FName = SDK::FName(skCrypt(L"Home").decrypt());
		Keys.Home.Name = skCrypt("Home").decrypt();

		Keys.Left.FName = SDK::FName(skCrypt(L"Left").decrypt());
		Keys.Left.Name = skCrypt("Left Arrow").decrypt();
		Keys.Up.FName = SDK::FName(skCrypt(L"Up").decrypt());
		Keys.Up.Name = skCrypt("Up Arrow").decrypt();
		Keys.Right.FName = SDK::FName(skCrypt(L"Right").decrypt());
		Keys.Right.Name = skCrypt("Right Arrow").decrypt();
		Keys.Down.FName = SDK::FName(skCrypt(L"Down").decrypt());
		Keys.Down.Name = skCrypt("Down Arrow").decrypt();

		Keys.Insert.FName = SDK::FName(skCrypt(L"Insert").decrypt());
		Keys.Insert.Name = skCrypt("Insert").decrypt();
		Keys.Delete.FName = SDK::FName(skCrypt(L"Delete").decrypt());
		Keys.Delete.Name = skCrypt("Delete").decrypt();

		Keys.Zero.FName = SDK::FName(skCrypt(L"Zero").decrypt());
		Keys.Zero.Name = skCrypt("0").decrypt();
		Keys.One.FName = SDK::FName(skCrypt(L"One").decrypt());
		Keys.One.Name = skCrypt("1").decrypt();
		Keys.Two.FName = SDK::FName(skCrypt(L"Two").decrypt());
		Keys.Two.Name = skCrypt("2").decrypt();
		Keys.Three.FName = SDK::FName(skCrypt(L"Three").decrypt());
		Keys.Three.Name = skCrypt("3").decrypt();
		Keys.Four.FName = SDK::FName(skCrypt(L"Four").decrypt());
		Keys.Four.Name = skCrypt("4").decrypt();
		Keys.Five.FName = SDK::FName(skCrypt(L"Five").decrypt());
		Keys.Five.Name = skCrypt("5").decrypt();
		Keys.Six.FName = SDK::FName(skCrypt(L"Six").decrypt());
		Keys.Six.Name = skCrypt("6").decrypt();
		Keys.Seven.FName = SDK::FName(skCrypt(L"Seven").decrypt());
		Keys.Seven.Name = skCrypt("7").decrypt();
		Keys.Eight.FName = SDK::FName(skCrypt(L"Eight").decrypt());
		Keys.Eight.Name = skCrypt("8").decrypt();
		Keys.Nine.FName = SDK::FName(skCrypt(L"Nine").decrypt());
		Keys.Nine.Name = skCrypt("9").decrypt();

		Keys.A.FName = SDK::FName(skCrypt(L"A").decrypt());
		Keys.A.Name = skCrypt("A").decrypt();
		Keys.B.FName = SDK::FName(skCrypt(L"B").decrypt());
		Keys.B.Name = skCrypt("B").decrypt();
		Keys.C.FName = SDK::FName(skCrypt(L"C").decrypt());
		Keys.C.Name = skCrypt("C").decrypt();
		Keys.D.FName = SDK::FName(skCrypt(L"D").decrypt());
		Keys.D.Name = skCrypt("D").decrypt();
		Keys.E.FName = SDK::FName(skCrypt(L"E").decrypt());
		Keys.E.Name = skCrypt("E").decrypt();
		Keys.F.FName = SDK::FName(skCrypt(L"F").decrypt());
		Keys.F.Name = skCrypt("F").decrypt();
		Keys.G.FName = SDK::FName(skCrypt(L"G").decrypt());
		Keys.G.Name = skCrypt("G").decrypt();
		Keys.H.FName = SDK::FName(skCrypt(L"H").decrypt());
		Keys.H.Name = skCrypt("H").decrypt();
		Keys.I.FName = SDK::FName(skCrypt(L"I").decrypt());
		Keys.I.Name = skCrypt("I").decrypt();
		Keys.J.FName = SDK::FName(skCrypt(L"J").decrypt());
		Keys.J.Name = skCrypt("J").decrypt();
		Keys.K.FName = SDK::FName(skCrypt(L"K").decrypt());
		Keys.K.Name = skCrypt("K").decrypt();
		Keys.L.FName = SDK::FName(skCrypt(L"L").decrypt());
		Keys.L.Name = skCrypt("L").decrypt();
		Keys.M.FName = SDK::FName(skCrypt(L"M").decrypt());
		Keys.M.Name = skCrypt("M").decrypt();
		Keys.N.FName = SDK::FName(skCrypt(L"N").decrypt());
		Keys.N.Name = skCrypt("N").decrypt();
		Keys.O.FName = SDK::FName(skCrypt(L"O").decrypt());
		Keys.O.Name = skCrypt("O").decrypt();
		Keys.P.FName = SDK::FName(skCrypt(L"P").decrypt());
		Keys.P.Name = skCrypt("P").decrypt();
		Keys.Q.FName = SDK::FName(skCrypt(L"Q").decrypt());
		Keys.Q.Name = skCrypt("Q").decrypt();
		Keys.R.FName = SDK::FName(skCrypt(L"R").decrypt());
		Keys.R.Name = skCrypt("R").decrypt();
		Keys.S.FName = SDK::FName(skCrypt(L"S").decrypt());
		Keys.S.Name = skCrypt("S").decrypt();
		Keys.T.FName = SDK::FName(skCrypt(L"T").decrypt());
		Keys.T.Name = skCrypt("T").decrypt();
		Keys.U.FName = SDK::FName(skCrypt(L"U").decrypt());
		Keys.U.Name = skCrypt("U").decrypt();
		Keys.V.FName = SDK::FName(skCrypt(L"V").decrypt());
		Keys.V.Name = skCrypt("V").decrypt();
		Keys.W.FName = SDK::FName(skCrypt(L"W").decrypt());
		Keys.W.Name = skCrypt("W").decrypt();
		Keys.X.FName = SDK::FName(skCrypt(L"X").decrypt());
		Keys.X.Name = skCrypt("X").decrypt();
		Keys.Y.FName = SDK::FName(skCrypt(L"Y").decrypt());
		Keys.Y.Name = skCrypt("Y").decrypt();
		Keys.Z.FName = SDK::FName(skCrypt(L"Z").decrypt());
		Keys.Z.Name = skCrypt("Z").decrypt();

		Keys.NumPadZero.FName = SDK::FName(skCrypt(L"NumPadZero").decrypt());
		Keys.NumPadZero.Name = skCrypt("Num0").decrypt();
		Keys.NumPadOne.FName = SDK::FName(skCrypt(L"NumPadOne").decrypt());
		Keys.NumPadOne.Name = skCrypt("Num1").decrypt();
		Keys.NumPadTwo.FName = SDK::FName(skCrypt(L"NumPadTwo").decrypt());
		Keys.NumPadTwo.Name = skCrypt("Num2").decrypt();
		Keys.NumPadThree.FName = SDK::FName(skCrypt(L"NumPadThree").decrypt());
		Keys.NumPadThree.Name = skCrypt("Num3").decrypt();
		Keys.NumPadFour.FName = SDK::FName(skCrypt(L"NumPadFour").decrypt());
		Keys.NumPadFour.Name = skCrypt("Num4").decrypt();
		Keys.NumPadFive.FName = SDK::FName(skCrypt(L"NumPadFive").decrypt());
		Keys.NumPadFive.Name = skCrypt("Num5").decrypt();
		Keys.NumPadSix.FName = SDK::FName(skCrypt(L"NumPadSix").decrypt());
		Keys.NumPadSix.Name = skCrypt("Num6").decrypt();
		Keys.NumPadSeven.FName = SDK::FName(skCrypt(L"NumPadSeven").decrypt());
		Keys.NumPadSeven.Name = skCrypt("Num7").decrypt();
		Keys.NumPadEight.FName = SDK::FName(skCrypt(L"NumPadEight").decrypt());
		Keys.NumPadEight.Name = skCrypt("Num8").decrypt();
		Keys.NumPadNine.FName = SDK::FName(skCrypt(L"NumPadNine").decrypt());
		Keys.NumPadNine.Name = skCrypt("Num9").decrypt();

		Keys.Multiply.FName = SDK::FName(skCrypt(L"Multiply").decrypt());
		Keys.Multiply.Name = skCrypt("Multiply").decrypt();
		Keys.Add.FName = SDK::FName(skCrypt(L"Add").decrypt());
		Keys.Add.Name = skCrypt("Add").decrypt();
		Keys.Subtract.FName = SDK::FName(skCrypt(L"Subtract").decrypt());
		Keys.Subtract.Name = skCrypt("Subtract").decrypt();
		Keys.Decimal.FName = SDK::FName(skCrypt(L"Decimal").decrypt());
		Keys.Decimal.Name = skCrypt("Decimal").decrypt();
		Keys.Divide.FName = SDK::FName(skCrypt(L"Divide").decrypt());
		Keys.Divide.Name = skCrypt("Divide").decrypt();

		Keys.F1.FName = SDK::FName(skCrypt(L"F1").decrypt());
		Keys.F1.Name = skCrypt("F1").decrypt();
		Keys.F2.FName = SDK::FName(skCrypt(L"F2").decrypt());
		Keys.F2.Name = skCrypt("F2").decrypt();
		Keys.F3.FName = SDK::FName(skCrypt(L"F3").decrypt());
		Keys.F3.Name = skCrypt("F3").decrypt();
		Keys.F4.FName = SDK::FName(skCrypt(L"F4").decrypt());
		Keys.F4.Name = skCrypt("F4").decrypt();
		Keys.F5.FName = SDK::FName(skCrypt(L"F5").decrypt());
		Keys.F5.Name = skCrypt("F5").decrypt();
		Keys.F6.FName = SDK::FName(skCrypt(L"F6").decrypt());
		Keys.F6.Name = skCrypt("F6").decrypt();
		Keys.F7.FName = SDK::FName(skCrypt(L"F7").decrypt());
		Keys.F7.Name = skCrypt("F7").decrypt();
		Keys.F8.FName = SDK::FName(skCrypt(L"F8").decrypt());
		Keys.F8.Name = skCrypt("F8").decrypt();
		Keys.F9.FName = SDK::FName(skCrypt(L"F9").decrypt());
		Keys.F9.Name = skCrypt("F9").decrypt();
		Keys.F10.FName = SDK::FName(skCrypt(L"F10").decrypt());
		Keys.F10.Name = skCrypt("F10").decrypt();
		Keys.F11.FName = SDK::FName(skCrypt(L"F11").decrypt());
		Keys.F11.Name = skCrypt("F11").decrypt();
		Keys.F12.FName = SDK::FName(skCrypt(L"F12").decrypt());
		Keys.F12.Name = skCrypt("F12").decrypt();

		Keys.NumLock.FName = SDK::FName(skCrypt(L"NumLock").decrypt());
		Keys.NumLock.Name = skCrypt("NumLock").decrypt();
		Keys.ScrollLock.FName = SDK::FName(skCrypt(L"ScrollLock").decrypt());
		Keys.ScrollLock.Name = skCrypt("ScrollLock").decrypt();
		Keys.LeftShift.FName = SDK::FName(skCrypt(L"LeftShift").decrypt());
		Keys.LeftShift.Name = skCrypt("LShift").decrypt();
		Keys.RightShift.FName = SDK::FName(skCrypt(L"RightShift").decrypt());
		Keys.RightShift.Name = skCrypt("RShift").decrypt();
		Keys.LeftControl.FName = SDK::FName(skCrypt(L"LeftControl").decrypt());
		Keys.LeftControl.Name = skCrypt("LCtrl").decrypt();
		Keys.RightControl.FName = SDK::FName(skCrypt(L"RightControl").decrypt());
		Keys.RightControl.Name = skCrypt("RCtrl").decrypt();
		Keys.LeftAlt.FName = SDK::FName(skCrypt(L"LeftAlt").decrypt());
		Keys.LeftAlt.Name = skCrypt("LAlt").decrypt();
		Keys.RightAlt.FName = SDK::FName(skCrypt(L"RightAlt").decrypt());
		Keys.RightAlt.Name = skCrypt("RAlt").decrypt();
		Keys.LeftCommand.FName = SDK::FName(skCrypt(L"LeftCommand").decrypt());
		Keys.LeftCommand.Name = skCrypt("LCmd").decrypt();
		Keys.RightCommand.FName = SDK::FName(skCrypt(L"RightCommand").decrypt());
		Keys.RightCommand.Name = skCrypt("RCmd").decrypt();
		Keys.Semicolon.FName = SDK::FName(skCrypt(L"Semicolon").decrypt());
		Keys.Semicolon.Name = skCrypt("Semicolon").decrypt();
		Keys.Equals.FName = SDK::FName(skCrypt(L"Equals").decrypt());
		Keys.Equals.Name = skCrypt("Equals").decrypt();
		Keys.Comma.FName = SDK::FName(skCrypt(L"Comma").decrypt());
		Keys.Comma.Name = skCrypt("Comma").decrypt();
		Keys.Underscore.FName = SDK::FName(skCrypt(L"Underscore").decrypt());
		Keys.Underscore.Name = skCrypt("Underscore").decrypt();
		Keys.Period.FName = SDK::FName(skCrypt(L"Period").decrypt());
		Keys.Period.Name = skCrypt("Period").decrypt();
		Keys.Slash.FName = SDK::FName(skCrypt(L"Slash").decrypt());
		Keys.Slash.Name = skCrypt("Slash").decrypt();
		Keys.Tilde.FName = SDK::FName(skCrypt(L"Tilde").decrypt());
		Keys.Tilde.Name = skCrypt("Tilde").decrypt();
		Keys.LeftBracket.FName = SDK::FName(skCrypt(L"LeftBracket").decrypt());
		Keys.LeftBracket.Name = skCrypt("LBracket").decrypt();
		Keys.Backslash.FName = SDK::FName(skCrypt(L"Backslash").decrypt());
		Keys.Backslash.Name = skCrypt("Backslash").decrypt();
		Keys.RightBracket.FName = SDK::FName(skCrypt(L"RightBracket").decrypt());
		Keys.RightBracket.Name = skCrypt("RBracket").decrypt();
		Keys.Quote.FName = SDK::FName(skCrypt(L"Quote").decrypt());
		Keys.Quote.Name = skCrypt("Quote").decrypt();
		Keys.Asterix.FName = SDK::FName(skCrypt(L"Asterix").decrypt());
		Keys.Asterix.Name = skCrypt("Asterix").decrypt();
		Keys.Ampersand.FName = SDK::FName(skCrypt(L"Ampersand").decrypt());
		Keys.Ampersand.Name = skCrypt("Ampersand").decrypt();
		Keys.Caret.FName = SDK::FName(skCrypt(L"Caret").decrypt());
		Keys.Caret.Name = skCrypt("Caret").decrypt();
		Keys.Dollar.FName = SDK::FName(skCrypt(L"Dollar").decrypt());
		Keys.Dollar.Name = skCrypt("Dollar").decrypt();
		Keys.Exclamation.FName = SDK::FName(skCrypt(L"Exclamation").decrypt());
		Keys.Exclamation.Name = skCrypt("Exclamation").decrypt();
		Keys.Colon.FName = SDK::FName(skCrypt(L"Colon").decrypt());
		Keys.Colon.Name = skCrypt("Colon").decrypt();

		Keys.A_AccentGrave.FName = SDK::FName(skCrypt(L"A_AccentGrave").decrypt());
		Keys.A_AccentGrave.Name = skCrypt("A_Grave").decrypt();
		Keys.E_AccentGrave.FName = SDK::FName(skCrypt(L"E_AccentGrave").decrypt());
		Keys.E_AccentGrave.Name = skCrypt("E_Grave").decrypt();
		Keys.E_AccentAigu.FName = SDK::FName(skCrypt(L"E_AccentAigu").decrypt());
		Keys.E_AccentAigu.Name = skCrypt("E_Aigu").decrypt();
		Keys.C_Cedille.FName = SDK::FName(skCrypt(L"C_Cedille").decrypt());
		Keys.C_Cedille.Name = skCrypt("C_Cedille").decrypt();

		Keys.Section.FName = SDK::FName(skCrypt(L"Section").decrypt());
		Keys.Section.Name = skCrypt("Section").decrypt();

		Keys.Gamepad_LeftX.FName = SDK::FName(skCrypt(L"Gamepad_LeftX").decrypt());
		Keys.Gamepad_LeftX.Name = skCrypt("Gamepad_LeftX").decrypt();
		Keys.Gamepad_LeftY.FName = SDK::FName(skCrypt(L"Gamepad_LeftY").decrypt());
		Keys.Gamepad_LeftY.Name = skCrypt("Gamepad_LeftY").decrypt();
		Keys.Gamepad_RightX.FName = SDK::FName(skCrypt(L"Gamepad_RightX").decrypt());
		Keys.Gamepad_RightX.Name = skCrypt("Gamepad_RightX").decrypt();
		Keys.Gamepad_RightY.FName = SDK::FName(skCrypt(L"Gamepad_RightY").decrypt());
		Keys.Gamepad_RightY.Name = skCrypt("Gamepad_RightY").decrypt();
		Keys.Gamepad_LeftTriggerAxis.FName = SDK::FName(skCrypt(L"Gamepad_LeftTriggerAxis").decrypt());
		Keys.Gamepad_LeftTriggerAxis.Name = skCrypt("Gamepad_LeftTriggerAxis").decrypt();
		Keys.Gamepad_RightTriggerAxis.FName = SDK::FName(skCrypt(L"Gamepad_RightTriggerAxis").decrypt());
		Keys.Gamepad_RightTriggerAxis.Name = skCrypt("Gamepad_RightTriggerAxis").decrypt();
		Keys.Gamepad_LeftThumbstick.FName = SDK::FName(skCrypt(L"Gamepad_LeftThumbstick").decrypt());
		Keys.Gamepad_LeftThumbstick.Name = skCrypt("Gamepad_LeftThumbstick").decrypt();
		Keys.Gamepad_RightThumbstick.FName = SDK::FName(skCrypt(L"Gamepad_RightThumbstick").decrypt());
		Keys.Gamepad_RightThumbstick.Name = skCrypt("Gamepad_RightThumbstick").decrypt();
		Keys.Gamepad_Special_Left.FName = SDK::FName(skCrypt(L"Gamepad_Special_Left").decrypt());
		Keys.Gamepad_Special_Left.Name = skCrypt("Gamepad_Special_Left").decrypt();
		Keys.Gamepad_Special_Left_X.FName = SDK::FName(skCrypt(L"Gamepad_Special_Left_X").decrypt());
		Keys.Gamepad_Special_Left_X.Name = skCrypt("Gamepad_Special_Left_X").decrypt();
		Keys.Gamepad_Special_Left_Y.FName = SDK::FName(skCrypt(L"Gamepad_Special_Left_Y").decrypt());
		Keys.Gamepad_Special_Left_Y.Name = skCrypt("Gamepad_Special_Left_Y").decrypt();
		Keys.Gamepad_Special_Right.FName = SDK::FName(skCrypt(L"Gamepad_Special_Right").decrypt());
		Keys.Gamepad_Special_Right.Name = skCrypt("Gamepad_Special_Right").decrypt();
		Keys.Gamepad_FaceButton_Bottom.FName = SDK::FName(skCrypt(L"Gamepad_FaceButton_Bottom").decrypt());
		Keys.Gamepad_FaceButton_Bottom.Name = skCrypt("Gamepad_FaceButton_Bottom").decrypt();
		Keys.Gamepad_FaceButton_Right.FName = SDK::FName(skCrypt(L"Gamepad_FaceButton_Right").decrypt());
		Keys.Gamepad_FaceButton_Right.Name = skCrypt("Gamepad_FaceButton_Right").decrypt();
		Keys.Gamepad_FaceButton_Left.FName = SDK::FName(skCrypt(L"Gamepad_FaceButton_Left").decrypt());
		Keys.Gamepad_FaceButton_Left.Name = skCrypt("Gamepad_FaceButton_Left").decrypt();
		Keys.Gamepad_FaceButton_Top.FName = SDK::FName(skCrypt(L"Gamepad_FaceButton_Top").decrypt());
		Keys.Gamepad_FaceButton_Top.Name = skCrypt("Gamepad_FaceButton_Top").decrypt();
		Keys.Gamepad_LeftShoulder.FName = SDK::FName(skCrypt(L"Gamepad_LeftShoulder").decrypt());
		Keys.Gamepad_LeftShoulder.Name = skCrypt("Gamepad_LeftShoulder").decrypt();
		Keys.Gamepad_RightShoulder.FName = SDK::FName(skCrypt(L"Gamepad_RightShoulder").decrypt());
		Keys.Gamepad_RightShoulder.Name = skCrypt("Gamepad_RightShoulder").decrypt();
		Keys.Gamepad_LeftTrigger.FName = SDK::FName(skCrypt(L"Gamepad_LeftTrigger").decrypt());
		Keys.Gamepad_LeftTrigger.Name = skCrypt("Gamepad_LeftTrigger").decrypt();
		Keys.Gamepad_RightTrigger.FName = SDK::FName(skCrypt(L"Gamepad_RightTrigger").decrypt());
		Keys.Gamepad_RightTrigger.Name = skCrypt("Gamepad_RightTrigger").decrypt();
		Keys.Gamepad_DPad_Up.FName = SDK::FName(skCrypt(L"Gamepad_DPad_Up").decrypt());
		Keys.Gamepad_DPad_Up.Name = skCrypt("Gamepad_DPad_Up").decrypt();
		Keys.Gamepad_DPad_Down.FName = SDK::FName(skCrypt(L"Gamepad_DPad_Down").decrypt());
		Keys.Gamepad_DPad_Down.Name = skCrypt("Gamepad_DPad_Down").decrypt();
		Keys.Gamepad_DPad_Right.FName = SDK::FName(skCrypt(L"Gamepad_DPad_Right").decrypt());
		Keys.Gamepad_DPad_Right.Name = skCrypt("Gamepad_DPad_Right").decrypt();
		Keys.Gamepad_DPad_Left.FName = SDK::FName(skCrypt(L"Gamepad_DPad_Left").decrypt());
		Keys.Gamepad_DPad_Left.Name = skCrypt("Gamepad_DPad_Left").decrypt();
		Keys.Gamepad_LeftStick_Up.FName = SDK::FName(skCrypt(L"Gamepad_LeftStick_Up").decrypt());
		Keys.Gamepad_LeftStick_Up.Name = skCrypt("Gamepad_LeftStick_Up").decrypt();
		Keys.Gamepad_LeftStick_Down.FName = SDK::FName(skCrypt(L"Gamepad_LeftStick_Down").decrypt());
		Keys.Gamepad_LeftStick_Down.Name = skCrypt("Gamepad_LeftStick_Down").decrypt();
		Keys.Gamepad_LeftStick_Right.FName = SDK::FName(skCrypt(L"Gamepad_LeftStick_Right").decrypt());
		Keys.Gamepad_LeftStick_Right.Name = skCrypt("Gamepad_LeftStick_Right").decrypt();
		Keys.Gamepad_LeftStick_Left.FName = SDK::FName(skCrypt(L"Gamepad_LeftStick_Left").decrypt());
		Keys.Gamepad_LeftStick_Left.Name = skCrypt("Gamepad_LeftStick_Left").decrypt();
		Keys.Gamepad_RightStick_Up.FName = SDK::FName(skCrypt(L"Gamepad_RightStick_Up").decrypt());
		Keys.Gamepad_RightStick_Up.Name = skCrypt("Gamepad_RightStick_Up").decrypt();
		Keys.Gamepad_RightStick_Down.FName = SDK::FName(skCrypt(L"Gamepad_RightStick_Down").decrypt());
		Keys.Gamepad_RightStick_Down.Name = skCrypt("Gamepad_RightStick_Down").decrypt();
		Keys.Gamepad_RightStick_Right.FName = SDK::FName(skCrypt(L"Gamepad_RightStick_Right").decrypt());
		Keys.Gamepad_RightStick_Right.Name = skCrypt("Gamepad_RightStick_Right").decrypt();
		Keys.Gamepad_RightStick_Left.FName = SDK::FName(skCrypt(L"Gamepad_RightStick_Left").decrypt());
		Keys.Gamepad_RightStick_Left.Name = skCrypt("Gamepad_RightStick_Left").decrypt();
	}

	DEBUG_LOG(skCrypt("Input system initialized!").decrypt());
}