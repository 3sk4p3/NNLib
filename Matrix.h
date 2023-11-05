#pragma once
#include <iostream>
#include <vector>
#include <random>
#include "raylib.h"

extern "C" {
#include "olive.c"
}
#define MAT_PRINT(mat) std::cout<<(#mat)<<"\n"<<(mat);
class myMatrix
{

private:
	size_t m_rows;
	size_t m_cols;
	float* m_data;
public:

	//myMatrix &getRow(size_t row);
	float* getRow(size_t row);
	myMatrix& getCol(size_t row);
	myMatrix& popRow(size_t row);
	void FillRandom();
	void FillWith(float i_val);
	void Save(FILE* out);
	void Load(FILE* in);
	void ShuffleRows();
	void AddRow(float* i_row);
	size_t getRowsNo() { return m_rows; }
	size_t getColsNo() { return m_cols; } 
	float* getData(size_t i_offset) { return &m_data[i_offset]; }
	void Activate();
	friend std::ostream& operator<<(std::ostream& os, myMatrix& i_m);
	 friend myMatrix operator+(myMatrix& mat_left,myMatrix& mat_right);
	float* operator[](size_t row) { return &m_data[m_cols*row]; }
	//const float& operator[](std::size_t idx) const { return mVector[idx]; }
	myMatrix(size_t i_rows, size_t i_cols, float* i_data);
	myMatrix(size_t i_rows, size_t i_cols);
	myMatrix(myMatrix& i_mat);
	myMatrix();//??
	~myMatrix();

};
	myMatrix operator*(myMatrix mat_left, myMatrix& mat_right);
	float sigmoidf(float  x); 
	float cost(myMatrix m, myMatrix ti, myMatrix to);
