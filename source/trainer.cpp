#include <cstdio>
#include <cstdlib>
#include "Data.h" //Defines the type of data; 데이터타입을 정의한 객체
#include "Matrix.h" //Defines operations of Matrix; 행렬 연산 구현한 객체
#include "Func.h" //Defines activation functions; 활성함수 구현한 객체
#include "Batch.h" //Defines data feeders(input, target); input, target를 배치로 묶는 객체
#include "Layer.h" //Defines the format of machine leaning layers; 학습층을 구현한 객체
#include "IO.h" //Defines the data loaders such as setting and weights, variables, inputs, targets, ... so on; 모든 설정값, 가중치, 편향, input, target을 불러오는 객체
int main(int argc, char *argv[]){
	IO io; //reads the file "conf" and loads settings; 이 생성자를 호출시 conf파일을 읽어 설정값을 불러옴
	Layer layer(io.layer_size); //sets the size of the layer; layer_size만큼 층의 크기를 설정
	layer.set_input_batch(&io.input_batch); //feeds input batches into the layer; input 배치를 layer에 넣음
	layer.set_target_batch(&io.target_batch); //feeds target batches into the layer; target 배치를 layer에 넣음
	layer.set_learning_rate(io.lr); //sets leaning layer; 학습률 설정
	for(int i=1; i<=io.layer_size; i++){
		layer.set_weight(i, io.w[i]); //sets the weight of ~th layer; ~층 가중치 설정
		if(io.is_bias_using[i]) layer.set_bias(i, io.b[i]); //sets the bias of ~th layer; ~층 편향 설정
		layer.set_activate_func(i, io.activate_func[i], 0); //sets the activation function of ~th layer; ~층 활성함수 설정
	}
	if(io.load) layer.load();
	printf("Cost before train = %f\n", layer.get_cost_sum());
	FILE * fstep = fopen("step_cost.csv", "wt");
	for(int i=0; i<io.repeat; i++){
		if(io.show_cost && i%100==0) printf("%0.2f%% %d : Cost = %f\n", ((double)i/io.repeat)*100, i, layer.get_cost_sum());
		layer.train(); //performs forward and backward propagation once each; 순전파, 역전파 각각 1회 실행
		if(i%20==0) fprintf(fstep, "%d, %f\n",i, layer.get_cost_sum());
	}
	printf("Cost after train = %f\n", layer.get_cost_sum());
	
	if(io.save) layer.save(); //saves weights and biases of the layer; 가중치, 편향 저장
	if(io.print_csv) layer.print_csv(); //saves the csv file of the delta x; x에 따른 변화량을 그리기 위해 csv파일로 저장
	fclose(fstep);
	return 0;
}
