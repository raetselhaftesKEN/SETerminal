/**
* @file Monster.cpp
*/

#include "Monster.h"
#include "Player.h"
#include "Item/Medkit/Medkit.h"
#include "Item/Clip/Clip.h"
#include "Scene/FightScene/FightScene.h"

bool Monster::isPlayerSuperDamage_ = false;

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void Monster::receiveDamage(int damage)
{
	int realDamage = static_cast<int>(damage * (1 - shield_));
	//玩家打开一击必杀模式
	if (isPlayerSuperDamage_)
	{
		realDamage = 999;
	}
	if (realDamage >= health_)
	{

		dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG))->monsterDestroyed();

		die();
	}
	else
	{
		health_ -= realDamage;
	}

}

void Monster::move()
{
	switch (typeOfThisMonster)
	{
		case enemyType_::Default_Shoot:
		case enemyType_::Default_Shoot_Fast:
		{
			auto nextPosition = FightScene::getRandomPosition();
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
			auto moveOnce = cocos2d::MoveBy::create((realDest * 50).length() / moveSpeed_, realDest * 50);
			facingPoint_ = nextPosition;

			//怪物在move1和move2中间的随机位置发射子弹的动作，使用lambda表达式实现
			auto shootStar = cocos2d::CallFunc::create([=]() {
				//生成敌人子弹
				for (int i = 0; i < ShootFreq; i++)
				{
					Bullet* enemyBullet = Bullet::create("BulletEnemy.png");
					if (enemyBullet == nullptr)
					{
						problemLoading("BulletEnemy.png");
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
						auto shootPos = playerPositionInScene - getPosition();
						shootPos.normalize();

						runningScene->addChild(enemyBullet);

						float starSpeed = 800;

						enemyBullet->shoot(shootPos, starSpeed);
					}
				}
				});
			//怪物发射子弹时略微停顿
			auto delay = cocos2d::DelayTime::create(0.1);

			//在这里死循环递归，只要怪物还活着，就一直跑
			runAction(cocos2d::Sequence::create(moveOnce, shootStar, cocos2d::CallFunc::create([=] {move(); }), nullptr));
			break;
		}

		case enemyType_::Default_Shoot_Elite:
		{
			if (autoShoot)
			{
				shoot();
				autoShoot = false;
			}
			auto nextPosition = FightScene::getRandomPosition();
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
			auto moveOnce = cocos2d::MoveBy::create((realDest).length() * 5 / moveSpeed_, realDest * 5);
			facingPoint_ = nextPosition;

			//在这里死循环递归，只要怪物还活着，就一直跑

			runAction(cocos2d::Sequence::create(moveOnce, cocos2d::CallFunc::create([=] {move(); }), nullptr));
			break;
		}
		default:
			break;
	}


}

void Monster::shoot()
{
	auto shootStar = cocos2d::CallFunc::create([=]() {
		int ShootVar = rand() % 3 - 1;
		for (int i = 0; i < ShootFreq + ShootVar; i++)
		{
			Bullet* enemyBullet = Bullet::create("BulletEnemy2.png");
			if (enemyBullet == nullptr)
			{
				problemLoading("BulletEnemy2.png");
			}
			else
			{
				enemyBullet->setPosition(getPosition());
				auto physicsBody = cocos2d::PhysicsBody::createBox(enemyBullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
				physicsBody->setDynamic(false);
				physicsBody->setCategoryBitmask(MOSNTER_BULLET_CATEGORY_MASK);
				physicsBody->setContactTestBitmask(MONSTER_BULLET_CONTACT_MASK);
				enemyBullet->setPhysicsBody(physicsBody);
				enemyBullet->setTag(MONSTER_BULLET_TAG);

				auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
				Player* playerOfNode = nullptr;
				if (runningScene != nullptr)
				{
					playerOfNode = dynamic_cast<Player*>(runningScene->getChildByTag(PLAYER_TAG));
				}
				cocos2d::Vec2 playerPositionInScene = cocos2d::Vec2::ZERO;
				if (playerOfNode == nullptr)
				{
					return;
				}
				auto playerOfPlayer = dynamic_cast<Player*>(playerOfNode);
				playerPositionInScene = playerOfPlayer->getPosition();
				auto shootPos = playerPositionInScene - getPosition();
				shootPos.normalize();

				runningScene->addChild(enemyBullet);

				float starSpeed = 1200;

				enemyBullet->shoot(shootPos , starSpeed);
			}
		}
		});

	auto delay = cocos2d::DelayTime::create(shootGap + (rand() % 3 - 1));

	runAction(cocos2d::Sequence::create(shootStar, delay, cocos2d::CallFunc::create([=] {shoot(); }), nullptr));
}

void Monster::die()
{
	int dropItem = rand() % 10;
	auto scene = dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG));
	if (dropItem == 9)
	{

		auto medkitNode = dynamic_cast<cocos2d::Node*>(Medkit::create(getPosition()));


		if (scene && medkitNode)
		{
			scene->setDropNode(medkitNode);
			scene->scheduleOnce(CC_SCHEDULE_SELECTOR(FightScene::updateDropNode), 0.f);
		}
	}
	else if (dropItem == 8 || dropItem == 7)
	{
		auto ClipNode = dynamic_cast<cocos2d::Node*>(Clip::create(static_cast<bulletType_>(rand() % 3), rand() % 30 + 30));
		ClipNode->setPosition(getPosition());

		if (scene && ClipNode)
		{
			scene->setDropNode(ClipNode);
			scene->scheduleOnce(CC_SCHEDULE_SELECTOR(FightScene::updateDropNode), 0.f);
		}
	}
	isAlive_ = false;
	health_ = 0;

	auto deathParticle = cocos2d::ParticleExplosion::create();
	deathParticle->setDuration(0.1f);
	deathParticle->setLife(0.5f);
	deathParticle->setLifeVar(0.1);
	deathParticle->setScale(1.f);
	deathParticle->setSpeed(150);
	deathParticle->setStartColor(cocos2d::Color4F::BLACK);
	deathParticle->setEndColor(cocos2d::Color4F::BLACK);
	deathParticle->setStartColorVar(cocos2d::Color4F::BLACK);
	deathParticle->setEndColorVar(cocos2d::Color4F::BLACK);
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
	if (runningScene != nullptr)
	{
		runningScene->addChild(deathParticle, 10);
	}
	deathParticle->setPosition(getPosition());

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
		auto monsterPosition = FightScene::getRandomPosition();
		monster->bindCharacterAnimate("MONSTER2");

		monster->moveSpeed_ = 100.f;
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

Monster* Monster::create(enemyType_ type)
{
	auto monster = new(std::nothrow) Monster();
	if (!monster)
	{
		return nullptr;
	}

	std::string filename;
	switch (type)
	{
		case enemyType_::Default_Shoot:
			filename = "MONSTER2/idle_down/idle_down1.png";
			break;
		case enemyType_::Default_Shoot_Fast:
			filename = "MONSTER3/idle_down/idle_down1.png";
			break;
		case enemyType_::Default_Shoot_Elite:
			filename = "MONSTER1/idle_down/idle_down1.png";
			break;
		default:
			return nullptr;
			break;
	}
	monster->bindPictureSprite(cocos2d::Sprite::create(filename));
	monster->statusChanged_ = true;

	//为了在Monster类内使用外部的东西，使用以下语句获得当前进行的场景
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG);
	auto runningSceneSize = runningScene->getContentSize();

	if (monster && monster->sprite_)
	{
		auto monsterPosition = FightScene::getRandomPosition();

		switch (type)
		{
			case enemyType_::Default_Shoot:
			{
				monster->ShootFreq = 1;
				monster->moveSpeed_ = 100.f;
				monster->maxHealth_ = 30;
				monster->bindCharacterAnimate("MONSTER2");
				monster->typeOfThisMonster = type;
				break;
			}

			case enemyType_::Default_Shoot_Fast:
			{
				monster->ShootFreq = 1;
				monster->moveSpeed_ = 50.f;
				monster->maxHealth_ = 30;
				monster->bindCharacterAnimate("MONSTER3");
				monster->typeOfThisMonster = type;
				break;
			}

			case enemyType_::Default_Shoot_Elite:
			{
				monster->ShootFreq = 3;
				monster->moveSpeed_ = 150.f;
				monster->maxHealth_ = 60;
				monster->bindCharacterAnimate("MONSTER1");
				monster->typeOfThisMonster = type;
				monster->autoShoot = true;
				break;
			}

			default:
				return nullptr;
				break;
		}

		monster->health_ = monster->maxHealth_;
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
	facingPoint_ = FightScene::getRandomPosition();
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