/**
* @file SettingLayer.cpp
*/
#include "SettingLayer.h"
#include "ui/UIButton.h"

#define BOARD_IMAGE_WIDTH 700
#define BOARD_IMAGE_HEIGHT 500
#define STANDARD_LEFT 100
#define CLOSE_X 10000
#define CLOSE_Y 10000

SettingLayer* SettingLayer::create()
{
	auto settingLayer = new(std::nothrow) SettingLayer();
	/*if (settingLayer == nullptr)
	{
		return nullptr;
	}*/

	if (settingLayer && settingLayer->init())
	{
		
		settingLayer->autorelease();

		return settingLayer;
	}
	delete settingLayer;
	settingLayer = nullptr;
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

void SettingLayer::initMusic()
{
	backgroundMusicID_ = cocos2d::AudioEngine::play2d("Audio/bgm_1Low.mp3", true, .5);
	isBackgroundMusicPlaying_ = true;
}


bool SettingLayer::init()
{
	


	this->setPosition(cocos2d::Vec2(CLOSE_X, CLOSE_Y));
	
	backgroundMusicID_ = cocos2d::AudioEngine::play2d("Audio/bgm_1Low.mp3", true, .5);
	isBackgroundMusicPlaying_ = true;

	// 设置背景框
	pauseBoardImg_ = cocos2d::ui::Scale9Sprite::create("Setting/pause_board.png");
	pauseBoardImg_->setOpacity(64);
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
		//pauseBoardImg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		pauseBoardImg_->setPosition(cocos2d::Vec2(CLOSE_X,CLOSE_Y));
		this->addChild(pauseBoardImg_, 0);
	}

	//设置关闭按钮
	auto closeButton = cocos2d::ui::Button::create("close.png", "close_pressed.png");
	auto closeButtonSize = closeButton->getContentSize();
	closeButton->setPosition(cocos2d::Vec2((BOARD_IMAGE_WIDTH - closeButtonSize.width / 2), (BOARD_IMAGE_HEIGHT - closeButtonSize.height / 2)));
	pauseBoardImg_->addChild(closeButton);

	//设置背景音乐部分
	auto musicButton = cocos2d::ui::Button::create("btn_default.png", "btn_default_pressed.png");
	auto musicImage = cocos2d::Sprite::create("music.png");
	musicButton->setPosition(cocos2d::Vec2(STANDARD_LEFT + 5, BOARD_IMAGE_HEIGHT - 100));
	musicImage->setPosition(cocos2d::Vec2(STANDARD_LEFT, BOARD_IMAGE_HEIGHT - 100));
	musicButton->setScale9Enabled(true);
	musicButton->setContentSize(cocos2d::Size(110, 100));
	pauseBoardImg_->addChild(musicButton, 3);
	pauseBoardImg_->addChild(musicImage, 4);

	//设置锁血功能
	superBodyButton_ = cocos2d::ui::Button::create("btn_default.png", "btn_default_pressed.png");
	superBodyButton_->setPosition(cocos2d::Vec2(STANDARD_LEFT + 175, BOARD_IMAGE_HEIGHT - 100));
	superBodyButton_->setScale9Enabled(true);
	superBodyButton_->setContentSize(cocos2d::Size(110, 100));
	pauseBoardImg_->addChild(superBodyButton_);

	closeButton->addClickEventListener([=](Ref*) {
		this->setPosition(cocos2d::Vec2(10000, 10000));
	});

	musicButton->addClickEventListener([&](Ref*) {
		if (!isBackgroundMusicPlaying_)
		{
			cocos2d::AudioEngine::resume(backgroundMusicID_);
			isBackgroundMusicPlaying_ = true;
		}
		else
		{
			cocos2d::AudioEngine::pause(backgroundMusicID_);
			isBackgroundMusicPlaying_ = false;
		}
	});

	return true;
}

bool SettingLayer::open()
{
	//为了在Monster类内使用外部的东西，使用以下几句
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
	auto playerOfNode = runningScene->getChildByTag(PLAYER_TAG);
	cocos2d::Vec2 playerPositionInScene = cocos2d::Vec2::ZERO;
	//如果场景已经被释放，找不到我方player位置，直接退出
	if (playerOfNode == nullptr)
	{
		return false;
	}
	//获得当前我方player位置
	auto playerOfPlayer = dynamic_cast<Player*>(playerOfNode);
	playerPositionInScene = playerOfPlayer->getPosition();
	pauseBoardImg_->setPosition(playerPositionInScene);

	superBodyButton_->addClickEventListener([=](Ref*) {
		if (!playerOfPlayer->superBody_)
		{
			playerOfPlayer->superBody_ = true;
		}
		else
		{
			playerOfPlayer->superBody_ = false;
		}
	});



	return true;
}