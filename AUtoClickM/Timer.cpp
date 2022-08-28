#include "Timer.h"

void Timer::CTimer::Start(unsigned int imsec, bool bimmediately_run)
{
#ifdef INIDEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif	//INIDEBUG
	if (m_func==NULL)
	{
		return;
	}
	if (imsec == 0 || imsec == static_cast<unsigned int>(-1)) // 间隔时间为0或默认无效值，直接返回
	{
		return;
	}
	m_bexit.store(false);
	m_imsec = imsec;
	m_bimmediately_run.store(bimmediately_run);
	if (Mode)
	{
#ifdef DebugL3
		qDebug() << "定时器模式2。";
#endif
		m_thread = std::thread(&CTimer::RunS, this);

	}
	else
	{
#ifdef DebugL3
		qDebug() << "定时器模式1。";
#endif
		m_thread = std::thread(&CTimer::Run, this);

	}
	
}

Timer::CTimer::CTimer()
{
	this->m_func = NULL;
	
}

Timer::CTimer::CTimer(void(*fc)())
{
	this->m_func = fc;
	Mode = false;
}

Timer::CTimer::CTimer(std::vector<CokFile::CokCC> c,void(*mfunc)(std::vector<CokFile::CokCC>, unsigned int&))
{
	cctco = c;
	if (!mfunc) {
#ifdef DebugL3
		qDebug() << "函数传递。";
#endif
	}
	this->mfunc = mfunc;
	if (this->mfunc)
	{
#ifdef DebugL3
		qDebug() << "函数传递c成功。";
#endif
	}
	Mode = true;
}

Timer::CTimer::~CTimer()
{
	this->Stop();
}


void Timer::CTimer::TimerStart(uint32_t time)
{
	this->sleepFor = UINT32_MAX;

	this->Start(time);
}

void Timer::CTimer::TimerStartForTime(uint32_t time, uint32_t sleepFor)
{
	this->sleepFor = sleepFor / time;


	this->Start(time);
	//std::this_thread::sleep_for(std::chrono::seconds(sleepFor));
}

void Timer::CTimer::TimerStartForCount(uint32_t time, uint32_t execution_Times)
{
	this->sleepFor = execution_Times;

	this->Start(time);
}

void Timer::CTimer::TimerStartForModeEN(uint32_t time, uint32_t cc)
{
#ifdef DebugL3
	qDebug() << "定时器配置。";
#endif
	this->sleepFor = cc;

	this->Start(time);
}

void Timer::CTimer::Stop()
{
#ifdef INIDEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif	//INIDEBUG
	m_bexit.store(true);
	m_cond.notify_all(); // 唤醒线程
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

void Timer::CTimer::SetExit(bool b_exit)
{
	m_bexit.store(b_exit);
}

void Timer::CTimer::SetEnAble(bool* c)
{
	this->IsEnable = c;
}

void Timer::CTimer::Run()
{
	*IsEnable = true;
	uint32_t sleepTimer=0;
	if (m_bimmediately_run.load()) // 立即执行判断
	{
		if (m_func)
		{
			m_func();
		}
	}

	while (!m_bexit.load())
	{
		{
			// 锁放在花括号内部，减小锁的粒度
			std::unique_lock<std::mutex> locker(m_mutex);

			// 如果是被唤醒的，需要判断先条件确定是不是虚假唤醒
			// wait_for是等待第三个参数满足条件，当不满足时，超时后继续往下执行
			m_cond.wait_for(locker, std::chrono::milliseconds(m_imsec), [this]() { return m_bexit.load(); });
		}

		if (m_bexit.load()) // 再校验下退出标识
		{
			*IsEnable = false;
			return;
		}

		if (m_func)
		{
			m_func();
			sleepTimer++;
		}

		if (sleepTimer>= sleepFor)
		{
			*IsEnable = false;
			return;
		}

	}
	*IsEnable = false;
}

void Timer::CTimer::RunS()
{
#ifdef DebugL3
	qDebug() << "定时开始。";
#endif
	*IsEnable = true;
	uint32_t sleepTimer = 0;
	if (m_bimmediately_run.load()) // 立即执行判断
	{
		if (mfunc)
		{
#ifdef DebugL3
			qDebug() << "mfunc 不为空。";
#endif
			mfunc(this->cctco, m_imsec);
		}
	}
	while (!m_bexit.load())
	{
		{
			// 锁放在花括号内部，减小锁的粒度
			std::unique_lock<std::mutex> locker(m_mutex);

			// 如果是被唤醒的，需要判断先条件确定是不是虚假唤醒
			// wait_for是等待第三个参数满足条件，当不满足时，超时后继续往下执行
			m_cond.wait_for(locker, std::chrono::milliseconds(m_imsec), [this]() { return m_bexit.load(); });
		}

		if (m_bexit.load()) // 再校验下退出标识
		{
			*IsEnable = false;
			return;
		}
		if (KEY_DOWN(VK_F9))
		{
			*IsEnable = false;
			return;
		}
		if (mfunc)
		{
#ifdef DebugL3
			qDebug() << "mfunc 不为空。";
#endif
			mfunc(cctco, m_imsec);
			sleepTimer++;
		}
		if (sleepTimer >= sleepFor)
		{
			*IsEnable = false;
			return;
		}


	}
	*IsEnable = false;
}
