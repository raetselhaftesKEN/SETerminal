/**
* @file Player.cpp
*/

#include "Obstacle.h"
#include <cmath>
#include <vector>

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
		return obstacle;
	}

	return nullptr;

}

bool Obstacle::bindPhysicsBody()
{
	auto physicsBody = cocos2d::PhysicsBody::createBox(getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(0b11111111);
	physicsBody->setCategoryBitmask(0b11111111);
	setPhysicsBody(physicsBody);

	return true;
}

void Obstacle::collision(Sprite* targetSprite, bool flag)
{

	auto obstaclePosition = this->getPosition();
	//要检测的东西的位置
	auto targetPosition = targetSprite->getPosition();
	auto obstacleSize = sprite_->getContentSize();
	//检测到碰撞
	if (fabs(obstaclePosition.x - targetPosition.x) < obstacleSize.width / 2 && fabs(obstaclePosition.y - targetPosition.y) < obstacleSize.height / 2)
	{
		//当前离4个边界的垂直距离
		float leftDistance = targetPosition.x - (obstaclePosition.x - obstacleSize.width / 2);		
		float rightDistance = (obstaclePosition.x + obstacleSize.width / 2) - targetPosition.x;
		float topDistance = (obstaclePosition.y + obstacleSize.height / 2) - targetPosition.y;
		float bottomDistance = targetPosition.y - (obstaclePosition.y - obstacleSize.height / 2);
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
			nextPositionX = obstaclePosition.x - obstacleSize.width / 2;
			targetSprite->setPosition(cocos2d::Vec2(nextPositionX, targetPosition.y));
			break;
		case 1:
			nextPositionX = obstaclePosition.x + obstacleSize.width / 2;
			targetSprite->setPosition(cocos2d::Vec2(nextPositionX, targetPosition.y));
			break;
		case 2:
			nextPositionY = obstaclePosition.y + obstacleSize.height / 2;
			targetSprite->setPosition(cocos2d::Vec2(targetPosition.x, nextPositionY));
			break;
		case 3:
			nextPositionY = obstaclePosition.y - obstacleSize.height / 2;
			targetSprite->setPosition(cocos2d::Vec2(targetPosition.x, nextPositionY));
			break;
		default:
			break;

		}
		

	}
	

}