#ifndef __MATRIX__
#define __MATRIX__

/*
#ifndef fp
	#define fp stdout
#endif
	*/
#define fp stdout

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "Data.h"

#include "Func.h"
#define MATRIX_NAME_SIZE 50
enum class MATRIX{ RAND, RANDOM, ZERO, ONE};
class Matrix
{
private:
	void init(void);
	void init(const int row, const int column);
	void init(const Matrix &m);
	int id;
	int row;
	int column;
	char name[MATRIX_NAME_SIZE];
	static bool is_seed_set;
	bool is_constant;
	_MDATA scalar;
	_MDATA ** v;
	_MDATA * ap;
public:
	bool has_name;
	static int count;
	const char* get_name(void) const;
	void set_name(const char * name);
	static void set_seed(unsigned int seed);
	Matrix(void);
	Matrix(const int row, const int column);
	Matrix(const Matrix &m);
	~Matrix(void);
	void show(void) const;
	int get_row(void) const;
	int get_column(void) const;
	_MDATA ** get_array_ptr(void)const;
	bool set_this_variable();
	bool set_this_constant();
	bool set(const int row, const int column, const _MDATA value);
	bool set(const MATRIX value);
	bool set(const _MDATA value);
	bool set(const Matrix &m);
	_MDATA get(const int row, const int column) const;
	_MDATA sum(void) const;
	bool is_vaild_range(const int a, const int b) const;
	bool is_same_range(const Matrix &m);
	static bool is_same_range(const Matrix &m1,const Matrix &m2);
	static bool is_multiplyable(const Matrix &m1, const Matrix &m2);
	static _MDATA to_scalar(const Matrix &m);
	void apply2(_MDATA(*activate_func)(_MDATA input, _MDATA theta));
	Matrix apply(_MDATA(*activate_func)(_MDATA input, _MDATA theta));
	Matrix transpose(void) const;
	Matrix square(void) const;
	Matrix minus(const Matrix &m1)const;
	Matrix add(const Matrix &m1, const Matrix &m2)const;
	void add(const Matrix &m);
	void add(const _MDATA a);
	Matrix sub(const Matrix &m1, const Matrix &m2)const;
	void sub(const Matrix &m);
	Matrix mul(const Matrix &m1, const Matrix &m2)const;
	Matrix mul(const Matrix &m1, const _MDATA a)const;
	Matrix mul(const Matrix &m1)const;
	static Matrix element_mul(const Matrix &m1, const Matrix &m2);
	Matrix operator- () const;
	Matrix operator+ (const Matrix &m) const;
	Matrix operator- (const Matrix &m) const;
	Matrix operator* (const Matrix &m) const;
	Matrix operator* (const _MDATA a) const;
	Matrix operator* (const int a) const;
	Matrix& operator= (const Matrix& m);
};
int Matrix::count=0;
bool Matrix::is_seed_set=false;
#include "Matrix.cpp"
#endif
