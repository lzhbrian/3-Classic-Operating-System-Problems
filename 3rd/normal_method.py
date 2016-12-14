# Brian Lin,Tzu-Heng's Work
# Normal way to read and reverse and write

# Mailto: lzhbrian@gmail.com
# Github: github.com/lzhbrian
# Linkedin: linkedin/in/lzhbrian

import sys
f1 = open('./data/one_gig_file.txt','r+')
f2 = open('./data/one_gig_file.txt','r+')

# f1 = open('./data/small_file.txt','r+')
# f2 = open('./data/small_file.txt','r+')

buffer_size = 65536

# inverse the data in buf1 & buf2
def inverse_buf(buf1, buf2):
	temp1 = buf2[::-1]
	temp2 = buf1[::-1]
	return [temp1, temp2]


# f2: move cursor to file end
f2.seek(0,2)
f2.seek(-1,1) # if \n is in last data

while 1:

	# f1, from front
	buf1 = f1.read(buffer_size).decode('UTF-8')
	# f2, from end
	f2.seek(-buffer_size,1)
	buf2 = f2.read(buffer_size).decode('UTF-8')

	# inverse buf1, buf2
	[buf1, buf2] = inverse_buf(buf1, buf2)

	# write f1
	f1.seek(-buffer_size,1)
	f1.write(buf1)
	# write f2
	f2.seek(-buffer_size,1)
	f2.write(buf2)

	# move cursor for next
	f2.seek(-buffer_size,1)

	gap = f2.tell() - f1.tell()
	if gap == 0:
		f1.close()
		f2.close()
		exit()
	elif gap < buffer_size*2: # last round

		buffer_size = gap/2

		# f1, from front
		buf1 = f1.read(buffer_size).decode('UTF-8')
		# f2, from end
		f2.seek(-buffer_size,1)
		buf2 = f2.read(buffer_size).decode('UTF-8')

		# inverse buf1, buf2
		[buf1, buf2] = inverse_buf(buf1, buf2)

		# write f1
		f1.seek(-buffer_size,1)
		f1.write(buf1)
		# write f2
		f2.seek(-buffer_size,1)
		f2.write(buf2)
		break

f1.close()
f2.close()




