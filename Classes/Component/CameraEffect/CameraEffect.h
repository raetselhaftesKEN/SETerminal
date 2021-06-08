/**
* @file CameraEffect.h
*/

#ifndef __CAMERA_EFFECT_H__
#define __CAMERA_EFFECT_H__

#include "cocos2d.h"
#include "Character/Character.h"
#include "Character/Player.h"

/*
摄像机及其效果
	@杨孟臻
	*/

class CameraEffect :public cocos2d::Camera
{
public:

	//设置或者绑定（如果有）一个正交摄像机
	static CameraEffect* create(cocos2d::Scene* scene);

	//摄像机抖动效果
	void Shake(float Strength, int Count);

	//
	void update(float dt);

	//设置摄像机的目标角色，让摄像机能够跟随其移动
	void LockPlayer(Player* player);

protected:

	cocos2d::Camera* CameraInstance;

	cocos2d::Vec2 OgPos;

	Player* TargetPlayer;
};

#endif 
#pragma once