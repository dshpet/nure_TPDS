import numpy
import time

def Test(function, data_size):
	amount = 50

	elapsed_sum = 0
	for i in range(0, amount):
		random_int_array = numpy.random.randint(100, size=data_size)

		start_time = time.clock()
		result = function(random_int_array)
		elapsed = time.clock() - start_time

		elapsed_sum += elapsed

	average = elapsed_sum / amount
	print("average over {0} iterations is {1} seconds for {2} array size".format(amount, elapsed, data_size))


