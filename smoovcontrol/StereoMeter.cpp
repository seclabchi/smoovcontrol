/*
 * StereoMeter.cpp
 *
 *  Created on: Jul 22, 2022
 *      Author: zaremba
 */

#include <iostream>
#include "StereoMeter.h"


using namespace std;

StereoMeter::StereoMeter(int _x, int _y, int _w, int _h, string _name) : Fl_Group(_x, _y, _w, _h, _name.c_str())  {
    this->labeltype(FL_NO_LABEL);
    this->box(FL_DOWN_FRAME);
    if(!_name.compare("")) {
        meter_label = new Fl_Multiline_Output(x(), y(), w(), 0);
    }
    else {
        meter_label = new Fl_Multiline_Output(x(), y(), w(), 25);
    }
    meter_label->value(label());
    meter_label->set_output();
    meter_label->box(FL_NO_BOX);
    meter_label->textsize(11);
    meter_label->textfont(FL_HELVETICA);
    meter_label->align(FL_ALIGN_CENTER);
    val_l = new Fl_Output(meter_label->x(), y() + h() - 16, meter_label->w()/2-2, 16);
    val_r = new Fl_Output(val_l->x()+val_l->w()+2, val_l->y(), val_l->w(), val_l->h());
	slide_l = new Fl_Fill_Slider(val_l->x(), meter_label->y()+meter_label->h(), val_l->w(), h() - val_l->h() - meter_label->h());
	slide_l->label(FL_NO_LABEL, "L");
	slide_l->color(FL_BLACK, FL_GREEN);
	slide_l->minimum(6);
	slide_l->maximum(-80);
	slide_l->value(-70);
	slide_l->callback(StereoMeter::slider_cb_wrapper, this);
    slide_l->set_output();
	slide_r = new Fl_Fill_Slider(slide_l->x() + slide_l->w() + 2, slide_l->y(), slide_l->w(), slide_l->h());
	slide_r->label(FL_NO_LABEL, "R");
	slide_r->color(FL_BLACK, FL_GREEN);
	slide_r->minimum(6);
	slide_r->maximum(-80);
	slide_r->value(-70);
	slide_r->callback(StereoMeter::slider_cb_wrapper, this);
    slide_r->set_output();
    
    val_l->textsize(9);
    val_l->set_output();
    val_r->set_output();
    val_r->textsize(9);
    val_l->value("0");
    val_r->value("0");
	this->end();
}

StereoMeter::~StereoMeter() {
}

void StereoMeter::adjust_h(uint32_t h) {
    this->h(h);
}

void StereoMeter::slider_cb_wrapper(Fl_Widget *w, void *me) {
	StereoMeter* sm = (StereoMeter*)me;
	sm->slider_cb(w);
}

void StereoMeter::slider_cb(Fl_Widget* w) {
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

void StereoMeter::set_level(double l, double r) {
    int intl, intr;
    intl = (int)l;
    intr = (int)r;
    
	slide_l->value(l);
    if(slide_l->value() > -0.0) {
        slide_l->color(FL_BLACK, FL_RED);
    }
    else if(slide_l->value() > -6.0) {
        slide_l->color(FL_BLACK, FL_YELLOW);
    }
    else {
        slide_l->color(FL_BLACK, FL_GREEN);
    }
    val_l->value(to_string(intl).c_str());
    
	slide_r->value(r);
    if(slide_r->value() > -0.0) {
        slide_r->color(FL_BLACK, FL_RED);
    }
    else if(slide_r->value() > -6.0) {
        slide_r->color(FL_BLACK, FL_YELLOW);
    }
    else {
        slide_r->color(FL_BLACK, FL_GREEN);
    }
    val_r->value(to_string(intr).c_str());
    
    slide_l->redraw();
    slide_r->redraw();
    val_l->redraw();
    val_r->redraw();
}

