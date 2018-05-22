// Indicatortest ///////////////////
#include<reversal_fractals.h>


function run()
{
	set(PLOTNOW);
	NumYears = 2;
	MaxBars = 2000;
	BarPeriod = 1;
	PlotScale = 8;
	PlotWidth = 2400;
	PlotHeight1 = 1000;
	PlotHeight2 = 240;//80;
	vars Price = series(price());

// plot fractals
	plot("+7B",-100.0*UpperFractal7B(0,true),NEW+BARS,RED); // Upper: Sell
	plot("-7B",100.0*LowerFractal7B(0,true),BARS,GREEN);	 // Lower: Buy
	plot("+5B",-100.00*UpperFractal5B(0,true),BARS,BLUE);	 // Upper: Sell
	plot("-5B",100.0*LowerFractal5B(0,true),BARS,PURPLE);  // Lower: Buy

// // plot some other indicators	
	// plot("ATR (PIP)",ATR(20)/PIP,NEW,RED);
	// plot("Doji",CDLDoji(),NEW+BARS,BLUE);
	// plot("FractalDim",FractalDimension(Price,30),NEW,RED);
	// plot("ShannonGain",ShannonGain(Price,40),NEW,RED);
	
}


