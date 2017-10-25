#!/usr/bin/env python

import TestFunction

'''
Sequential algorithm of prefix sum
'''

test_data = [2,6,2,3,5]

'''
Generator function to be used in list creation
'''
def accumulate(num_list):
	sum = 0
	for num in num_list:
		sum += num
		yield sum

'''
What is prefix sum?
A = [2,6,2,3,5], then R = AllPrefixSum(A) = [0,2,8,10,13,18]
# should zero be there by definition???
'''
def prefixSum(num_list):
	return list(accumulate(num_list)) 


TestFunction.Test(prefixSum, 256)
TestFunction.Test(prefixSum, 512)
TestFunction.Test(prefixSum, 1024)
TestFunction.Test(prefixSum, 2048)
TestFunction.Test(prefixSum, 4096)
TestFunction.Test(prefixSum, 8192)