#include "Block.h"

Block::Block()
{
	m_lineI = 0;
	m_rowI = 0;
	m_shadowSprite = NULL;
	m_sprite = Sprite::create("block.png");
	auto sSize = m_sprite->getContentSize();
	setContentSize(sSize);
	setAnchorPoint(Point(0.5, 0.5));
	addChild(m_sprite);
	m_sprite->setPosition(Point(sSize.width / 2, sSize.height / 2));
}

bool Block::init()
{
	if (!Node::init())
		return false;
	return true;
}

Block::~Block()
{
}

void Block::setSpriteColor(Color3B color)
{
	m_sprite->setColor(color);
}

void Block::setLRIndex(int lI, int rI)
{
	m_lineI = lI;
	m_rowI = rI;
}
bool Block::addShadowSprite(Sprite* sprite, int opacity)
{
	if (!sprite || m_shadowSprite){
		return false;
	}
	auto sSize = getContentSize();
	sprite->setPosition(Point(sSize.width / 2, sSize.height / 2));
	sprite->setOpacity(opacity);
	// 有一个问题，当m_shadowSprite的纹理与m_sprite的纹理不一致时，添加
	// 一个shadow会使gl calls +2.但是在这个node的父类中添加gl calls +1
	// 而纹理相同时 +0
	// 原因是?
	addChild(sprite);
	m_shadowSprite = sprite;
	return true;
}

void Block::removeShadowSprite()
{
	if (!m_shadowSprite)
		return;
	removeChild(m_shadowSprite);
	m_shadowSprite = NULL;
}

Sprite* Block::getShadowSprite()
{
	return m_shadowSprite;
}