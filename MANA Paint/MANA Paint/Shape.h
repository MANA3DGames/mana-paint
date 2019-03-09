#ifndef SHAPE_H
#define SHAPE_H

#include <Windows.h>
#include "BackBuffer.h"
using namespace MANA3DGames;

static BackBuffer* gBackBuffer = 0;	// The backbuffer we will render onto.

const int gClientWidth = 800;	// Client rectangle dimensions we will use.
const int gClientHeight = 600;	// Client rectangle dimensions we will use.


namespace MANA3DGames
{
	class Shape
	{
	public:
		Shape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);

		virtual ~Shape();

		void setStartPt(const POINT& p0);
		void setEndPt(const POINT& p1);

		virtual void draw(HDC hdc) = 0;


		static void initBackBuffer(HWND hWnd);
		static void deleteBackBuffer();

	protected:
		POINT mPt0;
		POINT mPt1;
		HPEN mhPen;
		HBRUSH mhBrush;

		HPEN mhOldPen;
		HBRUSH mhOldBrush;

	};


	class LineShape : public Shape
	{
	public:
		LineShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);

		void draw(HDC hdc);
	};


	class RectShape : public Shape
	{
	public:
		RectShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);

		void draw(HDC hdc);
	};


	class EllipseShape : public Shape
	{
	public:
		EllipseShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);

		void draw(HDC hdc);
	};

}

#endif	// SHAPE_H