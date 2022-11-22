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
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Adjuster.H>

#include "StereoMeter.h"
#include "Meter.hpp"
#include "Slidepot.hpp"
#include "Indicator.hpp"
#include "MeterDisplay.hpp"
#include "CompressorLimiter.hpp"
#include "adjuster_delay.hpp"
#include <FL/Fl_Counter.H>

#include <string>
#include "fmsmoov.pb.h"

class Commander;
class Subscriber;

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
    void set_commander(Commander* comm_thread);
    void set_subscriber(Subscriber* comm_thread);
	int go(int argc, char* argv[]);
    void set_live_data(const fmsmoov::ProcessorLiveData& pld);
    void logmsg(string msg);
    static void window_callback_wrapper(Fl_Widget *widget, void* user);
    void window_callback(Fl_Widget *widget, void* user);
    static void general_callback_wrapper(Fl_Widget *widget, void* user);
    void general_callback(Fl_Widget *widget, void* user);
    void handle_command_response(const fmsmoov::ProcessorResponse& rsp);
    static void cb_delay_wrapper(Fl_Widget *widget, void* user);
    void cb_delay(Fl_Widget *widget, void* user);
    static void cb_connect_wrapper(Fl_Widget *widget, void* user);
    void cb_connect(Fl_Widget *widget);
private:
    int connect(string _ipaddr);
    void disconnect();
    
    std::thread* cmd_thread;
    Commander* commander;
    
    Subscriber* subscriber;
    std::thread* sub_thread;
    
    std::shared_ptr<spdlog::logger> log;
    bool m_shutting_down;
    Fl_Double_Window* m_window;
    Fl_Text_Display* logoutput;
    Fl_Light_Button* m_button_master_bypass;
    Fl_Light_Button* m_button_phase_rotator_enable;
    Fl_Light_Button* m_button_stereo_enhance_enable;
    Fl_Light_Button* m_button_bass_enhancer_enable;
    Fl_Light_Button* m_button_complim_enable;
    
    Fl_Light_Button* m_button_connected;
    Fl_Input* m_input_ipaddr;
    
    Slidepot* m_slidepot_input_gain;
    StereoMeter* sm_master_in;
    StereoMeter* sm_master_out;
    Meter* meter_agc_adjust;
    Indicator* ind_agc_gate_active;
    Fl_Counter* m_counter_delay;
    MeterDisplay* test_meter_display;
    CompressorLimiter* compressor_limiter;
    Commander* m_commander;
    Subscriber* m_subscriber;
    
    static void timer_callback(void* v);
    static void timer_get_state(void* v);
    
    bool m_master_bypass;
    bool m_phase_rotator_enabled;
    bool m_stereo_enhance_enabled;
    bool m_bass_enhancer_enabled;
    bool m_complim_enabled;
    
    string m_fmsmoov_ip;
    bool m_connected;
};

#endif /* MAINWINDOW_H_ */
