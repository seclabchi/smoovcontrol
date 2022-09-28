/*
 * MainWindow.h
 *
 *  Created on: Jul 19, 2022
 *      Author: zaremba
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "common_defs.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "log_sink.h"

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>

#include "StereoMeter.h"

#include <string>
#include "fmsmoov.pb.h"

class CommThread;

using namespace std;

class MainWindow {

public:
	typedef enum {
			PROCESSOR_INPUT = 0,
			PROCESSR_OUTPUT = 1
	} METER_NAME;

public:
	MainWindow();
	virtual ~MainWindow();
    void set_comm_thread(CommThread* comm_thread);
	int go(int argc, char* argv[]);
    void set_live_data(const fmsmoov::ProcessorLiveData& pld);
    void logmsg(string msg);
    static void window_callback_wrapper(Fl_Widget *widget, void* user);
    void window_callback(Fl_Widget *widget, void* user);
private:
    std::shared_ptr<spdlog::logger> log;
    bool m_shutting_down;
    Fl_Double_Window* m_window;
    Fl_Text_Display* logoutput;
    StereoMeter* sm_master_in;
    StereoMeter* sm_master_out;
    CommThread* m_comm_thread;
};

#endif /* MAINWINDOW_H_ */
