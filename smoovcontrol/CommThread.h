/*
 * CommThread.h
 *
 *  Created on: Jul 30, 2022
 *      Author: zaremba
 */

#ifndef COMMTHREAD_H_
#define COMMTHREAD_H_

#include <string>
#include <mutex>
#include <condition_variable>

#include "common_defs.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <zmqpp/zmqpp.hpp>

#include "proto/fmsmoov.pb.h"


using namespace std;

class CommThread {
public:
	CommThread(std::mutex& _mutex_startup, std::condition_variable& _cv_startup, bool& _commthread_started);
	virtual ~CommThread();
	void operator ()(string params);
	void stop();
private:
	std::shared_ptr<spdlog::logger> log;
	std::mutex mutex_shutdown;
	std::mutex& mutex_startup;
	std::condition_variable& cv_startup;
	std::condition_variable cv_shutdown;
	bool& m_commthread_started;
	bool m_commthread_shutdown_complete;
	bool m_shutdown_signalled;
	static constexpr chrono::duration m_loopwait = chrono::milliseconds(5);
    fmsmoov::ProcessorLiveData* pb_live;
};

#endif /* COMMTHREAD_H_ */
