/**
* @file FightScene.cpp
*/

#include "cocos2d.h"
#include "Const/Const.h"
#include "FightScene.h"
#include "Component/WeaponUI/WeaponUI.h"
#include "Character/Monster.h"
#include "Item/Clip/Clip.h"

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

FightScene::FightScene(cocos2d::TMXTiledMap* map, const cocos2d::Vector<Obstacle*>& obstacle, const int& serial) : tileMap_(map), obstacle_(obstacle), sceneSerial_(serial)
{
	player_ = nullptr;
	dropNode_ = nullptr;
	mainCamera_ = nullptr;
	touchHolding_ = false;
};

FightScene* FightScene::create(cocos2d::TMXTiledMap* map, const cocos2d::Vector<Obstacle*>& obstacle, const int& serial)
{
	auto pRet = new(std::nothrow) FightScene(map, obstacle, serial);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
	return nullptr;
}

cocos2d::Scene* FightScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	if (scene != nullptr)
	{
		scene->addChild(this, 0);
		scene->getPhysicsWorld()->setAutoStep(true);
		scene->getPhysicsWorld()->setGravity(cocos2d::Vec2::ZERO);
		scene->retain();
		return scene;
	}
	return nullptr;
}

void FightScene::bindPlayer(Player* player)
{
	if (player != nullptr && player_ == nullptr)
	{
		this->player_ = player;
		this->addChild(player);
	}
	this->setCamera();
	this->setOperationListener();
	this->setUI();
}

void FightScene::setCamera()
{
	this->mainCamera_ = CameraEffect::create(this);
	if (this->mainCamera_ != nullptr && this->player_ != nullptr)
	{
		this->mainCamera_->LockPlayer(player_);
	}
}

void FightScene::setObstacle()
{
	for (auto i : this->obstacle_)
	{
		if (i->getParent() == nullptr && i->getTag() == OBSTACLE_TAG)
		{
			this->addChild(i, 1);
		}
	}
}

void FightScene::setUI()
{
	auto winSize = cocos2d::Director::getInstance()->getVisibleSize();
	healthBar_ = HealthBar::create(player_);
	healthBar_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	healthBar_->setPosition(cocos2d::Point(winSize.width / 2, 30));
	healthBar_->setScale(0.4f, 0.4f);
	addChild(healthBar_, 2);

	weaponUI_ = WeaponUI::create(player_);
	weaponUI_->setAnchorPoint(cocos2d::Point(0.5f, 0.f));
	weaponUI_->setPosition(cocos2d::Point(winSize.width / 2, 50));
	addChild(weaponUI_, 2);

	timer_ = Timer::create();
	timer_->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
	timer_->setPosition(cocos2d::Point(0, winSize.height));
	timer_->setScale(0.3f, 0.3f);
	addChild(timer_, 2);

	survivorCounter_ = SurvivorCounter::create();
	survivorCounter_->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
	survivorCounter_->setPosition(cocos2d::Point(200, winSize.height));
	survivorCounter_->setScale(0.3f, 0.3f);
	addChild(survivorCounter_, 2);
}

void FightScene::setOperationListener()
{
	if (player_ != nullptr && player_->getParent() == this)
	{
		auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
		touchListener->onTouchBegan = CC_CALLBACK_2(FightScene::onTouchBegan, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(FightScene::onTouchEnded, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, player_);

		auto mouseListener = cocos2d::EventListenerMouse::create();
		mouseListener->onMouseMove = CC_CALLBACK_1(FightScene::onMouseMove, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, player_);

		auto keyboardListener = cocos2d::EventListenerKeyboard::create();
		keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::listenToKeyPress, player_);
		keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::listenToKeyRelease, player_);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	}
}

void FightScene::setPhysicsListener()
{
	auto contactListener = cocos2d::EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(FightScene::onContactBegan, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(FightScene::onContactSeparated, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool FightScene::init()
{
	if (!(this->Scene::init() && this->Scene::initWithPhysics()))
	{
		return false;
	}
	this->setTag(FIGHT_SCENE_TAG);
	this->addChild(tileMap_, 0);
	this ->setObstacle();
	this->setPhysicsListener();

	this->schedule(CC_SCHEDULE_SELECTOR(FightScene::generateMonster), 1.5);

	auto clipTest1 = Clip::create(bulletType_::type556);
	clipTest1->setPosition(400, 200);
	addChild(clipTest1, 1);

	auto clipTest2 = Clip::create(bulletType_::type762);
	clipTest2->setPosition(700, 600);
	addChild(clipTest2, 1);

	auto clipTest3 = Clip::create(bulletType_::type9mm);
	clipTest3->setPosition(100, 600);
	addChild(clipTest3, 1);

	settingLayer_ = SettingLayer::create();
	this->addChild(settingLayer_, 5);
	buildSettingBtn();

	return true;
}

void FightScene::goToNextScene()
{
	auto map = cocos2d::TMXTiledMap::create(std::string("tilemap") + std::to_string(sceneSerial_ + 1) + ".png");
	map->setPosition(cocos2d::Vec2::ZERO);
	cocos2d::Vector<Obstacle*> obstacles;
	FightScene* nextScene = FightScene::create(map, obstacles, sceneSerial_ + 1);

	//TODO: Add obstacles

	player_->retain();
	player_->removeFromParent();
	nextScene->bindPlayer(player_);
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionSlideInT::create(2.0f, nextScene->createScene()));
	
}

cocos2d::Vector<Obstacle*> FightScene::getObstacles()
{
	return obstacle_;
}

void FightScene::generateMonster(float dt)
{
	auto monster = Monster::create("MONSTER2/idle_down/idle_down1.png");
	if (monster == nullptr)
	{
		problemLoading("monster.png");
	}
	else
	{
		addChild(monster, 1);
		monster->move();
	}
}

bool FightScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
{
	player_->isAttacking = true;
	touchHolding_ = true;
	return true;
}

bool FightScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
{
	player_->isAttacking = false;
	touchHolding_ = false;
	return true;
}

void FightScene::onMouseMove(cocos2d::EventMouse* mouse)
{
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	player_->listenToMouseEvent(convertToNodeSpace(mouse->getLocationInView() - winSize / 2), false);
}

bool FightScene::onContactBegan(cocos2d::PhysicsContact& physicsContact)
{
	auto nodeA = physicsContact.getShapeA()->getBody()->getNode();
	auto nodeB = physicsContact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == PLAYER_TAG && nodeB->getTag() == ITEM_TAG && !dynamic_cast<Item*>(nodeB)->isHeld())
		{
			contactBetweenPlayerAndItem(dynamic_cast<Player*>(nodeA), dynamic_cast<Item*>(nodeB));
		}
		else if (nodeA->getTag() == ITEM_TAG && nodeB->getTag() == PLAYER_TAG && !dynamic_cast<Item*>(nodeA)->isHeld())
		{
			contactBetweenPlayerAndItem(dynamic_cast<Player*>(nodeB), dynamic_cast<Item*>(nodeA));
		}
		else if ((nodeA->getTag() == PLAYER_TAG || nodeA->getTag() == MONSTER_TAG)
			&& (nodeB->getTag() == PLAYER_BULLET_TAG || nodeB->getTag() == MONSTER_BULLET_TAG))
		{
			contactBetweenCharacterAndBullet(dynamic_cast<Character*>(nodeA), dynamic_cast<Bullet*>(nodeB));
		}
		else if ((nodeA->getTag() == PLAYER_BULLET_TAG || nodeA->getTag() == MONSTER_BULLET_TAG)
			&& (nodeB->getTag() == PLAYER_TAG || nodeB->getTag() == MONSTER_TAG))
		{
			contactBetweenCharacterAndBullet(dynamic_cast<Character*>(nodeB), dynamic_cast<Bullet*>(nodeA));
		}
		else if ((nodeA->getTag() == PLAYER_BULLET_TAG || nodeA->getTag() == MONSTER_BULLET_TAG) && nodeB->getTag() == OBSTACLE_TAG)
		{
			contactBetweenObstacleAndBullet(dynamic_cast<Obstacle*>(nodeB), dynamic_cast<Bullet*> (nodeA));
		}
		else if ((nodeB->getTag() == PLAYER_BULLET_TAG || nodeB->getTag() == MONSTER_BULLET_TAG) && nodeA->getTag() == OBSTACLE_TAG)
		{
			contactBetweenObstacleAndBullet(dynamic_cast<Obstacle*>(nodeA), dynamic_cast<Bullet*> (nodeB));
		}
	}

	return true;
}

bool FightScene::onContactSeparated(cocos2d::PhysicsContact& physicsContact)
{
	auto nodeA = physicsContact.getShapeA()->getBody()->getNode();
	auto nodeB = physicsContact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		Player* playerNode = nullptr;
		if (nodeA->getTag() == PLAYER_TAG && nodeB->getTag() == ITEM_TAG)
		{
			playerNode = dynamic_cast<Player*>(nodeA);
		}
		else if (nodeA->getTag() == ITEM_TAG && nodeB->getTag() == PLAYER_TAG)
		{
			playerNode = dynamic_cast<Player*>(nodeB);
		}

		if (playerNode != nullptr && playerNode->getInteractItem() == nodeB)
		{
			cocos2d::log("item separate");
			playerNode->setInteractItem(nullptr);
			if (dynamic_cast<Item*>(nodeB)->getItemInfo() && dynamic_cast<Item*>(nodeB)->getItemInfo()->getParent() == nodeB)
			{
				dynamic_cast<Item*>(nodeB)->getItemInfo()->setVisible(false);
			}
		}
	}
	return true;
}

void FightScene::contactBetweenPlayerAndItem(Player* player, Item* item)
{
	if (player != nullptr && player->getInteractItem() == nullptr && item != nullptr)
	{
		player->setInteractItem(item);
		if (item->getItemInfo() && item->getItemInfo()->getParent() == item)
		{
			item->getItemInfo()->setVisible(true);
		}
	}
}

void FightScene::contactBetweenCharacterAndBullet(Character* character, Bullet* bullet)
{
	if (character && bullet)
	{
		character->receiveDamage(bullet->getBulletAtk());
		bullet->removeFromParentAndCleanup(true);
	}
}

void FightScene::contactBetweenObstacleAndBullet(Obstacle* obstacle, Bullet* bullet)
{
	if (obstacle && bullet)
	{
		bullet->removeFromParentAndCleanup(true);
	}
}

cocos2d::Node* FightScene::getDropNode()
{
	return dropNode_;
}
void FightScene::setDropNode(cocos2d::Node* node)
{
	dropNode_ = node;
}

void FightScene::updateDropNode(float dt)
{
	if (dropNode_ != nullptr)
	{
		auto dropTo = dropNode_->getPosition();
		dropNode_->setPosition(cocos2d::Point(dropTo.x, dropTo.y + 30));
		auto dropAction = cocos2d::MoveTo::create(0.2f, dropTo);
		dropNode_->runAction(dropAction);
		addChild(dropNode_, 2);
	}
}

void FightScene::update(float dt)
{
	if (player_ != nullptr)
	{
		auto pos = player_->getPosition();
		if (clear_ && pos.x >= GATE_POSITION_XMIN && pos.x <= GATE_POSITION_XMAX && pos.y >= GATE_POSITION_YMIN)
		{
			goToNextScene();
		}
	}
}

void FightScene::buildSettingBtn()
{
	auto btnSetting = cocos2d::ui::Button::create("Setting/btn_default.png", "Setting/btn_default_pressed.png");
	auto settingImg = cocos2d::Sprite::create("Setting/settings.png");

	auto closeButton = cocos2d::ui::Button::create("close.png", "close_pressed.png");

	if (btnSetting == nullptr || settingImg == nullptr || closeButton == nullptr)
	{
		problemLoading("Button picture");
	}
	else
	{
		btnSetting->setScale9Enabled(true);
		// 设置素材内容部分贴图大小
		//btnSetting->setCapInsets(Rect(12, 12, 30, 18));
		btnSetting->setContentSize(cocos2d::Size(100, 80));
		btnSetting->setPosition(cocos2d::Vec2(60, 40));
		settingImg->setPosition(cocos2d::Vec2(60, 40));
		btnSetting->addClickEventListener([&](Ref*) {
			cocos2d::log("Setting Pressed!");
			if (!settingLayer_->isOpen)
			{
				settingLayer_->setPosition(0, 0);
				settingLayer_->open();
			}
			else
			{
				settingLayer_->close();
			}
		}
		);

		auto closeButtonSize = closeButton->getContentSize();
		auto runningSceneSize = this->getContentSize();
		closeButton->setPosition(cocos2d::Vec2(runningSceneSize.width - closeButtonSize.width / 2, runningSceneSize.height - closeButtonSize.height / 2));
		closeButton->addClickEventListener([&](Ref*) {
			cocos2d::log("Close Button Pressed!");
			//cocos2d::Director::getInstance()->end();
			//cocos2d::Director::getInstance()->replaceScene(startMenu_);
			}
		);


		//设置始终在镜头左下角
		btnSetting->setCameraMask(2, true);
		settingImg->setCameraMask(2, true);
		closeButton->setCameraMask(2, true);
	}

	this->addChild(btnSetting, 3);
	this->addChild(settingImg, 4);
	this->addChild(closeButton, 3);
}