/*
 * MainWindow.cpp
 *
 *  Created on: Jul 19, 2022
 *      Author: zaremba
 */

#include "MainWindow.h"
#include "Subscriber.h"
#include "Commander.hpp"

#include <FL/abi-version.h>
#include <FL/Fl_Export.H>
#include <FL/fl_types.h>

#include "fmsmoov.pb.h"

MainWindow::MainWindow() {
    log = spdlog::stdout_color_mt("MAINWINDOW");
    log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
    log->set_level(spdlog::level::trace);
    m_shutting_down = false;
    m_master_bypass = false;
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

void MainWindow::set_subscriber(Subscriber* subscriber) {
    m_subscriber = subscriber;
}

void MainWindow::set_commander(Commander* commander) {
    m_commander = commander;
}

int MainWindow::go(int argc, char* argv[]) {
    LOGD("MainWindow::go");
    m_window = new Fl_Double_Window(1024, 768);
    m_window->label("FMSmoov Control");
    Fl_Box *box = new Fl_Box(10,10,1004,748, "This will eventually be the FM Smoov Control Panel in FLTK.\nIt will run on OSX, Linux, or Windows.");
    box->box(FL_BORDER_BOX);
    box->labelfont(FL_BOLD);
    box->labelsize(24);
    box->labeltype(FL_SHADOW_LABEL);
    sm_master_in = new StereoMeter("MASTER_IN", 10, 10, 40, 120);
    sm_master_out = new StereoMeter("MASTER_OUT", 60, 10, 40, 120);
    m_button_master_bypass = new Fl_Light_Button(40, 200, 200, 50, "MASTER BYPASS");
    m_button_master_bypass->selection_color(FL_RED);
    m_button_master_bypass->callback(general_callback_wrapper, this);
    logoutput = new Fl_Text_Display(30, 650, 960, 100);
    m_window->callback(window_callback_wrapper, this);
    m_window->end();
    m_window->show(argc, argv);
    
    fmsmoov::ProcessorCommand cmd;
    cmd.mutable_master_bypass_get();
    m_commander->enqueue_command(cmd);
    
    Fl::add_timeout(0.1, MainWindow::timer_callback);
    return Fl::run();
}

void MainWindow::set_live_data(const fmsmoov::ProcessorLiveData& pld) {
    if(false == m_shutting_down) {
        Fl::lock();
        sm_master_in->set_level(pld.inl(), pld.inr());
        sm_master_out->set_level(pld.outl(), pld.outr());
        sm_master_in->redraw();
        sm_master_out->redraw();
        Fl::unlock();
    }
    else {
        LOGT("Discarding live data call due to shutting down...");
    }
}

void MainWindow::logmsg(string msg) {
    logoutput->insert(msg.c_str());
}

void MainWindow::general_callback_wrapper(Fl_Widget *widget, void *user) {
    MainWindow* mw = reinterpret_cast<MainWindow*>(user);
    mw->general_callback(widget, user);
}

void MainWindow::general_callback(Fl_Widget *widget, void* user) {
    Fl::lock();
    LOGD("General callback from widget with label {}!", widget->label());
    fmsmoov::ProcessorCommand maincmd;
    string label = widget->label();
    
    if(!label.compare("MASTER BYPASS")) {
        if(m_master_bypass) {
            maincmd.mutable_master_bypass_set()->set_bypass(0);
        }
        else {
            maincmd.mutable_master_bypass_set()->set_bypass(1);
        }
        m_commander->enqueue_command(maincmd);
    }
    Fl::unlock();
}

void MainWindow::window_callback_wrapper(Fl_Widget *widget, void *user) {
    MainWindow* mw = reinterpret_cast<MainWindow*>(user);
    mw->window_callback(widget, user);
}

void MainWindow::window_callback(Fl_Widget *widget, void* user) {
    LOGD("Window callback!");
    m_shutting_down = true;
    m_window->hide();
}

void MainWindow::handle_command_response(const fmsmoov::ProcessorResponse& rsp) {
    if(false == m_shutting_down) {
        fmsmoov::ResponseCode rspcode = rsp.response();
        string rspcodestr = fmsmoov::ResponseCode_Name(rspcode);
        LOGD("Command response is {}:{}", rspcodestr, rsp.response_msg());
        
        if(rsp.has_master_bypass_set_rsp()) {
            m_master_bypass = rsp.master_bypass_set_rsp().bypass();
            m_button_master_bypass->value(m_master_bypass);
        }
        if(rsp.has_master_bypass_get_rsp()) {
            m_master_bypass = rsp.master_bypass_get_rsp().bypass();
            m_button_master_bypass->value(m_master_bypass);
        }
    }
}

void MainWindow::timer_callback(void* v) {
    Fl::repeat_timeout(1.0/10.0, MainWindow::timer_callback);
}
