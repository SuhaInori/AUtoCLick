#include "GlobaQApplication.h"

GlobaQApplication::GlobaQApplication(int& argc, char** argv) :
	 QApplication(argc,argv)
{

}

GlobaQApplication::~GlobaQApplication()
{
}

bool GlobaQApplication::notify(QObject*obj, QEvent*e)
{
	const QMetaObject* objMeta = obj->metaObject();
	QString clName = objMeta->className();

	if (e->type()==QEvent::KeyPress)
	{
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
		if (keyEvent->key()==Qt::Key_F1)
		{
			qDebug() << clName;
			qDebug() << "F1";
		}
	}
	else if (e->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
		if (mouseEvent->buttons() == Qt::LeftButton)
			qDebug() << "left";
	}
	
	return QApplication::notify(obj, e);
}

void GlobaQApplication::setWindowInstance(QWidget* wnd)
{
	widget = wnd;
}
