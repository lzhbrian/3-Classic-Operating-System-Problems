import sys
f = open('./data/one_gig_file.txt','rb')
# f = open('./data/small_file.txt','rb')

fp = open('./data/reversed_file.txt','w')

f.seek(0,2) # move cursor to file end
f.seek(-1,1) # \n
f.seek(-1,1) # last char

count = 0
while(1):
	a = f.read(1).decode('UTF-8')
	print(a, end = '', file = fp)
	f.seek(-1,1)
	if f.tell() == 0:
		break
	f.seek(-1,1)

	# indicator
	if count % 100000 == 1:
		print(count,' bytes read!')
	count += 1