# -*- encoding=utf8 -*-

# Brian Lin,Tzu-Heng's Work
# generate a one gigabytes file

# Mailto: lzhbrian@gmail.com
# Github: github.com/lzhbrian
# Linkedin: linkedin/in/lzhbrian


fp = open('./data/one_gig_file.txt','w')

data_length = 10000000

from random import Random
def random_str(randomlength=8):
    str = ''
    chars = 'AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789'
    length = len(chars) - 1
    random = Random()
    for i in range(randomlength):
        str += chars[random.randint(0, length)]
    return str

for x in range(1,101):
	a = random_str(data_length)
	print(a, file=fp, end='')
	print('Finished ', x, ' times')

