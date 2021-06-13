/**
* @file FightScene.h
*/

#ifndef __FIGHT_SCENE_H__
#define __FIGHT_SCENE_H__

#include "cocos2d.h"
#include "Character/Player.h"
#include "Component/CameraEffect/CameraEffect.h"
#include "Item/Bullet/Bullet.h"
#include "Obstacle/Obstacle.h"
#include "Component/HealthBar/HealthBar.h"
#include "Component/WeaponUI/WeaponUI.h"
#include "Component/SettingLayer/SettingLayer.h"

class FightScene : public cocos2d::Scene
{
public:

	FightScene(cocos2d::TMXTiledMap* map, const cocos2d::Vector<Obstacle*>& obstacle);


	static FightScene* create(cocos2d::TMXTiledMap* map, const cocos2d::Vector<Obstacle*>& obstacle);


	cocos2d::Scene* createScene();


	void bindPlayer(Player* player);


	void setCamera();


	void setObstacle();


	void setWeaponUI();


	void setOperationListener();


	void setPhysicsListener();


	virtual bool init() override;
	

	void generateMonster(float dt);


	void bindNextScene(cocos2d::Layer*);


	cocos2d::Vector<Obstacle*> getObstacles();


	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent);
	bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent);
	void onMouseMove(cocos2d::EventMouse* mouse);
	bool onContactBegan(cocos2d::PhysicsContact& physicsContact);
	bool onContactSeparated(cocos2d::PhysicsContact& physicsContact);


	void contactBetweenPlayerAndItem(Player* player, Item* Item);
	void contactBetweenCharacterAndBullet(Character* character, Bullet* bullet);
	void contactBetweenObstacleAndBullet(Obstacle* obstacle, Bullet* bullet);

	cocos2d::Node* getDropNode();
	void setDropNode(cocos2d::Node* node);
	void updateDropNode(float dt);

	void buildSettingBtn();



protected:

	cocos2d::TMXTiledMap* tileMap_;

	cocos2d::Vector<Obstacle*> obstacle_;

	Player* player_;

	cocos2d::Node* dropNode_;

	cocos2d::Layer* nextScene_;

	CameraEffect* mainCamera_;

	HealthBar* healthBar_ = nullptr;

	WeaponUI* weaponUI_ = nullptr;

	bool touchHolding_ = false;

	SettingLayer* settingLayer_;

};

#endif // !__FIGHT_SCENE_H__
