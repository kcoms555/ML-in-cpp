# ML-in-cpp
This is my toy cpp project implementing machine learning such as forward and backward propagation.  
It contains input, target batch files to train AND, OR and XOR operations.

# Installation
__1. Download or clone it__
```
git clone https://github.com/kcoms555/ML-in-cpp
```
__2. Go to the ML-in-cpp directory__
```
cd ML-in-cpp
```
__3. compile source files__
```
make  
```
or
```
g++ -o bin/runner source/runner.cpp  
g++ -o bin/trainer source/trainer.cpp  
```

# Example
### Train XOR data set
![image](https://user-images.githubusercontent.com/48780754/96157007-bb587600-0f4c-11eb-8c2b-2717882f7e80.png) (https://en.wikipedia.org/wiki/Exclusive_or)  
__1. Write a input batch for XOR__  
Open 'data/XOR.input' and write them  
```
2 1 4 <-- row, column, count. It means there are four (row) by (column) matrices 
0 0 <-- It will create a 2 by 1 matrix ((0), (0)). It always takes columns first.
0 1
1 0
1 1 
```
__2. Write a target batch for XOR__  
Open 'data/XOR.target' and write them  
```
1 1 4
0
1
1
0
```
__3. Set configuration file__  
Open 'bin/conf' and set input as `../data/XOR.input` and set target as `../data/XOR.input`.  
Set weights, biases, activation functions, learning rate and repetition as you want.
```
input = ../data/XOR.input
target = ../data/XOR.target
w = 1 2 7  <- sets 2 by 7 weight matrix in first layer
w = 2 7 1
b = 1 7 1 <- sets 7 by 1 bias matrix in first layer
b = 2 1 1
func = 1 tanh <- sets activate function of first layer as 'Hyperbolic tangent'
func = 2 x3
lr = 0.01 <- sets learning rate as 0.01
repeat = 10000 <- sets training repetition
ls = 2 <- sets the layer size. In case of single layer, set its value as 1.
print_csv = false
show_cost = true
load = true
save = true
```
The neural network will be built as shown in the picture below.
![image](https://user-images.githubusercontent.com/48780754/96163881-6a00b480-0f55-11eb-92df-1f854f57ae65.png)

__4. train and run__  
Go to `./bin` and execute `trainer` and `runner`  
![pi_PI4_-__ML-in-cpp_bin-2020-10-16-01-52-26](https://user-images.githubusercontent.com/48780754/96161949-e34ad800-0f52-11eb-82b5-a0375cbb8d0c.gif)

# Performance benchmarking
![output3](https://user-images.githubusercontent.com/48780754/112509840-35323c80-8dd4-11eb-9869-28dc8221b3fc.gif)  

- __Tensorflow 1.11.0 with Raspberry Pi 3 B+, running the sample code functionally identical to the XOR learning above__ : 46.753s  
- __ML-in-cpp with Raspberry Pi 3 B+, running the XOR learning above__ : 6.040s  

>_A configuration file of ML-in-cpp and the python(tensorflow) test code are in the test/_  

>_Because of random initialization, they are not printing same costs. If you want to see that they print the same costs, initialize the weights with same values. You can do it by editting MATRIX::RAND in source/IO.cpp to the specified value and random_normal() in test/XOR.py to fill() with the specified value you want._  

__ML-in-cpp__ is much faster than __tensorflow 1.11.0__ in that case by __7.74 times__  
