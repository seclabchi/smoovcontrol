/*
 * MainWindow.cpp
 *
 *  Created on: Jul 19, 2022
 *      Author: zaremba
 */

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>

#include "MainWindow.h"
#include "StereoMeter.h"

MainWindow::MainWindow() {
	// TODO Auto-generated constructor stub

}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

int MainWindow::go(int argc, char* argv[]) {
  Fl_Double_Window *window = new Fl_Double_Window(1024, 768);
  window->label("FMSmoov Control");
  Fl_Box *box = new Fl_Box(10,10,1004,748, "This will eventually be the FM Smoov Control Panel in FLTK.\nIt will run on OSX, Linux, or Windows.");
  box->box(FL_BORDER_BOX);
  box->labelfont(FL_BOLD);
  box->labelsize(24);
  box->labeltype(FL_SHADOW_LABEL);
  StereoMeter* sm = new StereoMeter(10, 10, 40, 120);
  window->end();
  window->show(argc, argv);
  return Fl::run();
}

void MainWindow::set_level(METER_NAME name, double l, double r) {

}
