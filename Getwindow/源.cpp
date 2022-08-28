#include <Windows.h>  
#include <stdio.h>  
#include <string.h>  
#include <iostream>  

int main(int argc, char* argv[])
{
    //根据窗口名获取QQ游戏登录窗口句柄  
    HWND hq = FindWindow(NULL, TEXT("osu!"));
    if (hq == NULL)
        std::cout << "找不到窗口" << std::endl;

    //if (std::cin)

    //得到QQ窗口大小  
    RECT rect;
    GetWindowRect(hq, &rect);
    int w = rect.right - rect.left, h = rect.bottom - rect.top;
    std::cout << "宽:" << w << " " << "高:" << h << std::endl;
    system("pause");
    //移动QQ窗口位置  
    MoveWindow(hq, 100, 100, w, h, false);

    //得到桌面窗口  
    HWND hd = GetDesktopWindow();
    GetWindowRect(hd, &rect);
    w = rect.right - rect.left;
    h = rect.bottom - rect.top;
    std::cout << "宽:" << w << " " << "高:" << h << std::endl;
    system("pause");
    return 0;
}