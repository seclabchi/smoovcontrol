//
//  Commander.hpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 9/27/22.
//

#ifndef Commander_hpp
#define Commander_hpp

#include <stdio.h>

#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "common_defs.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <zmqpp/zmqpp.hpp>

#include "fmsmoov.pb.h"

class MainWindow;

using namespace std;

class Commander {
public:
    Commander(std::mutex& _mutex_startup, std::condition_variable& _cv_startup, bool& _thread_started, string _ipaddr);
    virtual ~Commander();
    void operator ()(string params);
    void stop();
    void set_main_window(MainWindow* mw);
    void enqueue_command(const fmsmoov::ProcessorCommand& cmd);
private:
    string m_ipaddr;
    std::shared_ptr<spdlog::logger> log;
    std::mutex mutex_shutdown;
    std::mutex& mutex_startup;
    std::condition_variable& cv_startup;
    std::condition_variable cv_shutdown;
    bool& m_thread_started;
    bool m_shutdown_signalled;
    MainWindow* m_main_window;
    queue<fmsmoov::ProcessorCommand> m_cmd_queue;
    mutable mutex m_mutex_queue;
    condition_variable m_cv_queue;
};

#endif /* Commander_hpp */
