#ifndef GAME_OVER_LAYER_H_
#define GAME_OVER_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;
using namespace cocos2d;

class GameOverLayer : public Layer
{
public:
	CREATE_FUNC(GameOverLayer);
	virtual bool init();
	void retryCb(int tag);
	void exitGameCb();
};

#endif