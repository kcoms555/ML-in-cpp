void Batch::init(void){size;count=0;position=0;m_arr=NULL;is_allocated=false;}
Batch::Batch(void){
	init();
} 
Batch::~Batch(){
	if(m_arr){
		for(int i=0;i<count;i++)
			if(m_arr[i])
				delete m_arr[i];
		delete[] m_arr;
	}
}
void Batch::allocate(int size){
	this->size = size;
	m_arr = new Matrix*[size];
	for(int i=0; i<size; i++)
		m_arr[i]=NULL;
	is_allocated=true;
#ifdef DEBUG
	fprintf(fp, "allocate(%d)\n", size);
#endif
}
void Batch::push(Matrix& m){
	if(count>=size){
		fprintf(fp, "count:%d, size:%d, push() denied!\n");
		return;
	}
	m_arr[count] = new Matrix(m);
	count++;
#ifdef DEBUG
	fprintf(fp, "push()\n");
#endif
}
void Batch::rewind(void){
	position=0;
}
int Batch::get_row(void){
	return m_arr[0]->get_row();
}
int Batch::get_column(void){
	return m_arr[0]->get_column();
}
Matrix* Batch::get(int i){
	if(i<0 && i>=size)
		return NULL;
	return m_arr[i];
}
Matrix* Batch::peek(void){
	if(position>=size){
		//fprintf(fp, "peek() top! return NULL\n");
		return NULL;
	}
	return m_arr[position++];
}
