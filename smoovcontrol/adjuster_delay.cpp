//
//  adjuster_delay.cpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 10/29/22.
//

#include "adjuster_delay.hpp"
#include <iostream>
#include <FL/Fl.H>

using namespace std;

AdjusterDelay::AdjusterDelay(int _x, int _y, int _w, int _h, string _name): Fl_Adjuster(_x, _y, _w, _h, _name.c_str()) {
    
}

AdjusterDelay::~AdjusterDelay() {
    
}

int AdjusterDelay::handle(int event) {
    cout << "adjuster got event " << event << endl;
    if(FL_RELEASE == event) {
        //mouse went up on the adjuster, fire the delay adjust event
        
    }
    return Fl_Adjuster::handle(event);
}
