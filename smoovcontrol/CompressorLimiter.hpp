//
//  CompressorLimiter.hpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 10/28/22.
//

#ifndef CompressorLimiter_hpp
#define CompressorLimiter_hpp

#include <stdio.h>
#include <FL/Fl_Group.H>
#include "StereoMeter.h"
#include "Meter.hpp"

#include "fmsmoov.pb.h"


class CompressorLimiter : public Fl_Group {
public:
    CompressorLimiter(int _x, int _y, int _w, int _h, string _name);
    virtual ~CompressorLimiter();
    void set_levels(const fmsmoov::ProcessorLiveData& pld);
private:
    StereoMeter* m_meter_precomp_b0;
    StereoMeter* m_meter_precomp_b1;
    StereoMeter* m_meter_precomp_b2;
    StereoMeter* m_meter_precomp_b3;
    StereoMeter* m_meter_precomp_b4;
    StereoMeter* m_meter_precomp_b5;
    Meter* m_meter_comp_b0;
    Meter* m_meter_comp_b1;
    Meter* m_meter_comp_b2;
    Meter* m_meter_comp_b3;
    Meter* m_meter_comp_b4;
    Meter* m_meter_comp_b5;
    StereoMeter* m_meter_postcomp_b0;
    StereoMeter* m_meter_postcomp_b1;
    StereoMeter* m_meter_postcomp_b2;
    StereoMeter* m_meter_postcomp_b3;
    StereoMeter* m_meter_postcomp_b4;
    StereoMeter* m_meter_postcomp_b5;
    CompressorLimiter();
};

#endif /* CompressorLimiter_hpp */
