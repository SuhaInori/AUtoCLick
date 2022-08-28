#pragma once
#include <vector>
#include <qdebug.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <minwindef.h>
#include "Ini_Read_Write.h"
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif
#define OSUNAME "osu!"
#define OSULEFT 1/6
#define OSUTOP 1/12
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? true : false)
typedef struct 
{
	std::vector<int> xAdd;
	std::vector<int> yAdd;
	std::vector<int> delay;
}click;
/*
* GetWindowRect 获取窗口大小
*/

class osuFileAutoClick
{

public:
	osuFileAutoClick(std::string path);
	~osuFileAutoClick();
	//void reread_File();
	void reSetPath(std::string path);
	click_Node getClick(int);
	int getMaxObjects();
	void startOsuFileAutoClick();
	void PrintMessage();
	bool windowsSet();
private:
	void readOsuFile();
	std::string path;
	click click_node;
	int maxObjects;
	HWND osuHwnd;
	double GetHorzScale();
	double horzScale;
	int leftDistance, topDistance, osuleft, osutop;
	float osuPointProportionX, osuPointProportionY;
	bool fullWindow;
};

