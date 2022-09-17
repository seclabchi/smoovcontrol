/*
 * MainWindow.h
 *
 *  Created on: Jul 19, 2022
 *      Author: zaremba
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

class MainWindow {

public:
	typedef enum {
			PROCESSOR_INPUT = 0,
			PROCESSR_OUTPUT = 1
	} METER_NAME;

public:
	MainWindow();
	virtual ~MainWindow();
	int go(int argc, char* argv[]);
	void set_level(METER_NAME name, double l, double r);
};

#endif /* MAINWINDOW_H_ */
