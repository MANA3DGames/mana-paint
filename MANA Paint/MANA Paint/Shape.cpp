#include "Shape.h"
using namespace MANA3DGames;

#pragma region Shape Implementation

void Shape::initBackBuffer(HWND hWnd)
{
	// Create the backbuffer.
	gBackBuffer = new BackBuffer(hWnd, gClientWidth, gClientHeight);
}

void Shape::deleteBackBuffer()
{
	delete gBackBuffer;
}

Shape::Shape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb)
{
	mPt0.x = u.x;
	mPt0.y = u.y;
	mPt1.x = v.x;
	mPt1.y = v.y;

	mhPen = CreatePenIndirect(&lp);
	mhBrush = CreateBrushIndirect(&lb);

	mhOldPen = 0;
	mhOldBrush = 0;
}

Shape::~Shape()
{
	DeleteObject(mhPen);
	DeleteObject(mhBrush);
}

void Shape::setStartPt(const POINT& p0)
{
	mPt0 = p0;
}

void Shape::setEndPt(const POINT& p1)
{
	mPt1 = p1;
}

#pragma endregion


#pragma region LineShape Implementation

LineShape::LineShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb)
: Shape(u, v, lp, lb)
{

}

void LineShape::draw(HDC hdc)
{
	// Select the current pen and brush.
	mhOldPen = (HPEN)SelectObject(hdc, mhPen);
	mhOldBrush = (HBRUSH)SelectObject(hdc, mhBrush);

	// Draw the line.
	MoveToEx(hdc, mPt0.x, mPt0.y, 0);
	LineTo(hdc, mPt1.x, mPt1.y);

	// Restore the old pen and brush.
	SelectObject(hdc, mhOldPen);
	SelectObject(hdc, mhOldBrush);
}

#pragma endregion


#pragma region RectShape Implementation

RectShape::RectShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb)
: Shape(u, v, lp, lb)
{

}

void RectShape::draw(HDC hdc)
{
	// Select the current pen and brush.
	mhOldPen = (HPEN)SelectObject(hdc, mhPen);
	mhOldBrush = (HBRUSH)SelectObject(hdc, mhBrush);

	// Draw the rectangle.
	Rectangle(hdc, mPt0.x, mPt0.y, mPt1.x, mPt1.y);

	// Select the old pen and brush.
	SelectObject(hdc, mhOldPen);
	SelectObject(hdc, mhOldBrush);
}

#pragma endregion


#pragma region EllipseShape Implementation

EllipseShape::EllipseShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb)
: Shape(u, v, lp, lb)
{

}

void EllipseShape::draw(HDC hdc)
{
	// Select the current pen and brush.
	mhOldPen = (HPEN)SelectObject(hdc, mhPen);
	mhOldBrush = (HBRUSH)SelectObject(hdc, mhBrush);

	// Draw the ellipse.
	Ellipse(hdc, mPt0.x, mPt0.y, mPt1.x, mPt1.y);

	// Restore the old pen and brush.
	SelectObject(hdc, mhOldPen);
	SelectObject(hdc, mhOldBrush);
}

#pragma endregion
