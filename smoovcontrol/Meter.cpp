//
//  Meter.cpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 9/30/22.
//

#include "Meter.hpp"
#include <iostream>
#include "StereoMeter.h"


using namespace std;

Meter::Meter(int _x, int _y, int _w, int _h, string _name) : Fl_Group(_x, _y, _w, _h, _name.c_str())  {
    this->labeltype(FL_NO_LABEL);
    this->clip_children(1);
    this->box(FL_DOWN_FRAME);
    if(_name.compare("")) {
        meter_label = new Fl_Multiline_Output(x(), y(), w(), 25);
    }
    else {
        meter_label = new Fl_Multiline_Output(x(), y(), w(), 0);
    }
    meter_label->value(label());
    meter_label->set_output();
    meter_label->box(FL_NO_BOX);
    meter_label->textsize(11);
    meter_label->textfont(FL_HELVETICA);
    val = new Fl_Output(meter_label->x(), y() + h() - 16, meter_label->w(), 16);
    slider_meter = new Fl_Fill_Slider(val->x(), meter_label->y()+meter_label->h(), this->w(), h() - val->h() - meter_label->h());
    slider_meter->label(FL_NO_LABEL, "L");
    slider_meter->color(FL_BLACK, FL_GREEN);
    slider_meter->minimum(20);
    slider_meter->maximum(-20);
    slider_meter->callback(Meter::slider_cb_wrapper, this);
    slider_meter->set_output();

    val->textsize(9);
    val->set_output();
    val->value("0");
    this->end();
}

Meter::~Meter() {
}

void Meter::set_bounds(double a, double b) {
    slider_meter->bounds(a, b);
}

void Meter::adjust_h(uint32_t h) {
    this->h(h);
}

void Meter::set_colors(Fl_Color bg_col, Fl_Color fg_col) {
    slider_meter->color(bg_col, fg_col);
}

void Meter::slider_cb_wrapper(Fl_Widget *w, void *me) {
    Meter* m = (Meter*)me;
    m->slider_cb(w);
}

void Meter::slider_cb(Fl_Widget* w) {
    Fl_Fill_Slider* s = reinterpret_cast<Fl_Fill_Slider*>(w);
    if(s->value() > 0.0) {
        s->color(FL_BLACK, FL_RED);
    }
    else if(s->value() > -6.0) {
        s->color(FL_BLACK, FL_YELLOW);
    }
    else {
        s->color(FL_BLACK, FL_GREEN);
    }
}

void Meter::set_level(double l) {
    int intl;
    intl = (int)l;
    
    slider_meter->value(l);
    
    //TODO: user-defined color ranges
    /*if(slider_meter->value() > -0.0) {
        slider_meter->color(FL_BLACK, FL_RED);
    }
    else if(slider_meter->value() > -6.0) {
        slider_meter->color(FL_BLACK, FL_YELLOW);
    }
    else {
        slider_meter->color(FL_BLACK, FL_GREEN);
    }
    */
    val->value(to_string(intl).c_str());
    
    slider_meter->redraw();
    val->redraw();
}

void Meter::set_active(bool active) {
    if(active) {
        slider_meter->selection_color(FL_RED);
    }
    else {
        slider_meter->selection_color(FL_GREEN);
    }
}
