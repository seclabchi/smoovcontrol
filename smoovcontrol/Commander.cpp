//
//  Commander.cpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 9/27/22.
//

#include "Commander.hpp"
#include "MainWindow.h"

#include <sstream>

Commander::Commander(std::mutex& _mutex_startup, std::condition_variable& _cv_startup, bool& _thread_started, string _ipaddr) :
    mutex_startup(_mutex_startup), cv_startup(_cv_startup), m_thread_started(_thread_started), m_ipaddr(_ipaddr) {
    // TODO Auto-generated constructor stub
    log = spdlog::stdout_color_mt("COMMANDER");
    log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
    log->set_level(spdlog::level::debug);
    m_shutdown_signalled = false;
}

Commander::~Commander() {
    // TODO Auto-generated destructor stub
}

void Commander::stop() {
    m_shutdown_signalled = true;
    
    fmsmoov::ProcessorCommand cmd;
    cmd.mutable_internal_shutdown_cmd();
    
    enqueue_command(cmd);
    
    LOGD("Commander shutdown signal is set.");
}

void Commander::set_main_window(MainWindow* main_window) {
    m_main_window = main_window;
}

void Commander::enqueue_command(const fmsmoov::ProcessorCommand& cmd) {
    lock_guard<mutex> lock(m_mutex_queue);
    fmsmoov::ProcessorCommand enqueue_cmd;
    enqueue_cmd.CopyFrom(cmd);
    m_cmd_queue.push(enqueue_cmd);
    m_cv_queue.notify_one();
}

void Commander::operator ()(string params) {
    //auto handle = cmd_thread->native_handle();
//    pthread_setname_np("comm");
    
    {
        std::lock_guard lkstartup(mutex_startup);
        LOGD("Commander thread starting up...");
        m_thread_started = true;
        
        LOGD("Commander started.  Notifying thread parent...");
    }
    cv_startup.notify_one();
    
    LOGD("Commander in main polling loop...waiting for shutdown signal...");
    zmqpp::context_t context;
    zmqpp::socket_t socket_cmd(context, zmqpp::socket_type::request);
    stringstream constr;
    constr << "tcp://" << m_ipaddr << ":5556";
    socket_cmd.connect(constr.str().c_str());
    cout << "Connected to responder." << endl;
    
    while(false == m_shutdown_signalled) {
        unique_lock<mutex> qlock(m_mutex_queue);
        while(m_cmd_queue.empty()) {
            m_cv_queue.wait(qlock);
        }
        fmsmoov::ProcessorCommand cmd_to_q;
        cmd_to_q.CopyFrom(m_cmd_queue.front());
        m_cmd_queue.pop();
        LOGT("Popped command from queue...");
        zmqpp::message_t msg(cmd_to_q.SerializeAsString());
        socket_cmd.send(msg);
        LOGT("Message sent.");
    
        zmqpp::message_t rspmsg;
        socket_cmd.receive(rspmsg);
        fmsmoov::ProcessorResponse procrsp;
        string procrspstr;
        rspmsg >> procrspstr;
        procrsp.ParseFromString(procrspstr);
        LOGT("Message received.");

        m_main_window->handle_command_response(procrsp);
    }

    LOGD("Commander got shutdown signal. Exiting Commander func..");
    return;
}
