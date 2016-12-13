import random

fp = open('./Numbers.txt','w')

length = 1000000
min_x = 0
max_x = 60000

count = 0
for x in xrange(0,length):
	print >> fp, int(random.uniform(min_x, max_x))
	count += 1

print "Suc. print ", count, " numbers"



