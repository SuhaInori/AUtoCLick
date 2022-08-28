#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <functional>
#include <vector>
#include "CokFileR_W.h"
#include <Windows.h>
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? true : false)
namespace Timer {

	// 定时器类型
	/*
	   主要数据：
	   1.一个线程变量，保存定时器线程
	   2.一个互斥锁，配合条件变量使用
	   3.一个条件变量，结合互斥锁，可以是线程不执行任务时，睡眠一段时间，在退出调用时，可以唤醒线程完成退出
	   4.定时执行函数，具体的定时执行业务操作
	   5.间隔时间，定时器间隔一段时间调用定时执行函数
	   6.一个退出标识，标志是否退出定时线程循环
	   7.立即执行标识，标识新建状态的定时线程是否立即执行一次任务，而不需等待一个间隔时间才开始执行第一次任务
	*/
	class CTimer
	{
	public:
		//template<class F>
		/*CTimer(F func)
			: m_func(func) {};*/

		CTimer();
		CTimer(void(* fc)());
		CTimer(std::vector<CokFile::CokCC>,void (*mfunc)(std::vector<CokFile::CokCC>, unsigned int&));
		virtual ~CTimer();
		/// <summary>
		/// 无限定时
		/// </summary>
		/// <param name="time">每次触发时间</param>
		/// <param name="func">触发事件</param>
		void TimerStart(uint32_t time);
		/// <summary>
		/// 限时定时器
		/// </summary>
		/// <param name="time">每次触发时间</param>
		/// <param name="func">触发事件</param>
		/// <param name="sleepFor">此时间后关闭</param>
		void TimerStartForTime(uint32_t time ,uint32_t sleepFor = UINT32_MAX);
		/// <summary>
		/// 限次定时器
		/// </summary>
		/// <param name="time">每次触发时间</param>
		/// <param name="func">触发事件</param>
		/// <param name="execution_Times">触发execution_Times次后关闭</param>
		void TimerStartForCount(uint32_t time,  uint32_t execution_Times = UINT32_MAX);
		/// <summary>
		/// 模式二限次定时器
		/// </summary>
		/// <param name=""></param>
		/// <param name=""></param>
		void TimerStartForModeEN(uint32_t, uint32_t);
		//停止定时器
		void Stop();

		/// <summary>
		/// 定时器退出
		/// </summary>
		/// <param name="b_exit"></param>
		void SetExit(bool b_exit);

		void SetEnAble(bool* c);

		bool *IsEnable;
	private:
		void Run();
		void RunS();
		bool Mode;
		/// <summary>
		/// 启动定时器
		/// </summary>
		/// <param name="imsec">定时时间 ms</param>
		/// <param name="bimmediately_run">是否启动后立即执行</param>
		void Start(unsigned int imsec, bool bimmediately_run = false);
		unsigned int m_imsec = 1000;	// 间隔时间
	private: // 私有数据部分
		std::atomic_bool m_bexit = false;
		std::atomic_bool m_bimmediately_run = false; // 是否立即执行
		
		//std::function<void()> m_func;	// 执行函数
		void(* m_func)();
		void (*mfunc)(std::vector<CokFile::CokCC>, unsigned int&);
		std::vector<CokFile::CokCC> cctco;
		std::thread m_thread;
		std::mutex m_mutex;
		std::condition_variable m_cond;
		uint32_t sleepFor;
	};


	// 定时执行函数
	//void OnTimeFunc()
	//{
	//	std::cout << time(NULL) - g_index << "叮叮叮~" << std::endl;
	//}


}
