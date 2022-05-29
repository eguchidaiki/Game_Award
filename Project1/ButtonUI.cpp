#include "ButtonUI.h"

void UI_Button::Init(int activateTimerag, UINT graphHandle, float baseWidth, float baseHeight)
{
	this->activateTimerag = activateTimerag;
	uiSprite.Create(graphHandle);
	width = baseWidth / 2.0f;
	height = baseHeight / 2.0f;

	easeScaleX = 1.0f;
	easeSceleY = 1.0f;

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
		float rate = static_cast<float>(count) / static_cast<float>(activateTimerag / 2);
		
		if (rate < 1.0f) {

		}
	}
	//‰Ÿ‚³‚ê‚Ä‚¢‚È‚¢‚©
	else {

	}
}

void UI_Button::Draw(float centerX, float centerY, float x_scale, float y_scale)
{
	uiSprite.DrawExtendSprite(centerX - width * easeScaleX, centerY - height * easeSceleY,
		centerX + width * easeScaleX, centerY + height * easeSceleY);
	uiSprite.Draw();
}

void UI_Button::UI_Push()
{
	isUserPushed = true;
}