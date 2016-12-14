# Brian Lin,Tzu-Heng's Work
# Reverse a big char file by memmap in python.numpy

# Mailto: lzhbrian@gmail.com
# Github: github.com/lzhbrian
# Linkedin: linkedin/in/lzhbrian


import sys
import numpy as np

# open source fike
f = np.memmap('./data/one_gig_file.txt', mode='r')
# f = np.memmap('./data/small_file.txt', mode='r')

# calc length
data_length = len(f)

# open write file, 
# 'data_length-1' because of the \n in the last
fp = np.memmap('./data/reversed_file.txt', shape=(data_length-1), mode='w+')

# from 0 to data_length-2, does not include data_length-2
reversed_arr = f[0:data_length-1] 

# reverse!
reversed_arr = reversed_arr[::-1]

# copy to fp
fp[:] = reversed_arr[:]

# close file
fp.flush()

