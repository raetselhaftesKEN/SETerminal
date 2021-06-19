/**
* @file FightScene.cpp
*/

#include "cocos2d.h"
#include "Const/Const.h"
#include "FightScene.h"
#include "Component/WeaponUI/WeaponUI.h"
#include "Character/Monster.h"
#include "Item/Clip/Clip.h"
#include "Scene/StartMenuScene/StartMenuScene.h"
#include "Client/Client.h"

using namespace cocos2d;

class StartMenuScene;
class SettingLayer;
class Client;

bool FightScene::isShootMusicPlaying_ = true;

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

FightScene::FightScene(TMXTiledMap* map1, TMXTiledMap* map2, TMXTiledMap* map3, const Vector<Obstacle*>& obstacle, const int& serial)
	: tileMap1_(map1), tileMap2_(map2), tileMap3_(map3), obstacle_(obstacle), sceneSerial_(serial)
{
	player_ = nullptr;
	dropNode_ = nullptr;
	mainCamera_ = nullptr;
	touchHolding_ = false;
};

FightScene* FightScene::create(TMXTiledMap* map1, TMXTiledMap* map2, TMXTiledMap* map3, const cocos2d::Vector<Obstacle*>& obstacle, const int& serial)
{
	auto pRet = new(std::nothrow) FightScene(map1, map2, map3, obstacle, serial);
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

	timer_ = SETimer::create();
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
	this->addChild(tileMap1_, 0);
	this->addChild(tileMap2_, 0);
	this->addChild(tileMap3_, 0);
	this ->setObstacle();
	this->setPhysicsListener();

	this->schedule(CC_SCHEDULE_SELECTOR(FightScene::generateMonster), 1.5);
	this->schedule(CC_SCHEDULE_SELECTOR(FightScene::airDrop), 60.f);
	this->scheduleUpdate();

	settingLayer_ = SettingLayer::create();
	this->addChild(settingLayer_, 5);
	buildSettingBtn();
	endLayer_ = EndLayer::create();
	this->addChild(endLayer_, 6);

	return true;
}

cocos2d::Vector<Obstacle*> FightScene::getObstacles()
{
	return obstacle_;
}

bool FightScene::isInBound(cocos2d::Vec2 pos)
{
	return ((pos.x >= BOUND_XMIN && pos.x <= BOUND_XMAX) && (pos.y >= BOUND_YMIN && pos.y <= BOUND_YMAX));
}

bool FightScene::ifCollision(cocos2d::Vec2 pos)
{
	auto runningScene = dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG));
	for (auto i : runningScene->getObstacles())
	{
		auto obsPos = i->getPosition();
		auto obsSize = i->getSize();
		if (fabs(obsPos.x - pos.x) < (obsSize.width) && fabs(obsPos.y - pos.y) < (obsSize.height))
		{
			return true;
		}
	}
	return false;
}

void FightScene::generateMonster(float dt)
{
	if (SpawnedMonster < MonsterToSpawn && player_->isAlive() && MonsterInScene < MaxMonsterInScene)
	{
		int monsterType = rand() % 3;
		Monster* monster = nullptr;
		switch (monsterType)
		{
			case 0:
				monster = Monster::create(enemyType_::Default_Shoot);
				break;
			case 1:
				monster = Monster::create(enemyType_::Default_Shoot_Fast);
				break;
			case 2:
				monster = Monster::create(enemyType_::Default_Shoot_Elite);
				break;
			default:
				break;
		}

		if (monster == nullptr)
		{
			problemLoading("monster.png");
		}
		else
		{
			addChild(monster, 1);
			monster->move();
			SpawnedMonster++;
			MonsterInScene++;
		}
	}
}

void FightScene::monsterDestroyed()
{
	if (RemainingSurvivor > 1)
	{
		RemainingSurvivor--;
		MonsterInScene--;
	}
	else
	{
		//最后一个怪被消灭，显示结算界面
		endLayer_->setPosition(0, 0);
		endLayer_->open();
		auto changeSceneButton = cocos2d::ui::Button::create("Setting/close.png", "Setting/close_pressed.png");
		//auto closeButtonSize = changeSceneButton->getContentSize();
		auto runningSceneSize = this->getContentSize();
		changeSceneButton->setPosition(cocos2d::Vec2(runningSceneSize.width / 2, runningSceneSize.height / 2 - 150));
		this->addChild(changeSceneButton, 20);
		changeSceneButton->addClickEventListener([&](Ref*) {
			cocos2d::log("Close Button Pressed!");
			Client::getInstance()->Send("Quit");
			auto startMenuScene = StartMenuScene::create();
			startMenuScene->retain();
			//关闭音乐
			cocos2d::AudioEngine::stop(settingLayer_->backgroundMusicID_);
			settingLayer_->isBackgroundMusicPlaying_ = false;
			removeFromParent();
			cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionSlideInT::create(.2f, startMenuScene->createScene()));
			}
		);
		changeSceneButton->setCameraMask(2, true);
	}
}

bool FightScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
{
	player_->setAttackStatus(true);
	touchHolding_ = true;
	return true;
}

bool FightScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
{
	player_->setAttackStatus(false);
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
		////////////////////////////////
		auto particleSystem = cocos2d::ParticleExplosion::create();
		particleSystem->setDuration(0.05f);
		particleSystem->setLife(0.1);
		particleSystem->setLifeVar(0.05);
		particleSystem->setScale(0.5f);
		particleSystem->setSpeed(500);
		particleSystem->setStartColor(cocos2d::Color4F::RED);
		particleSystem->setEndColor(cocos2d::Color4F::RED);
		particleSystem->setStartColorVar(cocos2d::Color4F::BLACK);
		particleSystem->setEndColorVar(cocos2d::Color4F::BLACK);
		this->addChild(particleSystem, 10);
		particleSystem->setPosition(bullet->getPosition());
		///////////////////////////////

		if (isShootMusicPlaying_)
		{
			cocos2d::AudioEngine::play2d("Audio/hit2.mp3", false, 1.5f);
		}
		if (!player_->isAlive())
		{
			//player死亡
			endLayer_->setPosition(0, 0);
			endLayer_->open();
			auto changeSceneButton = cocos2d::ui::Button::create("Setting/close.png", "Setting/close_pressed.png");
			//auto closeButtonSize = changeSceneButton->getContentSize();
			auto runningSceneSize = this->getContentSize();
			changeSceneButton->setPosition(cocos2d::Vec2(runningSceneSize.width / 2, runningSceneSize.height / 2 - 150));
			this->addChild(changeSceneButton, 20);
			changeSceneButton->addClickEventListener([&](Ref*) {
				cocos2d::log("Close Button Pressed!");
				Client::getInstance()->Send("Quit");
				auto startMenuScene = StartMenuScene::create();
				startMenuScene->retain();
				//关闭音乐
				cocos2d::AudioEngine::stop(settingLayer_->backgroundMusicID_);
				settingLayer_->isBackgroundMusicPlaying_ = false;
				removeFromParent();
				cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionSlideInT::create(.2f, startMenuScene->createScene()));
				}
			);
			changeSceneButton->setCameraMask(2, true);

		}

		cocos2d::AudioEngine::play2d("Audio/hit2.mp3", false, 1.5f);
		

		character->receiveDamage(bullet->getBulletAtk());
		bullet->removeFromParentAndCleanup(true);
	}
}

void FightScene::contactBetweenObstacleAndBullet(Obstacle* obstacle, Bullet* bullet)
{
	if (obstacle && bullet)
	{
		////////////////////////////////
		auto particleSystem = cocos2d::ParticleExplosion::create();
		particleSystem->setDuration(0.05f);
		particleSystem->setLife(0.1);
		particleSystem->setLifeVar(0.05);
		particleSystem->setScale(0.5f);
		particleSystem->setSpeed(500);
		particleSystem->setStartColor(cocos2d::Color4F::GRAY);
		particleSystem->setEndColor(cocos2d::Color4F::GRAY);
		particleSystem->setStartColorVar(cocos2d::Color4F::BLACK);
		particleSystem->setEndColorVar(cocos2d::Color4F::BLACK);
		this->addChild(particleSystem, 10);
		particleSystem->setPosition(bullet->getPosition());
		///////////////////////////////

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
	//cocos2d::log("update per frame");
	char command[128];
	if (Client::getInstance()->Receive())
	{
		strcpy(command, Client::getInstance()->getReceiveBuffer());
	}

	if (strcmp(command, PLAYER_JOIN_COMMAND) == 0)
	{
		globalPromptDisplay("A teammate joined the battle! You are boosted!", 2);
		this->scheduleOnce(CC_SCHEDULE_SELECTOR(FightScene::airDrop), 0.f);

	}
	else if (strcmp(command, PLAYER_QUIT_COMMAND) == 0)
	{
		globalPromptDisplay("A teammate has left. Please be careful.", 2);
	}
	
}

void FightScene::buildSettingBtn()
{
	auto btnSetting = cocos2d::ui::Button::create("Setting/btn_default.png", "Setting/btn_default_pressed.png");
	auto settingImg = cocos2d::Sprite::create("Setting/settings.png");

	auto closeButton = cocos2d::ui::Button::create("Setting/close.png", "Setting/close_pressed.png");

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
			Client::getInstance()->Send("Quit");
			auto startMenuScene = StartMenuScene::create();
			startMenuScene->retain();
			//关闭音乐
			cocos2d::AudioEngine::stop(settingLayer_->backgroundMusicID_);
			settingLayer_->isBackgroundMusicPlaying_ = false;
			removeFromParent();
			cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionSlideInT::create(.2f, startMenuScene->createScene()));
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

void FightScene::airDrop(float dt)
{
	auto pos = FightScene::getRandomPosition();
	auto winSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto node = Weapon::create(static_cast<weaponType_>(rand() % 6));
	node->setPosition(cocos2d::Vec2(pos.x, pos.y + winSize.height));
	node->runAction(cocos2d::MoveTo::create(3.f, pos));
	node->retain();

	addChild(node, 1);

	auto playerPos = player_->getPosition();
	auto offset = pos - playerPos;
	std::string prompt = "Air drop comes in your ";
	if (offset.x > 0 && offset.y > 0)
		prompt += "northeast.";
	else if (offset.x > 0 && offset.y < 0)
		prompt += "southeast.";
	else if (offset.x < 0 && offset.y > 0)
		prompt += "northwest.";
	else if (offset.x < 0 && offset.y < 0)
		prompt += "southwest.";

	globalPromptDisplay(prompt);
}

cocos2d::Vec2 FightScene::getRandomPosition()
{
	cocos2d::Vec2 position;

	auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
	Player* playerNode = nullptr;
	cocos2d::Vec2 playerPos = cocos2d::Vec2(BOUND_XMID, BOUND_YMID);
	if (runningScene != nullptr)
	{
		playerNode = dynamic_cast<Player*>(runningScene->getChildByTag(PLAYER_TAG));
		playerPos = playerNode->getPosition();
	}

	auto winSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto orgin = cocos2d::Director::getInstance()->getVisibleOrigin();


	auto minY = playerPos.y - winSize.height;
	auto maxY = playerPos.y + winSize.height;
	auto minX = playerPos.x - winSize.width;
	auto maxX = playerPos.x + winSize.width;
	auto rangeY = maxY - minY;
	auto rangeX = maxX - minX;

	do
	{
		position.y = (rand() % static_cast<int>(rangeY)) + minY;
		position.x = (rand() % static_cast<int>(rangeX)) + minX;
	} while (!FightScene::isInBound(position) || (FightScene::ifCollision(position)));

	return position;
}

void FightScene::globalPromptDisplay(const std::string& prompt, int type)
{
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	cocos2d::Vec2 position = cocos2d::Vec2::ZERO;
	if (type == 1)
	{
		position.x = winSize.width / 2, position.y = 3 * winSize.height / 4;
	}
	else if (type == 2)
	{
		position.x = winSize.width / 2, position.y = 7 * winSize.height / 8;
	}

	auto label = cocos2d::Label::createWithTTF(prompt, "fonts/IRANYekanBold.ttf", 36);
	label->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	label->setPosition(position);
	auto remove = cocos2d::RemoveSelf::create();
	auto delay = cocos2d::DelayTime::create(1.f);
	auto fade2 = cocos2d::FadeTo::create(1, 0);
	label->runAction(cocos2d::Sequence::create(delay, fade2, remove, nullptr));
	label->setCameraMask(2, true);
	addChild(label, 3);
}

bool& FightScene::getShootMusicStatus()
{
	return isShootMusicPlaying_;
}