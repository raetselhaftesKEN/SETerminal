/**
* @file HelloWorldScene.h
*/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

/**
* @brief 标记角色，怪物和飞镖的tag
*/


#include "cocos2d.h"
#include "Character/Player.h"
#include "Character/Monster.h"
#include "Component/HealthBar/HealthBar.h"
#include "Const/Const.h"

class HelloWorld : public cocos2d::Scene
{
private:
    Player* player_;
    //地图类,对应相关的tmx文件
    HealthBar* healthBar_;
    cocos2d::TMXTiledMap* _tileMap;
    //地图中的一个层
    cocos2d::TMXLayer* _background;
public:

    /**
* @brief 创建场景实例
* @return 指向生成的场景的指针
*/
    static cocos2d::Scene* createScene();


    /**
* @brief 初始化场景
* @return 初始化是否成功
*/
    virtual bool init();


    /**
* @brief 生成怪物
*/
    void addMonster(float dt);


    /**
* @brief 响应场景内的屏幕触摸（即鼠标单击）
*/
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent);


    void onMouseMove(cocos2d::EventMouse* mouse);


    /**
* @brief 响应场景内的物理碰撞，击中敌人后，将敌人删除
*/
    bool onContactBegan(cocos2d::PhysicsContact& physicsContact);


    bool onContactSeparated(cocos2d::PhysicsContact& physicsContact);


    void contactBetweenPlayerAndItem(Player* player, Item* Item);
    void contactBetweenPlayerAndBullet(Player* player, cocos2d::Sprite* bullet);
    void contactBetweenMonsterAndBullet(Monster* monster, cocos2d::Sprite* bullet);

    //设置按钮按键
    void buildSettingBtn();


    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);


};

#endif // __HELLOWORLD_SCENE_H__
