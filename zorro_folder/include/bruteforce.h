// not tested yet, just saving this idea before I lose it. -AMD
// Note:
//   Zorro has a new brute force method, but the purpose of this script 
//   is to only brute force two parameters, not all of them.


// brute force optimization of up to two parameters at a time
typedef struct BRUTEVARS {
	var min1; var max1; var step1;
	var min2; var max2; var step2;
	var output1;
	var output2;
} BRUTEVARS;
void brute_force_optimize(BRUTEVARS* pbv);





void brute_force_optimize(BRUTEVARS* pbv){
	// calc max steps for each parameter
	int steps1 = (pbv->max1 - pbv->min1)/pbv->step1 + 1;
	int steps2 = (pbv->max2 - pbv->min2)/pbv->step2 + 1;
	int index = optimize(1,1,(steps1*steps2),1);
	
	// For brute force, we must follow a "mow the lawn" pattern to get stable parameters.
	// Left to right, then right to left.
	
	// Get row and column number from index.
	// Index starts with 1 and ends at steps1*steps2.
	// Rows(2) and columns(1) start at 0 
	// 	and end at steps2-1 and steps1-1 respectively.
	int row = (index-1)/steps1;
	int column = (index-1)%steps1;
	
	// row behavior is same every time
	pbv->output2 = pbv->min2 + (pbv->step2 * row);
	
	// Column behavior depends on whether this is odd or even row ("mow the lawn")
	if(row%2){
		pbv->output1 = pbv->min1 + (pbv->step1 * column);
	}
	else{
		pbv->output1 = pbv->max1 - (pbv->step1 * column);
	}
}
