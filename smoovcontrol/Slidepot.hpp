//
//  Slidepot.hpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 9/30/22.
//

#ifndef Slidepot_hpp
#define Slidepot_hpp

#include <stdio.h>

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Fill_Slider.H>
#include <FL/Fl_Output.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Multiline_Output.H>

#include "common_defs.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Commander.hpp"

#include <string>

using namespace std;

class MainWindow;

class Slidepot : public Fl_Group {
public:
    Slidepot(MainWindow* _mw, string _name, int _x, int _y, int _w, int _h);
    virtual ~Slidepot();
    void set_level(double l);
    void set_bounds(double a, double b);
    static void slider_cb_wrapper(Fl_Widget *w, void *me);
    void slider_cb(Fl_Widget* w);
    void set_commander(Commander* cmdr);
    float get_level();
private:
    MainWindow* mw;
    std::shared_ptr<spdlog::logger> log;
    Slidepot();
    Fl_Multiline_Output* name;
    Fl_Slider* slide;
    Fl_Output* val;
    Commander* m_cmdr;
};


#endif /* Slidepot_hpp */
