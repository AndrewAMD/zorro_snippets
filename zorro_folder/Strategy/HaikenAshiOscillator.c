// Indicatortest ///////////////////
void HAOscillator(double* BullBuffer, double* BearBuffer);

function run()
{
	set(PLOTNOW);
	NumYears = 2;
	MaxBars = 2000;
	BarPeriod = 1440;
	PlotScale = 8;
	PlotWidth = 1600;
	PlotHeight1 = 350;
	PlotHeight2 = 240;//80;
	vars Price = series(price());
	vars HAOscBull = series(0);
	vars HAOscBear = series(0);
	HAOscillator(HAOscBull, HAOscBear);
	
// plot Haiken Ashi Oscillator
	plot("HAO+",HAOscBull[0],NEW+BARS,BLUE);
	plot("HAO-",HAOscBear[0],BARS,RED);
	
		
}


// **** HAIKEN ASHI OSCILLATOR HELPER FUNCTIONS ****

// assigns a series to both existing buffers.
// Note: both buffers must be pre-initialized blank series.
void HAOscillator(double* BullBuffer, double* BearBuffer)
{
	// Open and close 
	//double open = HAOpen();
	double open = (priceOpen(1)+priceClose(1))/2.0;
	//double close = HAClose();
	double close = (priceOpen()+priceHigh()+priceLow()+priceClose())/4.0;
	
	// Median
	double* Median = series(0);
	Median[0] = (open + close) / 2.0;
	//FextMapBuffer4[pos] = (open + close) / 2;
	
	// Difference
	double diff = Median[0]-Median[1];
	//FextMapBuffer5[pos] = FextMapBuffer4[pos] - FextMapBuffer4[pos+1];

	// If bearish
	//if(open > close)
	if(diff<0)
	{
		BearBuffer[0] = diff;
		BullBuffer[0] = 0;
		
	// If bullish
	} else {
		
		BullBuffer[0] = diff;
		BearBuffer[0] = 0;
	}
}


