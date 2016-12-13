/*
	Brian Lin,Tzu-Heng's Work
	A quick sort implementation by multithreading

	Mailto: lzhbrian@gmail.com
	Github: github.com/lzhbrian
	Linkedin: linkedin/in/lzhbrian
*/

# include <pthread.h>	// 线程、互斥锁
# include <semaphore.h>	// 同步信号量 P, V
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <fstream>
# include <unistd.h>
# include <vector>
# include <time.h>
# include <sys/timeb.h>
using namespace std;

# define numbers_length 1000000
# define Datatype double

Datatype data[numbers_length];
int n = 0;
long long open_time = 0;	// 程序开始时间

// Get current time, return: ms
long long getSystemTime() {
	struct timeb t;
	ftime(&t);
	return 1000 * t.time + t.millitm;
}

// 传给 Quick_sort 的参数结构体
struct para
{   
	int start_index;  
	int end_index;   
};
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

// 小于 1000 时调用的 quicksort
void real_quick_sort(Datatype* a, int left, int right) {
	if (right <= left) return;
	int pos = partition(a,left,right);
	real_quick_sort(a,left,pos-1);
	real_quick_sort(a,pos+1,right);
}



int q_sort_call_time = 0;

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






