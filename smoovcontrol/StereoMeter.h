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

#include <string>

using namespace std;

class StereoMeter : public Fl_Group {
public:
	StereoMeter(string _name, int _x, int _y, int _w, int _h);
	virtual ~StereoMeter();
	void set_level(double l, double r);
	static void slider_cb_wrapper(Fl_Widget *w, void *me);
	void slider_cb(Fl_Widget* w);
private:
	StereoMeter();
	Fl_Box* box;
	Fl_Fill_Slider* slide_l;
	Fl_Fill_Slider* slide_r;
	int x, y, w, h;
    string m_name;
};

#endif /* STEREOMETER_H_ */
