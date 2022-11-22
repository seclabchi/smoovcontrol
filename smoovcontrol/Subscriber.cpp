/*
 * Subscriber.cpp
 *
 *  Created on: Sep 27, 2022
 *      Author: zaremba
 */

#include "Subscriber.h"

#include <string>
#include <sstream>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <pthread.h>

#include <zmqpp/context.hpp>
#include <zmqpp/poller.hpp>

#include "MainWindow.h"

Subscriber::Subscriber(std::mutex& _mutex_startup, std::condition_variable& _cv_startup, bool& _thread_started, string _ipaddr) :
	mutex_startup(_mutex_startup), cv_startup(_cv_startup), m_thread_started(_thread_started), m_ipaddr(_ipaddr) {
	// TODO Auto-generated constructor stub
	log = spdlog::stdout_color_mt("SUBSCRIBER");
	log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
	log->set_level(spdlog::level::debug);
    m_shutdown_signalled = false;
}

Subscriber::~Subscriber() {
	// TODO Auto-generated destructor stub
}

void Subscriber::stop() {
	
    m_shutdown_signalled = true;
    zmqpp::socket_t socket_shutdown(context, zmqpp::socket_type::request);
    socket_shutdown.connect("inproc://subscriber_shutdown");
    socket_shutdown.send("SHUTDOWN\n");
    LOGD("Subscriber shutdown signal is set.");
}

void Subscriber::set_main_window(MainWindow* main_window) {
    m_main_window = main_window;
}

void Subscriber::operator ()(string params) {
	//auto handle = cmd_thread->native_handle();
//	pthread_setname_np("comm");
    
    {
        std::lock_guard lkstartup(mutex_startup);
        LOGD("Subscriber thread starting up...");
        m_thread_started = true;
        
        LOGD("Subscriber started.  Notifying thread parent...");
    }
    cv_startup.notify_one();
	
    LOGD("Subscriber in main polling loop...waiting for shutdown signal...");
    
    zmqpp::socket_t socket_shutdown(context, zmqpp::socket_type::reply);
    socket_shutdown.bind("inproc://subscriber_shutdown");
    zmqpp::socket socket_sub(context, zmqpp::socket_type::subscribe);
    stringstream constr;
    constr << "tcp://" << m_ipaddr << ":5555";
    socket_sub.connect(constr.str().c_str());
    cout << "Connected to publisher." << endl;
    //# Initialize poll set
    socket_sub.subscribe("");
    zmqpp::poller poller;
    poller.add(socket_sub, zmqpp::poller::poll_in);
    poller.add(socket_shutdown, zmqpp::poller::poll_in);
    
    while(false == m_shutdown_signalled) {
    	LOGT("Subscriber thread loop...");
        if(poller.poll()) {
            if(poller.events(socket_sub) & ZMQ_POLLIN) {
                zmqpp::message message;
                socket_sub.receive(message);
                string msgstr;
                message >> msgstr;
                LOGT("Received msg from fmsmoov of size {}", msgstr.size());
                pb_live.ParseFromString(msgstr);
                m_main_window->set_live_data(pb_live);
            }
            else {
                LOGT("Poller got an event NOT on the subscriber socket");
            }
        }
    }

	LOGD("Subscriber got shutdown signal. Exiting Subscriber func..");
	return;
}
