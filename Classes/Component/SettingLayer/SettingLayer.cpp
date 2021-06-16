/**
* @file SettingLayer.cpp
*/
#include "SettingLayer.h"
#include "ui/UIButton.h"

#define BOARD_IMAGE_WIDTH 700
#define BOARD_IMAGE_HEIGHT 500
#define STANDARD_LEFT 125
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
		//pauseBoardImg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		pauseBoardImg_->setPosition(cocos2d::Vec2(CLOSE_X,CLOSE_Y));
		this->addChild(pauseBoardImg_, 0);
	}

	//设置关闭按钮
	closeButton_ = cocos2d::ui::Button::create("Setting/close.png", "Setting/close_pressed.png");
	auto closeButtonSize = closeButton_->getContentSize();
	closeButton_->setPosition(cocos2d::Vec2((BOARD_IMAGE_WIDTH - closeButtonSize.width / 2), (BOARD_IMAGE_HEIGHT - closeButtonSize.height / 2)));
	pauseBoardImg_->addChild(closeButton_);

	////设置背景音乐部分
	//musicButton_ = cocos2d::ui::Button::create("btn_default.png", "btn_default_pressed.png");
	//auto musicImage = cocos2d::Sprite::create("music.png");
	//musicButton_->setPosition(cocos2d::Vec2(STANDARD_LEFT + 5, BOARD_IMAGE_HEIGHT - 100));
	//musicImage->setPosition(cocos2d::Vec2(55, 50));
	//musicButton_->setScale9Enabled(true);
	//musicButton_->setContentSize(cocos2d::Size(110, 100));
	//pauseBoardImg_->addChild(musicButton_, 3);
	//musicButton_->addChild(musicImage, 2);
	//musicLabel_ = cocos2d::Label::createWithTTF("Music", "fonts/Marker Felt.ttf", 36);
	//musicLabel_->setPosition(cocos2d::Vec2(55,-40));
	//musicButton_->addChild(musicLabel_, 2);

	////设置锁血功能
	//superBodyButton_ = cocos2d::ui::Button::create("btn_default.png", "btn_default_pressed.png");
	//superBodyButton_->setPosition(cocos2d::Vec2(STANDARD_LEFT + 205, BOARD_IMAGE_HEIGHT - 100));
	//superBodyButton_->setScale9Enabled(true);
	//superBodyButton_->setContentSize(cocos2d::Size(110, 100));
	//pauseBoardImg_->addChild(superBodyButton_);
	//superBodyLabel_ = cocos2d::Label::createWithTTF("Super body", "fonts/Marker Felt.ttf", 36);
	//superBodyLabel_->setPosition(cocos2d::Vec2(55, -40));
	//superBodyButton_->addChild(superBodyLabel_, 2);

	////设置无后坐力功能
	//superAccuracyButton_ = cocos2d::ui::Button::create("btn_default.png", "btn_default_pressed.png");
	//superAccuracyButton_->setPosition(cocos2d::Vec2(STANDARD_LEFT + 405, BOARD_IMAGE_HEIGHT - 100));
	//superAccuracyButton_->setScale9Enabled(true);
	//superAccuracyButton_->setContentSize(cocos2d::Size(110, 100));
	//pauseBoardImg_->addChild(superAccuracyButton_);
	//superAccuracyLabel_ = cocos2d::Label::createWithTTF("No accuracy", "fonts/Marker Felt.ttf", 36);
	//superAccuracyLabel_->setPosition(cocos2d::Vec2(55, -40));
	//superAccuracyButton_->addChild(superAccuracyLabel_, 2);
	//
	////设置无限子弹功能
	//superBulletButton_ = cocos2d::ui::Button::create("btn_default.png", "btn_default_pressed.png");
	//superBulletButton_->setPosition(cocos2d::Vec2(STANDARD_LEFT + 5, BOARD_IMAGE_HEIGHT - 350));
	//superBulletButton_->setScale9Enabled(true);
	//superBulletButton_->setContentSize(cocos2d::Size(110, 100));
	//pauseBoardImg_->addChild(superBulletButton_);
	//superBulletLabel_ = cocos2d::Label::createWithTTF("No accuracy", "fonts/Marker Felt.ttf", 36);
	//superBulletLabel_->setPosition(cocos2d::Vec2(55, -40));
	//superBulletButton_->addChild(superBulletLabel_, 2);

	musicButton_ = settingSmallButton(5, 100, "Setting/Music.png", "Music");
	shortMusicButton_ = settingSmallButton(205, 100, "Setting/short_music.png", "Shoot Music");
	superBodyButton_ = settingSmallButton(405, 100, "Setting/Music.png", "Super body");
	superAccuracyButton_ = settingSmallButton(5, 350, "Setting/Music.png", "Accuracy");
	superBulletButton_ = settingSmallButton(205, 350, "Setting/Music.png", "Bullet");
	superDamageButton_ = settingSmallButton(405, 350, "Setting/Music.png", "Damage");

	return true;
}

bool SettingLayer::open()
{
	//为了在Monster类内使用外部的东西，使用以下几句
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
	auto contenteSize = runningScene->getContentSize();
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
	pauseBoardImg_->setPosition(contenteSize.width / 2, contenteSize.height / 2);
	pauseBoardImg_->setCameraMask(2, true);

	closeButton_->addClickEventListener([=](Ref*) {
		this->close();
	});

	musicButton_->addClickEventListener([&](Ref*) {
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

	shortMusicButton_->addClickEventListener([&](Ref*) {
		
	});

	superBodyButton_->addClickEventListener([=](Ref*) {
		if (!playerOfPlayer->settingSuperBody_)
		{
			playerOfPlayer->settingSuperBody_ = true;
		}
		else
		{
			playerOfPlayer->settingSuperBody_ = false;
		}
	});

	superAccuracyButton_->addClickEventListener([=](Ref*) {
		
	});

	isOpen = true;
	return true;
}

bool SettingLayer::close()
{
	pauseBoardImg_->setPosition(10000, 10000);
	pauseBoardImg_->setCameraMask(2, true);
	isOpen = false;
	return true;
}

cocos2d::ui::Button* SettingLayer::settingSmallButton(float deviationX, float deviationY, std::string spriteName, std::string texts)
{
	cocos2d::ui::Button* smallButton = new  cocos2d::ui::Button;
	smallButton = cocos2d::ui::Button::create("Setting/btn_default.png", "Setting/btn_default_pressed.png");
	auto Image = cocos2d::Sprite::create(spriteName);
	smallButton->setPosition(cocos2d::Vec2(STANDARD_LEFT + deviationX, BOARD_IMAGE_HEIGHT - deviationY));
	Image->setPosition(cocos2d::Vec2(55, 50));
	smallButton->setScale9Enabled(true);
	smallButton->setContentSize(cocos2d::Size(110, 100));
	pauseBoardImg_->addChild(smallButton, 3);
	smallButton->addChild(Image, 2);
	auto label = cocos2d::Label::createWithTTF(texts, "fonts/Marker Felt.ttf", 30);
	label->setPosition(cocos2d::Vec2(55, -40));
	smallButton->addChild(label, 2);

	return smallButton;
}