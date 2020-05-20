#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "timer.h"

static void OnTimer1(void *param)
{
	auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
	std::cout << "OnTimer1," << "time:" << now << std::endl;
}

static void OnTimer2(void *param)
{
	auto now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
	std::cout << "OnTimer2," << "time:" << now << std::endl;
}

int main()
{
	DemoTimer::Instance()->Start();

	int id1 = DemoTimer::Instance()->Register(4000, OnTimer1, nullptr, true);

	int id2 = DemoTimer::Instance()->Register(5000, OnTimer2, nullptr, true);

	while (true)
	{
		std::string cmd;
		std::cin >> cmd;
		if (cmd == "q")
			break;
	}

	DemoTimer::Instance()->UnRegister(id1);
	DemoTimer::Instance()->UnRegister(id2);

	DemoTimer::Instance()->Stop();
}
