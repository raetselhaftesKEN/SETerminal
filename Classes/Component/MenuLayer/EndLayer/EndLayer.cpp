/**
* @file SettingLayer.cpp
*/
#include "EndLayer.h"
#include "ui/UIButton.h"
#include "Scene/FightScene/FightScene.h"

#define BOARD_IMAGE_WIDTH 700
#define BOARD_IMAGE_HEIGHT 500
#define STANDARD_LEFT 125
#define CLOSE_X 10000
#define CLOSE_Y 10000

EndLayer* EndLayer::create()
{
	auto endLayer = new(std::nothrow) EndLayer();

	if (endLayer && endLayer->init())
	{
		
		endLayer->autorelease();

		return endLayer;
	}
	delete endLayer;
	endLayer = nullptr;
	return nullptr;

}

/**
 @brief  print file loading error
 @author Cocos
 */
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
}


bool EndLayer::init()
{
	this->setPosition(cocos2d::Vec2(CLOSE_X, CLOSE_Y));

	finalRank_ = 20;
	pauseBoardImg_ = cocos2d::ui::Scale9Sprite::create("Setting/EndMenuWin.png");

	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	endPrompt1_ = cocos2d::Label::createWithTTF(std::string(""), "fonts/IRANYekanBold.ttf", 100);
	endPrompt2_ = cocos2d::Label::createWithTTF(std::string(""), "fonts/IRANYekanBold.ttf", 72);
	endPrompt1_->setColor(cocos2d::Color3B::YELLOW);
	endPrompt1_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_RIGHT);
	endPrompt2_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
	endPrompt1_->setPosition(cocos2d::Vec2(winSize.width / 2, 5 * winSize.height / 6));
	endPrompt2_->setPosition(cocos2d::Vec2(winSize.width / 2, 5 * winSize.height / 6));
	pauseBoardImg_->addChild(endPrompt1_, 3);
	pauseBoardImg_->addChild(endPrompt2_, 3);

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	if (pauseBoardImg_ == nullptr)
	{
		problemLoading("'pause_board.png'");
		return false;
	}
	else
	{

		pauseBoardImg_->setPosition(cocos2d::Vec2(CLOSE_X, CLOSE_Y));
		this->addChild(pauseBoardImg_, 0);
	}

	return true;
}

bool EndLayer::open(int finalRank)
{
	finalRank_ = finalRank;
	cocos2d::Director::getInstance()->getOpenGLView()->setCursorVisible(true);
	//为了在Monster类内使用外部的东西，使用以下几句
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
	auto contenteSize = runningScene->getContentSize();

	if (finalRank != 1)
	{
		pauseBoardImg_->setTexture("Setting/EndMenuLose.png");
	}
	endPrompt1_->setString(std::to_string(finalRank) + "/");
	endPrompt2_->setString(std::to_string(dynamic_cast<FightScene*>(runningScene)->MonsterToSpawn));

	pauseBoardImg_->setPosition(contenteSize.width / 2, contenteSize.height / 2);
	pauseBoardImg_->setCameraMask(2, true);

	isOpen = true;
	return true;
}