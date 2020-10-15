#ifndef __IO__
#define __IO__
#ifndef fp
	#define fp stdout
#endif
#include <cstdio>
#include <cstring>
#include "Data.h"
#include "Batch.h"
#include "Matrix.h"
#include "Func.h"
#include <unistd.h>
#include <libgen.h>
#define IO_SIZE 10
class IO{
private:
	char base_directory[256];
	void get_base_directory();
public:
	typedef _MDATA (*FPTR)(_MDATA, _MDATA);
	Matrix ** w;
	Matrix ** b;
	bool * is_bias_using;
	bool show_cost;
	bool print_csv;
	bool load;
	bool save;
	_MDATA (** activate_func)(_MDATA input, _MDATA theta);
	int layer_size;
	double lr;
	int repeat;

	Batch input_batch;
	Batch target_batch;
	static void _MDATA_fscanf(FILE * filep, const char * sp, _MDATA * data);
	static bool read(const char * file_path, Batch * batch, int type);
	IO();
	~IO();
};
#include "IO.cpp"
#endif
