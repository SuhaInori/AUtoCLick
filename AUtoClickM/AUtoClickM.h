#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AUtoClickM.h"
#include "Timer.h"
#include <QTime>
#include <QString>
#include <Windows.h>
#include <qdebug.h>
#include <vector>
#include "IniW_R.h"
#include "CokFileR_W.h"
#include <windows.h>
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? true : false)

class AUtoClickM : public QMainWindow
{
    Q_OBJECT

public:
    AUtoClickM(QWidget *parent = nullptr);
    ~AUtoClickM();

    void updateMsg(std::string msg);

private:
    Ui::AUtoClickMClass ui;
    //Timer::CTimer* timer;
    std::thread* ts;
    std::string FilePath;
    IniFile::IniW_R* file;
    CokFile::CokFileR_W* cok;
    std::vector<CokFile::CokCC> cokVect;
    Timer::CTimer *timer;

    int time;
    bool isTimerEnable;
    bool clickList;
    void ReadIniFile();
    void SetIniFile();
    
    void Key_Capturer();
    void GetPath();

    void ReadCokFile();

    void Remove();
    void Clear();

    void ItemChangedC();
    void Save();

    void  StartAutoClick(std::vector<CokFile::CokCC>);
    void static AutoClick(std::vector<CokFile::CokCC>, unsigned int& x);
   
    void static KeyDown();
};
