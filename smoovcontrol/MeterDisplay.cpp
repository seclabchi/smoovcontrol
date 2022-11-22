//
//  MeterDisplay.cpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 10/5/22.
//

#include "MeterDisplay.hpp"

MeterDisplay::MeterDisplay(int _x, int _y, int _w, int _h, string _name) : Fl_Fill_Slider(_x, _y, _w, _h, _name.c_str()) {
    this->labeltype(FL_NO_LABEL);
}

MeterDisplay::~MeterDisplay() {
    
}
