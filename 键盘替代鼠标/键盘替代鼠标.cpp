// 键盘替代鼠标.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include "conio.h"
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? true : false)

int main()
{
    system("mode con cols=30 lines=10");
    int click =5;
    std::cout << "初始化成功" << std::endl;
    std::cout << "shift+鼠标左：连点5下" << std::endl;
    std::cout << "shift+ESC：进入设置" << std::endl;
    std::cout << "shift+l：进入连续点击" << std::endl;
    std::cout << "shift+p：进入连续点击" << std::endl;
    while (true)
    {
        if (KEY_DOWN(VK_SHIFT)&&KEY_DOWN(VK_LBUTTON))
        {
            
            // 模拟点击右键 MOUSEEVENTF_LEFTDOWN
            for (int i = 0; i < click; i++)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(5);
            }
            // mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            // mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
             
        }
        if (KEY_DOWN(27)&& KEY_DOWN(VK_SHIFT))
        {
            std::cout << "设置连点次数" << std::endl;
            std::cin >> click;
            std::cout << "设置完成 次数为："<<click << std::endl;
        }
        if (KEY_DOWN(VK_SHIFT)&& KEY_DOWN(76))
        {
            std::cout << "-- 进入连续点击" << click << std::endl;
            while (true)//!KEY_DOWN(27)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(100);
                if (KEY_DOWN(VK_SHIFT)&&KEY_DOWN(0x50))
                {
                    std::cout << "-- 退出连续点击" << click << std::endl;
                    break;
                }
            }
           
        }
        Sleep(50);

        //// 模拟按下'A'键
        //keybd_event('A', 0, 0, 0);
        //keybd_event('A', 0, KEYEVENTF_KEYUP, 0);

        //// 模拟按下 CTRL + F
        //keybd_event(VK_CONTROL, (BYTE)0, 0, 0);
        //keybd_event('F', (BYTE)0, 0, 0);

        //keybd_event('F', (BYTE)0, KEYEVENTF_KEYUP, 0);
        //keybd_event(VK_CONTROL, (BYTE)0, KEYEVENTF_KEYUP, 0);

    }
    
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
