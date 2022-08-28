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

	// ��ʱ������
	/*
	   ��Ҫ���ݣ�
	   1.һ���̱߳��������涨ʱ���߳�
	   2.һ���������������������ʹ��
	   3.һ��������������ϻ��������������̲߳�ִ������ʱ��˯��һ��ʱ�䣬���˳�����ʱ�����Ի����߳�����˳�
	   4.��ʱִ�к���������Ķ�ʱִ��ҵ�����
	   5.���ʱ�䣬��ʱ�����һ��ʱ����ö�ʱִ�к���
	   6.һ���˳���ʶ����־�Ƿ��˳���ʱ�߳�ѭ��
	   7.����ִ�б�ʶ����ʶ�½�״̬�Ķ�ʱ�߳��Ƿ�����ִ��һ�����񣬶�����ȴ�һ�����ʱ��ſ�ʼִ�е�һ������
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
		/// ���޶�ʱ
		/// </summary>
		/// <param name="time">ÿ�δ���ʱ��</param>
		/// <param name="func">�����¼�</param>
		void TimerStart(uint32_t time);
		/// <summary>
		/// ��ʱ��ʱ��
		/// </summary>
		/// <param name="time">ÿ�δ���ʱ��</param>
		/// <param name="func">�����¼�</param>
		/// <param name="sleepFor">��ʱ���ر�</param>
		void TimerStartForTime(uint32_t time ,uint32_t sleepFor = UINT32_MAX);
		/// <summary>
		/// �޴ζ�ʱ��
		/// </summary>
		/// <param name="time">ÿ�δ���ʱ��</param>
		/// <param name="func">�����¼�</param>
		/// <param name="execution_Times">����execution_Times�κ�ر�</param>
		void TimerStartForCount(uint32_t time,  uint32_t execution_Times = UINT32_MAX);
		/// <summary>
		/// ģʽ���޴ζ�ʱ��
		/// </summary>
		/// <param name=""></param>
		/// <param name=""></param>
		void TimerStartForModeEN(uint32_t, uint32_t);
		//ֹͣ��ʱ��
		void Stop();

		/// <summary>
		/// ��ʱ���˳�
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
		/// ������ʱ��
		/// </summary>
		/// <param name="imsec">��ʱʱ�� ms</param>
		/// <param name="bimmediately_run">�Ƿ�����������ִ��</param>
		void Start(unsigned int imsec, bool bimmediately_run = false);
		unsigned int m_imsec = 1000;	// ���ʱ��
	private: // ˽�����ݲ���
		std::atomic_bool m_bexit = false;
		std::atomic_bool m_bimmediately_run = false; // �Ƿ�����ִ��
		
		//std::function<void()> m_func;	// ִ�к���
		void(* m_func)();
		void (*mfunc)(std::vector<CokFile::CokCC>, unsigned int&);
		std::vector<CokFile::CokCC> cctco;
		std::thread m_thread;
		std::mutex m_mutex;
		std::condition_variable m_cond;
		uint32_t sleepFor;
	};


	// ��ʱִ�к���
	//void OnTimeFunc()
	//{
	//	std::cout << time(NULL) - g_index << "������~" << std::endl;
	//}


}
