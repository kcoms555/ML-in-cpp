void IO::_MDATA_fscanf(FILE * filep, const char * sp, _MDATA * data){
	//sp is not used;
	if(sizeof(_MDATA) == sizeof(float)){
		const char * p = "%f";
		fscanf(filep, p, data);
	}
	if(sizeof(_MDATA) == sizeof(double)){
		const char * p = "%lf";
		fscanf(filep, p, data);
	}
}
bool IO::read(const char * file_path, Batch * batch, int type=0){
	FILE * filep;
	const char * type_name;
	switch(type){
		case DATA::INPUT:
			type_name = "INPUT";
			break;
		case DATA::TARGET:
			type_name = "TARGET";
			break;
		default:
			type_name = "Default Type";
	}
	filep = fopen(file_path, "rt");
	if(filep==NULL){
		fprintf(fp, "file_path: '%s'\n", file_path);
		return false;
	}
	int row, column, count;
	_MDATA data;
	fscanf(filep, "%d %d %d", &row, &column, &count);
	batch->allocate(count);
	int num=0;
	while(count--){
		char name[20];
		Matrix m(row, column);
		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++){
				_MDATA_fscanf(filep, NULL ,&data);
				m.set(i,j,data);
			}
		}
		sprintf(name, "%s:%d", type_name, num);
		m.set_name(name);
		batch->push(m);
		num++;
	}
	fclose(filep);
	return true;
}
void IO::get_base_directory(){
	int d_ret = readlink("/proc/self/exe", base_directory, 100);
	base_directory[d_ret+1] = '\0';
	dirname(base_directory);
	strcat(base_directory, "/");
}
IO::IO(){
	get_base_directory();
	w = new Matrix*[IO_SIZE];
	b = new Matrix*[IO_SIZE];
	is_bias_using = new bool[IO_SIZE];
	activate_func = new FPTR[IO_SIZE];
	for(int i=0; i<IO_SIZE; i++){
		w[i] = NULL;
		b[i] = NULL;
		is_bias_using[i] = false;
		activate_func[i] = Func::linear;
	}
	lr = 0.1;
	layer_size=0;
	show_cost=false;
	print_csv=false;
	load=false;
	save=false;
	char config_path[256];
	sprintf(config_path, "%s./conf", base_directory);
	FILE * filep = fopen(config_path, "rt");
	if(filep == NULL){printf("IO fopen ERROR\n"); return;}
	char str[128];
	char * token;
	const char * to="= \n";
	while(1){
		fgets(str, 128, filep);
		if(feof(filep)) break;
		if(str[0]=='#' || str[0]=='\n') continue;
		token = strtok(str, to);
		if(strcmp(token, "input")==0){
			char * tmp = strtok(NULL, to);
			read(tmp, &input_batch, DATA::INPUT);
			printf("IO : read input %s\n", tmp);
		}
		if(strcmp(token, "target")==0){
			char * tmp = strtok(NULL, to);
			read(tmp, &target_batch, DATA::TARGET);
			printf("IO : read target %s\n", tmp);
		}
		if(strcmp(token, "w")==0){
			int layer = atoi(strtok(NULL, to));
			int row = atoi(strtok(NULL, to));
			int column = atoi(strtok(NULL, to));
			printf("IO : set w[%d] %dby%d Matrix\n", layer, row, column);
			w[layer] = new Matrix(row, column);
			char name[50];
			sprintf(name, "weight%d", layer);
			w[layer]->set_name(name);
			w[layer]->set(MATRIX::RAND);
		}
		if(strcmp(token, "b")==0){
			int layer = atoi(strtok(NULL, to));
			int row = atoi(strtok(NULL, to));
			int column = atoi(strtok(NULL, to));
			printf("IO : set b[%d] %dby%d Matrix\n", layer, row, column);
			is_bias_using[layer]=true;
			b[layer] = new Matrix(row, column);
			char name[50];
			sprintf(name, "bias%d", layer);
			b[layer]->set_name(name);
			b[layer]->set(MATRIX::RAND);
		}
		if(strcmp(token, "lr")==0 || strcmp(token, "learning_rate")==0){
			lr = atof(strtok(NULL, to));
			printf("IO : set learning rate %f\n", lr);
		}
		if(strcmp(token, "repeat")==0){
			repeat = atoi(strtok(NULL, to));
			printf("IO : set repeat %d\n", repeat);
		}
		if(strcmp(token, "ls")==0 || strcmp(token, "layer_size")==0){
			layer_size = atoi(strtok(NULL, to));
			printf("IO : set layer_size %d\n", layer_size);
		}
		if(strcmp(token, "func")==0){
			int layer = atoi(strtok(NULL, to));
			const char * act_tmp = strtok(NULL, to);
			printf("IO : set activate function[%d] %s\n", layer, act_tmp);
			activate_func[layer] = Func::get_func_by_name(act_tmp);
		}
		if(strcmp(token, "show_cost")==0){
			if(strcmp("true", strtok(NULL, to))==0) show_cost=true;
			printf("IO : set show_cost %d\n", show_cost);
		}
		if(strcmp(token, "print_csv")==0){
			if(strcmp("true", strtok(NULL, to))==0) print_csv=true;
			printf("IO : set print_csv %d\n", print_csv);
		}
		if(strcmp(token, "load")==0){
			if(strcmp("true", strtok(NULL, to))==0) load=true;
			printf("IO : set load %d\n", load);
		}
		if(strcmp(token, "save")==0){
			if(strcmp("true", strtok(NULL, to))==0) save=true;
			printf("IO : set save %d\n", save);
		}
	}
	fclose(filep);
}
IO::~IO(){
	for(int i=0; i<IO_SIZE; i++){
		if(w[i] != NULL) delete w[i];
		if(b[i] != NULL) delete b[i];
	}
	delete[] w;
	delete[] b;
	delete[] is_bias_using;
	delete[] activate_func;
}
