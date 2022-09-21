#include "ejemplo1.h"


ejemplo1::ejemplo1(): Ui_Counter()
{

	setupUi(this);
	show();

    t1 = high_resolution_clock::now();

	connect(button, SIGNAL(clicked()), this, SLOT(doButton()));
	connect(showTimeButton, SIGNAL(clicked()), this, SLOT(showRealTime()));
    connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

	

	mytimer.connect(std::bind(&ejemplo1::cuenta, this));
	mytimer.connect(std::bind(&ejemplo1::cuentaPeriod, this));


    mytimer.start(1000);    
   
}

ejemplo1::~ejemplo1()
{}

void ejemplo1::doButton()
{
	static bool stopped = false;
	stopped = !stopped;
	if(stopped)
		mytimer.stop();
	else
		mytimer.start(period);
	qDebug() << "click on button";
}


void ejemplo1::setValue(int)
{
     period = horizontalSlider->value();


}

void ejemplo1::showRealTime()
{
 
qDebug() << "Show real time";
    t2 = high_resolution_clock::now();


    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;


    std::cout << ms_double.count() /1000  << "s\n";
    double calc = ms_double.count() /1000;
    string r = to_string(calc);
    r.append(" - secs elapsed \n");

    QString text = QString::fromLocal8Bit(r.c_str());
    plainTextEdit->insertPlainText(text);
}

void ejemplo1::cuentaPeriod()
{
    lcdNumberPeriod->display(period);
    mytimer.setPeriod(period);
	
}


void ejemplo1::cuenta()
{
    lcdNumber->display(++cont);
	trick++;
}

