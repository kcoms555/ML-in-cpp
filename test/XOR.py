import tensorflow as tf
import numpy as np
import time

x_train = np.array([[-1,0,0],
					[-1,0,1],
					[-1,1,0],
					[-1,1,1]],dtype="f")
y = np.array([[0],[1],[1],[0]], dtype="f")

optimizer = tf.train.GradientDescentOptimizer(0.01)

with tf.name_scope('data_input'):
	X = tf.placeholder(tf.float32, shape=[None, 3])
	Y = tf.placeholder(tf.float32, shape=[None, 1])
with tf.name_scope('Activation_Function'):
	W1 = tf.Variable(tf.random_normal([3,7]), name='W1')
	W2 = tf.Variable(tf.random_normal([7,1]), name='W2')
	H1 = (tf.nn.tanh(tf.matmul(X, W1)))
	H2 = (tf.nn.tanh(tf.matmul(H1, W2)))
with tf.name_scope('Training_Function'):
	cost = tf.reduce_sum(tf.square(Y-H2))/2
	train = optimizer.minimize(cost)

with tf.Session() as sess:
	sess.run(tf.global_variables_initializer())
	start = time.time()
	for step in range(10000):
		cost_val, _ = sess.run([cost, train], feed_dict={X:x_train, Y:y})
		if step%100 == 0:
			print('{0:0.2f}% {1} : Cost = {2:0.6f}'.format(step/100., step, cost_val))
	end = time.time()
	h1, h2 = sess.run([H1, H2], feed_dict={X:x_train, Y:y})
	print(h2)
	print('runtime : {0:0.3f}'.format(end-start))
