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

#include <string>

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
	int go(int argc, char* argv[]);
	void set_level(METER_NAME name, double l, double r);
    void logmsg(string msg);
private:
    std::shared_ptr<spdlog::logger> log;
    Fl_Text_Display* logoutput;
};

#endif /* MAINWINDOW_H_ */
