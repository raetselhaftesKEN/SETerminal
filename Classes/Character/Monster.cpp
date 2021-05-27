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


		//标记角色
		monster->setTag(ENEMY);
		//为角色设置物理躯干
		monster->bindPhysicsBody();

		monster->autorelease();
		return monster;
	}
	return nullptr;
}

void Monster::move(float sizeX, float sizeY, cocos2d::Vec2 playerPostion)
{
	auto monsterPosition = getPosition();


	//move1：怪物从右侧移动到中间随机位置，花费时间为randomDuration1
	auto move1 = cocos2d::MoveTo::create(3.5f, cocos2d::Vec2(-200, -20));
	//move1：怪物从中间随机位置移动到左侧，花费时间为randomDuration2
	//auto move2 = cocos2d::MoveTo::create(2.0f, cocos2d::Vec2(200,200));
	//actionRemove：释放怪物对象
	auto actionRemove = cocos2d::RemoveSelf::create();

	////怪物在move1和move2中间的随机位置发射子弹的动作，使用lambda表达式实现
	//auto shootStar = cocos2d::CallFunc::create([=]() {
	//	//生成敌人子弹
	//	Sprite* enemyBullet = Sprite::create("dart_enemy.png");
	//	if (enemyBullet == nullptr)
	//	{
	//		problemLoading("dart_enemy.png");
	//	}
	//	else
	//	{
	//		enemyBullet->setPosition(monster_->getPosition());
	//		//设置敌方子弹的物理躯干
	//		auto physicsBody = cocos2d::PhysicsBody::createBox(enemyBullet->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	//		physicsBody->setDynamic(false);
	//		physicsBody->setCategoryBitmask(3);
	//		physicsBody->setContactTestBitmask(4);
	//		enemyBullet->setPhysicsBody(physicsBody);
	//		enemyBullet->setTag(ENEMY_BULLET);
	//		this->addChild(enemyBullet);
	//		//为敌方子弹绑定发射动画
	//		float starSpeed = 120;
	//		float maxX = this->getContentSize().width;
	//		float starDuration = (float)randomX / starSpeed;
	//		auto eDartMove = cocos2d::MoveTo::create(starDuration, playerPostion);
	//		auto eDartRemove = cocos2d::RemoveSelf::create();
	//		enemyBullet->runAction(cocos2d::Sequence::create(eDartMove, eDartRemove, nullptr));
	//	}
	//	});
	////怪物发射子弹时略微停顿
	//auto delay = cocos2d::DelayTime::create(0.05);

	monster_->runAction(cocos2d::Sequence::create(move1, actionRemove, nullptr));

}

bool Monster::bindPhysicsBody()
{
	auto physicsBody = cocos2d::PhysicsBody::createBox(getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(1);
	physicsBody->setCategoryBitmask(3);
	setPhysicsBody(physicsBody);

	return true;
}

