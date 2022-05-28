#include "ButtonUI.h"

void UI_Button::Init(int activateTimerag, UINT graphHandle, float baseWidth, float baseHeight)
{
	this->activateTimerag = activateTimerag;
	uiSprite.Create(graphHandle);
	width = baseWidth / 2.0f;
	height = baseHeight / 2.0f;

	isUserPushed = false;
	isFunctionActivate = false;
}

void UI_Button::Update()
{

	//‰Ÿ‚³‚ê‚½‚©
	if (isUserPushed) {
		count++;
		if (count > activateTimerag) {
			isFunctionActivate = true;
		}
		float rate = static_cast<float>(count) / static_cast<float>(activateTimerag);
		

	}
	//‰Ÿ‚³‚ê‚Ä‚¢‚È‚¢‚©
	else {

	}
}

void UI_Button::Draw(float centerX, float centerY, float x_scale, float y_scale)
{
	uiSprite.DrawExtendSprite(centerX - width * x_scale, centerY - height * y_scale,
		centerX + width * x_scale, centerY + height * y_scale);
	uiSprite.Draw();
}

void UI_Button::UI_Push()
{
	isUserPushed = true;
}