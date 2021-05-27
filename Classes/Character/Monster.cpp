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

Monster* Monster::create(const std::string& filename, float sizeX, float sizeY)
{
	auto monster = new(std::nothrow) Monster();
	if (!monster)
	{
		return nullptr;
	}
	monster->bindPictureMonster(cocos2d::Sprite::create(filename));

	if (monster && monster->monster_)
	{
		//怪物在右侧随机位置出现，计算怪物生成和发射子弹的合法坐标范围
		auto minY = monster->getContentSize().height / 2;
		auto maxY = sizeY - minY;
		auto rangeY = maxY - minY;
		int randomY = (rand() % static_cast<int>(rangeY)) + minY;
		//设置怪物生成坐标
		monster->setPosition(sizeX + monster->getContentSize().width / 2, randomY);

		//为角色设置物理躯干
		monster->bindPhysicsBody();
		//标记角色
		monster->setTag(ENEMY);
		

		monster->autorelease();
		return monster;
	}
	return nullptr;
}

void Monster::move(float sizeX, float sizeY, cocos2d::Vec2 playerPostionInScene)
{
	auto monsterPosition = getPosition();
	//产生一个0-屏幕宽度范围之间的数，用以随机停止
	int randomX = (rand() % static_cast<int>(monsterPosition.x));
	float randomDuration1 = randomX / monsterSpeed;
	float randomDuration2 = (monsterPosition.x - randomX) / monsterSpeed;

	//move1：怪物从右侧移动到中间随机位置，花费时间为randomDuration1，注意这里的坐标原点是以怪物为0,0来确定的
	auto move1 = cocos2d::MoveTo::create(randomDuration1, cocos2d::Vec2(-randomX, 0));
	//move2：怪物从中间随机位置移动到左侧，花费时间为randomDuration2
	auto move2 = cocos2d::MoveTo::create(randomDuration2, cocos2d::Vec2(-monsterPosition.x, 0));
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
			enemyBullet->setPosition(cocos2d::Vec2(-randomX, 0));
			//设置敌方子弹的物理躯干
			auto physicsBody = cocos2d::PhysicsBody::createBox(enemyBullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
			physicsBody->setDynamic(false);
			physicsBody->setCategoryBitmask(3);
			physicsBody->setContactTestBitmask(4);
			enemyBullet->setPhysicsBody(physicsBody);
			enemyBullet->setTag(ENEMY_BULLET);
			this->addChild(enemyBullet);
			//为敌方子弹绑定发射动画
            float starSpeed = 1200;
			//在Monster视角下的player的坐标（Monster坐标为0,0）
			cocos2d::Vec2 playerPositionInMonster;
			playerPositionInMonster.x = playerPostionInScene.x - monsterPosition.x;
			playerPositionInMonster.y = playerPostionInScene.y - monsterPosition.y;
			auto eDartMove = cocos2d::MoveTo::create(3.0f, playerPositionInMonster);
            auto eDartRemove = cocos2d::RemoveSelf::create();
			enemyBullet->runAction(cocos2d::Sequence::create(eDartMove, eDartRemove, nullptr));
		}
		});
	////怪物发射子弹时略微停顿
	auto delay = cocos2d::DelayTime::create(0.1);

	monster_->runAction(cocos2d::Sequence::create(move1,  delay, move2, nullptr));
	auto monsterPosition1 = getPosition();

}

bool Monster::bindPhysicsBody()
{
	cocos2d::Size t;
	t.width = 50;
	t.height = 50;
	auto physicsBody = cocos2d::PhysicsBody::createBox(getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(1);
	physicsBody->setCategoryBitmask(3);
	setPhysicsBody(physicsBody);

	return true;
}


