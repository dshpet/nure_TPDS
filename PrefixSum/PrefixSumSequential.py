#!/usr/bin/env python

'''
Sequential algorithm of prefix sum
'''

test_data = [2,6,2,3,5]

'''
Generator function to be used in list creation
'''
def accumulate(_numList):
	sum = 0
	for num in _numList:
		sum += num
		yield sum

'''
What is prefix sum?
A = [2,6,2,3,5], then R = AllPrefixSum(A) = [0,2,8,10,13,18]
# should zero be there by definition???
'''
def prefixSum(_numList):
	return list(accumulate(test_data)) 

result = prefixSum(test_data)
print(result)