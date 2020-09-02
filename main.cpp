#include <cstdio>
#include <cstdlib>
#include "Data.h" //데이터타입을 정의한 객체
#include "Matrix.h" //행렬 연산 구현한 객체
#include "Func.h" //활성함수 구현한 객체
#include "Batch.h" //input, target를 배치로 묶는 객체
#include "Layer.h" //층을 구현한 객체
#include "IO.h" //모든 설정값, 가중치, 편향, input, target을 불러오는 객체
int main(int argc, char *argv[]){
	IO io; //이 생성자를 호출시 conf파일을 읽어 설정값을 불러옴
	Layer layer(io.layer_size); //layer_size만큼 층의 크기를 설정
	layer.set_input_batch(&io.input_batch); //input 배치를 layer에 넣음
	layer.set_target_batch(&io.target_batch); //target 배치를 layer에 넣음
	layer.set_learning_rate(io.lr); //학습률을 불러 layer에 넣음
	for(int i=1; i<=io.layer_size; i++){
		layer.set_weight(i, io.w[i]); //1~layer_size층 가중치 설정
		if(io.is_bias_using[i]) layer.set_bias(i, io.b[i]); //~층 편향 설정
		layer.set_activate_func(i, io.activate_func[i], 0); //~층 활성함수 설정
	}
	if(io.load) layer.load();
	printf("Cost before train = %f\n", layer.get_cost_sum());
	FILE * fstep = fopen("step_cost.csv", "wt");
	for(int i=0; i<io.repeat; i++){
		layer.train(); //순전파, 역전파 각각 1회 실행
		if(io.show_cost && i%200==0) printf("%0.2f%% %d : Cost = %f\n", ((double)i/io.repeat)*100, i, layer.get_cost_sum());
		if(i%20==0) fprintf(fstep, "%d, %f\n",i, layer.get_cost_sum());
	}
	printf("Cost after train = %f\n", layer.get_cost_sum());
	
	if(io.save) layer.save(); //가중치, 편향 저장
	if(io.print_csv) layer.print_csv(); //x에 따른 변화량을 그리기 위해 csv파일로 저장
	fclose(fstep);
	return 0;
}
