/**
* @file Monster.cpp
*/

#include "Monster.h"
#include "Player.h"
#include "Item/Medkit/Medkit.h"
#include "Const/Const.h"
#include "Scene/FightScene/FightScene.h"

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

cocos2d::Vec2 Monster::getRandomPosition()
{
	cocos2d::Vec2 position;
	if (sprite_ == nullptr)
	{
		return cocos2d::Vec2::ZERO;
	}

	//为了在Monster类内使用外部的东西，使用以下语句获得当前进行的场景
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
	auto runningSceneSize = runningScene->getContentSize();
	//怪物在屏幕中随机位置出现，计算怪物生成和发射子弹的合法坐标范围
	auto monsterHeight = sprite_->getContentSize().height;
	auto monsterWidth = sprite_->getContentSize().width;
	auto minY = monsterHeight / 2;
	auto maxY = runningSceneSize.height - monsterHeight / 2;
	auto minX = monsterWidth / 2;
	auto maxX = runningSceneSize.width - monsterWidth / 2;
	auto rangeY = maxY - minY;
	auto rangeX = maxX - minX;
	position.y = (rand() % static_cast<int>(rangeY)) + minY;
	position.x = (rand() % static_cast<int>(rangeX)) + minX;
	return position;
}

void Monster::receiveDamage(int damage)
{
	int realDamage = static_cast<int>(damage * (1 - shield_));
	if (realDamage >= health_)
	{
		die();
	}
	else
	{
		health_ -= realDamage;
	}

}

void Monster::move() {
	auto nextPosition = getRandomPosition();
	Player* playerNode = nullptr;
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
	if (runningScene != nullptr)
	{
		auto playerNode = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG)->getChildByTag(PLAYER_TAG);
	}
	if (playerNode != nullptr)
	{
		nextPosition = playerNode->getPosition();
	}
	auto realDest = nextPosition - getPosition();
	realDest.normalize();
	auto moveOnce = cocos2d::MoveBy::create(2.f, realDest * 50);
	facingPoint_ = nextPosition;

	//怪物在move1和move2中间的随机位置发射子弹的动作，使用lambda表达式实现
	auto shootStar = cocos2d::CallFunc::create([=]() {
		//生成敌人子弹
		Bullet* enemyBullet = Bullet::create("dart_enemy.png");
		if (enemyBullet == nullptr)
		{
			problemLoading("dart_enemy.png");
		}
		else
		{
			//setPosition是指的自己和他父节点的相对位置，子弹的父节点设为场景Helloworld
			enemyBullet->setPosition(getPosition());
			//设置敌方子弹的物理躯干
			auto physicsBody = cocos2d::PhysicsBody::createBox(enemyBullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
			physicsBody->setDynamic(false);
			physicsBody->setCategoryBitmask(MOSNTER_BULLET_CATEGORY_MASK);
			physicsBody->setContactTestBitmask(MONSTER_BULLET_CONTACT_MASK);
			enemyBullet->setPhysicsBody(physicsBody);
			enemyBullet->setTag(MONSTER_BULLET_TAG);

			//为了在Monster类内使用外部的东西，使用以下几句
			auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
			Player* playerOfNode = nullptr;
			if (runningScene != nullptr)
			{
				playerOfNode = dynamic_cast<Player*>(runningScene->getChildByTag(PLAYER_TAG));
			}
			cocos2d::Vec2 playerPositionInScene = cocos2d::Vec2::ZERO;
			//如果场景已经被释放，找不到我方player位置，直接退出
			if (playerOfNode == nullptr)
			{
				return;
			}
			//获得当前我方player位置
			auto playerOfPlayer = dynamic_cast<Player*>(playerOfNode);
			playerPositionInScene = playerOfPlayer->getPosition();


			runningScene->addChild(enemyBullet);
			//为敌方子弹绑定发射动画，速度暂时用不到，先用1s时间模拟
			float starSpeed = 1200;

			//在Monster视角下的player的坐标（Monster坐标为0,0）
			auto eDartMove = cocos2d::MoveTo::create(1.0f, playerPositionInScene);
			auto eDartRemove = cocos2d::RemoveSelf::create();
			enemyBullet->runAction(cocos2d::Sequence::create(eDartMove, eDartRemove, nullptr));
		}
		});
	//怪物发射子弹时略微停顿
	auto delay = cocos2d::DelayTime::create(0.1);

	//在这里死循环递归，只要怪物还活着，就一直跑
	runAction(cocos2d::Sequence::create(moveOnce, shootStar, cocos2d::CallFunc::create([=] {move(); }), nullptr));
}

void Monster::die()
{	
	int dropItem = rand() % 10;
	if (dropItem == 9)
	{
		auto scene = dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG));
		auto medkitNode = dynamic_cast<cocos2d::Node*>(Medkit::create(getPosition()));

		if (scene && medkitNode)
		{
			scene->setDropNode(medkitNode);
			scene->scheduleOnce(CC_SCHEDULE_SELECTOR(FightScene::updateDropNode), 0.f);
		}
	}
	isAlive_ = false;
	health_ = 0;
	removeFromParentAndCleanup(true);
}

Monster* Monster::create(const std::string& filename)
{
	auto monster = new(std::nothrow) Monster();
	if (!monster)
	{
		return nullptr;
	}
	monster->bindPictureSprite(cocos2d::Sprite::create(filename));
	monster->statusChanged_ = true;

	//为了在Monster类内使用外部的东西，使用以下语句获得当前进行的场景
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
	auto runningSceneSize = runningScene->getContentSize();

	if (monster && monster->sprite_)
	{
		auto monsterPosition = monster->getRandomPosition();
		monster->bindCharacterAnimate("MONSTER2");

		monster->health_ = MONSTER_MAX_HEALTH;
		monster->maxHealth_ = MONSTER_MAX_HEALTH;
		monster->shield_ = MONSTER_DEFAULT_SHIELD;

		//设置怪物生成坐标
		monster->setPosition(monsterPosition);

		//为角色设置物理躯干
		monster->bindPhysicsBody();

		//标记角色
		monster->setTag(MONSTER_TAG);


		monster->autorelease();
		return monster;
	}
	return nullptr;
}

bool Monster::bindPhysicsBody()
{
	//空指针，绑定失败
	if (sprite_ == nullptr)
	{
		return false;
	}
	auto physicsBody = cocos2d::PhysicsBody::createBox(sprite_->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(1);
	physicsBody->setCategoryBitmask(3);
	setPhysicsBody(physicsBody);

	return true;
}

void Monster::updateFacingStatus()
{
	facingPoint_ = getRandomPosition();
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
	Player* playerNode = nullptr;
	if (runningScene != nullptr)
	{
		playerNode = dynamic_cast<Player*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG)->getChildByTag(PLAYER_TAG));
	}
	if (playerNode != nullptr)
	{
		facingPoint_ = playerNode->getPosition();
	}
	auto direction = facingPoint_ - getPosition();
	preFacingStatus_ = curFacingStatus_;

	if (direction.x > 0 && abs(direction.y) <= direction.x)
	{
		curFacingStatus_ = FacingStatus::right;
	}
	else if (direction.x < 0 && abs(direction.y) <= abs(direction.x))
	{
		curFacingStatus_ = FacingStatus::left;
	}
	else if (direction.y > 0 && abs(direction.x) <= direction.y)
	{
		curFacingStatus_ = FacingStatus::up;
	}
	else if (direction.y < 0 && abs(direction.x) <= abs(direction.y))
	{
		curFacingStatus_ = FacingStatus::down;
	}

	if (preFacingStatus_ != curFacingStatus_)
	{
		statusChanged_ = true;
	}
}

void Monster::updateWalkingStatus()
{
	preWalkingStatus_ = curWalkingStatus_;
	curWalkingStatus_ = WalkingStatus::walk;
	if (preFacingStatus_ != curFacingStatus_)
	{
		statusChanged_ = true;
	}
}

void Monster::update(float dt)
{
	updateFacingStatus();
	updateWalkingStatus();
	updateMoveAnimate();
	statusChanged_ = false;
	detectCollision();
}