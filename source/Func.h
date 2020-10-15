#ifndef __FUNC__
#define __FUNC__
#include "Data.h"
#include <cmath>
class Func{
	public:
	static bool activing_threshold;
	static _MDATA threshold(_MDATA input, _MDATA theta){
		if(activing_threshold) return (input>theta)?1.0:0.0;
		else return linear(input, theta);
	}
	static _MDATA linear(_MDATA input, _MDATA theta){
		//return (input+theta>1)?1.0:((input+theta<0)?0.0:input+theta);
		return input+theta;
	}
	static _MDATA tanh(_MDATA input, _MDATA theta){
		return std::tanh(input+theta);
	}
	static _MDATA x3(_MDATA input, _MDATA theta){
		return pow(input+theta, 3);
	}
	static _MDATA sigmoid(_MDATA input, _MDATA theta){
		return 1/(1+exp(-input+theta));
	}
	static _MDATA (*get_func_by_name(const char * name))(_MDATA, _MDATA){
		if(strcmp(name, "threshold")==0) return threshold;
		if(strcmp(name, "linear")==0) return linear;
		if(strcmp(name, "tanh")==0) return tanh;
		if(strcmp(name, "x3")==0) return x3;
		if(strcmp(name, "sigmoid")==0) return sigmoid;
		printf("get_func_by_name error! name : %s\n", name);
		return linear;
	}
};
bool Func::activing_threshold=true;
#endif
