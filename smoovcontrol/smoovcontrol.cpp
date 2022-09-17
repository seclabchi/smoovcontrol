//============================================================================
// Name        : smoovcontrol.cpp
// Author      : tonekids.com
// Version     :
// Copyright   : BSD License
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "common_defs.h"


#include "MainWindow.h"
#include "CommThread.h"

using namespace std;

int main(int argc, char* argv[]) {
	std::shared_ptr<spdlog::logger> log;
	log = spdlog::stdout_color_mt(string("MAIN"), spdlog::color_mode::always);
	log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
	log->set_level(spdlog::level::trace);

	LOGI("Starting smoovcontrol...");

	MainWindow* mw = new MainWindow();

	bool comm_thread_started = false;
	std::mutex mutex_startup;
	std::condition_variable cv_startup;
	std::unique_lock lk(mutex_startup);
	bool commthread_started = false;

	CommThread* comm_thread = new CommThread(mutex_startup, cv_startup, commthread_started);

	// Start thread for 0mq/protobuf handler
	std::thread* cmd_thread = new std::thread(std::ref(*comm_thread), "1234");
	LOGD("Waiting for startup confirmation from ProcessorMain...");
	cv_startup.wait(lk, [&]{return commthread_started;}); // @suppress("Invalid arguments")

	LOGI("Starting UI");
	mw->go(argc, argv);
	comm_thread->stop();
	LOGI("UI closed.  Goodbye.");
	return 0;
}
