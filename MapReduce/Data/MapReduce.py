#!/usr/bin/env python

'''
A simple word-counter MapReduce Python implementation

https://en.wikipedia.org/wiki/MapReduce
https://courses.cs.washington.edu/courses/cse490h/08au/lectures/mapred.pdf
http://michaelnielsen.org/blog/write-your-first-mapreduce-program-in-20-minutes/
http://www.chencer.com/dave/blog/2008/07/09/mapreduce-in-10-or-so-lines-of-python/

'''

import string
import itertools

#
# MapReduce algorithm
#

'''
Data - dictionary
Mapper - function (key, value) and returns list(key, intermediate_value)
Reducer - function (key, list) and returns list(key, fina;_value)

Returns a list with final values
'''
def map_reduce(data, mapper, reducer):
	print("\r\n Original data : \r\n")
	print(data)
	
	# Agregate in a single array (TODO use numpy array)
	intermediate = []
	for (key, value) in data.items():
		intermediate.extend(mapper(key, value))

	print("\r\n After mapping: \r\n")
	print(intermediate)

	# Sort intermediate array (group by key)
	# A little hack with groupby that produces slightly altered values
	groups = {}
	for (key, group) in itertools.groupby(sorted(intermediate), lambda x: x[0]):
		groups[key] = list([v for (k, v) in group])

	print("\r\n After sorting: \r\n")
	print(groups)

	# Return final list from reduce phase
	return [reducer(intermediate_key, groups[intermediate_key]) for intermediate_key in groups]

#
# Word counter
#

'''
Removes punctuation and junk
'''
def sanitize_string(s):
	return s.lower().translate(string.maketrans("",""), string.punctuation)

'''
Returns intermediate list with string values
'''
def map_words(key, value):
	return [(word, 1) for word in sanitize_string(value).split()]

'''
Returns sum of the occurencies
'''
def reduce_words(key, value_list):
	return (key, sum(value_list))

#
# Main
#

filenames = ["a.txt", "b.txt", "c.txt"]

files_data = {}
for filename in filenames:
	f = open(filename)
	files_data[filename] = f.read()
	f.close()

res = map_reduce(files_data, map_words, reduce_words)
print("\r\n MapReduced: \r\n")
print(res)