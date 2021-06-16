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
#include "Component/Functional/SurvivorCounter.h"
#include "Component/Functional/SETimer.h"

using namespace cocos2d;

class FightScene : public cocos2d::Scene
{
public:

	FightScene(TMXTiledMap*, TMXTiledMap*, TMXTiledMap*, const cocos2d::Vector<Obstacle*>& obstacle, const int& serial);


	static FightScene* create(TMXTiledMap*, TMXTiledMap*, TMXTiledMap*, const cocos2d::Vector<Obstacle*>& obstacle, const int& serial);


	cocos2d::Scene* createScene();


	void bindPlayer(Player* player);


	void setCamera();


	void setObstacle();


	void setUI();


	void setOperationListener();


	void setPhysicsListener();


	virtual bool init() override;
	

	void generateMonster(float dt);


	void goToNextScene();


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

	void update(float dt) override;

	void monsterDestroyed();

	int RemainingSurvivor = 20;

protected:

	cocos2d::TMXTiledMap* tileMap1_;
	cocos2d::TMXTiledMap* tileMap2_;
	cocos2d::TMXTiledMap* tileMap3_;

	cocos2d::Vector<Obstacle*> obstacle_;

	Player* player_;

	cocos2d::Node* dropNode_;

	CameraEffect* mainCamera_;

	HealthBar* healthBar_ = nullptr;

	WeaponUI* weaponUI_ = nullptr;

	SETimer* timer_ = nullptr;

	SurvivorCounter* survivorCounter_ = nullptr;

	bool touchHolding_ = false;

	int sceneSerial_ = 1;

	bool clear_ = false;

	int MonsterToSpawn = 20;
	int SpawnedMonster = 0;
};

#endif // !__FIGHT_SCENE_H__
