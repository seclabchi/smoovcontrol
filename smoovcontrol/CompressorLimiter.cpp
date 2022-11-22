//
//  CompressorLimiter.cpp
//  smoovcontrol
//
//  Created by Paul Zaremba on 10/28/22.
//

#include "CompressorLimiter.hpp"
#include <FL/Enumerations.H>

CompressorLimiter::CompressorLimiter(int _x, int _y, int _w, int _h, string _name) : Fl_Group(_x, _y, _w, _h, _name.c_str()) {
    this->labeltype(FL_NO_LABEL);
    this->clip_children(1);
    
    this->box(FL_DOWN_FRAME);
    
    m_meter_precomp_b0 = new StereoMeter(x(), y(), 50, h()/3, "\n150 Hz");
    m_meter_precomp_b1 = new StereoMeter(m_meter_precomp_b0->x() + m_meter_precomp_b0->w() + 10, y(), m_meter_precomp_b0->w(), m_meter_precomp_b0->h(), "\n420 Hz");
    m_meter_precomp_b2 = new StereoMeter(m_meter_precomp_b1->x() + m_meter_precomp_b1->w() + 10, y(), m_meter_precomp_b0->w(), m_meter_precomp_b0->h(), "\n700 Hz");
    m_meter_precomp_b3 = new StereoMeter(m_meter_precomp_b2->x() + m_meter_precomp_b2->w() + 10, y(), m_meter_precomp_b0->w(), m_meter_precomp_b0->h(), "\n1.6 kHz");
    m_meter_precomp_b4 = new StereoMeter(m_meter_precomp_b3->x() + m_meter_precomp_b3->w() + 10, y(), m_meter_precomp_b0->w(), m_meter_precomp_b0->h(), "\n3.7 kHz");
    m_meter_precomp_b5 = new StereoMeter(m_meter_precomp_b4->x() + m_meter_precomp_b4->w() + 10, y(), m_meter_precomp_b0->w(), m_meter_precomp_b0->h(), "\n6.2 kHz");
    this->w(m_meter_precomp_b5->x() + m_meter_precomp_b5->w() - m_meter_precomp_b0->x());
    m_meter_comp_b0 = new Meter(m_meter_precomp_b0->x() + m_meter_precomp_b0->w()/4, m_meter_precomp_b0->y() + m_meter_precomp_b0->h() + 5, 25, h()/3, "");
    m_meter_comp_b1 = new Meter(m_meter_precomp_b1->x() + m_meter_precomp_b1->w()/4, m_meter_comp_b0->y(), m_meter_comp_b0->w(), m_meter_comp_b0->h(), "");
    m_meter_comp_b2 = new Meter(m_meter_precomp_b2->x() + m_meter_precomp_b2->w()/4, m_meter_comp_b1->y(), m_meter_comp_b1->w(), m_meter_comp_b1->h(), "");
    m_meter_comp_b3 = new Meter(m_meter_precomp_b3->x() + m_meter_precomp_b3->w()/4, m_meter_comp_b2->y(), m_meter_comp_b2->w(), m_meter_comp_b2->h(), "");
    m_meter_comp_b4 = new Meter(m_meter_precomp_b4->x() + m_meter_precomp_b4->w()/4, m_meter_comp_b3->y(), m_meter_comp_b3->w(), m_meter_comp_b3->h(), "");
    m_meter_comp_b5 = new Meter(m_meter_precomp_b5->x() + m_meter_precomp_b5->w()/4, m_meter_comp_b4->y(), m_meter_comp_b4->w(), m_meter_comp_b4->h(), "");
    m_meter_comp_b0->set_bounds(0, -10);
    m_meter_comp_b0->set_colors(FL_MAGENTA, FL_BLACK);
    m_meter_comp_b1->set_bounds(0, -10);
    m_meter_comp_b1->set_colors(FL_MAGENTA, FL_BLACK);
    m_meter_comp_b2->set_bounds(0, -10);
    m_meter_comp_b2->set_colors(FL_MAGENTA, FL_BLACK);
    m_meter_comp_b3->set_bounds(0, -10);
    m_meter_comp_b3->set_colors(FL_MAGENTA, FL_BLACK);
    m_meter_comp_b4->set_bounds(0, -10);
    m_meter_comp_b4->set_colors(FL_MAGENTA, FL_BLACK);
    m_meter_comp_b5->set_bounds(0, -10);
    m_meter_comp_b5->set_colors(FL_MAGENTA, FL_BLACK);
    m_meter_postcomp_b0 = new StereoMeter(m_meter_precomp_b0->x(), m_meter_comp_b0->y() + m_meter_comp_b0->h() + 5, m_meter_precomp_b0->w(), h()/3, "");
    m_meter_postcomp_b1 = new StereoMeter(m_meter_postcomp_b0->x() + m_meter_postcomp_b0->w() + 10, m_meter_postcomp_b0->y(), m_meter_postcomp_b0->w(), m_meter_postcomp_b0->h(), "");
    m_meter_postcomp_b2 = new StereoMeter(m_meter_postcomp_b1->x() + m_meter_postcomp_b0->w() + 10, m_meter_postcomp_b0->y(), m_meter_postcomp_b0->w(), m_meter_postcomp_b0->h(), "");
    m_meter_postcomp_b3 = new StereoMeter(m_meter_postcomp_b2->x() + m_meter_postcomp_b0->w() + 10, m_meter_postcomp_b0->y(), m_meter_postcomp_b0->w(), m_meter_postcomp_b0->h(), "");
    m_meter_postcomp_b4 = new StereoMeter(m_meter_postcomp_b3->x() + m_meter_postcomp_b0->w() + 10, m_meter_postcomp_b0->y(), m_meter_postcomp_b0->w(), m_meter_postcomp_b0->h(), "");
    m_meter_postcomp_b5 = new StereoMeter(m_meter_postcomp_b4->x() + m_meter_postcomp_b0->w() + 10, m_meter_postcomp_b0->y(), m_meter_postcomp_b0->w(), m_meter_postcomp_b0->h(), "");
    this->h(m_meter_postcomp_b0->y() + m_meter_postcomp_b0->h());
        
    this->end();
}

CompressorLimiter::~CompressorLimiter() {
    delete m_meter_precomp_b0;
    delete m_meter_precomp_b1;
    delete m_meter_precomp_b2;
    delete m_meter_precomp_b3;
    delete m_meter_precomp_b4;
    delete m_meter_precomp_b5;
}

void CompressorLimiter::set_levels(const fmsmoov::ProcessorLiveData& pld) {
    m_meter_precomp_b0->set_level(pld.lev_precomp_b0l(), pld.lev_precomp_b0r());
    m_meter_precomp_b1->set_level(pld.lev_precomp_b1l(), pld.lev_precomp_b1r());
    m_meter_precomp_b2->set_level(pld.lev_precomp_b2l(), pld.lev_precomp_b2r());
    m_meter_precomp_b3->set_level(pld.lev_precomp_b3l(), pld.lev_precomp_b3r());
    m_meter_precomp_b4->set_level(pld.lev_precomp_b4l(), pld.lev_precomp_b4r());
    m_meter_precomp_b5->set_level(pld.lev_precomp_b5l(), pld.lev_precomp_b5r());
    m_meter_comp_b0->set_level(pld.lev_comp_b0());
    m_meter_comp_b1->set_level(pld.lev_comp_b1());
    m_meter_comp_b2->set_level(pld.lev_comp_b2());
    m_meter_comp_b3->set_level(pld.lev_comp_b3());
    m_meter_comp_b4->set_level(pld.lev_comp_b4());
    m_meter_comp_b5->set_level(pld.lev_comp_b5());
    m_meter_postcomp_b0->set_level(pld.lev_postcomp_b0l(), pld.lev_postcomp_b0r());
    m_meter_postcomp_b1->set_level(pld.lev_postcomp_b1l(), pld.lev_postcomp_b1r());
    m_meter_postcomp_b2->set_level(pld.lev_postcomp_b2l(), pld.lev_postcomp_b2r());
    m_meter_postcomp_b3->set_level(pld.lev_postcomp_b3l(), pld.lev_postcomp_b3r());
    m_meter_postcomp_b4->set_level(pld.lev_postcomp_b4l(), pld.lev_postcomp_b4r());
    m_meter_postcomp_b5->set_level(pld.lev_postcomp_b5l(), pld.lev_postcomp_b5r());
}
