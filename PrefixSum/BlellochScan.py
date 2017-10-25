#!/usr/bin/env python
'''
from Udacity GPU parallel courses by NVIDIA
https://youtu.be/mmYv3Haj6uc

https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch39.html

fuck nvidia (c) Linus Torvalds
https://www.mimuw.edu.pl/~ps209291/kgkp/slides/scan.pdf
'''

import threading
import math

test_data = [1, 2, 3, 4, 5, 6, 7, 8]

'''
Generic sum function 
'''
def accumulate(in_list, in_index1, in_index2, out_list, out_index):
	sum = in_list[in_index1] + in_list[in_index2]

	out_list[out_index] = sum

def downsweep_helper(in_list, in_index, out_list, out_index1, out_index2):
	t = in_list[out_index1]
	out_list[out_index1] = in_list[in_index]
	out_list[out_index2] = t + in_list[in_index]

# upsweep
# 1: for d = 0 to log2 n – 1 do
# 2:      for all k = 0 to n – 1 by 2 d+1 in parallel do
# 3:           x[k +  2 d+1 – 1] = x[k +  2 d  – 1] + x[k +  2 d +1 – 1]
# downsweep
# 1: x[n – 1] = 0
# 2: for d = log2 n – 1 down to 0 do
# 3:       for all k = 0 to n – 1 by 2 d +1 in parallel do
# 4:            t = x[k +  2 d  – 1]
# 5:            x[k +  2 d  – 1] = x[k +  2 d +1 – 1]
# 6:            x[k +  2 d +1 – 1] = t +  x[k +  2 d +1 – 1]

def scan(num_list):
	# shared memory
	out = list(num_list)
	current_list = list(num_list)
	
	# upsweep
	n = len(num_list)
	iterations = int(math.log(n, 2))

	for d in range(0, iterations):
		jobs = []

		stride = 2 ** (d + 1)
		for k in range(0, n - 1, stride):
			out_index = k + stride - 1
			in_index1 = k + 2 ** d - 1
			in_index2 = k + stride - 1
			thread = threading.Thread(target = accumulate(current_list, in_index1, in_index2, out, out_index))
			jobs.append(thread)
		
		for job in jobs:
			job.start()

		for job in jobs:
			job.join()

		current_list = list(out)
		# print(out)
	# upsweep finished
	# print(out)

	# downsweep
	identity = 0
	out[n-1] = identity
	current_list = list(out)
	for d in reversed(range(0, iterations)):
		jobs = []

		stride = 2 ** (d + 1) 
		for k in range(0, n - 1, stride):
			out_index1 = k + 2 ** d - 1
			out_index2 = k + 2 ** (d + 1) - 1
			in_index = out_index2
			thread = threading.Thread(target = downsweep_helper(current_list, in_index, out, out_index1, out_index2))
			jobs.append(thread)

		for job in jobs:
			job.start()

		for job in jobs:
			job.join()

		current_list = list(out)
		# print(out)
	# downsweep finished

	return out

result = scan(test_data)
print(result)