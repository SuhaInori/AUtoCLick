#include "osuFileAutoClick.h"
#include <WinUser.h>

osuFileAutoClick::osuFileAutoClick(std::string path):path(path), maxObjects(0)
{
#ifdef INIDEBUG
	qDebug() << "���캯��=========";
	std::cout << "���캯��=========" << std::endl;
#endif
	readOsuFile();
	horzScale = GetHorzScale();
}

osuFileAutoClick::~osuFileAutoClick()
{

}


void osuFileAutoClick::readOsuFile()
{
	int tempT = 0;
#ifdef INIDEBUG
	qDebug() << "��ʼ��ȡ�ļ�";
	std::cout << "��ʼ��ȡ�ļ�"<<std::endl;
#endif
	bool isOnHit = false;
	std::ifstream in_conf_file(path.c_str());
	if (!in_conf_file)
	{
		MessageBox(NULL, TEXT("�ļ���ʧ��"), TEXT("����"), MB_OK);
		return;
	}
	std::string str_line = "";
#ifdef INIDEBUG
	qDebug() << "��ʼ��ȡ�ļ�";
	std::cout << "��ʼ�����ļ�" << std::endl;
#endif
	while (getline(in_conf_file, str_line))
	{
		click_Node temp;
		
		if (strstr(str_line.c_str(), "[HitObjects]") != NULL && !isOnHit)
		{
#ifdef INIDEBUG
			qDebug() << "��ȡ��object" << maxObjects;
#endif
			isOnHit = true;
			continue;
		}


		if (isOnHit)
		{
#ifdef INIDEBUG
			qDebug() << "��ʼ��ȡ�ļ�" << maxObjects;
#endif
			temp=CMyINI::StringSplit(str_line, ",");
			click_node.xAdd.push_back(temp.x) ;
			click_node.yAdd.push_back(temp.y);
			if (maxObjects==0)
			{
				tempT = temp.time;
				maxObjects++;
				continue;
			}
			click_node.delay.push_back(temp.time - tempT);
			tempT = temp.time;
			maxObjects++;
		}
	}
	click_node.delay.push_back(40);
}

bool osuFileAutoClick::windowsSet()
{
#ifdef INIDEBUG
	qDebug() << "����osu";
#endif
	osuHwnd = FindWindow(NULL, TEXT(OSUNAME));
	if (osuHwnd==NULL)
	{
		MessageBox(NULL, TEXT("osu δ�򿪣�"), TEXT("����"), MB_OK);
		return false;
	}
#ifdef INIDEBUG
	qDebug() << "�ҵ�osu";
#endif
	RECT rect;
	GetWindowRect(osuHwnd, &rect);
	int w = (rect.right - rect.left)/horzScale, h = (rect.bottom - rect.top)/horzScale;
	osuleft = w * OSULEFT; osutop = h * OSUTOP;
	osuPointProportionX = (double)(w - 2 * osuleft) / 512; osuPointProportionY = (double)(h - 2 * osutop) / 384;
	
	HWND hd = GetDesktopWindow();
	GetWindowRect(hd, &rect);
	int ww = (rect.right - rect.left)/horzScale,wh = (rect.bottom - rect.top)/horzScale;

	if (w/ww==1&&h/wh==1)
		fullWindow = true;
	else
	{
		leftDistance = (ww - w) / 2;
		topDistance = (wh - h) / 2;
	}
#ifdef INIDEBUG
	PrintMessage();
#endif
	return true;
}
/// <summary>
/// ��ȡ��Ļ����
/// </summary>
/// <returns></returns>
double osuFileAutoClick::GetHorzScale()
{
	// ��ȡ���ڵ�ǰ��ʾ�ļ�����
	HWND hWnd = GetDesktopWindow();//������Ҫ�����滻���Լ�����ľ�� 
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	// ��ȡ�������߼������߶�
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
	int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

	// ��ȡ��������������߶�
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;
	int cyPhysical = dm.dmPelsHeight;

	//���ű�������
	double horzScale = ((double)cxPhysical / (double)cxLogical);
	double vertScale = ((double)cyPhysical / (double)cyLogical);

	//�������ű�
	return horzScale;
}

void osuFileAutoClick::reSetPath(std::string path)
{
	this->path = path;
	maxObjects = 0;
	click_node.delay.clear();
	click_node.xAdd.clear();
	click_node.yAdd.clear();
	readOsuFile();
}

/// <summary>
/// ���Ҷ�Ӧ�ڵ��click
/// </summary>
/// <param name="">����</param>
/// <returns></returns>
click_Node osuFileAutoClick::getClick(int index)
{
	click_Node temp;
	temp.x = click_node.xAdd[index];
	temp.y = click_node.yAdd[index];
	temp.time = click_node.delay[index];
	return temp;
}

int osuFileAutoClick::getMaxObjects()
{
	return maxObjects;
}

void osuFileAutoClick::startOsuFileAutoClick()
{
	if (fullWindow)
	{
		for (int i = 0; i < maxObjects; i++)
		{
			SetCursorPos(click_node.xAdd[i]* osuPointProportionX+osuleft,
				click_node.yAdd[i]* osuPointProportionY+osutop);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			if (KEY_DOWN(VK_F9))
				return;
			Sleep(click_node.delay[i]);
		}
	}
	else
	{
		for (int i = 0; i < maxObjects; i++)
		{
			SetCursorPos(click_node.xAdd[i] * osuPointProportionX + osuleft+leftDistance,
				click_node.yAdd[i] * osuPointProportionY + osutop+topDistance);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			if (KEY_DOWN(VK_F9))
				return;
			Sleep(click_node.delay[i]);
		}
	}
}

void osuFileAutoClick::PrintMessage()
{
	qDebug() << "�Ƿ�ȫ����" << fullWindow << "���ű�:" << horzScale;
	qDebug() << "x�������ţ�" << osuPointProportionX << "y�������ţ�" << osuPointProportionY;
	qDebug() << "����:" << leftDistance << "�Ҽ�ࣺ" << topDistance << "osu���ࣺ" << osuleft << "osu�Ҽ��" << osutop;
	
	int temp = 109 * osuPointProportionX + osuleft + leftDistance, temp2 = 168 * osuPointProportionY + osutop + topDistance;
	qDebug() << "��������x��" << temp << "��������y��" << temp2;

}
