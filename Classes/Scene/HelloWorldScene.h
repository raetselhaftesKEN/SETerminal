/**
* @file HelloWorldScene.h
*/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

/**
* @brief 标记角色，怪物和飞镖的tag
*/
#define ME 100
#define ENEMY 200
#define ME_BULLET 10
#define ENEMY_BULLET 20

#include "cocos2d.h"
#include "./Character/Player.h"

class HelloWorld : public cocos2d::Scene
{
    Player* player_;
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


    /**
* @brief 响应场景内的物理碰撞
*/
    bool onContactBegan(cocos2d::PhysicsContact& physicsContact);


    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
