/**
* @file StartMenuScene.cpp
*/

#include "cocos2d.h"
#include "StartMenuScene.h"
#include "Client/Client.h"
#include "AudioEngine.h"

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

cocos2d::Scene* StartMenuScene::createScene()
{
	return this;
}


void StartMenuScene::initStartButton()
{
	auto winSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	auto startBtnPic = cocos2d::MenuItemImage::create("Menu/Start.png", "Menu/Start.png", CC_CALLBACK_1(StartMenuScene::startCallback, this));
	if (startBtnPic == nullptr ||
		startBtnPic->getContentSize().width <= 0 ||
		startBtnPic->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + winSize.width / 16 - startBtnPic->getContentSize().width / 16;
		float y = origin.y + winSize.height / 2 - startBtnPic->getContentSize().height / 2;
		startBtnPic->setAnchorPoint(cocos2d::Vec2::ZERO);
		startBtnPic->setPosition(cocos2d::Vec2(x, y));
	}

	startButton_ = cocos2d::Menu::create(startBtnPic, nullptr);
	this->addChild(startButton_, 1);
	startButton_->setPosition(cocos2d::Vec2::ZERO);
}

void StartMenuScene::initOnlineButton()
{
	auto winSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	auto olBtnPic = cocos2d::MenuItemImage::create("Menu/StartOnLine.png", "Menu/StartOnLine.png", CC_CALLBACK_1(StartMenuScene::startOnlineCallback, this));
	if (olBtnPic == nullptr ||
		olBtnPic->getContentSize().width <= 0 ||
		olBtnPic->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + winSize.width / 16 - olBtnPic->getContentSize().width / 16;
		float y = origin.y + 3 * winSize.height / 8 - olBtnPic->getContentSize().height / 2;
		olBtnPic->setAnchorPoint(cocos2d::Vec2::ZERO);
		olBtnPic->setPosition(cocos2d::Vec2(x, y));
	}

	onlineButton_ = cocos2d::Menu::create(olBtnPic, nullptr);
	this->addChild(onlineButton_, 1);
	onlineButton_->setPosition(cocos2d::Vec2::ZERO);
}

void StartMenuScene::initExitButton()
{
	auto winSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	auto exitBtnPic = cocos2d::MenuItemImage::create("Menu/Quit.png", "Menu/Quit.png", CC_CALLBACK_1(StartMenuScene::exitCallback, this));
	if (exitBtnPic == nullptr ||
		exitBtnPic->getContentSize().width <= 0 ||
		exitBtnPic->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + winSize.width / 16 - exitBtnPic->getContentSize().width / 16;
		float y = origin.y + winSize.height / 4 - exitBtnPic->getContentSize().height / 2;
		exitBtnPic->setAnchorPoint(cocos2d::Vec2::ZERO);
		exitBtnPic->setPosition(cocos2d::Vec2(x, y));
	}

	exitButton_ = cocos2d::Menu::create(exitBtnPic, nullptr);
	this->addChild(exitButton_, 1);
	exitButton_->setPosition(cocos2d::Vec2::ZERO);
}

bool StartMenuScene::init()
{
	if (!this->cocos2d::Scene::init())
	{
		return false;
	}

	auto winSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto bkg = cocos2d::Sprite::create("Menu/BG.png");
	bkg->setAnchorPoint(cocos2d::Vec2::ZERO);
	addChild(bkg, 0);
	initStartButton();
	initExitButton();
	initOnlineButton();
	loadingFightScene();

	cocos2d::Director::getInstance()->getOpenGLView()->setCursorVisible(true);

	return true;
}

void StartMenuScene::startCallback(cocos2d::Ref* pSender)
{
	cocos2d::Director::getInstance()->getOpenGLView()->setCursorVisible(false);
	goToFightScene();
}

void StartMenuScene::startOnlineCallback(cocos2d::Ref* pSender)
{
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	auto label = cocos2d::Label::createWithTTF("Connecting to Server...", "fonts/IRANYekanBold.ttf", 36);
	label->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	label->setPosition(winSize / 2);
	label->enableOutline(cocos2d::Color4B(194, 69, 69, 255), 5);
	auto remove = cocos2d::RemoveSelf::create();
	auto delay = cocos2d::DelayTime::create(.1f);
	auto fade2 = cocos2d::FadeTo::create(1, 0);

	auto connect = cocos2d::CallFunc::create([=]() {
		Client::getInstance()->initialization();
		Client::getInstance()->Send("Join");
		cocos2d::Director::getInstance()->getOpenGLView()->setCursorVisible(false);
		goToFightScene();
		});
	label->runAction(cocos2d::Sequence::create(delay, connect, fade2, remove, nullptr));
	addChild(label, 3);
}

void StartMenuScene::exitCallback(cocos2d::Ref* pSender)
{
	cocos2d::Director::getInstance()->end();
}

void StartMenuScene::goToFightScene()
{
	fightScene_->settingLayer_->backgroundMusicID_ = cocos2d::AudioEngine::play2d("audio/bgm_1low.mp3", true, .5);

	cocos2d::Director::getInstance()->replaceScene(fightScene_->createScene());
}

void StartMenuScene::loadingFightScene()
{
	auto _tileMap1 = TMXTiledMap::create("map/map_1/map/bottomMap.tmx");
	_tileMap1->setPosition(Vec2(0, 0));

	auto _tileMap2 = TMXTiledMap::create("map/map_1/map/middleMap.tmx");
	_tileMap2->setPosition(Vec2(0, 50 * 32));

	auto _tileMap3 = TMXTiledMap::create("map/map_1/map/topMap.tmx");
	_tileMap3->setPosition(Vec2(0, 100 * 32));

	fightScene_ = FightScene::create(_tileMap1, _tileMap2, _tileMap3, Obstacle::createObsSet(1), 1);
	fightScene_->bindPlayer(Player::create("MIKU/idle_down/idle_down1.png"));
	fightScene_->retain();
	//≥ı º ±∫Úº”‘ÿ“Ù¿÷
	AudioEngine::preload("audio/bgm_1low.mp3");
}