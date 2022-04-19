#include "InputManger.h"

bool InputManger::Escape()
{
	return Input::isKey(DIK_ESCAPE);
}

bool InputManger::EscapeTrigger()
{
	return Input::isKeyTrigger(DIK_ESCAPE);
}

bool InputManger::EscapeReturn()
{
	return Input::isKeyReleased(DIK_ESCAPE);
}

bool InputManger::Up()
{
	return Input::isKey(DIK_W);
}

bool InputManger::UpTrigger()
{
	return Input::isKeyTrigger(DIK_W);
}

bool InputManger::UpReturn()
{
	return Input::isKeyReleased(DIK_W);
}

bool InputManger::Down()
{
	return Input::isKey(DIK_S);
}

bool InputManger::DownTrigger()
{
	return Input::isKeyTrigger(DIK_S);
}

bool InputManger::DownReturn()
{
	return Input::isKeyReleased(DIK_S);
}

bool InputManger::Left()
{
	return Input::isKey(DIK_A);
}

bool InputManger::LeftTrigger()
{
	return Input::isKeyTrigger(DIK_A);
}

bool InputManger::LeftReturn()
{
	return Input::isKeyReleased(DIK_A);
}

bool InputManger::Right()
{
	return Input::isKey(DIK_D);
}

bool InputManger::RightTrigger()
{
	return Input::isKeyTrigger(DIK_D);
}

bool InputManger::RightReturn()
{
	return Input::isKeyReleased(DIK_D);
}

bool InputManger::SubUp()
{
	return Input::isKey(DIK_UP);
}

bool InputManger::SubUpTrigger()
{
	return Input::isKeyTrigger(DIK_UP);
}

bool InputManger::SubUpReturn()
{
	return Input::isKeyReleased(DIK_UP);
}

bool InputManger::SubDown()
{
	return Input::isKey(DIK_DOWN);
}

bool InputManger::SubDownTrigger()
{
	return Input::isKeyTrigger(DIK_DOWN);
}

bool InputManger::SubDownReturn()
{
	return Input::isKeyReleased(DIK_DOWN);
}

bool InputManger::SubLeft()
{
	return Input::isKey(DIK_LEFT);
}

bool InputManger::SubLeftTrigger()
{
	return Input::isKeyTrigger(DIK_LEFT);
}

bool InputManger::SubLeftReturn()
{
	return Input::isKeyReleased(DIK_LEFT);
}

bool InputManger::SubRight()
{
	return Input::isKey(DIK_RIGHT);
}

bool InputManger::SubRightTrigger()
{
	return Input::isKeyTrigger(DIK_RIGHT);
}

bool InputManger::SubRightReturn()
{
	return Input::isKeyReleased(DIK_RIGHT);
}

bool InputManger::Reset()
{
	return Input::isKey(DIK_R);
}

bool InputManger::ResetTrigger()
{
	return Input::isKeyTrigger(DIK_R);
}

bool InputManger::ResetReturn()
{
	return Input::isKeyReleased(DIK_R);
}

bool InputManger::Act1()
{
	return Input::isKey(DIK_SPACE);
}

bool InputManger::Act1Trigger()
{
	return Input::isKeyTrigger(DIK_SPACE);
}

bool InputManger::Act1Return()
{
	return Input::isKeyReleased(DIK_SPACE);
}
