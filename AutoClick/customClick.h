#pragma once
#include <qvector.h>
#include <vector>
#include "atltypes.h"
#include <windows.h>
#include <winuser.h>
#include <qdebug.h>//#include <qdebug.h>
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? true : false)
//#define INIDEBUG
//#include "AutoClick.h"
class customClick
{
public:
	customClick();
	~customClick();
	void add_Coordinates(int &,int &,int&);
	void delete_Coordinates();
	void clear_Coordinates();
	void startCustomClick();
	void SetKeep(bool);
	int GetMaxIndex();
	void find_Coordinates(int, int*);
	void fileReadAdd(int, int, int);
private:
	std::vector<int> coordinateX;
	std::vector<int> coordinateY;
	std::vector<int> sleepTime;
	int maxIndex;
	bool keepClick;
	CPoint m_mouse;

};

