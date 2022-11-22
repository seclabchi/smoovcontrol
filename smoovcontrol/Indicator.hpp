//
//  Indicator.hpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 10/5/22.
//

#ifndef Indicator_hpp
#define Indicator_hpp

#include <stdio.h>

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Light_Button.H>
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

class Indicator : public Fl_Group {
public:
    Indicator(MainWindow* _mw, string _name, int _x, int _y, int _w, int _h);
    virtual ~Indicator();
    void set_status(bool status);
    bool get_status();
private:
    MainWindow* mw;
    std::shared_ptr<spdlog::logger> log;
    Indicator();
    Fl_Multiline_Output* name;
    Fl_Light_Button* button;
};

#endif /* Indicator_hpp */
