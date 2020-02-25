// Uses Zorro's matrix functions to calculate Robert Carver's Diversification Multiplier.
#include<default.c>

function run() 
{
	assetList("");
	mat m = matrix(4,0); // input correlation matrix
	mat w = matrix(4,1); // weights matrix
	mat w_trans = matrix(1,4); //transposed weights matrix
	mat m_calc1 = matrix(4,1); //intermediate calc matrix
	mat m_out = matrix(1,1); //single element matrix for output
	
							me(m,1,0) = 0.90; me(m,2,0) = 0.60; me(m,3,0) = 0.25;
	me(m,0,1) = 0.90; 						me(m,2,1) = 0.90; me(m,3,1) = 0.25;
	me(m,0,2) = 0.60; me(m,1,2) = 0.90; 						me(m,3,2) = 0.25;
	me(m,0,3) = 0.25; me(m,1,3) = 0.25; me(m,2,3) = 0.25;
	
	me(w,0,0) = 0.21;
	me(w,1,0) = 0.08;
	me(w,2,0) = 0.21;
	me(w,3,0) = 0.50;
	
	var v_out=0;
	
	matMul(m_calc1,m,w);
	matTrans(w_trans,w);
	matMul(m_out,w_trans,m_calc1);
	v_out = 1 / sqrt(me(m_out,0,0));
	
	printf("\nv_out: %0.4f",v_out);
	
	quit("#Done.");
}
