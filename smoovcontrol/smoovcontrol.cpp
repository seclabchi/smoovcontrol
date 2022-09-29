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

#include "common_defs.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


#include "log_sink.h"
#include "spdlog/details/null_mutex.h"
#include <mutex>
using log_sink_mt = log_sink<std::mutex>;
using log_sink_st = log_sink<spdlog::details::null_mutex>;


#include "MainWindow.h"
#include "Subscriber.h"
#include "Commander.hpp"

using namespace std;


/* The following two functions are to disable the App Nap bullshit that caused me no end
 of pain until I figured out what was going on. */
#define OBJC_OLD_DISPATCH_PROTOTYPES 1

#if defined(__APPLE__)

#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
# include <objc/runtime.h>
# include <objc/message.h>
#else
# include <objc/objc-runtime.h>
#endif

/* Globals */
static int osx_latencycritical_count = 0;
static id osx_latencycritical_activity = nil;

/* Tell App Nap that this is latency critical */
void osx_latencycritical_start() {
    Class pic;      /* Process info class */
    SEL pisl;       /* Process info selector */
    SEL bawo;       /* Begin Activity With Options selector */
    id pi;          /* Process info */
    id str;         /* Reason string */

    if (osx_latencycritical_count++ != 0)
        return;

    /* Avoid triggering an exception when run on older OS X */
    if ((pic = (Class)objc_getClass("NSProcessInfo")) == nil)
        return;

    if (class_getClassMethod(pic, (pisl = sel_getUid("processInfo"))) == NULL)
        return;

    if (class_getInstanceMethod(pic,
          (bawo = sel_getUid("beginActivityWithOptions:reason:"))) == NULL)
        return;

    /* Get the process instance */
    if ((pi = objc_msgSend((id)pic, pisl)) == nil)
        return;

    /* Create a reason string */
    str = objc_msgSend((id)(objc_getClass("NSString")), sel_getUid("alloc"));
    str = objc_msgSend(str, sel_getUid("initWithUTF8String:"), "Timing Crititcal");

    /* Start activity that tells App Nap to mind its own business: */
    /* NSActivityUserInitiatedAllowingIdleSystemSleep */
    /* | NSActivityLatencyCritical */
    osx_latencycritical_activity = objc_msgSend(pi, bawo,
                    0x00FFFFFFULL | 0xFF00000000ULL, str);
}

/* Done with latency critical */
void osx_latencycritical_end() {
    if (osx_latencycritical_count > 0) {
        osx_latencycritical_count--;
        if (osx_latencycritical_count == 0
         && osx_latencycritical_activity != nil) {
            objc_msgSend(
                         objc_msgSend((id)(objc_getClass("NSProcessInfo")),
                                      sel_getUid("processInfo")),
                                      sel_getUid("endActivity:"),
                                      osx_latencycritical_activity);
            osx_latencycritical_activity = nil;
        }
    }
}

#endif /* __APPLE__ */


int main(int argc, char* argv[]) {
    osx_latencycritical_start();
	std::shared_ptr<spdlog::logger> log;
	log = spdlog::stdout_color_mt(string("MAIN"), spdlog::color_mode::always);
   	log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
	log->set_level(spdlog::level::trace);

	LOGI("Starting smoovcontrol...");

	MainWindow* mw = new MainWindow();

	std::mutex mutex_commander_startup;
	std::condition_variable cv_commander_startup;
	std::unique_lock lk_cmd(mutex_commander_startup);
	bool commander_started = false;

    LOGD("Starting Commander...");
	Commander* commander = new Commander(mutex_commander_startup, cv_commander_startup, commander_started);
	// Start thread for 0mq/protobuf commands
	std::thread* cmd_thread = new std::thread(std::ref(*commander), "1234");
	LOGD("Waiting for startup confirmation from Commander...");
	cv_commander_startup.wait(lk_cmd, [&]{return commander_started;}); // @suppress("Invalid arguments")

    std::mutex mutex_subscriber_startup;
    std::condition_variable cv_subscriber_startup;
    std::unique_lock lk_sub(mutex_subscriber_startup);
    bool subscriber_started = false;
    
    LOGD("Starting Subscriber...");
    Subscriber* subscriber = new Subscriber(mutex_subscriber_startup, cv_subscriber_startup, subscriber_started);
    // Start thread for 0mq/protobuf subscription
    std::thread* sub_thread = new std::thread(std::ref(*subscriber), "1234");
    LOGD("Waiting for startup confirmation from Subscriber...");
    cv_subscriber_startup.wait(lk_sub, [&]{return subscriber_started;}); // @suppress("Invalid arguments")
    
    commander->set_main_window(mw);
    subscriber->set_main_window(mw);
    mw->set_commander(commander);
    mw->set_subscriber(subscriber);
    
	LOGI("Starting UI");
	mw->go(argc, argv);
    LOGI("UI closed.");
    LOGD("Stopping commander thread and waiting to join...");
    commander->stop();
    cmd_thread->join();
    LOGD("Stopping subscriber thread and waiting to join...");
    subscriber->stop();
    sub_thread->join();
    LOGI("All shut down.  Goodbye.");
    osx_latencycritical_end();   
	return 0;
}
