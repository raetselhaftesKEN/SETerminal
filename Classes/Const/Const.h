/**
* @file Const.h
*/

#ifndef __CONST_H__
#define __CONST_H__
#include <vector>
using namespace std::string_literals;

//masks
#define MONSTER_CONTACT_MASK 0x03					//0100
#define MONSTER_CATEGORY_MASK 0x01				//0001

#define MONSTER_BULLET_CONTACT_MASK 0x04	//0100
#define MOSNTER_BULLET_CATEGORY_MASK 0x03	//0011

#define PLAYER_CONTACT_MASK 0x01						//0001
#define PLAYER_CATEGORY_MASK 0x05					//0101

#define PLAYER_BULLET_CONTACT_MASK 0x02		//0010
#define PLAYER_BULLET_CATEGORY_MASK 0x05		//0101

#define ITEM_CONTACT_MASK 0x01							//0001
#define ITEM_CATEGORY_MASK 0x05						//0101

#define OBSTACLE_CONTACT_MASK 0x0f					//1111
#define OBSTACLE_CATEGORY_MASK 0x0f				//1111

//tags
#define PLAYER_TAG 100
#define PLAYER_BULLET_TAG 10
#define MONSTER_TAG 200
#define MONSTER_BULLET_TAG 20
#define ITEM_TAG 50
#define OBSTACLE_TAG 60
#define FIGHT_SCENE_TAG 70
#define SUVR_CNT_TAG 80

//default values
#define PLAYER_MAX_HEALTH 100
#define MONSTER_MAX_HEALTH 10
#define PLAYER_DEFAULT_SHIELD_PROTECTION 0.75f
#define PLAYER_DEFAULT_SHIELD 100
#define PLAYER_DEFAULT_MAX_SHIELD 100
#define MONSTER_DEFAULT_SHIELD_PROTECTION 0.f
#define MONSTER_DEFAULT_SHIELD 0
#define MONSTER_DEFAULT_MAX_SHIELD 0
#define PLAYER_DEFAULT_MOVE_SPEED 400.f
#define MONSTER_DEFAULT_MOVE_SPEED 2.f
#define MEDKIT_DEFAULT_RECOVERABILITY 30
#define MEDKIT_MAX_NUM 3
#define BULLET_MAX_NUM 180
#define DISCARD_ITEM_DISTANCE 60
#define CLIP_BULLET_NUM 90
#define BOUND_XMIN 1104
#define BOUND_XMAX 4112
#define BOUND_YMIN 464
#define BOUND_YMAX 4336
#define BOUND_XMID 2608
#define BOUND_YMID 2400
#define GATE_POSITION_XMIN 1440
#define GATE_POSITION_XMAX 1568
#define GATE_POSITION_YMIN 3840

//network
#define PLAYER_JOIN 1
#define PLAYER_QUIT 2
#define PLAYER_JOIN_COMMAND "A player joined"
#define PLAYER_QUIT_COMMAND "A player quitted"

enum bulletType_ { type556, type762, type9mm };
const std::vector<std::string> bulletName{ "Bullet-556", "Bullet-762", "Bullet-9mm" };

enum weaponType_ { AK47, AKM, FAL, M4, MP5, SVD };

enum enemyType_ { Default_Shoot, Default_Shoot_Fast, Default_Shoot_Elite };

#endif  //!__CONST_H__