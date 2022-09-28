/*
 * MainWindow.cpp
 *
 *  Created on: Jul 19, 2022
 *      Author: zaremba
 */

#include "MainWindow.h"
#include "CommThread.h"

MainWindow::MainWindow() {
    log = spdlog::stdout_color_mt("MAINWINDOW");
    log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
    log->set_level(spdlog::level::trace);
    m_shutting_down = false;
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

void MainWindow::set_comm_thread(CommThread* comm_thread) {
    m_comm_thread = comm_thread;
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
    logoutput = new Fl_Text_Display(30, 650, 960, 100);
    m_window->callback(window_callback_wrapper, this);
    m_window->end();
    m_window->show(argc, argv);
    return Fl::run();
}

void MainWindow::set_live_data(const fmsmoov::ProcessorLiveData& pld) {
    if(false == m_shutting_down) {
        Fl::lock();
        sm_master_in->set_level(pld.inl(), pld.inr());
        sm_master_out->set_level(pld.outl(), pld.outr());
        Fl::awake();
        Fl::unlock();
    }
    else {
        LOGT("Discarding live data call due to shutting down...");
    }
}

void MainWindow::logmsg(string msg) {
    logoutput->insert(msg.c_str());
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
