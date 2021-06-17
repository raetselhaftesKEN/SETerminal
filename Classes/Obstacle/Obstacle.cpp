/**
* @file Obstacle.cpp
*/

#include "Obstacle.h"
#include "Const/Const.h"
#include "Scene/FightScene/FightScene.h"
#include <cmath>

Obstacle* Obstacle::create(const std::string& filename)
{
	auto obstacle = new(std::nothrow) Obstacle();
	if (obstacle == nullptr)
	{
		return nullptr;
	}
	obstacle->bindPictureSprite(cocos2d::Sprite::create(filename));

	if (obstacle && obstacle->sprite_)
	{
		obstacle->autorelease();
		obstacle->setTag(OBSTACLE_TAG);
		obstacle->bindPhysicsBody();
		return obstacle;
	}

	return nullptr;

}

void Obstacle::bindPictureSprite(cocos2d::Sprite* sprite)
{
	sprite_ = sprite;
	addChild(sprite_);
}


void Obstacle::collision(Character* targetSprite, bool flag)
{

	auto obstaclePosition = this->getPosition();
	//要检测的东西的位置
	auto targetPosition = targetSprite->getPosition();
	auto obstacleSize = sprite_->getContentSize();
	auto targetSize = targetSprite->getPictureSprite()->getContentSize();
	//检测到碰撞
	if (fabs(obstaclePosition.x - targetPosition.x) < (obstacleSize.width / 2 + targetSize.width / 2) && fabs(obstaclePosition.y - targetPosition.y) < (obstacleSize.height / 2 + targetSize.height / 2))
	{
		//当前离4个边界的垂直距离
		float leftDistance = (targetPosition.x + targetSize.width / 2) - (obstaclePosition.x - obstacleSize.width / 2);
		float rightDistance = (obstaclePosition.x + obstacleSize.width / 2) - (targetPosition.x - targetSize.width / 2);
		float topDistance = (obstaclePosition.y + obstacleSize.height / 2) - (targetPosition.y - targetSize.height / 2);
		float bottomDistance = (targetPosition.y + targetSize.height / 2) - (obstaclePosition.y - obstacleSize.height / 2);
		std::vector<float> distance = { leftDistance, rightDistance, topDistance, bottomDistance };
		float minDistance = 99999;
		int minIndex = -1;
		//找到是从哪个方向撞到的
		for (int i = 0; i < 4; ++i)
		{
			if (distance[i] < minDistance)
			{
				minDistance = distance[i];
				minIndex = i;
			}
		}
		float nextPositionX = 0, nextPositionY = 0;
		switch (minIndex)
		{
		case 0:
			nextPositionX = obstaclePosition.x - obstacleSize.width / 2 - targetSize.width / 2;
			targetSprite->setPosition(cocos2d::Vec2(nextPositionX, targetPosition.y));
			break;
		case 1:
			nextPositionX = obstaclePosition.x + obstacleSize.width / 2 + targetSize.width / 2;
			targetSprite->setPosition(cocos2d::Vec2(nextPositionX, targetPosition.y));
			break;
		case 2:
			nextPositionY = obstaclePosition.y + obstacleSize.height / 2 + targetSize.height / 2;
			targetSprite->setPosition(cocos2d::Vec2(targetPosition.x, nextPositionY));
			break;
		case 3:
			nextPositionY = obstaclePosition.y - obstacleSize.height / 2 - targetSize.height / 2;
			targetSprite->setPosition(cocos2d::Vec2(targetPosition.x, nextPositionY));
			break;
		default:
			break;
		}
	}
}

bool Obstacle::bindPhysicsBody()
{
	auto physicsBody = cocos2d::PhysicsBody::createBox(sprite_->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setContactTestBitmask(OBSTACLE_CONTACT_MASK);
	physicsBody->setCategoryBitmask(OBSTACLE_CATEGORY_MASK);
	setPhysicsBody(physicsBody);

	return true;
}

cocos2d::Vector<Obstacle*> Obstacle::createObsSet(const int& serial)
{
	cocos2d::Vector<Obstacle*> obs;

	auto obs1 = Obstacle::create("map/map_1/Obstacles/1408_3920.png");
	obs1->setPosition(1408, 3920);
	obs1->setVisible(false);

	auto obs2 = Obstacle::create("map/map_1/Obstacles/1536_2384.png");
	obs2->setPosition(1536, 2384);
	obs2->setVisible(false);

	auto obs3 = Obstacle::create("map/map_1/Obstacles/1792_1536.png");
	obs3->setPosition(1792, 1536);
	obs3->setVisible(false);

	auto obs4 = Obstacle::create("map/map_1/Obstacles/2336_3584.png");
	obs4->setPosition(2336, 3584);
	obs4->setVisible(false);

	auto obs5 = Obstacle::create("map/map_1/Obstacles/2560_1856.png");
	obs5->setPosition(2560, 1856);
	obs5->setVisible(false);

	auto obs6 = Obstacle::create("map/map_1/Obstacles/2816_2480.png");
	obs6->setPosition(2816, 2480);
	obs6->setVisible(false);

	auto obs7 = Obstacle::create("map/map_1/Obstacles/2912_1680.png");
	obs7->setPosition(2912, 1680);
	obs7->setVisible(false);

	auto obs8 = Obstacle::create("map/map_1/Obstacles/3040_2608.png");
	obs8->setPosition(3040, 2608);
	obs8->setVisible(false);

	auto obs9 = Obstacle::create("map/map_1/Obstacles/3168_1600.png");
	obs9->setPosition(3168, 1600);
	obs9->setVisible(false);

	auto obs10 = Obstacle::create("map/map_1/Obstacles/3584_2688.png");
	obs10->setPosition(3584, 2688);
	obs10->setVisible(false);

	auto obs11 = Obstacle::create("map/map_1/Obstacles/3584_3808.png");
	obs11->setPosition(3584, 3808);
	obs11->setVisible(false);

	auto obs12 = Obstacle::create("map/map_1/Obstacles/3680_2384.png");
	obs12->setPosition(3680, 2384);
	obs12->setVisible(false);

	auto obs13 = Obstacle::create("map/map_1/Obstacles/3936_1392.png");
	obs13->setPosition(3936, 1392);
	obs13->setVisible(false);

	auto obs14 = Obstacle::create("map/map_1/Obstacles/1104_2400.png");
	obs14->setPosition(1104, 2400);
	obs14->setVisible(false);

	auto obs15 = Obstacle::create("map/map_1/Obstacles/2608_464.png");
	obs15->setPosition(2608, 464);
	obs15->setVisible(false);

	auto obs16 = Obstacle::create("map/map_1/Obstacles/2608_4336.png");
	obs16->setPosition(2608, 4336);
	obs16->setVisible(false);

	auto obs17 = Obstacle::create("map/map_1/Obstacles/4112_2400.png");
	obs17->setPosition(4112, 2400);
	obs17->setVisible(false);

	obs.pushBack(obs1);
	obs.pushBack(obs2);
	obs.pushBack(obs3);
	obs.pushBack(obs4);
	obs.pushBack(obs5);
	obs.pushBack(obs6);
	obs.pushBack(obs7);
	obs.pushBack(obs8);
	obs.pushBack(obs9);
	obs.pushBack(obs10);
	obs.pushBack(obs11);
	obs.pushBack(obs12);
	obs.pushBack(obs13);
	obs.pushBack(obs14);
	obs.pushBack(obs15);
	obs.pushBack(obs16);
	obs.pushBack(obs17);

	return obs;
}

cocos2d::Size Obstacle::getSize()
{
	return this->sprite_->getContentSize();
}