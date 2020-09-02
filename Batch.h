#ifndef __BATCH__
#define __BATCH__
#ifndef fp
	#define fp stdout
#endif
#include "Matrix.h"
class Batch{
private:
	int row;
	int column;
	int size;
	int count;
	int position;
	bool is_allocated;
	Matrix ** m_arr;
public:
	void init(void);
	Batch(void);
	void allocate(int size);
	void push(Matrix& m);
	void rewind(void);
	int get_row(void);
	int get_column(void);
	Matrix* get(int i);
	Matrix* peek(void);
	~Batch();
};
#include "Batch.cpp"
#endif
