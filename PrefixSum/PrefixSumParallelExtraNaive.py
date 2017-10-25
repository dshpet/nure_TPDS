#!/usr/bin/env python

'''
Naive parallel algorithm of prefix sum
http://people.cs.vt.edu/yongcao/teaching/cs5234/spring2013/slides/Lecture10.pdf
'''

import threading
# look maybe multiprocessing lib
import TestFunction

test_data = [2,6,2,3,5]

'''
Generic sum function 
'''
def accumulate(in_list, amount, out_list, out_index):
	sum = 0
	for i in range(0, amount):
		sum += in_list[i]

	out_list[out_index] = sum

'''
What is prefix sum?
A = [2,6,2,3,5], then R = AllPrefixSum(A) = [0,2,8,10,13,18]
'''
def prefixSum(num_list):
	# create new output holder of the same size
	out = [0] * len(num_list)

	jobs = []
	for i in range(0, len(num_list)):
		thread = threading.Thread(target = accumulate(num_list, i + 1, out, i))
		jobs.append(thread)

	for job in jobs:
		job.start()

	for job in jobs:
		job.join()

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