#include "Candle.h"





void Candle::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void Candle::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x ;
	b = y ;
	/*r = x + CANDLE_BBOX_WIDTH;
	b = y + CANDLE_BBOX_HEIGHT;*/
}
