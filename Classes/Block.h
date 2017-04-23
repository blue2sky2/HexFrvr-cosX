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

	//�ڵ�ͼ�е���������
	int m_lineI;
	int m_rowI;
private:
	//�󶨵ľ���
	Sprite* m_sprite;
    //���־���
	Sprite* m_shadowSprite;
};
#endif 