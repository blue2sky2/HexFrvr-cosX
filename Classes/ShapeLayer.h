#ifndef SHAPE_LAYER_H_
#define SHAPE_LAYER_H_

#include "cocos2d.h"
#include "BlockShape.h"
using namespace cocos2d;

class ShapeLayer : public Layer
{
public:
	ShapeLayer();
	~ShapeLayer();
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(ShapeLayer);
	BlockShape* m_currentShapes[3];
	void updateShape(int oriI, float oriX, float oriY);
	void updateShapes();
private:
	BlockShape* m_shapes[21];
	int m_refer[21];
	Color3B m_colors[7];
};

#endif