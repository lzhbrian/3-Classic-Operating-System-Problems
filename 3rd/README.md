# 存储管理问题 - 文件字节倒放问题

[TOC]

## 一、问题描述

* 生成一个由随机产生的字符型数据组成大的数据文件（例如，大小$\geq1GB$）。将该文件中的所有字节进行倒放，存入原文件，即将文件中的首字节与尾字节对换，次字节与次尾字节对换，以此类推。
* 编写两个程序，一个程序采用常规的文件访问方法，另一个程序采用内存映射文件方法。请记录两种方法完成字节倒放所需要的时间，并进行比较。

### 1. 实验环境

- 操作系统平台可选Windows或Linux，编程语言不限。

### 2. 函数参考

* Windows：CreateFileMapping创建一个文件映射对象，MapViewOfFile将文件映射对象映射到当前进程的地址空间，UnmapViewOfFile在当前进程的内存地址空间解除对一个文件映射对象的映射；
* Linux：mmap把一个文件映射到当前进程的地址空间，munmap解除内存映射。

<div style="page-break-after: always;"></div>

## 二、实现

### 1. 思路：

- 因为我的电脑是 $*nix$ 系列的，所以我采用 __posix__ 的函数来实现进程、线程的操作
- 本实验中，我采用 __多线程__、__共享内存__ 的方式来实现快速排序，主要思路如下：
  - `Quick_sort()`函数：按照快速排序的思想，选定最后序列里最后一个数作为基准，将自己分成两半（大于基准，小于基准），再创建两个新线程递归地调用自己，分别处理这两半序列。
  - 如果新线程所需要处理的数据数量小于1000，则调用`real_quick_sort()`函数实现这些数的快速排序
  - 这里采用多线程的方式，所以我全局的申明了一个 `Datatype data[number_length]` 数组，即为这所有线程的共享内存；
    - 同时，他们访问 `data[]` 的时候也并不会发生冲突，因为每个线程处理的都是这个数组里面不同范围的数据。

### 2. 产生一个有$1,000,000$个数的文件：

- 我们首先编写 python 程序`generate_1m_numbers.py`来生成 $1,000,000$个随机小数，并将其写到一个文件`./Numbers.txt`里面，代码如下：

  ```python
  import random
  fp = open('./Numbers.txt','w')
  length = 1000000
  min_x = 0
  max_x = 60000
  count = 0
  for x in xrange(0,length):
  		print >> fp, (random.uniform(min_x, max_x))
  		count += 1
  print "Suc. print ", count, " numbers"
  ```

### 3. 实现快速排序：

1. `Quick_sort()`函数：

   ```c++
   // quick sort unit
   void *Quick_sort(void* para_s) {

      	// get received arg
      	struct para *recv_para;  
      	recv_para = (struct para *)para_s;  
      	int start_index = (*recv_para).start_index;  
      	int end_index = (*recv_para).end_index;  

      	// length of data this thread can control
      	int data_length = end_index - start_index + 1;
      	if (data_length <= 1000) {
      		real_quick_sort(data, start_index, end_index);
      		return 0;
      	}

      	int *pos = new int(1);
      	*pos = partition(data, start_index, end_index);

      	pthread_t front_thread;
      	pthread_t back_thread;

      	struct para para_front;
      	struct para para_back;
      	
      	para_front.start_index = start_index;  
      	para_front.end_index = *pos-1;

      	para_back.start_index = *pos+1;  
      	para_back.end_index = end_index;

      	// Create front,back threads
      	if(pthread_create(&front_thread, NULL, Quick_sort, &(para_front))) {  
      		cout << "\n ERROR creating front thread!" << endl;  
      		exit(1);
      	}
      	if(pthread_create(&back_thread, NULL, Quick_sort, &(para_back))) {  
      		cout << "\n ERROR creating back thread!" << endl;  
      		exit(1);
      	}

      	// Join front,back threads
      	if(pthread_join(front_thread, NULL)) {
      		cout << "\n ERROR joining front thread!" << endl; 
      		exit(1);
      	}
      	if(pthread_join(back_thread, NULL)) {
      		cout << "\n ERROR joining back thread!" << endl; 
      		exit(1);
      	}

      	return 0;
   }
   ```

2. 个数<1000时的快速排序`real_quick_sort`：

   ```c++
   // 小于 1000 时调用的 quicksort
   void real_quick_sort(Datatype* a, int left, int right) {
      	if (right <= left) return;
      	int pos = partition(a,left,right);
      	real_quick_sort(a,left,pos-1);
      	real_quick_sort(a,pos+1,right);
   }
   // exchange
   void exch(Datatype* a, Datatype* b) {
      	Datatype temp = *a;
      	*a = *b;
      	*b = temp;
   }
   int partition(Datatype* a, int low, int up) {  
      	Datatype pivot = a[up];  
      	int i = low-1;  
      	for (int j = low; j < up; j++)  
      	{  
      		if(a[j] <= pivot)  
      		{  
      			i++;  
      			exch(&a[i], &a[j]);  
      		}  
      	}  
      	exch(&a[i+1], &a[up]);  
      	return i+1;  
   }
   ```

3. 主函数：

   ```c++
   /******* Main func **************/
   int main(int argc, char * argv[])  
   {

      	// Start time
      	open_time = getSystemTime();

      	// Read 1m numbers
      	ifstream file("./Numbers.txt");
      	while (!file.eof()) {
      		file >> data[n];
      		n = n + 1;
      	}

      	// indicator of success read files
      	cout << getSystemTime() - open_time << " ms used to read " << numbers_length << " numbers..." << endl;

      	// test for read numbers
      	// cout << data[100] << ", " << data[200] << ", " << data[numbers_length-1] << ", " << data[numbers_length] << endl;

      	pthread_t main_thread;

      	struct para pata_main;  
      	pata_main.start_index = 0;  
      	pata_main.end_index = numbers_length-1;

      	// Create main thread
      	if(pthread_create( &main_thread, NULL, Quick_sort, &(pata_main) )) {  
      		cout << "\n ERROR creating first main thread!" << endl;  
      		exit(1);
      	}

      	// Join main thread
      	if(pthread_join(main_thread, NULL)) {
      		cout << "\n ERROR joining first main thread!" << endl; 
      		exit(1);
      	}
      	

      	// Check result
      	cout << "First 10 numbers: \n";
      	for (int i = 0; i < 10; ++i) {
      		cout << data[i] << endl;
      	}
      	cout << endl;
      	cout << "Last 10 numbers: \n";
      	for (int i = numbers_length-10; i < numbers_length; ++i) {
      		cout << data[i] << endl;
      	}

      	// indicator of runtime
      	cout << getSystemTime() - open_time << " ms used to finish the whole program! " << endl;

      	// write sorted result to file
      	ofstream fout("./sorted_Numbers.txt");
      	for (int i = 0; i < numbers_length; ++i) {
      		fout << data[i] << endl;
      	}
      	return 0;
   }
   ```

<div style="page-break-after: always;"></div>

## 三、实验结果

1. 通过`generate_1m_numbers.py`生成的文件为：`Numbers.txt`，我们设置的数范围为：0～60,000

2. 通过`quick_sort_multithread.cpp`的操作之后生成的排序后的文件为：`sorted_Numbers.txt`，其前十行、后十行代码如下：

   ```
   0.0153327
   0.13771
   0.241184
   0.24939
   0.292859
   0.359523
   0.375892
   0.419432
   0.48103
   0.507452
   ```

   ```
   59999.3
   59999.4
   59999.5
   59999.5
   59999.6
   59999.6
   59999.7
   59999.8
   60000
   60000
   ```

   可以看到，我们的程序成功地对原数据排了序。

<div style="page-break-after: always;"></div>

## 四、思考题

1. 采用常规的文件访问方法时，改变缓冲区的大小对程序的性能有什么影响？请用图表描述缓冲区的大小与程序性能之间的关系。
   * ​
2. 内存映射文件方法和常规的文件访问方法在性能上有什么差异，试分析其原因。




## 五、感想

- 有了第一题创建线程的经验基础，再做这道题的时候就比较熟练了
- 在这次实验中，我对线程的创建、使用又有了更加深刻的理解







