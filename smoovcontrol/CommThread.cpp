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

#include <zmqpp/context.hpp>
#include <zmqpp/poller.hpp>

#include "MainWindow.h"

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
	
    m_shutdown_signalled = true;
    LOGD("CommThread shutdown signal is set.");
}

void CommThread::set_main_window(MainWindow* main_window) {
    m_main_window = main_window;
}

void CommThread::operator ()(string params) {
	//auto handle = cmd_thread->native_handle();
//	pthread_setname_np("comm");
    
    {
        std::lock_guard lkstartup(mutex_startup);
        LOGD("CommThread thread starting up...");
        m_commthread_started = true;
        
        LOGD("CommThread started.  Notifying thread parent...");
    }
    cv_startup.notify_one();
	
    LOGD("CommThread in main polling loop...waiting for shutdown signal...");
    
    zmqpp::context_t context;
    zmqpp::socket socket_sub(context, zmqpp::socket_type::subscribe);
    socket_sub.connect("tcp://rpi-fmsmoov:5555");
    cout << "Connected to publisher." << endl;
    //# Initialize poll set
    socket_sub.subscribe("");
    zmqpp::poller poller;
    poller.add(socket_sub, zmqpp::poller::poll_in);
    
    while(false == m_shutdown_signalled) {
    	LOGD("Comm thread loop...");
        if(poller.poll()) {
            zmqpp::message message;
            socket_sub.receive(message);
            string msgstr;
            message >> msgstr;
            LOGT("Received msg from fmsmoov of size {}", msgstr.size());
            pb_live.ParseFromString(msgstr);
            m_main_window->set_live_data(pb_live);
        }
    }

	LOGD("CommThread got shutdown signal. Exiting CommThread func..");
	return;
}
