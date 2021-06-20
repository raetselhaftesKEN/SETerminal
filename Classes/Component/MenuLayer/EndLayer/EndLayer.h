#ifndef  _END_LAYER_H_
#define  _END_LAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "Character/Player.h"
#include "Const/Const.h"
#include "../MenuLayer.h"



class EndLayer : public MenuLayer 
{
public:
	static EndLayer* create();

	//void initMusic();

	virtual bool init();

	virtual bool open(bool win = true);



protected:
	

};



#endif