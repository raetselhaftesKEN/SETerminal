/**
* @file SettingLayer.cpp
*/
#include "MenuLayer.h"
#include "ui/UIButton.h"

#define BOARD_IMAGE_WIDTH 700
#define BOARD_IMAGE_HEIGHT 500
#define STANDARD_LEFT 125
#define CLOSE_X 10000
#define CLOSE_Y 10000

/**
 @brief  print file loading error
 @author Cocos
 */
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
}


bool MenuLayer::open()
{
	cocos2d::Director::getInstance()->getOpenGLView()->setCursorVisible(true);
	//为了在Monster类内使用外部的东西，使用以下几句
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
	auto contenteSize = runningScene->getContentSize();

	pauseBoardImg_->setPosition(contenteSize.width / 2, contenteSize.height / 2);
	pauseBoardImg_->setCameraMask(2, true);

	isOpen = true;
	return true;
}

cocos2d::ui::Button* MenuLayer::settingSmallButton(float deviationX, float deviationY, std::string spriteName, std::string texts)
{
	cocos2d::ui::Button* smallButton = new  cocos2d::ui::Button;
	smallButton = cocos2d::ui::Button::create("Setting/btn_default.png", "Setting/btn_default_pressed.png");
	auto Image = cocos2d::Sprite::create(spriteName);
	smallButton->setPosition(cocos2d::Vec2(STANDARD_LEFT + deviationX, BOARD_IMAGE_HEIGHT - deviationY));
	Image->setPosition(cocos2d::Vec2(70, 40));
	Image->setScale(0.5f, 0.5f);
	smallButton->setScale9Enabled(true);
	smallButton->setContentSize(cocos2d::Size(140, 80));
	pauseBoardImg_->addChild(smallButton, 3);
	smallButton->addChild(Image, 2);
	auto label = cocos2d::Label::createWithTTF(texts, "fonts/Marker Felt.ttf", 30);
	label->setPosition(cocos2d::Vec2(55, -40));
	smallButton->addChild(label, 2);

	return smallButton;
}