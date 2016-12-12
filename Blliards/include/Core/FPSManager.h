#pragma once
#include <./boost/chrono.hpp>
#include <./boost/timer.hpp>
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

namespace rinfw {
namespace core {

#define FPS 120
class FPSManager
{
private:
	double targetFPS;
	int fpsCount1, fpsCount2;
	int frameData[FPS];
	double ave;
	double oldTime;
	double deltaTime;


	FPSManager() {
		fpsCount1 = fpsCount2 = 0;
		ave = 0.0;
		deltaTime = 0.0;
		oldTime = 0.0;
		for (int i = 0; i < 60; ++i)
			frameData[i] = 0;
	}
public:
	static FPSManager &getInstance() {
		static FPSManager ins;
		return ins;
	}

	void waitFPS() {
		int term, i;
		boost::chrono::milliseconds gnt = boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now().time_since_epoch());
		static int t = 0;
		if (fpsCount1 == 0) {
			if (t == 0)
				term = 0;
			else
				term = (int)(fpsCount2 + 1000 - gnt.count());
		}
		else
			term = (int) (fpsCount2 + fpsCount1 *(1000.0f / FPS)) - gnt.count();

		if (term > 0)
			Sleep(term);
		if (fpsCount1 == 0)
			fpsCount2 = gnt.count();

		frameData[fpsCount1] = gnt.count() - t;
		t = gnt.count();

		//•½‹ÏŒvŽZ
		if (fpsCount1 == FPS - 1) {
			ave = 0;
			for (i = 0; i < FPS; i++)
				ave += frameData[i];
			ave /= FPS;
		}
		fpsCount1 = (++fpsCount1) % FPS;
		if (oldTime != 0)
			deltaTime = (gnt.count() - oldTime) / 1000.0;
		oldTime = (double)gnt.count();
	}
	double getFPS() {
		return 1000.0f / ave;
	}
	double getDeltaTime() {
		return deltaTime;
	}

};
}
}