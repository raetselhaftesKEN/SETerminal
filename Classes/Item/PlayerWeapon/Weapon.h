/**
* @file Weapon.h
*/

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"
#include "../Item.h"
#include "PlayerAimPoint.h"
#include "Const/Const.h"

/**
*@brief 武器类
*/

class Weapon : public Item
{
public:

	virtual ~Weapon() = default;

	int getAccuracy();
	void setAccuracy(int accuracy);

	float getRecoil();
	void setRecoil(float recoil);

	float getRecoiRecoverl();
	void setRecoilRecover(float recoilRecover);

	float getReloadTime();
	void setReloadTime(float reloadTime);

	int getMagazineSize();
	void setMagazineSize(int magSize);

	/**
* @brief 生成一个Weapon实例
* @param Weapon实例对应的素材文件名
* @return 指向该Weapon实例的指针
* @孟宇
*/
	static Weapon* create(const std::string& filename);
	static Weapon* create(weaponType_ type);
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

	int getCurrentMagazine();
	/*
	再装填
	@杨孟臻
	*/
	void Reload();
	void Reload(std::vector<int>& BulletStock);

	void PlayerReload(std::vector<int>& BulletStock);

	void ReloadingStatusReset();

	void Reset();
	/*
恢复武器后坐力
@杨孟臻
*/
	void RecoverRecoil(float Boost = 1);

	/**
*@brief 绑定物理躯干
* @author 孟宇
*/
	virtual bool bindPhysicsBody();

	virtual void interact();

	PlayerAimPoint* getMyAimPoint();
	void setMyAimPoint(PlayerAimPoint*);

	PlayerAimPoint* getReloadAimPoint();
	void setReloadAimPoint(PlayerAimPoint*);

	std::string bulletFilename_;	//武器所使用的子弹

	std::string aimPointFilename_;  //准星名称

	PlayerAimPoint* ActiveAimPoint;

	bulletType_ TypeOfBullet = bulletType_::type762;

	float BulletDamage = 12;

	float BulletSpeed = 2000;



protected:

	int Accuracy = 95;//1-Accuracy是扩散角度

	float Recoil = 15;//准星上弹程度

	float RecoilRecover = 60;//准星回复  

	int MagazineSize = 30;//弹夹容量

	float ReloadTime = 2;//再装填时间

	float ShootingSpeed = 10;//每秒能够attack的次数

	bool CanShoot = true;

	float MaxRecoil = 250;

	int CurrentMagazine = 30;

	PlayerAimPoint* MyAimPoint;
	PlayerAimPoint* ReloadAimPoint;

};

#endif // !__WEAPON_H__
