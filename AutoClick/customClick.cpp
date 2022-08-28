#include "customClick.h"

customClick::customClick() :maxIndex(0), keepClick(false)
{
}

customClick::~customClick()
{
}

void customClick::add_Coordinates(int &x,int&y,int &sleep)
{
	if (GetCursorPos(&m_mouse)) 
	{
#ifdef INIDEBUG
		qDebug() << "添加中---"<<x<<" "<<y<<" "<<sleep;
#endif
		x = m_mouse.x;
		y = m_mouse.y;
		coordinateX.push_back(m_mouse.x);
		coordinateY.push_back(m_mouse.y);
		sleepTime.push_back(sleep);
		//qDebug() << sleep<<"  "<< sleepTime[sleepTime.size()-1];

		maxIndex++;
#ifdef INIDEBUG
		qDebug() << "添加完成" << coordinateX[maxIndex-1] << " " << coordinateY[maxIndex-1] << " " << sleepTime[maxIndex-1];
#endif
	}
}

void customClick::delete_Coordinates()
{
	if (maxIndex <= 0)
		return;
	coordinateX.pop_back();
	coordinateY.pop_back();
	sleepTime.pop_back();
	maxIndex--;
}

void customClick::clear_Coordinates()
{
	coordinateX.clear();
	coordinateY.clear();
	sleepTime.clear();
	maxIndex = 0;
}

void customClick::startCustomClick()
{
	do
	{
#ifdef INIDEBUG
		qDebug() << "进入录制点击";
#endif
		for (int i = 0; i < maxIndex; i++)
		{
			SetCursorPos(coordinateX[i], coordinateY[i]);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			if (KEY_DOWN(VK_F9))
				return;
			Sleep(sleepTime[i]);
		}
	} while (keepClick);
}

void customClick::SetKeep(bool keep)
{
	keepClick = keep;
}

int customClick::GetMaxIndex()
{
	return maxIndex;
}

 void customClick::find_Coordinates(int index,int *arr)
{
	 if (index >= maxIndex)
		 return;
	 arr[0] = coordinateX[index];
	 arr[1] = coordinateY[index];
	 arr[2] = sleepTime[index];
	// qDebug() << arr[2];
}

 void customClick::fileReadAdd(int x, int y , int sleep)
 {
	 coordinateX.push_back(x);
	 coordinateY.push_back(y);
	 sleepTime.push_back(sleep);
	 //qDebug() << sleep<<"  "<< sleepTime[sleepTime.size()-1];

	 maxIndex++;
 }
