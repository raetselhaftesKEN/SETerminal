/**
* @file Weapon.h
*/

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"
#include "../Item.h"
#include "PlayerAimPoint.h"

/**
*@brief 武器类
*/

class Weapon : public Item
{
public:

	virtual ~Weapon() = default;


	/**
* @brief 生成一个Weapon实例
* @param Weapon实例对应的素材文件名
* @return 指向该Weapon实例的指针
* @孟宇
*/
	static Weapon* create(const std::string& filename);

	/*
	设定武器是否装备为主武器
	@杨孟臻
	*/
	void Active(bool ActivateStatus);

	/*
	武器发射
	@杨孟臻
	*/
	void Attack(cocos2d::Vec2 pos, cocos2d::Vec2 dir);


	/*
	再装填
	@杨孟臻
	*/
	void Reload();

	void PlayerReload();

	/*
恢复武器后坐力
@杨孟臻
*/
	void RecoverRecoil();

	/**
*@brief 绑定物理躯干
* @author 孟宇
*/
	virtual bool bindPhysicsBody() { return true; }

	virtual void interact(){}

	std::string bulletFilename_;	//武器所使用的子弹

	std::string aimPointFilename_;  //准星名称

	PlayerAimPoint* MyAimPoint;
	PlayerAimPoint* ReloadAimPoint;

	PlayerAimPoint* ActiveAimPoint;

protected:



	float ShootingSpeed = 10;//每秒能够attack的次数

	bool CanShoot = true;

	float BulletSpeed = 2000;

	int Accuracy = 97;//1-Accuracy是扩散角度

	float Recoil = 10;//准星上弹程度

	float RecoilRecover = 60;//准星回复  

	int MagazineSize = 30;//弹夹容量

	int CurrentMagazine = 30;

	float ReloadTime = 2;//再装填时间

};

#endif // !__WEAPON_H__
