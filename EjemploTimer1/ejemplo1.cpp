#include "ejemplo1.h"

ejemplo1::ejemplo1(): Ui_Counter()
{
	setupUi(this);
	show();
	

	
	connect(button, SIGNAL(clicked()), this, SLOT(doButton()));
	connect(buttonStart, SIGNAL(clicked()), this, SLOT(doButtonStart()));
	
	connect(&clk, SIGNAL(timeout()), this, SLOT(doIncrease()));
	
}




void ejemplo1::doButton()
{
	qDebug() << "click on button";
	clk.stop();
}



void ejemplo1::doButtonStart()
{
 
 clk.start(1000);
}


void ejemplo1::doIncrease()
{
static int cont = 0;
lcdNumber->display(cont++);

}
