#include "InputManger.h"
#include <DxLib.h>

bool InputManger::Escape()
{
	return Input::isKey(KEY_INPUT_ESCAPE);
}

bool InputManger::EscapeTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_ESCAPE);
}

bool InputManger::EscapeReturn()
{
	return Input::isKeyReturn(KEY_INPUT_ESCAPE);
}

bool InputManger::Up()
{
	return Input::isKey(KEY_INPUT_W);
}

bool InputManger::UpTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_W);
}

bool InputManger::UpReturn()
{
	return Input::isKeyReturn(KEY_INPUT_W);
}

bool InputManger::Down()
{
	return Input::isKey(KEY_INPUT_S);
}

bool InputManger::DownTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_S);
}

bool InputManger::DownReturn()
{
	return Input::isKeyReturn(KEY_INPUT_S);
}

bool InputManger::Left()
{
	return Input::isKey(KEY_INPUT_A);
}

bool InputManger::LeftTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_A);
}

bool InputManger::LeftReturn()
{
	return Input::isKeyReturn(KEY_INPUT_A);
}

bool InputManger::Right()
{
	return Input::isKey(KEY_INPUT_D);
}

bool InputManger::RightTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_D);
}

bool InputManger::RightReturn()
{
	return Input::isKeyReturn(KEY_INPUT_D);
}

bool InputManger::SubUp()
{
	return Input::isKey(KEY_INPUT_UP);
}

bool InputManger::SubUpTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_UP);
}

bool InputManger::SubUpReturn()
{
	return Input::isKeyReturn(KEY_INPUT_UP);
}

bool InputManger::SubDown()
{
	return Input::isKey(KEY_INPUT_DOWN);
}

bool InputManger::SubDownTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_DOWN);
}

bool InputManger::SubDownReturn()
{
	return Input::isKeyReturn(KEY_INPUT_DOWN);
}

bool InputManger::SubLeft()
{
	return Input::isKey(KEY_INPUT_LEFT);
}

bool InputManger::SubLeftTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_LEFT);
}

bool InputManger::SubLeftReturn()
{
	return Input::isKeyReturn(KEY_INPUT_LEFT);
}

bool InputManger::SubRight()
{
	return Input::isKey(KEY_INPUT_RIGHT);
}

bool InputManger::SubRightTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_RIGHT);
}

bool InputManger::SubRightReturn()
{
	return Input::isKeyReturn(KEY_INPUT_RIGHT);
}

bool InputManger::Reset()
{
	return Input::isKey(KEY_INPUT_R);
}

bool InputManger::ResetTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_R);
}

bool InputManger::ResetReturn()
{
	return Input::isKeyReturn(KEY_INPUT_R);
}

bool InputManger::Act1()
{
	return Input::isKey(KEY_INPUT_SPACE);
}

bool InputManger::Act1Trigger()
{
	return Input::isKeyTrigger(KEY_INPUT_SPACE);
}

bool InputManger::Act1Return()
{
	return Input::isKeyReturn(KEY_INPUT_SPACE);
}
