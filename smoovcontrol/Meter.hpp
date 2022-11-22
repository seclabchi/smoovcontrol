//
//  Meter.hpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 9/30/22.
//

#ifndef Meter_hpp
#define Meter_hpp

#include <stdio.h>

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Fill_Slider.H>
#include <FL/Fl_Output.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Multiline_Output.H>

#include <string>

using namespace std;

class Meter : public Fl_Group {
public:
    Meter(int _x, int _y, int _w, int _h, string _name);
    virtual ~Meter();
    void set_level(double l);
    void set_active(bool active);
    void set_bounds(double a, double b);
    void set_colors(Fl_Color bg_col, Fl_Color fg_col);
    static void slider_cb_wrapper(Fl_Widget *w, void *me);
    void slider_cb(Fl_Widget* w);
    void adjust_h(uint32_t h);
private:
    Meter();
    Fl_Multiline_Output* meter_label;
    Fl_Fill_Slider* slider_meter;
    Fl_Output* val;
};

#endif /* Meter_hpp */
