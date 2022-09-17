/*
 * StereoMeter.cpp
 *
 *  Created on: Jul 22, 2022
 *      Author: zaremba
 */

#include <iostream>
#include "StereoMeter.h"
#include <FL/Enumerations.H>
#include <FL/Fl_Multiline_Output.H>

using namespace std;

StereoMeter::StereoMeter(int _x, int _y, int _w, int _h)
		: x(_x), y(_y), w(_w), h(_h), Fl_Group(_x, _y, _w, _h)  {
	box = new Fl_Box(FL_BORDER_BOX, x, y, w, h, "");
	slide_l = new Fl_Fill_Slider(x+2, y+2, w/2-4, h-24);
	slide_l->label(FL_NO_LABEL, "L");
	slide_l->color(FL_BLACK, FL_GREEN);
	slide_l->minimum(6);
	slide_l->maximum(-60);
	slide_l->value(-55);
	slide_l->callback(StereoMeter::slider_cb_wrapper, this);
	slide_r = new Fl_Fill_Slider(x + w/2+2, y+2, w/2-4, h-24);
	slide_r->label(FL_NO_LABEL, "R");
	slide_r->color(FL_BLACK, FL_GREEN);
	slide_r->minimum(6);
	slide_r->maximum(-60);
	slide_r->value(-55);
	slide_r->callback(StereoMeter::slider_cb_wrapper, this);
	this->end();
}

StereoMeter::~StereoMeter() {
	delete box;
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
	slide_l->value(l);
	slide_r->value(r);
}

