/**
* @file Player.cpp
*/

#include "Obstacle.h"
#include "Const/Const.h"
#include <cmath>

std::vector<Obstacle*> Obstacle::obstacles_{};

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
		obstacles_.push_back(obstacle);
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


std::vector<Obstacle*>* Obstacle::getObstacles()
{
	return &obstacles_;
}