#ifndef TIMER_HPP__
#define TIMER_HPP__

#include <thread>
#include <condition_variable>
#include <mutex>
#include <map>
#include <chrono>
#include <ctime>


using pFunTimerCB = void(*)(void* param);

class DemoTimer
{
public:
	static DemoTimer * Instance()
	{
		static DemoTimer instance;
		return &instance;
	}

	void Start() {
		event_loop_ = std::thread(&DemoTimer::EventDispatch, this);
	}

	void Stop() {
		stop_ = true;
		cond_.notify_all();
		event_loop_.join();
	}

	int Register(int query_interval, pFunTimerCB callback, void* userdate, bool repeat = true);

	bool UnRegister(int timer_id);

private:
	void EventDispatch();

	DemoTimer();
	~DemoTimer();
private:
	struct Item {
		std::chrono::time_point<std::chrono::system_clock> next_hit;
		int			interval;
		pFunTimerCB fun_cb;
		bool		repeat;
		void*		userdata;
	};


	std::map<int, Item>		task_list_;
	volatile bool			stop_;
	std::thread				event_loop_;
	std::condition_variable cond_;
	std::mutex				mutex_;
};



#endif  //TIMER_HPP__