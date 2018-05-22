// Indicatortest ///////////////////
double UpperFractal7B(int shift, bool bk);
double LowerFractal7B(int shift, bool bk);
double UpperFractal5B(int shift, bool bk);
double LowerFractal5B(int shift, bool bk);

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
	plot("+7B",-100.0*(UpperFractal7B(0,true) > 0),NEW+BARS,RED); // Upper: Sell
	plot("-7B",100.0*(LowerFractal7B(0,true) > 0),BARS,GREEN);	 // Lower: Buy
	plot("+5B",-100.00*(UpperFractal5B(0,true) > 0),BARS,BLUE);	 // Upper: Sell
	plot("-5B",100.0*(LowerFractal5B(0,true) > 0),BARS,PURPLE);  // Lower: Buy

// // plot some other indicators	
	// plot("ATR (PIP)",ATR(20)/PIP,NEW,RED);
	// plot("Doji",CDLDoji(),NEW+BARS,BLUE);
	// plot("FractalDim",FractalDimension(Price,30),NEW,RED);
	// plot("ShannonGain",ShannonGain(Price,40),NEW,RED);
	
}



// **** REVERSAL FRACTALS HELPER FUNCTIONS ****

// Returns 7B sell signal if identified.
bool UpperFractal7B(int shift, bool bk)
{
   double middle = priceHigh(shift + 3);
   double v1 = priceHigh(shift);
   double v2 = priceHigh(shift+1);
   double v3 = priceHigh(shift+2);
   double v5 = priceHigh(shift + 4);
   double v6 = priceHigh(shift + 5);
   double v7 = priceHigh(shift + 6);
   double v1_c = priceLow(shift);
   double v7_c = priceLow(shift + 5);
   if((
		(middle > v1) && 
      (middle > v2) &&
      (middle > v3) &&
      (middle > v5) &&
      (middle > v6) &&
      (middle > v7)
	) && ((bk == false) || (v1_c < v7_c)))
   {
      return(true);
   } else {
      return(0);
   }
}

// Returns 7B buy signal if identified.
bool LowerFractal7B(int shift, bool bk)
{
   double middle = priceLow(shift + 3);
   double v1 = priceLow(shift);
   double v2 = priceLow(shift+1);
   double v3 = priceLow(shift+2);
   double v5 = priceLow(shift + 4);
   double v6 = priceLow(shift + 5);
   double v7 = priceLow(shift + 6);
   double v1_c = priceHigh(shift);
   double v7_c = priceHigh(shift + 5);
   if((
		(middle < v1) && 
      (middle < v2) &&
      (middle < v3) &&
      (middle < v5) &&
      (middle < v6) &&
      (middle < v7)
		) && ((bk == false) || (v1_c > v7_c)))
   {
      return(true);
   } else {
      return(0);
   }
}


// Returns 5B sell signal if identified.
bool UpperFractal5B(int shift, bool bk)
{
   double middle = priceHigh(shift + 2);
   double v1 = priceHigh(shift);
   double v2 = priceHigh(shift+1);
   double v3 = priceHigh(shift + 3);
   double v4 = priceHigh(shift + 4);
   double v1_c = priceLow(shift);
   double v4_c = priceLow(shift + 3);
   if((
		(middle > v1) && 
		(middle > v2) && 
		(middle > v3) && 
		(middle > v4)
		) && ((bk == false) || (v1_c < v4_c)))
   {
      return(true);
   } else {
      return(0);
   }
}

// Returns 5B buy signal if identified.
bool LowerFractal5B(int shift, bool bk)
{
   double middle = priceLow(shift + 2);
   double v1 = priceLow(shift);
   double v2 = priceLow(shift+1);
   double v3 = priceLow(shift + 3);
   double v4 = priceLow(shift + 4);
   double v1_c = priceHigh(shift);
   double v4_c = priceHigh(shift + 3);
   if((
		(middle < v1) && 
		(middle < v2) && 
		(middle < v3) && 
		(middle < v4)
		)  && ((bk == false) || (v1_c > v4_c)))
   {
      return(true);
   } else {
      return(0);
   }
}




