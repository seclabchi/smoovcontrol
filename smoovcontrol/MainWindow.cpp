/*
 * MainWindow.cpp
 *
 *  Created on: Jul 19, 2022
 *      Author: zaremba
 */

#include "MainWindow.h"
#include "Subscriber.h"
#include "Commander.hpp"

#include <sstream>

#include <FL/abi-version.h>
#include <FL/Fl_Export.H>
#include <FL/fl_types.h>
#include <FL/Fl_Input.H>

#include "fmsmoov.pb.h"

//std::shared_ptr<spdlog::logger> MainWindow::log;

MainWindow::MainWindow() {
    log = spdlog::stdout_color_mt("MAINWINDOW");
    log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
    log->set_level(spdlog::level::debug);
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
    m_window = new Fl_Double_Window(900, 768);
    m_window->label("FMSmoov Control");
    Fl_Box *box = new Fl_Box(10,10,1004,748);
    box->box(FL_BORDER_BOX);
    sm_master_in = new StereoMeter(10, 10, 50, 410, "MAIN\nIN");
    
    meter_agc_adjust = new Meter(sm_master_in->x()+sm_master_in->w() + 10, sm_master_in->y(), 30, sm_master_in->h(), "AGC\nADJ");
    meter_agc_adjust->set_bounds(40, -40);
    
    compressor_limiter = new CompressorLimiter(meter_agc_adjust->x() + meter_agc_adjust->w() + 10, meter_agc_adjust->y(), 330, sm_master_in->h(), "COMPRESSOR_LIMITER");
    
    sm_master_out = new StereoMeter(compressor_limiter->x()+compressor_limiter->w() + 10, compressor_limiter->y(), sm_master_in->w(), compressor_limiter->h(), "MAIN\nOUT");
    
    m_input_ipaddr = new Fl_Input(sm_master_out->x() + sm_master_out->w() + 100, sm_master_out->y(), 100, 30, "IP_ADDR");
    m_button_connected = new Fl_Light_Button(m_input_ipaddr->x(), m_input_ipaddr->y() + m_input_ipaddr->h() + 5, 100, 30, "CONNECT");
    m_button_connected->callback(cb_connect_wrapper, this);
    
    sm_master_in->adjust_h(sm_master_out->h());
    meter_agc_adjust->adjust_h(sm_master_in->h());
    
    m_button_master_bypass = new Fl_Light_Button(sm_master_in->x(), sm_master_in->y()+sm_master_in->h()+5, 120, 25, "MASTER BYPASS");
    m_button_master_bypass->labelsize(10);
    m_button_master_bypass->selection_color(FL_RED);
    m_button_master_bypass->callback(general_callback_wrapper, this);
    
    m_button_phase_rotator_enable = new Fl_Light_Button(m_button_master_bypass->x() + m_button_master_bypass->w() + 2, m_button_master_bypass->y(), m_button_master_bypass->w(), m_button_master_bypass->h(), "PHASE ROTATOR");
    m_button_phase_rotator_enable->labelsize(10);
    m_button_phase_rotator_enable->selection_color(FL_GREEN);
    m_button_phase_rotator_enable->callback(general_callback_wrapper, this);
    
    m_button_stereo_enhance_enable = new Fl_Light_Button(m_button_phase_rotator_enable->x() + m_button_phase_rotator_enable->w() + 2, m_button_phase_rotator_enable->y(), m_button_phase_rotator_enable->w(), m_button_phase_rotator_enable->h(), "STEREO ENHANCE");
    m_button_stereo_enhance_enable->labelsize(10);
    m_button_stereo_enhance_enable->selection_color(FL_GREEN);
    m_button_stereo_enhance_enable->callback(general_callback_wrapper, this);
    
    m_button_bass_enhancer_enable = new Fl_Light_Button(m_button_stereo_enhance_enable->x() + m_button_stereo_enhance_enable->w() + 2, m_button_stereo_enhance_enable->y(), m_button_stereo_enhance_enable->w(), m_button_stereo_enhance_enable->h(), "BASS ENHANCER");
    m_button_bass_enhancer_enable->labelsize(10);
    m_button_bass_enhancer_enable->selection_color(FL_GREEN);
    m_button_bass_enhancer_enable->callback(general_callback_wrapper, this);
    
    m_button_complim_enable = new Fl_Light_Button(m_button_bass_enhancer_enable->x() + m_button_bass_enhancer_enable->w() + 2, m_button_bass_enhancer_enable->y(), m_button_bass_enhancer_enable->w(), m_button_bass_enhancer_enable->h(), "COMP/LIM");
    m_button_complim_enable->labelsize(10);
    m_button_complim_enable->selection_color(FL_GREEN);
    m_button_complim_enable->callback(general_callback_wrapper, this);
    
    m_slidepot_input_gain = new Slidepot(this, "INPUT_GAIN", m_button_master_bypass->x()+(m_button_master_bypass->w()/2)-15, m_button_master_bypass->y()+m_button_master_bypass->h() + 4, 30, 200);
    m_slidepot_input_gain->callback(general_callback_wrapper, this);
    
    ind_agc_gate_active = new Indicator(this, "AGC GATE CLOSED", m_button_master_bypass->x(), m_slidepot_input_gain->y() +  m_slidepot_input_gain->h() + 5, 125, 25);
    
    
    
    m_counter_delay = new Fl_Counter(ind_agc_gate_active->x(), ind_agc_gate_active->y() + ind_agc_gate_active->h() + 5, 200, 30, "DELAY");
    m_counter_delay->bounds(0, 20000.0);
    m_counter_delay->step(100.0);
    m_counter_delay->lstep(1000.0);
    m_counter_delay->callback(cb_delay_wrapper, this);

    logoutput = new Fl_Text_Display(30, 650, 960, 100);

    
    //    test_meter_display = new MeterDisplay(sm_master_out->x() + sm_master_out->w() + 5, sm_master_out->y(), 20, 200, "TEST_METER_DISPLAY");
    
    m_window->callback(window_callback_wrapper, this);
    m_window->end();
    m_window->show(argc, argv);
    
    LOGT("Starting general window timer...");
    Fl::add_timeout(0.1, MainWindow::timer_callback);
    
    return Fl::run();
}

int MainWindow::connect(string _ipaddr) {
    
    std::mutex mutex_commander_startup;
    std::condition_variable cv_commander_startup;
    std::unique_lock lk_cmd(mutex_commander_startup);
    bool commander_started = false;

    LOGD("Starting Commander...");
    commander = new Commander(mutex_commander_startup, cv_commander_startup, commander_started, _ipaddr);
    // Start thread for 0mq/protobuf commands
    cmd_thread = new std::thread(std::ref(*commander), "1234");
    LOGD("Waiting for startup confirmation from Commander...");
    cv_commander_startup.wait(lk_cmd, [&]{return commander_started;}); // @suppress("Invalid arguments")

    std::mutex mutex_subscriber_startup;
    std::condition_variable cv_subscriber_startup;
    std::unique_lock lk_sub(mutex_subscriber_startup);
    bool subscriber_started = false;
    
    LOGD("Starting Subscriber...");
    subscriber = new Subscriber(mutex_subscriber_startup, cv_subscriber_startup, subscriber_started, _ipaddr);
    // Start thread for 0mq/protobuf subscription
    sub_thread = new std::thread(std::ref(*subscriber), "1234");
    LOGD("Waiting for startup confirmation from Subscriber...");
    cv_subscriber_startup.wait(lk_sub, [&]{return subscriber_started;}); // @suppress("Invalid arguments")
    
    commander->set_main_window(this);
    subscriber->set_main_window(this);
    this->set_commander(commander);
    this->set_subscriber(subscriber);
    
    LOGT("Starting get processor state timer...");
    Fl::add_timeout(1.0, MainWindow::timer_get_state, this);
    
    return 0;
}

void MainWindow::disconnect() {
    LOGD("Stopping commander thread and waiting to join...");
    commander->stop();
    cmd_thread->join();
    LOGD("Stopping subscriber thread and waiting to join...");
    subscriber->stop();
    sub_thread->join();
}

void MainWindow::set_live_data(const fmsmoov::ProcessorLiveData& pld) {
    if(false == m_shutting_down) {
        Fl::lock();
        sm_master_in->set_level(pld.inl(), pld.inr());
        sm_master_out->set_level(pld.outl(), pld.outr());
        meter_agc_adjust->set_level(pld.agc_adj());
        ind_agc_gate_active->set_status(pld.agc_gate_active());
        meter_agc_adjust->set_active(pld.agc_gate_active());
        compressor_limiter->set_levels(pld);
        
        Fl::unlock();
        if(pld.has_started()) {
            fmsmoov::ProcessorCommand cmd;
            cmd.mutable_master_bypass_get();
            m_commander->enqueue_command(cmd);
        }
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
    LOGT("General callback from widget with label {}!", widget->label());
    fmsmoov::ProcessorCommand maincmd;
    
    string label(widget->label());
    
    if(widget->user_data()) {
        if(!label.compare("MASTER BYPASS")) {
            if(m_master_bypass) {
                maincmd.mutable_master_bypass_set()->set_bypass(0);
                m_master_bypass = 0;
            }
            else {
                maincmd.mutable_master_bypass_set()->set_bypass(1);
                m_master_bypass = 1;
            }
            m_commander->enqueue_command(maincmd);
        }
        else if(!label.compare("PHASE ROTATOR")) {
            if(m_phase_rotator_enabled) {
                maincmd.mutable_phase_rotator_set()->set_enable(false);
                m_phase_rotator_enabled = 0;
            }
            else {
                maincmd.mutable_phase_rotator_set()->set_enable(true);
                m_phase_rotator_enabled = 1;
            }
            m_commander->enqueue_command(maincmd);
        }
        else if(!label.compare("STEREO ENHANCE")) {
            if(m_stereo_enhance_enabled) {
                maincmd.mutable_stereo_enhance_set()->set_enable(false);
                m_stereo_enhance_enabled = 0;
            }
            else {
                maincmd.mutable_stereo_enhance_set()->set_enable(true);
                m_stereo_enhance_enabled = 1;
            }
            m_commander->enqueue_command(maincmd);
        }
        else if(!label.compare("BASS ENHANCER")) {
            if(m_bass_enhancer_enabled) {
                maincmd.mutable_bass_enhancer_set()->set_enable(false);
                m_bass_enhancer_enabled = 0;
            }
            else {
                maincmd.mutable_bass_enhancer_set()->set_enable(true);
                m_bass_enhancer_enabled = 1;
            }
            m_commander->enqueue_command(maincmd);
        }
        else if(!label.compare("COMP/LIM")) {
            if(m_complim_enabled) {
                maincmd.mutable_complim_set()->set_enable(false);
                m_complim_enabled = 0;
            }
            else {
                maincmd.mutable_complim_set()->set_enable(true);
                m_complim_enabled = 1;
            }
            m_commander->enqueue_command(maincmd);
        }
        else if(!label.compare("INPUT_GAIN")) {
            Fl_Slider* s = reinterpret_cast<Fl_Slider*>(widget);
            maincmd.mutable_gain_set()->set_gain(s->value());
            maincmd.mutable_gain_set()->set_which(fmsmoov::GainControlType::MAIN_IN);
            maincmd.mutable_gain_set()->set_type(fmsmoov::GainType::LOG);
            m_commander->enqueue_command(maincmd);
        }
    }
    else {
        LOGW("Callback from widget with NULL userdata.");
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

void MainWindow::cb_connect_wrapper(Fl_Widget *widget, void* user) {
    MainWindow* mw = reinterpret_cast<MainWindow*>(user);
    mw->cb_connect(widget);
}

void MainWindow::cb_connect(Fl_Widget *widget) {
    if(m_connected) {
        this->disconnect();
        m_connected = false;
    }
    else {
        this->connect(m_input_ipaddr->value());
        m_connected = true;
    }
}


void MainWindow::handle_command_response(const fmsmoov::ProcessorResponse& rsp) {
    if(false == m_shutting_down) {
        fmsmoov::ResponseCode rspcode = rsp.response();
        string rspcodestr = fmsmoov::ResponseCode_Name(rspcode);
        LOGD("Command response is {}:{}", rspcodestr, rsp.response_msg());
        
        if(rsp.has_master_bypass_set_rsp()) {
            //TODO: something?
        }
        if(rsp.has_master_bypass_get_rsp()) {
            m_master_bypass = rsp.master_bypass_get_rsp().bypass();
            m_button_master_bypass->value(m_master_bypass);
        }
        if(rsp.has_gain_set_rsp()) {
            //TODO: something?
        }
        if(rsp.has_gain_get_rsp()) {
            m_slidepot_input_gain->set_level(rsp.gain_get_rsp().gain());
        }
        if(rsp.has_phase_rotator_get_rsp()) {
            m_phase_rotator_enabled = rsp.phase_rotator_get_rsp().enabled();
            m_button_phase_rotator_enable->value(m_phase_rotator_enabled);
        }
        if(rsp.has_phase_rotator_get_rsp()) {
            m_phase_rotator_enabled = rsp.phase_rotator_get_rsp().enabled();
            m_button_phase_rotator_enable->value(m_phase_rotator_enabled);
        }
        if(rsp.has_stereo_enhance_get_rsp()) {
            m_stereo_enhance_enabled = rsp.stereo_enhance_get_rsp().enabled();
            m_button_stereo_enhance_enable->value(m_stereo_enhance_enabled);
        }
        if(rsp.has_bass_enhancer_get_rsp()) {
            m_bass_enhancer_enabled = rsp.bass_enhancer_get_rsp().enabled();
            m_button_bass_enhancer_enable->value(m_bass_enhancer_enabled);
        }
        if(rsp.has_complim_get_rsp()) {
            m_complim_enabled = rsp.complim_get_rsp().enabled();
            m_button_complim_enable->value(m_complim_enabled);
        }
    }
}

void MainWindow::timer_get_state(void* v) {
    MainWindow* mw = reinterpret_cast<MainWindow*>(v);
    fmsmoov::ProcessorCommand cmd;
    cmd.mutable_master_bypass_get();
    mw->m_commander->enqueue_command(cmd);
    cmd.mutable_gain_get()->set_which(fmsmoov::GainControlType::MAIN_IN);
    mw->m_commander->enqueue_command(cmd);
    cmd.mutable_phase_rotator_get();
    mw->m_commander->enqueue_command(cmd);
    cmd.mutable_stereo_enhance_get();
    mw->m_commander->enqueue_command(cmd);
    cmd.mutable_bass_enhancer_get();
    mw->m_commander->enqueue_command(cmd);
    cmd.mutable_complim_get();
    mw->m_commander->enqueue_command(cmd);
}

void MainWindow::timer_callback(void* v) {
    Fl::repeat_timeout(1.0/10.0, MainWindow::timer_callback);
}

void MainWindow::cb_delay_wrapper(Fl_Widget *widget, void *user) {
    MainWindow* mw = reinterpret_cast<MainWindow*>(user);
    mw->cb_delay(widget, user);
}

void MainWindow::cb_delay(Fl_Widget *widget, void* user) {
    Fl::lock();
    Fl_Counter* adj = reinterpret_cast<Fl_Counter*>(widget);
    LOGD("Delay changed to {} ms", adj->value());
    fmsmoov::ProcessorCommand maincmd;
    maincmd.mutable_delay_set()->set_delay_ms(adj->value());
    this->m_commander->enqueue_command(maincmd);
    Fl::unlock();
}
