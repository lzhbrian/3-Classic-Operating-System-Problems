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
int data[numbers_length];
int n = 0;
long long open_time = 0;	// 程序开始时间

// Get current time, return: ms
long long getSystemTime() {
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}


// quick sort unit
void *Quick_sort(void* start_index, void* end_index) {

	pthread_t front_thread;
	pthread_t back_thread;

	// Create front,back threads
	if(pthread_create(&front_thread, NULL, Quick_sort, &tid_s[i])) {  
		cout << "\n ERROR creating first main thread!" << endl;  
		exit(1);
	}
	if(pthread_create(&back_thread, NULL, Quick_sort, &tid_s[i])) {  
		cout << "\n ERROR creating first main thread!" << endl;  
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
	// cout << data[100] << ", " << data[200] << ", " << data[numbers_length-1] 
			<< ", " << data[numbers_length] << endl;

	pthread_t main_thread;

	// Create main thread
	if(pthread_create(&main_thread, NULL, Quick_sort, &tid_s[i])) {  
		cout << "\n ERROR creating first main thread!" << endl;  
		exit(1);
	}

	// Join main thread
	if(pthread_join(main_thread, NULL)) {
		cout << "\n ERROR joining first main thread!" << endl; 
		exit(1);
	}
	

	return 0;
}






