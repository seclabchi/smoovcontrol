/*
 * CommThread.cpp
 *
 *  Created on: Jul 30, 2022
 *      Author: zaremba
 */

#include "CommThread.h"

#include <string>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <pthread.h>


CommThread::CommThread(std::mutex& _mutex_startup, std::condition_variable& _cv_startup, bool& _commthread_started) :
	mutex_startup(_mutex_startup), cv_startup(_cv_startup), m_commthread_started(_commthread_started) {
	// TODO Auto-generated constructor stub
	log = spdlog::stdout_color_mt("COMMTHREAD");
	log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
	log->set_level(spdlog::level::trace);
}

CommThread::~CommThread() {
	// TODO Auto-generated destructor stub
}

void CommThread::stop() {
	/* Set shutdown flag and notify thread */
	{
		std::lock_guard lk(mutex_shutdown);
		m_shutdown_signalled = true;
		LOGD("CommThread shutdown signal is set.");
	}
	cv_shutdown.notify_one();

	/* Wait for thread to finish shutting down jack */
	{
		LOGD("Waiting for CommThread shutdown...");
		std::unique_lock lk(mutex_shutdown);
		cv_shutdown.wait(lk, [&]{return m_commthread_shutdown_complete;});
	}

	LOGD("CommThread shutdown is complete.");
}

void CommThread::operator ()(string params) {
	//auto handle = cmd_thread->native_handle();
//	pthread_setname_np("comm");
	std::unique_lock lk(mutex_shutdown);

	{
		std::lock_guard lkstartup(mutex_startup);
		//LOGD("ProcessorMain thread starting jack...");
		//start_jack();
		m_commthread_started = true;
		//LOGD("Jack started.  Notifying thread parent...");
	}
	cv_startup.notify_one();

    LOGD("CommThread in main polling loop...waiting for shutdown signal...");


    while(false == m_shutdown_signalled) {
    	LOGD("Comm thread loop...");
    	usleep(500000);
    	cv_shutdown.wait_for(lk, m_loopwait, [&]{return m_shutdown_signalled;});
    }

	LOGD("CommThread got shutdown signal...");

	//stop_jack();

	m_commthread_shutdown_complete = true;
	LOGD("CommThread shutdown complete.  Notifying thread parent...");

	lk.unlock();
	cv_shutdown.notify_one();

	LOGD("Exiting CommThread thread func");
	return;
}
