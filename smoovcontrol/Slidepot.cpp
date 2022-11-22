//
//  Slidepot.cpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 9/30/22.
//

#include "Slidepot.hpp"

#include <sstream>

#include "fmsmoov.pb.h"

using namespace std;

Slidepot::Slidepot(MainWindow* _mw, string _name, int _x, int _y, int _w, int _h) : mw(_mw), Fl_Group(_x, _y, _w, _h, _name.c_str())  {
    log = spdlog::stdout_color_mt(string("SLIDEPOT"), spdlog::color_mode::always);
    log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
    log->set_level(spdlog::level::trace);
    this->labeltype(FL_NO_LABEL);
    this->clip_children(1);
    name = new Fl_Multiline_Output(x(), y(), w(), 24);
    name->value("IN\nGAIN");
    name->box(FL_NO_BOX);
    name->textsize(11);
    name->set_output();
    
    val = new Fl_Output(x(), y()+h()-16, w(), 16);
    val->align(FL_ALIGN_BOTTOM_LEFT);
    val->set_output();
    val->textsize(9);
    
    slide = new Fl_Slider(x(), y() + name->h(), w(), h() - val->h() - name->h(), _name.c_str());
    slide->labeltype(FL_NO_LABEL);
    slide->type(FL_VERT_NICE_SLIDER);
    slide->bounds(60, -20.0);
    slide->value(0.0);
    slide->box(FL_FLAT_BOX);
    slide->callback(slider_cb_wrapper, this);
    slide->selection_color(FL_YELLOW);
    slide->step(0.1);
    slide->callback(Slidepot::slider_cb_wrapper, this);
    
    this->end();
}

Slidepot::~Slidepot() {
}

void Slidepot::set_commander(Commander *cmdr) {
    m_cmdr = cmdr;
}

void Slidepot::slider_cb_wrapper(Fl_Widget *w, void *me) {
    Slidepot* sp = (Slidepot*)me;
    sp->slider_cb(w);
}

void Slidepot::slider_cb(Fl_Widget* w) {
    stringstream ss;
    ss.precision(1);
    ss << std::fixed << slide->value();
    val->value(ss.str().c_str());
    this->do_callback(slide, mw);
}

void Slidepot::set_level(double l) {
    slide->value(l);
    stringstream ss;
    ss.precision(1);
    ss << std::fixed << slide->value();
    val->value(ss.str().c_str());
    slide->redraw();
    val->redraw();
}

float Slidepot::get_level() {
    return slide->value();
}
