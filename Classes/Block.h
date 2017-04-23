#ifndef BLOCK_H_
#define BLOCK_H_

#include "cocos2d.h"
using namespace cocos2d;

class Block : public Node
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Block);
	Block();
	~Block();
	virtual bool init();
	void setSpriteColor(Color3B color);
	void setLRIndex(int lI, int rI);
	bool addShadowSprite(Sprite* sprite, int opacity);
	void removeShadowSprite();
	Sprite* getShadowSprite();

	//在地图中的行列索引
	int m_lineI;
	int m_rowI;
private:
	//绑定的精灵
	Sprite* m_sprite;
    //遮罩精灵
	Sprite* m_shadowSprite;
};
#endif 