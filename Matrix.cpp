void Matrix::init(void){
	v=NULL;
	ap=NULL;
	has_name=false;
	is_seed_set=false;
	is_constant=true;
	id=count++;
//	fprintf(fp, "id:%d created!\n", id);
}
void Matrix::init(const int row, const int column){
	init();
	this->row=row;
	this->column=column;
	ap = new _MDATA[row*column];
	v = new _MDATA*[row];
	for(int i=0; i<row; i++){
		v[i] = &ap[i*column];
	}
	memset(ap, 0, sizeof(_MDATA)*row*column);
}
void Matrix::init(const Matrix &m){
	init(m.get_row(), m.get_column());
	if(m.has_name)
		set_name(m.get_name());
	_MDATA ** const v1 = m.get_array_ptr();
	memcpy(ap, &v1[0][0], sizeof(_MDATA)*row*column);
}
const char* Matrix::get_name(void) const{
	return name;
}
void Matrix::set_name(const char * name){
	strcpy(this->name, name);
	has_name=true;
}
void Matrix::set_seed(unsigned int seed){
	std::srand(seed);
	is_seed_set=true;
}
Matrix::Matrix(){
	init();
}
Matrix::Matrix(const int row, const int column){
	init(row,column);
}
Matrix::Matrix(const Matrix &m){
	init(m);
}
Matrix::~Matrix(void){
	//fprintf(fp, "%d delete\n", id);
	if(ap != NULL)
		delete[] ap;
	if(v != NULL)
		delete[] v;
}
void Matrix::show(void) const{
	fprintf(fp, "Matrix ID:%d", id);
	if(has_name)
		fprintf(fp, ", name:'%s'", name);
	fprintf(fp, ", shape=(%d, %d)\n", row, column);
	for(int i=0; i<row; i++){
		for(int j=0; j<column; j++){
			fprintf(fp, "%.6f ", v[i][j]);
		}
		fprintf(fp, "\n");
	}
}
int Matrix::get_row(void)const{
	return row;
}
int Matrix::get_column(void)const{
	return column;
}
_MDATA ** Matrix::get_array_ptr(void)const{
	return v;
}
bool Matrix::set_this_variable(void){
	is_constant = false;
	return true;
}
bool Matrix::set_this_constant(void){
	is_constant = true;
	return true;
}
bool Matrix::set(const int row, const int column, const _MDATA value){
	if(!is_vaild_range(row, column)){
		fprintf(fp, "Matrix::set Error!\n");
		return false;
	}
	v[row][column] = value;
	return true;
}
bool Matrix::set(const MATRIX value){
	switch(value){
		case MATRIX::RAND:
		case MATRIX::RANDOM:
			if(is_seed_set==false)
				set_seed(count);
			for(int i=0;i<row;i++)
				for(int j=0;j<column;j++)
					v[i][j] = (_MDATA)((std::rand()%20000)-10000)/100000;
			break;
		case MATRIX::ZERO:
			set(0.0);
			break;
		case MATRIX::ONE:
			set(1.0);
			break;
		default:
			set(-1.0);
			fprintf(fp, "Matrix::set Error\n");
			return false;
	}
	return true;
}
bool Matrix::set(const Matrix &m){
	if(is_same_range(m)){
		_MDATA ** const v1 = m.get_array_ptr();
		memcpy(ap, &v1[0][0], sizeof(_MDATA)*row*column);
		return true;
	}
	else{
		fprintf(fp, "set(Matrix &m) error!\n");
		fprintf(fp,"*this.show():\n");
		show();
		fprintf(fp,"m.show():\n");
		m.show();
		return false;
	}
}
bool Matrix::set(const _MDATA value){
	for(int i=0;i<row;i++)
		for(int j=0;j<column;j++)
			v[i][j] = value;
	return true;
}
_MDATA Matrix::get(const int row, const int column)const {
	if(!is_vaild_range(row, column)){
		fprintf(fp, "Matrix::get Error!");
		show();
		return 0;
	}
	return v[row][column];
}
_MDATA Matrix::sum(void) const{
	_MDATA sum=0;
	for(int i=0;i<row;i++)
		for(int j=0;j<column;j++)
			sum+=v[i][j];
	return sum;
}
Matrix Matrix::transpose(void)const {
	Matrix m(column, row);
	for(int i=0; i<column; i++)
		for(int j=0; j<row; j++){
			m.set(i,j,v[j][i]);
		}
	char name[MATRIX_NAME_SIZE];
	sprintf(name, "%s%s", get_name(),".transposed");
	m.set_name(name);
	return m;
}
bool Matrix::is_vaild_range(const int a, const int b) const{
	if(a<0 || b<0 || a>=this->row || b>=this->column){
		fprintf(fp, "row=%d, column=%d, a=%d, b=%d, not vaild!",this->row, this->column,a,b);
		return false;
	}
	return true;
}
bool Matrix::is_same_range(const Matrix &m){
	if(get_row() == m.get_row())
		if(get_column() == m.get_column())
			return true;
	return false;
}
bool Matrix::is_same_range(const Matrix &m1,const  Matrix &m2){
	if(m1.get_row() == m2.get_row())
		if(m1.get_column() == m2.get_column())
			return true;
	return false;
}
bool Matrix::is_multiplyable(const Matrix &m1,const  Matrix &m2){
	if(m1.get_column() == m2.get_row())
		return true;
	return false;
}
_MDATA Matrix::to_scalar(const Matrix &m){
	if(m.get_row()==1 && m.get_column()==1){
		return m.get(0,0);
	}
	fprintf(fp, "to_scalar Error!");
	return 0;
}
void Matrix::apply2(_MDATA(*activate_func)(_MDATA input, _MDATA theta)){
	for(int i=0;i<row;i++)
		for(int j=0;j<column;j++){
			v[i][j] = activate_func(v[i][j], 0.0);
		}
}
Matrix Matrix::apply(_MDATA(*activate_func)(_MDATA input, _MDATA theta)){
	Matrix m(*this);
	char name[MATRIX_NAME_SIZE];
	sprintf(name, "func(%s)", get_name());
	m.set_name(name);
	m.apply2(activate_func);
	return m;
}
Matrix Matrix::square(void) const{
	Matrix m(get_row(), get_column());
	int row = m.get_row();
	int column = m.get_column();
	for(int i=0; i<row; i++)
		for(int j=0; j<column; j++)
			m.set(i,j, get(i,j)*get(i,j));
	return m;
}
Matrix Matrix::minus(const Matrix &m1)const{
	Matrix m(m1);
	int row = m.get_row();
	int column = m.get_column();
	for(int i=0; i<row; i++)
		for(int j=0; j<column; j++)
			m.set(i,j, -m.get(i,j));
	return m;
}
Matrix Matrix::add(const Matrix &m1,const  Matrix &m2)const{
	if(!is_same_range(m1,m2)){
		fprintf(fp, "add&sub Error!\n");
		m1.show();
		m2.show();
		return Matrix();
	}
	Matrix m(m1);
	int row = m.get_row();
	int column = m.get_column();
	for(int i=0; i<row; i++)
		for(int j=0; j<column; j++)
			m.set(i,j, m.get(i,j)+m2.get(i,j));
	return m;
}
void Matrix::add(const Matrix &m){
	if(!is_same_range(*this,m)){
		(*this).show();
		m.show();
		fprintf(fp, "void add Error!\n");
	}
	for(int i=0; i<row; i++)
		for(int j=0; j<column; j++)
			v[i][j] += m.get(i,j);
}
void Matrix::add(const _MDATA a){
	for(int i=0; i<row; i++)
		for(int j=0; j<column; j++)
			v[i][j] += a;
}
Matrix Matrix::sub(const Matrix &m1,const  Matrix &m2)const{
	return add(m1, -m2);
}
void Matrix::sub(const Matrix &m){
	if(!is_same_range(*this,m)){
		fprintf(fp, "void sub Error!\n");
		(*this).show();
		m.show();
	}
	for(int i=0; i<row; i++)
		for(int j=0; j<column; j++)
			v[i][j] -= m.get(i,j);
}
Matrix Matrix::mul(const Matrix &m1,const  Matrix &m2)const{
	if(!is_multiplyable(m1, m2)){
		fprintf(fp, "mul Error! ");
		if(m1.has_name && m2.has_name)
			fprintf(fp, "%s(%d, %d) * %s(%d, %d)\n", m1.get_name(), m1.get_row(), m1.get_column(), m2.get_name(), m2.get_row(), m2.get_column());
		else
			fprintf(fp, "(%d, %d) * (%d, %d)\n", m1.get_row(), m1.get_column(), m2.get_row(), m2.get_column());
		m1.show();
		m2.show();
		Matrix m(0, 0);
		return m;
	}
	Matrix m(m1.get_row(), m2.get_column());

	char name[MATRIX_NAME_SIZE];
	if(m1.has_name && m2.has_name){
		sprintf(name, "%s * %s", m1.get_name(), m2.get_name());
		m.set_name(name);
	}

	_MDATA ** v = m.get_array_ptr();
	_MDATA ** const v1 = m1.get_array_ptr();
	_MDATA ** const v2 = m2.get_array_ptr();

	int m1row = m1.get_row();
	int m1col = m1.get_column();
	int m2row = m2.get_row();
	int m2col = m2.get_column();

	for(int m1j=0; m1j<m1col; m1j++){
		for(int m1i=0; m1i<m1row; m1i++){
			for(int m2j=0; m2j<m2col; m2j++){
				v[m1i][m2j] += v1[m1i][m1j]*v2[m1j][m2j];
			}
		}
	}
	return m;
}
Matrix Matrix::element_mul(const Matrix &m1, const Matrix &m2){
	Matrix m(m1);
	int row = m.get_row();
	int column = m.get_column();
	for(int i=0; i<row; i++)
		for(int j=0; j<column; j++)
			m.set(i,j, m1.get(i,j)*m2.get(i,j));
	return m;
}
Matrix Matrix::mul(const Matrix &m1,const _MDATA a)const{
	Matrix m(m1);
	int row = m.get_row();
	int column = m.get_column();
	for(int i=0; i<row; i++)
		for(int j=0; j<column; j++)
			m.set(i,j, m.get(i,j)*a);
	return m;
}
Matrix Matrix::mul(const Matrix &m1)const{
	return mul(*this, m1);
}
Matrix Matrix::operator- ()const {
	return minus(*this);
}
Matrix Matrix::operator+ (const Matrix& m)const {
	return add(*this, m);
}
Matrix Matrix::operator- (const Matrix& m)const {
	return sub(*this, m);
}
Matrix Matrix::operator* (const Matrix& m)const {
	return mul(*this, m);
}
Matrix Matrix::operator* (const _MDATA a)const {
	return mul(*this, a);
}
Matrix Matrix::operator* (const int a)const {
	return mul(*this, (_MDATA)a);
}
Matrix& Matrix::operator= (const Matrix& m){
	init(m);
	return *this;
}
