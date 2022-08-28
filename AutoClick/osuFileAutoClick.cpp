#include "osuFileAutoClick.h"
#include <WinUser.h>

osuFileAutoClick::osuFileAutoClick(std::string path):path(path), maxObjects(0)
{
#ifdef INIDEBUG
	qDebug() << "构造函数=========";
	std::cout << "构造函数=========" << std::endl;
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
	qDebug() << "开始读取文件";
	std::cout << "开始读取文件"<<std::endl;
#endif
	bool isOnHit = false;
	std::ifstream in_conf_file(path.c_str());
	if (!in_conf_file)
	{
		MessageBox(NULL, TEXT("文件打开失败"), TEXT("错误"), MB_OK);
		return;
	}
	std::string str_line = "";
#ifdef INIDEBUG
	qDebug() << "开始读取文件";
	std::cout << "开始逐行文件" << std::endl;
#endif
	while (getline(in_conf_file, str_line))
	{
		click_Node temp;
		
		if (strstr(str_line.c_str(), "[HitObjects]") != NULL && !isOnHit)
		{
#ifdef INIDEBUG
			qDebug() << "读取到object" << maxObjects;
#endif
			isOnHit = true;
			continue;
		}


		if (isOnHit)
		{
#ifdef INIDEBUG
			qDebug() << "开始读取文件" << maxObjects;
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
	qDebug() << "查找osu";
#endif
	osuHwnd = FindWindow(NULL, TEXT(OSUNAME));
	if (osuHwnd==NULL)
	{
		MessageBox(NULL, TEXT("osu 未打开！"), TEXT("错误"), MB_OK);
		return false;
	}
#ifdef INIDEBUG
	qDebug() << "找到osu";
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
/// 获取屏幕缩放
/// </summary>
/// <returns></returns>
double osuFileAutoClick::GetHorzScale()
{
	// 获取窗口当前显示的监视器
	HWND hWnd = GetDesktopWindow();//根据需要可以替换成自己程序的句柄 
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	// 获取监视器逻辑宽度与高度
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
	int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

	// 获取监视器物理宽度与高度
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;
	int cyPhysical = dm.dmPelsHeight;

	//缩放比例计算
	double horzScale = ((double)cxPhysical / (double)cxLogical);
	double vertScale = ((double)cyPhysical / (double)cyLogical);

	//返回缩放比
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
/// 查找对应节点的click
/// </summary>
/// <param name="">索引</param>
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
	qDebug() << "是否全屏：" << fullWindow << "缩放比:" << horzScale;
	qDebug() << "x方向缩放：" << osuPointProportionX << "y方向缩放：" << osuPointProportionY;
	qDebug() << "左间距:" << leftDistance << "右间距：" << topDistance << "osu左间距：" << osuleft << "osu右间距" << osutop;
	
	int temp = 109 * osuPointProportionX + osuleft + leftDistance, temp2 = 168 * osuPointProportionY + osutop + topDistance;
	qDebug() << "测试数据x：" << temp << "测试数据y：" << temp2;

}
