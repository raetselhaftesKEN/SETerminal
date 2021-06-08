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

	//场景中正在渲染的正交摄像机
	cocos2d::Camera* CameraInstance;

	//UI专用的摄像机，仅渲染CameraMask为2的物体 如果要把某物体作为UI显示，在创建物体后加上  XXX->setCameraMask(2, true);   即可
	cocos2d::Camera* UICameraInstance;

	cocos2d::Vec2 OgPos;

	Player* TargetPlayer;
};

#endif 
#pragma once