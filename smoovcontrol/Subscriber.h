/*
 * Subscriber.h
 *
 *  Created on: Sep 27, 2022
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

#include "fmsmoov.pb.h"

class MainWindow;

using namespace std;

class Subscriber {
public:
    Subscriber(std::mutex& _mutex_startup, std::condition_variable& _cv_startup, bool& _commthread_started, string _ipaddr);
	virtual ~Subscriber();
	void operator ()(string params);
	void stop();
    void set_main_window(MainWindow* mw);
    void send_command(const fmsmoov::ProcessorCommand);
private:
    string m_ipaddr;
	std::shared_ptr<spdlog::logger> log;
	std::mutex mutex_shutdown;
	std::mutex& mutex_startup;
	std::condition_variable& cv_startup;
	std::condition_variable cv_shutdown;
	bool& m_thread_started;
	bool m_shutdown_signalled;
	static constexpr chrono::duration m_loopwait = chrono::milliseconds(5);
    fmsmoov::ProcessorLiveData pb_live;
    MainWindow* m_main_window;
    zmqpp::context_t context;
};

#endif /* COMMTHREAD_H_ */
