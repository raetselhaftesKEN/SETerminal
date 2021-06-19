/**
* @file StartMenuScene.cpp
*/

#include "cocos2d.h"
#include "StartMenuScene.h"
//#include "Client/Client.h"

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
	loadingFightScene();

	cocos2d::Director::getInstance()->getOpenGLView()->setCursorVisible(true);

	return true;
}

void StartMenuScene::startCallback(cocos2d::Ref* pSender)
{
	//Client::getInstance()->initialization();
	//Client::getInstance()->SendAndRecv(std::string("test"));
	cocos2d::Director::getInstance()->getOpenGLView()->setCursorVisible(false);
	goToFightScene();
}

void StartMenuScene::exitCallback(cocos2d::Ref* pSender)
{
	cocos2d::Director::getInstance()->end();
}

void StartMenuScene::goToFightScene()
{
	//AudioEngine::preload("Audio/bgm_1Low.mp3");
	fightScene_->settingLayer_->backgroundMusicID_ = cocos2d::AudioEngine::play2d("Audio/bgm_1Low.mp3", true, .5);
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
	AudioEngine::preload("Audio/bgm_1Low.mp3");
}