#include <cstdio>
#include <cstdlib>
#include "Data.h" //Defines the type of data; 데이터타입을 정의한 객체
#include "Matrix.h" //Defines operations of Matrix; 행렬 연산 구현한 객체
#include "Func.h" //Defines activation functions; 활성함수 구현한 객체
#include "Batch.h" //Defines data feeders(input, target); input, target를 배치로 묶는 객체
#include "Layer.h" //Defines the format of machine leaning layers; 학습층을 구현한 객체
#include "IO.h" //Defines the data loaders such as setting and weights, variables, inputs, targets, ... so on; 모든 설정값, 가중치, 편향, input, target을 불러오는 객체
int main(int argc, char *argv[]){
	IO io; //reads the file "conf" and loads settins; 이 생성자를 호출시 conf파일을 읽어 설정값을 불러옴
	Layer layer(io.layer_size); //sets the size of the layer; layer_size만큼 층의 크기를 설정
	layer.set_input_batch(&io.input_batch); //feeds input batches into the layer; input 배치를 layer에 넣음
	for(int i=1; i<=io.layer_size; i++){
		layer.set_weight(i, io.w[i]); //sets the weight of ~th layer; ~층 가중치 설정
		if(io.is_bias_using[i]) layer.set_bias(i, io.b[i]); //sets the bias of ~th layer; ~층 편향 설정
		layer.set_activate_func(i, io.activate_func[i], 0); //sets the activation function of ~th layer; ~층 활성함수 설정
	}
	if(io.load) layer.load();

	layer.show_case();
	
	return 0;
}
