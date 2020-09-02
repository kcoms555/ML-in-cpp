#ifndef __LAYER__
#define __LAYER__
#include "Batch.h"
#include "Func.h"
#include <cmath>
class Layer{
private:
	Batch * input_batch;
	Batch * target_batch;
	Matrix ** weights;
	Matrix ** d_weights;
	Matrix ** d_biases;
	Matrix ** biases;
	Matrix * x;
	Matrix * d_x;
	Matrix * fx;
	Matrix * input;
	Matrix * target;
	_MDATA (**activate_func)(_MDATA input, _MDATA theta);
	_MDATA *activate_func_input;
	_MDATA *activate_func_theta;
	double der, cost2, cost1, w2, w1, x2, x1, y2, y1;
	double cost;
	double lr;
	int layer_size;
	bool is_initialized;
	bool is_input_batch_initialized;
	bool is_target_batch_initialized;
	bool * is_bias_using;
	
public:
	const static int END=-1;
	const static int START=0;
	typedef _MDATA (*FPTR)(_MDATA, _MDATA);
	Layer(int layer_size):layer_size(layer_size),cost(0){
		is_initialized=false;
		is_input_batch_initialized=false;
		is_target_batch_initialized=false;
		input_batch=NULL;
		target_batch=NULL;
		lr = 0.1;
		weights = new Matrix*[layer_size+1];//0번 인덱스 사용 안함
		d_weights = new Matrix*[layer_size+1];//0번 인덱스 사용 안함
		d_biases = new Matrix*[layer_size+1];
		d_x = new Matrix[layer_size+1];
		biases = new Matrix*[layer_size+1];//0번 인덱스 사용 안함
		is_bias_using = new bool[layer_size+1];
		activate_func = new FPTR[layer_size+1];
		x = new Matrix[layer_size+1]; //0번 인덱스 = x0 = input;
		fx = new Matrix[layer_size+1]; //0번 인덱스는 사용 안함//fx[0]=input;
		activate_func_input = new _MDATA[layer_size+1];
		activate_func_theta = new _MDATA[layer_size+1];
		for(int i=0; i<layer_size+1; i++){
			activate_func_input[i]=0;
			activate_func_theta[i]=0;
			weights[i] = NULL;
			d_weights[i] = NULL;
			d_biases[i] = NULL;
			biases[i] = NULL;
			activate_func[i] = NULL;
			is_bias_using[i] = false;
		}
	}
	~Layer(){
		for(int i=0;i<layer_size+1;i++){
			if(d_weights[i] != NULL)
				delete d_weights[i];
			if(d_biases[i] != NULL)
				delete d_biases[i];
		}
		delete[] weights;
		delete[] d_weights;
		delete[] d_x;
		delete[] biases;
		delete[] activate_func;
		delete[] x;
		delete[] fx;
		delete[] is_bias_using;
	}
	void rewind(){
		input_batch->rewind();
		target_batch->rewind();
	}
	void set_input_batch(Batch * batch){
		input_batch = batch;
		is_input_batch_initialized = true;
	}
	void set_target_batch(Batch * batch){
		target_batch = batch;
		is_target_batch_initialized = true;
	}
	void set_weight(int layer, Matrix * w){
		weights[layer] = w;
	}
	void set_bias(int layer, Matrix * b){
		biases[layer] = b;
		is_bias_using[layer] = true;
	}
	void set_activate_func(int layer, _MDATA (*activate_func)(_MDATA input, _MDATA theta)){
		this->activate_func[layer] = activate_func;
	}
	void set_activate_func(int layer, _MDATA (*activate_func)(_MDATA input, _MDATA theta), _MDATA theta){
		this->activate_func[layer] = activate_func;
		this->activate_func_theta[layer] = theta;
	}
	void set_learning_rate(double lr){
		this->lr = lr;
	}
	void forward_propagation(int start, int end){ //start층 부터 end층까지 순전파
		if(!is_initialized) initialize();
		if(start < 0) start = layer_size + 1 + start;
		if(end < 0) end = layer_size + 1 + end;
		if(start != START){
			fx[start].set(x[start].apply(activate_func[start]));
		}
		for(int i=start+1; i<end+1; i++){
			if(is_bias_using[i])
				x[i].set((weights[i]->transpose() * fx[i-1])+ (*biases[i]));
			else
				x[i].set(weights[i]->transpose() * fx[i-1]);
			fx[i].set(x[i].apply(activate_func[i]));
			//x[i].show();
		}
		if(end == layer_size){ //마지막 층까지 순전파가 진행될시 cost값을 계산 할 수 있으니 cost값을 계산
			cost = (double)((*target)-fx[layer_size]).square().sum()/2;
			//cost = ((t-o)^2)/2
		}
	}
	void back_propagation(){ //역전파
		if(!is_initialized) initialize();
		//printf("=======================================\n");
		//input->show(); target->show(); fx[layer_size].show();
		Matrix tfx;
		_MDATA tfxsum;
		for(int k=layer_size;k>0;k--){
			int row		=	x[k].get_row();
			int column	=	x[k].get_column();
			//printf("k=%d\n", k); weights[k]->show();
			if(k==layer_size)
				d_x[k].set(1.0);
			else
				d_x[k].set(*weights[k+1] * d_x[k+1]); //일단 앞 단계의 미분값을 가져옴
			if(k==layer_size){
				tfx = -(*target-fx[layer_size]);
				tfxsum = tfx.sum();
			}
			for(int i=0; i<row; i++){
				for(int j=0; j<column; j++){
					x1 = x[k].get(i,j);
					x2 = x1 + fabs(x1*0.0000001);
					y1 = activate_func[k](x1,activate_func_theta[k]);
					y2 = activate_func[k](x2,activate_func_theta[k]);
					//printf("%.4f %.4f %.4f %.4f\n", x1,x2,y1,y2);
					if(k==layer_size){
						if(activate_func[k]==Func::threshold) der = tfxsum*1;
						else der = (tfx.get(i,j))*(y2-y1)/(x2-x1);
					}
						//else der = (-(*target-fx[layer_size]).sum())*(y2-y1)/(x2-x1);
					else
						if(activate_func[k]==Func::threshold) der = 1;
						else der = (y2-y1)/(x2-x1);
					d_x[k].set(i,j,d_x[k].get(i,j)*der); 
				}
			}
			//x[k].show(); d_x[k].transpose().show(); ((fx[k-1])*(d_x[k].transpose())).show();
			d_weights[k]->set(fx[k-1]*d_x[k].transpose());//d_w = d cost/d w
			if(is_bias_using[k])
				d_biases[k]->set(d_x[k]); //d_b = d cost/d b

			weights[k]->sub(*d_weights[k] * lr); //w = w - d_w * learning_rate
			if(is_bias_using[k])
				biases[k]->sub(*d_biases[k] * lr); //b = b - d_b * learning_rate
			//정적 변동 (나중에 동시에 변동됨)
		}
	}
	void show_case(){
		if(!is_initialized) initialize();
		rewind();
		while(input=input_batch->peek()){
			set_input(*input);
			forward_propagation(START, END);
			fx[layer_size].show();
		}
	}
	void set_input(Matrix m){
		x[0].set(m);
		fx[0].set(m);
	}
	double get_result(){
		forward_propagation(START, END);
		return Matrix::to_scalar(fx[layer_size]);
	}
	double get_cost(){
		return cost;
	}
	double get_cost_sum(){
		if(!is_initialized) initialize();
		double cost = 0;
		rewind();
		while((input=input_batch->peek())!=NULL && (target=target_batch->peek())!=NULL){
			set_input(*input);
			forward_propagation(START, END);
			//printf("%f ", this->cost);
			cost += this->cost;
		}
		return cost;
	}
	void train(){ //학습
		if(!is_initialized) initialize();
		if(is_input_batch_initialized == false){
			fprintf(fp, "input_batch is not available!\n");
			return;
		}
		rewind();
		while((input=input_batch->peek())!=NULL && (target=target_batch->peek())!=NULL){
			set_input(*input); //입력값을 input배치에서 하나 꺼낸 값으로 설정
			forward_propagation(START, END); //순전파
			back_propagation(); //역전파
		}
	}
	void initialize(){
		char name[20];
		if(is_input_batch_initialized == false){
			fprintf(fp, "input batch is NULL!\n");
			return;
		}
		if(is_target_batch_initialized == false){
			fprintf(fp, "target batch is NULL!\n");
		}
		fx[0]=x[0]=Matrix(input_batch->get_row(), input_batch->get_column());
		sprintf(name, "x%d", 0);
		x[0].set_name(name);
		sprintf(name, "fx%d", 0);
		fx[0].set_name(name);
		for(int i=0; i<layer_size+1; i++){
			if(weights[i] != NULL){
				fx[i]=x[i]=Matrix((weights[i]->transpose()) * x[i-1]);
				sprintf(name, "x%d",i);
				x[i].set_name(name);
				sprintf(name, "fx%d",i);
				fx[i].set_name(name);
				d_x[i]=Matrix(x[i].get_row(), x[i].get_column());
				sprintf(name, "d_x %d", i);
				d_x[i].set_name(name);
				d_weights[i] = new Matrix(weights[i]->get_row(), weights[i]->get_column());
				sprintf(name, "d_weights%d", i);
				d_weights[i]->set_name(name);
				if(is_bias_using[i]){
					d_biases[i] = new Matrix(biases[i]->get_row(), biases[i]->get_column());
					sprintf(name, "d_bias%d", i);
					d_biases[i]->set_name(name);
				}
			}
		}
		is_initialized=true;
	}
	unsigned int wb_to_value(){
		unsigned int value=0;
		for(int i=1; i<=layer_size; i++){
			unsigned int a = weights[i]->get_row(); //input값도 고려하기 위해 weights를 씀
			unsigned int b = weights[i]->get_column();
			value += (((a>>b)^(b>>a))^value + a*3 + b)^value;
			if(i>0) if(is_bias_using[i]){
				unsigned int a = biases[i]->get_row();
				unsigned int b = biases[i]->get_column();
				value += (((a>>b)^(b>>a))^value + a*3 + b)^value;
			}
		}
		return value;
	}
	void load(){
		char fn[50];
		sprintf(fn, "wb%d", wb_to_value());
		printf("fn : %s\n", fn);
		FILE * f = fopen(fn, "rt");
		if(!f){
			fprintf(stderr, "Failed to load a layer file\n");
			return;
		}
		int _;
		for(int k=1; k<=layer_size; k++){
			int row, column;
			fscanf(f, "w = %d %d %d\n", &_, &row, &column);
			//printf("%d:%d, %d:%d, %d:%d\n"
			if((row != weights[k]->get_row()) || (column != weights[k]->get_column())){
				fprintf(stderr, "layer load errer! : row and column are not matched\n");
				exit(1);
			}
			for(int i=0; i<weights[k]->get_row(); i++){
				for(int j=0; j<weights[k]->get_column(); j++){
					_MDATA value;
					if(sizeof(_MDATA) == sizeof(double)) fscanf(f, "%lf ", &value);
					else if(sizeof(_MDATA) == sizeof(float)) fscanf(f, "%f ", &value);
					else{
						fprintf(stderr, "layer load errer! : Unknown Data Type\n");
						exit(1);
					}
					weights[k]->set(i, j, value);
				}
			}
			if(is_bias_using[k]){
				fscanf(f, "b = %d %d %d\n", &_, &row, &column);
				for(int i=0; i<biases[k]->get_row(); i++){
					for(int j=0; j<biases[k]->get_column(); j++){
						_MDATA value;
						if(sizeof(_MDATA) == sizeof(double)) fscanf(f, "%lf ", &value);
						else if(sizeof(_MDATA) == sizeof(float)) fscanf(f, "%f ", &value);
						else{
							fprintf(stderr, "layer load errer! : Unknown Data Type\n");
							exit(1);
						}
						biases[k]->set(i, j, value);
					}
				}
			}
		}
		fclose(f);
	}
	void save(){
		char fn[50];
		sprintf(fn, "wb%d", wb_to_value());
		printf("fn : %s\n", fn);
		FILE * f = fopen(fn, "wt");
		if(!f){
			fprintf(stderr, "Failed to save a layer file\n");
			return;
		}
		for(int k=1; k<=layer_size; k++){
			fprintf(f, "w = %d %d %d\n", k, weights[k]->get_row(), weights[k]->get_column());
			for(int i=0; i<weights[k]->get_row(); i++){
				for(int j=0; j<weights[k]->get_column(); j++){
					fprintf(f,"%f ", weights[k]->get(i,j));
				}
				fprintf(f, "\n");
			}
			if(is_bias_using[k]){
				fprintf(f, "b = %d %d %d\n", k, biases[k]->get_row(), biases[k]->get_column());
				for(int i=0; i<biases[k]->get_row(); i++){
					for(int j=0; j<biases[k]->get_column(); j++){
						fprintf(f, "%f ", biases[k]->get(i,j));
					}
					fprintf(f, "\n");
				}
			}
		}
		fclose(f);
	}
	void print_csv(){
		if(input_batch->get_row()!=2 || input_batch->get_column() != 1){
			printf("input이 2by1행렬일때만 가능합니다\n");
			return;
		}
		FILE * f = fopen("data.csv", "w");
		if(f==NULL){
			printf("print_csv() error\n");
			return;
		}
		double x1, x2, result;
		Matrix x(2, 1);
		for(int i=0; i<25; i++){
			for(int j=0; j<25; j++){
				x1 = (double)(i-6.25)/12.5;
				x2 = (double)(j-6.25)/12.5;
				x.set(0,0,x1);
				x.set(1,0,x2);
				set_input(x);
				result = get_result();
				fprintf(f, "%f, %f, %f\n", x1, x2, result);
			}
		}
		fclose(f);
	}
};
#endif
