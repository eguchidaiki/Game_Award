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

	count = 0;
}

void UI_Button::Update()
{

	//押されたか
	if (isUserPushed) {
		count++;
		if (count > activateTimerag) {
			isFunctionActivate = true;
		}
		
		
		if (count < activateTimerag / 8) {
			float rate = static_cast<float>(count) / static_cast<float>(activateTimerag / 8);
			easeScaleX = EASE_SCALE_START_X + (EASE_SCALE_X - EASE_SCALE_START_X) * rate;
			easeSceleY = EASE_SCALE_START_X + (EASE_SCALE_X - EASE_SCALE_START_X) * rate;

		}
		else if(count < activateTimerag / 4) {
			float rate = static_cast<float>(count - activateTimerag / 8) / static_cast<float>((activateTimerag / 4) - (activateTimerag / 8));
			easeScaleX = EASE_SCALE_X + (EASE_SCALE_START_X - EASE_SCALE_X) * rate;
			easeSceleY = EASE_SCALE_X + (EASE_SCALE_START_X - EASE_SCALE_X) * rate;
		}

	}
	//押されていないか
	else {

	}
}

void UI_Button::Draw(float centerX, float centerY, float x_scale, float y_scale)
{
	uiSprite.DrawExtendSprite(centerX - width * easeScaleX * x_scale, centerY - height * easeSceleY * y_scale,
		centerX + width * easeScaleX * x_scale, centerY + height * easeSceleY * y_scale);
	uiSprite.Draw();
}

void UI_Button::UI_Push()
{
	isUserPushed = true;
}