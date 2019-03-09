#include <Windows.h>
#include <string>
#include <vector>
#include "Shape.h"
#include "resource.h"

using namespace std;


//=========================================================
// Globals.

const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);

HWND ghMainWnd = 0;
HINSTANCE ghAppInst = 0;
HMENU ghMenu = 0;

vector<Shape*> gShapes;
Shape* gShape = 0;

bool gMouseDown = false;

int gCurrPrimSel = ID_PRIMITIVE_LINE;
int gCurrPenColSel = ID_PENCOLOR_BLACK;
int gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
int gCurrPenStyleSel = ID_PENSTYLE_SOLID;
int gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;

LOGPEN gLogPen;
LOGBRUSH gLogBrush;


// Pad window dimensions so that there is room for window borders, caption bar, and menu.
const int gWindowWidth = gClientWidth + 6;
const int gWindowHeight = gClientHeight + 52;

//=========================================================
// Function Prototypes
//=========================================================
bool InitMainWindow();

int Run();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);




//=========================================================
// Name: WinMain
// Desc: Program execution starts here.
//=========================================================
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	// Save handle to application instance.
	ghAppInst = hInstance;

	// Create the main window.
	if (!InitMainWindow())
	{
		MessageBoxA(0, "Window Creation Failed.", "Error", MB_OK);
		return 0;
	}

	// Enter the message loop.
	return Run();
}


/*
// WinMain: Entry point for a Windows application.
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	// Save handle to application instance.
	ghAppInst = hInstance;

	// Step 2: Fill out a WNDCLASS instance.
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghAppInst;
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = TEXT("MyWndClassName");

	// Step 3: Register the WNDCLASS instance with Windows.
	RegisterClass(&wc);

	// Step 4: Create the window, and save handle in globl window handle variable ghMainWnd.
	ghMenu = LoadMenu(ghAppInst, MAKEINTRESOURCE(IDR_MENU1));

	ghMainWnd = CreateWindowExW(WS_EX_CLIENTEDGE, TEXT("MyWndClassName"), TEXT("My Paint Program"), WS_OVERLAPPEDWINDOW, 200, 200, 640, 480, 0, ghMenu, ghAppInst, 0);
	if (ghMainWnd == 0)
	{
		::MessageBoxA(0, "CreateWindow - Failed", 0, 0);
		return false;
	}

	// Step 5: Show and update the window.
	ShowWindow(ghMainWnd, showCmd);
	UpdateWindow(ghMainWnd);

	// Step 6: Enter the message loop and don't quit until a WM_QUIT message is received.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	for (int i = 0; i < gShapes.size(); ++i)
		delete gShapes[i];

	// Return exit code back to operating system.
	return (int)msg.wParam;
}
*/


//=========================================================
// Name: InitMainWindow
// Desc: Creates the main window upon which we will
// draw the game graphics onto.
//=========================================================
bool InitMainWindow()
{
	// Step 2: Fill out a WNDCLASS instance.
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghAppInst;
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = TEXT("MyWndClassName");

	// Step 3: Register the WNDCLASS instance with Windows.
	RegisterClass(&wc);

	// Step 4: Create the window, and save handle in globl window handle variable ghMainWnd.
	ghMenu = LoadMenu(ghAppInst, MAKEINTRESOURCE(IDR_MENU1));

	// WS_OVERLAPPED | WS_SYSMENU: Window cannot be resized and does not have a min/max button.
	ghMainWnd = CreateWindowExW(WS_EX_CLIENTEDGE, TEXT("MyWndClassName"), TEXT("MANA Paint"), WS_OVERLAPPED | WS_SYSMENU, 200, 200, 800, 600, 0, ghMenu, ghAppInst, 0);
	//ghMainWnd = CreateWindowExW(WS_EX_CLIENTEDGE, TEXT("MyWndClassName"), TEXT("My Paint Program"), WS_OVERLAPPEDWINDOW, 200, 200, 640, 480, 0, ghMenu, ghAppInst, 0);
	if (ghMainWnd == 0)
	{
		::MessageBoxA(0, "CreateWindow - Failed", 0, 0);
		return false;
	}

	ShowWindow(ghMainWnd, SW_NORMAL);
	UpdateWindow(ghMainWnd);

	return true;
}

//=========================================================
// Name: Run
// Desc: Encapsulates the message loop.
//=========================================================
int Run()
{
	// Step 6: Enter the message loop and don't quit until a WM_QUIT message is received.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		// IF there is a Windows message then process it.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// ELSE, do game stuff.
		else
		{
			//// Get the backbuffer DC.
			//HDC bbDC = gBackBuffer->getDC();

			//// Clear the entire backbuffer black. This gives up a black background.
			//HBRUSH oldBrush = (HBRUSH)SelectObject(bbDC, GetStockObject(BLACK_BRUSH));
			//Rectangle(bbDC, 0, 0, 800, 600);

			//// Draw the base of the tank--a rectangle surrounding the tank's center position point.
			//SelectObject(bbDC, GetStockObject(DKGRAY_BRUSH));
			//Rectangle(bbDC,
			//	(int)gTankPos.x - 50,
			//	(int)gTankPos.y - 75,
			//	(int)gTankPos.x + 50,
			//	(int)gTankPos.y + 75);

			//// Draw the gun base--an ellipse surrounding the tank's center position point.
			//SelectObject(bbDC, GetStockObject(GRAY_BRUSH));
			//Ellipse(bbDC,
			//	(int)gTankPos.x - 40,
			//	(int)gTankPos.y - 40,
			//	(int)gTankPos.x + 40,
			//	(int)gTankPos.y + 40);

			//// Draw the gun itself--a line from the tank's center position point to the tip of the gun.
			//HPEN oldPen = (HPEN)SelectObject(bbDC, gGunPen);
			//MoveToEx(bbDC, (int)gTankPos.x, (int)gTankPos.y, 0);
			//LineTo(bbDC,
			//	(int)(gTankPos.x + gGunDir.x),
			//	(int)(gTankPos.y + gGunDir.y));

			//// Draw any bullets that where fired.
			//SelectObject(bbDC, GetStockObject(WHITE_BRUSH));
			//SelectObject(bbDC, oldPen);

			//// Bullet velocity is 5X the gun's direction's magnitude.
			//Vec2 bulletVel = gGunDir * 5.0f;
			//list<Vec2>::iterator i = gBulletList.begin();

			//while (i != gBulletList.end())
			//{
			//	// Update the bullet position.
			//	*i += bulletVel * deltaTime;

			//	// Get POINT form.
			//	//POINT p = (POINT)*i;
			//	POINT p;
			//	p.x = i->x;
			//	p.y = i->y;

			//	// Only draw bullet if it is still inside the map boundaries, otherwise, delete it.
			//	if (!PtInRect(&gMapRect, p))
			//		i = gBulletList.erase(i);
			//	else
			//	{
			//		// Draw bullet as a circle.
			//		Ellipse(bbDC,
			//			p.x - 4,
			//			p.y - 4,
			//			p.x + 4,
			//			p.y + 4);
			//		++i; // Next in list.
			//	}
			//}

			//SelectObject(bbDC, oldBrush);

			//// Now present the backbuffer contents to the main window client area.
			//gBackBuffer->present();

			//// Free 20 miliseconds to Windows so we don't hog the system resources.
			//Sleep(20);
		}
	}

	MessageBoxA(0, "Yo!!!", 0, 0);
	
	for (int i = 0; i < gShapes.size(); ++i)
		delete gShapes[i];

	// Return exit code back to operating system.
	return (int)msg.wParam;
}


// Step 1. Define and implement the window procedure.
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Objects for painting.
	HDC hdc = 0;
	PAINTSTRUCT ps;

	// Local POINT variables we will use in some of the case statements.
	POINT p0;
	POINT p1;

	switch (msg)
	{
	case WM_CREATE:
		// This will work when the window being created at the beginnning, this will check the default items
		CheckMenuItem(ghMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_PENCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);

		// init BackBuffer
		Shape::initBackBuffer(hWnd);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//=======================================
			// File Menu
			//=======================================
		case ID_FILE_EXIT:
			DestroyWindow(ghMainWnd);
			return 0;
		case ID_EDIT_UNDO:
			if (gShapes.size() > 0)
			{
				gShapes.pop_back();
				delete gShape;
				gShape = 0;
				InvalidateRect(hWnd, 0, true);
			}
			else
				MessageBoxA(0, "There is no more actions to undo!", 0, 0);
			return 0;
			//=======================================
			// Primitive Types (Shape Types)
			//=======================================
		case ID_PRIMITIVE_LINE:
			CheckMenuItem(ghMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_LINE;
			return 0;
		case ID_PRIMITIVE_RECTANGLE:
			CheckMenuItem(ghMenu, ID_PRIMITIVE_RECTANGLE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_RECTANGLE;
			return 0;
		case ID_PRIMITIVE_ELLIPSE:
			CheckMenuItem(ghMenu, ID_PRIMITIVE_ELLIPSE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_ELLIPSE;
			return 0;
			//=======================================
			// Pen Colors
			//=======================================
		case ID_PENCOLOR_BLACK:
			CheckMenuItem(ghMenu, ID_PENCOLOR_BLACK, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_BLACK;
			gLogPen.lopnColor = BLACK;
			return 0;
		case ID_PENCOLOR_WHITE:
			CheckMenuItem(ghMenu, ID_PENCOLOR_WHITE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_WHITE;
			gLogPen.lopnColor = WHITE;
			return 0;
		case ID_PENCOLOR_RED:
			CheckMenuItem(ghMenu, ID_PENCOLOR_RED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_RED;
			gLogPen.lopnColor = RED;
			return 0;
		case ID_PENCOLOR_GREEN:
			CheckMenuItem(ghMenu, ID_PENCOLOR_GREEN, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_GREEN;
			gLogPen.lopnColor = GREEN;
			return 0;
		case ID_PENCOLOR_BLUE:
			CheckMenuItem(ghMenu, ID_PENCOLOR_BLUE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_BLUE;
			gLogPen.lopnColor = BLUE;
			return 0;
			//=======================================
			// Brush Colors
			//=======================================
		case ID_BRUSHCOLOR_BLACK:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
			gLogBrush.lbColor = BLACK;
			return 0;
		case ID_BRUSHCOLOR_WHITE:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_WHITE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_WHITE;
			gLogBrush.lbColor = WHITE;
			return 0;
		case ID_BRUSHCOLOR_RED:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_RED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_RED;
			gLogBrush.lbColor = RED;
			return 0;
		case ID_BRUSHCOLOR_GREEN:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_GREEN, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_GREEN;
			gLogBrush.lbColor = GREEN;
			return 0;
		case ID_BRUSHCOLOR_BLUE:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLUE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_BLUE;
			gLogBrush.lbColor = BLUE;
			return 0;
			//=======================================
			// Pen Styles
			//=======================================
		case ID_PENSTYLE_SOLID:
			CheckMenuItem(ghMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_SOLID;
			gLogPen.lopnStyle = PS_SOLID;
			return 0;
		case ID_PENSTYLE_DOTTED:
			CheckMenuItem(ghMenu, ID_PENSTYLE_DOTTED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_DOTTED;
			gLogPen.lopnStyle = PS_DOT;
			return 0;
		case ID_PENSTYLE_DASHED:
			CheckMenuItem(ghMenu, ID_PENSTYLE_DASHED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_DASHED;
			gLogPen.lopnStyle = PS_DASH;
			return 0;
			//=======================================
			// Brush Styles
			//=======================================
		case ID_BRUSHSTYLE_SOLID:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;
			gLogBrush.lbStyle = BS_SOLID;
			return 0;
		case ID_BRUSHSTYLE_NULL:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_NULL, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_NULL;
			gLogBrush.lbStyle = BS_NULL;
			return 0;
		case ID_BRUSHSTYLE_DIAGONAL:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_DIAGONAL, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_DIAGONAL;
			gLogBrush.lbStyle = BS_HATCHED;
			gLogBrush.lbHatch = HS_BDIAGONAL;
			return 0;
		case ID_BRUSHSTYLE_CROSS:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_CROSS, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_CROSS;
			gLogBrush.lbStyle = BS_HATCHED;
			gLogBrush.lbHatch = HS_CROSS;
			return 0;
		}
		// Handle left mouse button click message.
	case WM_LBUTTONDOWN:
		// Capture the mouse (we still get mouse input even after the mouse cursor moves off the client area.
		SetCapture(hWnd);
		gMouseDown = true;

		// Point that was clicked is stored in the lParam.
		p0.x = LOWORD(lParam);
		p0.y = HIWORD(lParam);

		// We don’t know the end point yet, so set to zero.
		p1.x = 0;
		p1.y = 0;

		// Create the shape based on what shape the user has selected in the menu.
		switch (gCurrPrimSel)
		{
		case ID_PRIMITIVE_LINE:
			gShape = new LineShape(p0, p1, gLogPen, gLogBrush);
			break;
		case ID_PRIMITIVE_RECTANGLE:
			gShape = new RectShape(p0, p1, gLogPen, gLogBrush);
			break;
		case ID_PRIMITIVE_ELLIPSE:
			gShape = new EllipseShape(p0, p1, gLogPen, gLogBrush);
			break;
		};
		return 0;
		// Message sent whenever the mouse moves.
	case WM_MOUSEMOVE:
		if (gMouseDown)
		{
			// Current mouse position is stored in the lParam.
			p1.x = LOWORD(lParam);
			p1.y = HIWORD(lParam);

			// Update the end point of the current temporary shape based on the mouse position.
			gShape->setEndPt(p1);

			// Repaint the window so the temporary shape is redrawn interactively as the mouse moves.
			//InvalidateRect(hWnd, 0, true);
		}
		return 0;
	case WM_LBUTTONUP:
		// Release the captured mouse when the left mouse button is lifted.
		ReleaseCapture();
		gMouseDown = false;

		// Current mouse position is stored in the lParam.
		p1.x = LOWORD(lParam);
		p1.y = HIWORD(lParam);

		// Update the end point of the current temporary shape based on the mouse position.
		gShape->setEndPt(p1);

		// The user lifted the left mouse button, so the shape becomes permanent, so add it to the shape container.
		gShapes.push_back(gShape);

		// Repaint the window so the new permanent shape will be displayed.
		InvalidateRect(hWnd, 0, true);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Draw all the permenent shapes.
		for (int i = 0; i < gShapes.size(); ++i)
			gShapes[i]->draw(hdc);

		// Only draw temporary shape if the mouse is down.
		if (gMouseDown)
			gShape->draw(hdc);

		EndPaint(hWnd, &ps);
		// Handle key down message.
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		if (::MessageBoxA(hWnd, "Are you sure want to quit this awesome program?!!!", "You want to QUIT!!!!!!!!", MB_YESNO) == IDYES)
			DestroyWindow(ghMainWnd);
		return 0;
		// Handle destroy window message.
	case WM_DESTROY:
		Shape::deleteBackBuffer();
		PostQuitMessage(0);
		return 0;
	}
	// Forward any other messages we didn't handle to the default window procedure.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

