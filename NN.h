#pragma once
#include "Matrix.h"
#define IMG_WIDTH 800
#define IMG_HEIGHT 600
class NN
{
public:
	NN(size_t* i_architecture, size_t i_architecture_count); 
	friend std::ostream& operator<<( std::ostream& os, const NN& i_NN );
	~NN() {};
	void Forward();
	void FillRandom();
	void FillWith(float i_val);
	void Learn(NN g, float rate);
	void Render(Olivec_Canvas img);
	void RenderRaylib(int rx,int ry,int rw, int rh);
	float Cost(myMatrix In, myMatrix Out);
	void Feed(myMatrix In);
public:
	size_t m_count;
	myMatrix* m_weights;
	myMatrix* m_biases;
	myMatrix* m_activations;


};
void finite_diff(NN& nn, NN &g, float eps, myMatrix ti, myMatrix to);
void back_prop(NN& nn, NN& g, myMatrix ti, myMatrix to);
