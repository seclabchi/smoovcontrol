//
//  MeterDisplay.hpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 10/5/22.
//

#ifndef MeterDisplay_hpp
#define MeterDisplay_hpp

#include <stdio.h>

#include <FL/Fl_Fill_Slider.H>
#include <FL/Enumerations.H>

#include <string>

using namespace std;

class MeterDisplay : public Fl_Fill_Slider {
public:
    MeterDisplay(int _x, int _y, int _w, int _h, string _name);
    virtual ~MeterDisplay();
private:
    MeterDisplay();
};

#endif /* MeterDisplay_hpp */
