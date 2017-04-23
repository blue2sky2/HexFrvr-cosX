#ifndef MAP_LAYER_H_
#define MAP_LAYER_H_

#include "cocos2d.h"
#include "Block.h"
#include "Global.h"

using namespace cocos2d;

class MapLayer : public cocos2d::LayerColor{
public:
	virtual bool init();
	MapLayer();
	~MapLayer();
	// implement the "static create()" method manually
	CREATE_FUNC(MapLayer);

	Block* m_blocks[gMapLineM][gMapRowM];
	int m_Map[gMapLineM][gMapRowM];
	Color3B m_color;
	void addShadowForBlock(Block* target);
	void removeShadowForBlock(Block* target);
	void mapClear();

private:
	Sprite* m_shadowS[4];
	int m_shadowV[4];
};

#endif