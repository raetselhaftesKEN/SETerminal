/**
* @file SettingLayer.cpp
*/
#include "EndLayer.h"
#include "ui/UIButton.h"

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

	// 设置背景框
	finalRank_ = 20;
	pauseBoardImg_ = cocos2d::ui::Scale9Sprite::create("Setting/pause_board.png");
	pauseBoardImg_->setOpacity(96);
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	if (pauseBoardImg_ == nullptr)
	{
		problemLoading("'pause_board.png'");
		return false;
	}
	else
	{
		pauseBoardImg_->setCapInsets(cocos2d::Rect(6, 6, 79, 61));
		pauseBoardImg_->setContentSize(cocos2d::Size(BOARD_IMAGE_WIDTH, BOARD_IMAGE_HEIGHT));

		pauseBoardImg_->setPosition(cocos2d::Vec2(CLOSE_X,CLOSE_Y));
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

	pauseBoardImg_->setPosition(contenteSize.width / 2, contenteSize.height / 2);
	pauseBoardImg_->setCameraMask(2, true);

	cocos2d::log((std::string("final rank") + (std::to_string(finalRank))).c_str());

	isOpen = true;
	return true;
}

