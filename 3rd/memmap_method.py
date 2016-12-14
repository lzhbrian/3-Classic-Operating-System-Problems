# Brian Lin,Tzu-Heng's Work
# Reverse a big char file by memmap in python.numpy

# Mailto: lzhbrian@gmail.com
# Github: github.com/lzhbrian
# Linkedin: linkedin/in/lzhbrian


import sys
import numpy as np

# open source fike
f = np.memmap('./data/one_gig_file.txt', mode='r+')

# calc length
data_length = len(f)

# 'data_length-1' because of the \n in the last
reversed_arr = f[0:data_length-1][::-1]

# overwrite f
f[0:data_length-1] = reversed_arr[:]

# close file
f.flush()