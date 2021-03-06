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
#include "Component/MenuLayer/SettingLayer/SettingLayer.h"
#include "Component/MenuLayer/EndLayer/EndLayer.h"

class SettingLayer;

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


	void globalPromptDisplay(const std::string&, int type = 1);


	cocos2d::Vector<Obstacle*> getObstacles();


	static bool isInBound(cocos2d::Vec2);
	static bool ifCollision(cocos2d::Vec2);

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

	void update(float) override;

	void monsterDestroyed();

	int RemainingSurvivor = 20;

	void buildSettingBtn();

	SettingLayer* settingLayer_;
	EndLayer* endLayer_;

	void airDrop(float dt);

	static cocos2d::Vec2 getRandomPosition();

	static bool& getShootMusicStatus();

	int monsterToSpawn();

protected:

	cocos2d::TMXTiledMap* tileMap1_;
	cocos2d::TMXTiledMap* tileMap2_;
	cocos2d::TMXTiledMap* tileMap3_;

	cocos2d::Vector<Obstacle*> obstacle_;

	Player* player_;

	cocos2d::Node* dropNode_ = nullptr;

	CameraEffect* mainCamera_;

	HealthBar* healthBar_ = nullptr;

	WeaponUI* weaponUI_ = nullptr;

	SETimer* timer_ = nullptr;

	SurvivorCounter* survivorCounter_ = nullptr;

	bool touchHolding_ = false;

	int sceneSerial_ = 1;

	int MonsterToSpawn = 20;
	int SpawnedMonster = 0;
	int MaxMonsterInScene = 3;
	int MonsterInScene = 0;

	static bool isShootMusicPlaying_;

	cocos2d::Sprite* ToxicFog;
	cocos2d::Label* ToxicFogWarn;

	float ToxicFogMarch = 0.7f;
	float ToxicFogMax = 2400;

	int ToxicFogDamage = 10;
	float ToxicFogInterval = 3;

	bool ToxicFogCanDamage = true;

};

#endif // !__FIGHT_SCENE_H__
