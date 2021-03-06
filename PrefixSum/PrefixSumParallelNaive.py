#!/usr/bin/env python

'''
Naive parallel algorithm of prefix sum
http://people.cs.vt.edu/yongcao/teaching/cs5234/spring2013/slides/Lecture10.pdf
'''

import threading
import math

import TestFunction

# test_data = [2,6,2,3,5]
test_data = [3, 1, 7, 0, 4, 1, 6, 3]

'''
Generic sum function 
'''
def accumulate(in_list, index, stride, out_list, out_index):
	sum = in_list[index] + in_list[index - stride]

	out_list[out_index] = sum

'''
What is prefix sum?
A = [2,6,2,3,5], then R = AllPrefixSum(A) = [0,2,8,10,13,18]
'''
def prefixSum(num_list):
	# create new output holder of the same size
	out = list(num_list)
	current_list = list(num_list)

	size = len(num_list)
	iterations = int(math.log(size, 2)) # not sure if base 2
	# print(str(iterations))
	stride = 1

	for iteration in range(0, iterations):
		jobs = []
		for i in range(stride, size):
			thread = threading.Thread(target = accumulate(current_list, i, stride, out, i))
			jobs.append(thread)

		for job in jobs:
			job.start()

		for job in jobs:
			job.join()

		stride *= 2
		current_list = list(out)
		# print(out)

	return out

result = prefixSum(test_data)
# print(result)

TestFunction.Test(prefixSum, 64)
TestFunction.Test(prefixSum, 128)
TestFunction.Test(prefixSum, 256)
TestFunction.Test(prefixSum, 256)
TestFunction.Test(prefixSum, 512)
TestFunction.Test(prefixSum, 1024)
TestFunction.Test(prefixSum, 2048)
TestFunction.Test(prefixSum, 4096)
TestFunction.Test(prefixSum, 8192)