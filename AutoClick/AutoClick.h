#pragma once

#include <QtWidgets/QWidget>
#include "ui_AutoClick.h"
#include <qfile.h>
#include "Ini_Read_Write.h"
#include "osuFileAutoClick.h"
#include <QTime>

//#include "keymonitor.h"
//#include "Capturer.h"

#include <thread>
#include "customClick.h"

//#define INIDEBUG


class AutoClick : public QWidget
{
    Q_OBJECT

public:
    AutoClick(QWidget *parent = nullptr);
    ~AutoClick();

private:
    Ui::AutoClickClass ui;
    void Key_Capturer();
    void DelCClick();
    void clearCClick();
    void getPatn();
    void readIniFile();
    void setIniValue();
    void readIniValue();
    void readOsuFile();
    void displayOsuFile();
    bool osuFileClick;
    QString Text;
    std::thread *ts;
    customClick cClick;
    QString filePath;
    QString fileName;
    QFile *file_Open_into;
    CMyINI* iniFile;
    osuFileAutoClick* osuFile;
};
