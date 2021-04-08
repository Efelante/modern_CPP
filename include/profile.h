#pragma once

#include <iostream>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

class LogDuration {
public:
	explicit LogDuration(const string &msg):
		message(msg + ": "),
		start(steady_clock::now())
	{
		// Empty constructor
	}
	~LogDuration()
	{
		auto finish = steady_clock::now();
		auto duration = finish - start;
		cerr << message
			<< duration_cast<milliseconds>(duration).count()
			<< " ms" << endl;
	}

private:
	string message;
	steady_clock::time_point start;
};

#define UNIQ_ID_IMPL(linenum) _a_local_var_##linenum
#define UNIQ_ID(linenum) UNIQ_ID_IMPL(linenum)

#define LOG_DURATION(message) \
	LogDuration UNIQ_ID(__LINE__)(message);
