// Indicatortest ///////////////////
#define RF5B_BUY  1<<0
#define RF5B_SELL 1<<1
#define RF7B_BUY  1<<2
#define RF7B_SELL 1<<3
int rf_signals(int shift, bool bk);
bool UpperFractal7B(int shift, bool bk);
bool LowerFractal7B(int shift, bool bk);
bool UpperFractal5B(int shift, bool bk);
bool LowerFractal5B(int shift, bool bk);


// returns bitwise buy and sell signals
int rf_signals(int shift, bool bk)
{
	int output = 0;
	if(UpperFractal7B(0,true)) output |= RF7B_SELL;
	if(LowerFractal7B(0,true)) output |= RF7B_BUY;
	if(UpperFractal5B(0,true)) output |= RF5B_SELL;
	if(LowerFractal5B(0,true)) output |= RF5B_BUY;
	return output;
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




