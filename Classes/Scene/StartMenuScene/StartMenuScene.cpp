/**
* @file StartMenuScene.cpp
*/

#include "cocos2d.h"
#include "StartMenuScene.h"

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

	auto startBtnPic = cocos2d::MenuItemImage::create("buttony.png", "buttonb.png", CC_CALLBACK_1(StartMenuScene::startCallback, this));
	if (startBtnPic == nullptr ||
		startBtnPic->getContentSize().width <= 0 ||
		startBtnPic->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + winSize.width / 2 - startBtnPic->getContentSize().width / 2;
		float y = origin.y + winSize.height / 2 - startBtnPic->getContentSize().height / 2;
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

	auto exitBtnPic = cocos2d::MenuItemImage::create("buttonb.png", "buttony.png", CC_CALLBACK_1(StartMenuScene::exitCallback, this));
	if (exitBtnPic == nullptr ||
		exitBtnPic->getContentSize().width <= 0 ||
		exitBtnPic->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + winSize.width / 2 - exitBtnPic->getContentSize().width / 2;
		float y = origin.y + winSize.height / 4 - exitBtnPic->getContentSize().height / 2;
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

	initStartButton();
	initExitButton();

	return true;
}

void StartMenuScene::startCallback(cocos2d::Ref* pSender)
{
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionSlideInT::create(0.2f, firstFightScene_));
}

void StartMenuScene::exitCallback(cocos2d::Ref* pSender)
{
	cocos2d::Director::getInstance()->end();
}

void StartMenuScene::bindFirstFightScene(cocos2d::Scene* scene)
{
		firstFightScene_ = scene;
}