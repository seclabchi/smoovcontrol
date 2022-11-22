//
//  adjuster_delay.hpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 10/29/22.
//

#ifndef adjuster_delay_hpp
#define adjuster_delay_hpp

#include <stdio.h>
#include <string>
#include <FL/Fl_Adjuster.H>

using namespace std;

class AdjusterDelay : public Fl_Adjuster {
public:
    AdjusterDelay(int _x, int _y, int _w, int _h, string _name);
    virtual ~AdjusterDelay();
    virtual int handle(int event);
};

#endif /* adjuster_delay_hpp */
