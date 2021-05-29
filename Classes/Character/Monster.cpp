/**
* @file Monster.cpp
*/

#include "Monster.h"
#include "./Scene/HelloWorldScene.h"

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

Monster* Monster::create(const std::string& filename)
{
	auto monster = new(std::nothrow) Monster();
	if (!monster)
	{
		return nullptr;
	}
	monster->bindPictureSprite(cocos2d::Sprite::create(filename));
	monster->moveSpeed_ = 160.f;
	monster->health_ = 3;
	monster->shield_ = 0.5f;
	
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
	auto runningSceneSize = runningScene->getContentSize();

	if (monster && monster->sprite_)
	{
		//怪物在右侧随机位置出现，计算怪物生成和发射子弹的合法坐标范围
		auto minY = monster->getContentSize().height / 2;
		auto maxY = runningSceneSize.height - minY;
		auto rangeY = maxY - minY;
		int randomY = (rand() % static_cast<int>(rangeY)) + minY;
		//设置怪物生成坐标
		monster->setPosition(runningSceneSize.width + monster->getContentSize().width / 2, randomY);


		//标记角色
		monster->setTag(ENEMY);
		//为角色设置物理躯干
		monster->bindPhysicsBody();

		monster->autorelease();
		return monster;
	}
	return nullptr;
}

void Monster::move()
{
	auto monsterPosition = getPosition();
	//产生一个0-屏幕宽度范围之间的数，用以随机停止
	int randomX = (rand() % static_cast<int>(monsterPosition.x));
	float randomDuration1 = (monsterPosition.x - randomX) / moveSpeed_;
	float randomDuration2 = randomX / moveSpeed_;

	//move1：怪物从右侧移动到中间随机位置，花费时间为randomDuration1
	auto move1 = cocos2d::MoveTo::create(randomDuration1, cocos2d::Vec2(randomX, monsterPosition.y));
	//move2：怪物从中间随机位置移动到左侧，花费时间为randomDuration2
	auto move2 = cocos2d::MoveTo::create(randomDuration2, cocos2d::Vec2(0, monsterPosition.y));
	//actionRemove：释放怪物对象
	auto actionRemove = cocos2d::RemoveSelf::create();

	//怪物在move1和move2中间的随机位置发射子弹的动作，使用lambda表达式实现
	auto shootStar = cocos2d::CallFunc::create([=]() {
		//生成敌人子弹
		Sprite* enemyBullet = Sprite::create("dart_enemy.png");
		if (enemyBullet == nullptr)
		{
			problemLoading("dart_enemy.png");
		}
		else
		{
			//settPosition是指的自己和他父节点的相对位置，子弹的父节点设为场景Helloworld
			enemyBullet->setPosition(cocos2d::Vec2(randomX, monsterPosition.y));
			//设置敌方子弹的物理躯干
			auto physicsBody = cocos2d::PhysicsBody::createBox(enemyBullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
			physicsBody->setDynamic(false);
			physicsBody->setCategoryBitmask(3);
			physicsBody->setContactTestBitmask(4);
			enemyBullet->setPhysicsBody(physicsBody);
			enemyBullet->setTag(ENEMY_BULLET);

			//为了在Monster类内使用外部的东西，使用以下几句
			auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
			auto playerOfNode = runningScene->getChildByTag(ME);
			cocos2d::Vec2 playerPositionInScene = cocos2d::Vec2::ZERO;
			//获得当前我方player位置
			if (playerOfNode != nullptr)
			{
				auto playerOfPlayer = dynamic_cast<Player*>(playerOfNode);
				playerPositionInScene = playerOfPlayer->getPosition();
			}

			runningScene->addChild(enemyBullet);
			//为敌方子弹绑定发射动画，速度暂时用不到，先用1s时间模拟
			float starSpeed = 1200;

			//在Monster视角下的player的坐标（Monster坐标为0,0）
			auto eDartMove = cocos2d::MoveTo::create(1.0f, playerPositionInScene);
			auto eDartRemove = cocos2d::RemoveSelf::create();
			enemyBullet->runAction(cocos2d::Sequence::create(eDartMove, eDartRemove, nullptr));
		}
		});
	////怪物发射子弹时略微停顿
	auto delay = cocos2d::DelayTime::create(0.1);

	runAction(cocos2d::Sequence::create(move1, shootStar, delay, move2, actionRemove, nullptr));

}


bool Monster::bindPhysicsBody()
{
	auto physicsBody = cocos2d::PhysicsBody::createBox(sprite_->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(1);
	physicsBody->setCategoryBitmask(3);
	setPhysicsBody(physicsBody);

	return true;
}

