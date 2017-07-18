#include "MathUtility.h"

bool Dragon::Math::MathUtility::RectInterSect(Rect & rect1, Rect & rect2)
{
	int xOffset = rect2.GetRT().x - rect1.GetRT().x;
	int yOffset = rect2.GetRT().y - rect1.GetRT().y;
	bool xIntersect = false;
	bool yIntersect = false;
	if ((xOffset >= 0 && xOffset <= rect1.GetWidth()) || (xOffset <= 0 && -xOffset <= rect2.GetWidth()))
		xIntersect = true;
	if ((yOffset >= 0 && yOffset <= rect1.GetHeight()) || (yOffset <= 0 && -yOffset <= rect2.GetHeight()))
		yIntersect = true;
	return (xIntersect && yIntersect);
}
