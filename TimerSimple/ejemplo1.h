#ifndef ejemplo1_H
#define ejemplo1_H

#include <QtGui>
#include "ui_counterDlg.h"
#include <chrono>
#include "timer.h"
#include <iostream>
#include <thread>

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


class ejemplo1 : public QWidget, public Ui_Counter
{
Q_OBJECT
    public:
        ejemplo1();
        virtual ~ejemplo1();
    
    public slots:
	void doButton();
	void showRealTime();
    void setValue(int);

	
        
    private:
        Timer mytimer, mytimerLong;
        int cont = 0;
	    int period = 1000;


    std::chrono::_V2::system_clock::time_point t1 = high_resolution_clock::now();
    std::chrono::_V2::system_clock::time_point t2 = high_resolution_clock::now();


	void cuentaPeriod();
	void cuentaRealTime();
		
		// dos callbacks con diferente número de parámetros
        void cuenta();
		
		int trick = 5;
};

#endif // ejemplo1_H
