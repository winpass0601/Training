#pragma once
#include <./boost/chrono.hpp>
#include <./boost/timer.hpp>
#include <stack>


namespace rinfw{
namespace core{
class Time {
public:
	Time() {
		deltaTime = 0;
		oldTime = boost::chrono::system_clock::now();
	};

	void push(){
		st.push(boost::chrono::system_clock::now());
	};
	double pop() {
		if (st.empty())
			return 0.0;
		double elapsed = boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now() - st.top()).count();
		st.pop();
		return elapsed;
	};

	void CulDeltaTime(){
		 boost::chrono::system_clock::time_point nowTime = boost::chrono::system_clock::now();
		deltaTime = boost::chrono::duration_cast<boost::chrono::milliseconds>(nowTime - oldTime).count()/1000.0f;
		oldTime = nowTime;

		if (!st.empty())
			MessageBox(nullptr, "Pop Error", "Time", MB_OK);
	};
	double getDeltaTime(){
		return deltaTime;
	};

private:
	std::stack< boost::chrono::system_clock::time_point> st;

	 boost::chrono::system_clock::time_point oldTime;
	double deltaTime;
};
}
}