#include "timer.h"
#include <iostream>

DemoTimer::DemoTimer()
	:stop_(false)
{

}

DemoTimer::~DemoTimer()
{

}

int DemoTimer::Register(int query_interval, pFunTimerCB callback, void* userdata, bool repeat /*= true*/)
{
	Item timer_item;
	timer_item.fun_cb = callback;
	timer_item.repeat = repeat;
	timer_item.userdata = userdata;
	timer_item.interval = query_interval;
	timer_item.next_hit = std::chrono::system_clock::now() + std::chrono::milliseconds(query_interval);

	static int g_TIMER_INDEX = 0;

	std::unique_lock<std::mutex> lock(mutex_);
	task_list_[++g_TIMER_INDEX] = timer_item;
	cond_.notify_all();
	return g_TIMER_INDEX;
}

bool DemoTimer::UnRegister(int timer_id)
{
	std::unique_lock<std::mutex> lock(mutex_);
	task_list_.erase(timer_id);
	cond_.notify_all();
	return true;
}

void DemoTimer::EventDispatch()
{
	while (!stop_)
	{
		std::unique_lock<std::mutex> lock(mutex_);

		cond_.wait(lock, [this]() {
			return !task_list_.empty(); 
		});
		
		auto now = std::chrono::system_clock::now();
		for (auto iter = task_list_.begin(); iter != task_list_.end();)
		{
			Item &timer_item = iter->second;

			if (stop_)
				return;
			if (timer_item.next_hit < now)
			{
				pFunTimerCB callback = timer_item.fun_cb;
				if (callback)
				{
					callback(timer_item.userdata);
					if (timer_item.repeat)
					{
						timer_item.next_hit = std::chrono::system_clock::now() + std::chrono::milliseconds(timer_item.interval);
					}
					else
					{
						iter = task_list_.erase(iter);
					}
				}
			}

			iter++;
		}
		cond_.wait_for(lock, std::chrono::microseconds(100));
	}
}
