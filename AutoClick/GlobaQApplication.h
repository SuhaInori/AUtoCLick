#pragma once
#include <QtWidgets/QApplication>
#include <QKeyEvent> 
#include <qdebug.h>

//#define QDBG qDebug()<<_FILE_<<_FUNCTION_<<_LINE_<<
class GlobaQApplication:public QApplication
{
public:
	GlobaQApplication(int& argc, char** argv);
	~GlobaQApplication();

	bool notify(QObject*, QEvent*);
	void setWindowInstance(QWidget* wnd);
private:
	QWidget* widget;
};

