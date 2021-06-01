/**
* @file Item.cpp
*/

#include "cocos2d.h"
#include "Item.h"
#include "Scene/HelloWorldScene.h"

void Item::bindPictureSprite(cocos2d::Sprite* sprite)
{
	sprite_ = sprite;
	addChild(sprite_);
}

cocos2d::Label* Item::getItemInfo()
{
	return itemInfo_;
}

bool Item::onContactBegan(cocos2d::PhysicsContact& physicsContact)
{
	auto nodeA = physicsContact.getShapeA()->getBody()->getNode();
	auto nodeB = physicsContact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		Player* playerNode = nullptr;
		if (nodeA->getTag() == ME && nodeB == this)
		{
			playerNode = dynamic_cast<Player*>(nodeA);
		}
		else if (nodeA == this && nodeB->getTag() == ME)
		{
			playerNode = dynamic_cast<Player*>(nodeB);
		}

		if (playerNode != nullptr && playerNode->getInteractItem() == nullptr)
		{
			playerNode->setInteractItem(this);
			if (itemInfo_ && itemInfo_->getParent() == this)
			{
				itemInfo_->setVisible(true);
			}
		}
	}
	return true;
}


bool Item::onCantactSeparate(cocos2d::PhysicsContact& physicsContact)
{
	auto nodeA = physicsContact.getShapeA()->getBody()->getNode();
	auto nodeB = physicsContact.getShapeB()->getBody()->getNode();
	if (nodeA && nodeB)
	{
		Player* playerNode = nullptr;
		if (nodeA->getTag() == ME && nodeB == this)
		{
			playerNode = dynamic_cast<Player*>(nodeA);
		}
		else if (nodeA == this && nodeB->getTag() == ME)
		{
			playerNode = dynamic_cast<Player*>(nodeB);
		}

		if (playerNode != nullptr && playerNode->getInteractItem() == this)
		{
			cocos2d::log("item separate");
			playerNode->setInteractItem(nullptr);
			if (itemInfo_ && itemInfo_->getParent() == this)
			{
				itemInfo_->setVisible(false);
			}
		}
	}
	return true;
}