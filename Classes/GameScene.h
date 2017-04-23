#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "MapLayer.h"
#include "ShapeLayer.h"
#include "StatusLayer.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	
	void replayGame(int tag);

	GameScene();
	~GameScene();

private:
	void projection();
	void projectionC();
	Vec2 mconvertToWorldPos(Vec2 pos, Node* node);
	void update(float dt) override;
	void customUpdate(float dt);
	void projectioned();
	void projectionCheck();
	void removeShape();
	void fullLineClean();

	MapLayer* m_mapLayer;
	ShapeLayer* m_shapeLayer;
	StatusLayer* m_statusLayer;
	bool m_isGameOver;
	int m_tmpBSL1;
	int m_tmpBSR1;
};

#endif // __HELLOWORLD_SCENE_H__
