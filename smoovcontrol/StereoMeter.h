/*
 * StereoMeter.h
 *
 *  Created on: Jul 22, 2022
 *      Author: zaremba
 */

#ifndef STEREOMETER_H_
#define STEREOMETER_H_

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Fill_Slider.H>
#include <FL/Fl_Output.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Multiline_Output.H>

#include <string>

using namespace std;

class StereoMeter : public Fl_Group {
public:
	StereoMeter(int _x, int _y, int _w, int _h, string _name);
	virtual ~StereoMeter();
	void set_level(double l, double r);
	static void slider_cb_wrapper(Fl_Widget *w, void *me);
	void slider_cb(Fl_Widget* w);
    void adjust_h(uint32_t h);
private:
	StereoMeter();
    Fl_Multiline_Output* meter_label;
	Fl_Fill_Slider* slide_l;
	Fl_Fill_Slider* slide_r;
    Fl_Output* val_l;
    Fl_Output* val_r;
};

#endif /* STEREOMETER_H_ */
