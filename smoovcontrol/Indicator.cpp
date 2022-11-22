//
//  Indicator.cpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 10/5/22.
//

#include "Indicator.hpp"

#include <sstream>

#include "fmsmoov.pb.h"

using namespace std;

Indicator::Indicator(MainWindow* _mw, string _name, int _x, int _y, int _w, int _h) : mw(_mw), Fl_Group(_x, _y, _w, _h, _name.c_str())  {
    this->labeltype(FL_NO_LABEL);
    log = spdlog::stdout_color_mt(string("INDICATOR"), spdlog::color_mode::always);
    log->set_pattern("%^[%H%M%S.%e][%s:%#][%n][%l] %v%$");
    log->set_level(spdlog::level::trace);
    //this->labeltype(FL_NO_LABEL);
    this->clip_children(1);
    button = new Fl_Light_Button(x(), y(), w(), h(), _name.c_str());
    button->labelsize(10);
    button->set_output();
    button->selection_color(FL_RED);
    //name = new Fl_Multiline_Output(x(), y(), w(), 24);
    //name->value("IN\nGAIN");
    //name->box(FL_NO_BOX);
    //name->textsize(11);
    //name->set_output();
    this->end();
}

Indicator::~Indicator() {
}

void Indicator::set_status(bool status) {
    button->value(status);
}
