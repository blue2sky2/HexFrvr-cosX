#ifndef BLOCK_SHAPE_H_
#define BLOCK_SHAPE_H_

#include "cocos2d.h"
#include "Block.h"
using namespace cocos2d;

class BlockShape : public Node
{
public:
	BlockShape(int shape, Color3B color = Color3B::WHITE);
	~BlockShape();
	void setColor(Color3B);
	void plusSpace(float space);
	void runScaleAction();
	/*
		�����ɷ���ʱ����¼���ʼλ�ã��Ա��ƻ�
	*/
	void setOriPos(float posx, float posy, bool setitpos = false);

	int m_shape;
	Color3B m_color;
	float m_oriPosX;
	float m_oriPosY;
	Block* m_blocks[4];
	/*
		�¼������е�ǰ������target
	*/
	static BlockShape* sTarget;
	static bool sTouchEnd;
	static bool sTouchBegan;

private:
	void setBlock(Block* block, int lI, int rI);
	void generateShape(bool onlyHandlePos = false);
	void addTouchEvent();

	float m_space;
	//Ĭ�ϵ�space
	const float m_dspace = 2;
};
#endif