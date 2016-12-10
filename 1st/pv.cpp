/*
	Brian Lin,Tzu-Heng's Work
	The bank teller service problem

	Mailto: lzhbrian@gmail.com
	Github: github.com/lzhbrian
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


# define customer_num 40   	// 最大顾客数目
# define server_num 4     	// 柜员数目
vector<int> queueing_cus_list(0);		// 正在等待的顾客 list
vector<int> start_served_cus_list(0);	// 完成服务的顾客 list
	// server 从 queueing_cus_list 里面找寻正在等待的 customer	
	// 若全部完成 则 start_served_cus_list 充满所有的 customer
long long open_time = 0;	// 程序开始时间
long long now = 0;			

int n = 0;	// 从文件读入有多少个顾客

// Get current time, return: ms
long long getSystemTime() {
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}

/******* Semaphore and Mutex ****/
// 申请 Semaphore
sem_t *sem_customers; 	// 顾客同步信号量
sem_t *sem_servers;		// 柜员同步信号量


// 申请 mutex
pthread_mutex_t mutex_customers = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_servers = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cout = PTHREAD_MUTEX_INITIALIZER;
/********************************/


/******* Customer 结构体 *********/
typedef struct Customer_struct
{

	int id;
	int enter_time;			// known
	int serve_time;			// known

	int start_serve_time;	// unknown
	int leave_time; 		// unknown
	int server_No;			// unknown

	Customer_struct(int id_s, int enter_time_s, int serve_time_s) {
		id = id_s;
		enter_time = enter_time_s;
		serve_time = serve_time_s;
		start_serve_time = 0;
		leave_time = 0;
		server_No = 0;
	}
	Customer_struct(){
		id = 0;
		enter_time = 0;			// known
		serve_time = 0;			// known

		start_serve_time = 0;	// unknown
		leave_time = 0; 		// unknown
		server_No = 0;
	}

} Customer;

Customer Customers[customer_num];
/********************************/



/******* 柜员、顾客 执行函数 *******/
// 柜员服务 func
void *Serve_do(void* server_No_v) {
	int server_No = *(int*)server_No_v;
	while(1) {

		// V: 顾客少一个资源，相当于柜员叫号
		sem_wait(sem_customers);


		// 有没有柜员叫同一个号
		pthread_mutex_lock(&mutex_servers);		// lock server
			int cus_No = queueing_cus_list[0];		// 获取第一个顾客信息
			queueing_cus_list.erase(queueing_cus_list.begin());

			start_served_cus_list.push_back(cus_No);	// 只要开始服务就加入队列
			// cout << start_served_cus_list.size() << endl;

			Customers[cus_No].server_No = server_No;	// 记录服务这个顾客的柜员
		pthread_mutex_unlock(&mutex_servers);	// unlock server


		pthread_mutex_lock(&mutex_cout);
			now = getSystemTime();
			cout << (now - open_time)/1000 << ", Customer " << Customers[cus_No].id << " starts being served by server " << server_No << endl;
			Customers[cus_No].start_serve_time = (now - open_time)/1000;
		pthread_mutex_unlock(&mutex_cout);


		sleep(Customers[cus_No].serve_time);	// 服务


		pthread_mutex_lock(&mutex_servers);		// lock server
			if (start_served_cus_list.size() == n)
			{
				pthread_mutex_lock(&mutex_cout);
					now = getSystemTime();
					cout << (now - open_time)/1000 << ", Customer " << Customers[cus_No].id << " suc. served by server " << server_No << endl;
					Customers[cus_No].leave_time = (now - open_time)/1000;
				pthread_mutex_unlock(&mutex_cout);
				
				cout << server_No << " server end!" << endl;
				// 如果所有顾客都完成服务了，就break
				pthread_mutex_unlock(&mutex_servers);	// unlock server
				break;
			}
		pthread_mutex_unlock(&mutex_servers);	// unlock server


		pthread_mutex_lock(&mutex_cout);
			now = getSystemTime();
			cout << (now - open_time)/1000 << ", Customer " << Customers[cus_No].id << " suc. served by server " << server_No << endl;
			Customers[cus_No].leave_time = (now - open_time)/1000;
		pthread_mutex_unlock(&mutex_cout);


		// 柜员多一个资源，相当于柜员完成一个服务
		sem_post(sem_servers);

	}
	return 0;
}

// 顾客服务 func
void *Customer_do(void* cus_No_v) {

	int cus_No = *(int*)cus_No_v;

	// debug 用 输出当前顾客信息
	// pthread_mutex_lock(&mutex_cout);
	// 	cout << "call customer " << Customers[cus_No].id << endl;
	// 	cout << "sleep time: " << Customers[cus_No].enter_time << endl;
	// pthread_mutex_unlock(&mutex_cout);

	sleep(Customers[cus_No].enter_time); 	// 睡到进入的时间

	pthread_mutex_lock(&mutex_cout);
		now = getSystemTime();
		cout << (now - open_time)/1000 << ", Customer " << Customers[cus_No].id << " comes to the bank!" << endl;
	pthread_mutex_unlock(&mutex_cout);


	// P: 顾客多一个资源，相当于顾客拿号
	sem_post(sem_customers);
		pthread_mutex_lock(&mutex_customers);	// lock customer
			queueing_cus_list.push_back(cus_No);// 顾客入队
			now = getSystemTime();
			Customers[cus_No].start_serve_time = (now - open_time)/1000;	// 记录开始服务的时间
		pthread_mutex_unlock(&mutex_customers);	// free customer
	// V: 柜员少一个资源，相当于等待柜员叫号
	sem_wait(sem_servers);
	return 0;
}
/********************************/






/******* Main func **************/
int main(int argc, char * argv[])  
{

	int id, enter_time, serve_time;

	// Read File into struct
	// n: 一共有多少 顾客;
	ifstream file("./Customer.txt");
	while (!file.eof())
	{
		file >> id >> enter_time >> serve_time;
		Customers[n].id = id;
		Customers[n].enter_time = enter_time;
		Customers[n].serve_time = serve_time;
		n = n + 1;
	}
	// debug 用 cout 读入的数据
	// for (int i = 0; i < n; ++i)
	// {
	// 	cout << Customers[i].id << "," << Customers[i].enter_time << ","
	// 		<< Customers[i].serve_time << endl;
	// }

	// Semaphore init
	sem_unlink("sem_servers");  
	sem_unlink("sem_customers");
	sem_servers = sem_open("sem_servers", O_CREAT, 0, server_num);  
	sem_customers = sem_open("sem_customers", O_CREAT, 0, 0);

	// Start time
	open_time = getSystemTime();

	// Servers, Customers threads init
	pthread_t server_threads[server_num];
	pthread_t customer_threads[customer_num];
	int tid_s[server_num];
	for (int i = 0; i < server_num; ++i) {
		tid_s[i] = i;
		if(pthread_create(&server_threads[i], NULL, Serve_do, &tid_s[i])) {  
			printf("\n ERROR creating server thread %d", tid_s[i]);  
			exit(1);
		}
	}
	int tid_c[customer_num];
	for (int j = 0; j < n; ++j) {
		tid_c[j] = j;
		if(pthread_create(&customer_threads[j], NULL, Customer_do, &tid_c[j])) {  
			printf("\n ERROR creating customer thread %d", tid_c[j]);  
			exit(1);
		}
	}


	// Wait for threads to complete
	for (int i = 0; i < server_num; ++i) {
		if(pthread_join(server_threads[i], NULL)) {
			cout << "\n ERROR joining server thread: " << i << endl; 
			exit(1);
		} 
	}
	for (int j = 0; j < n; ++j) {
		if(pthread_join(customer_threads[j], NULL)) {
			cout << "\n ERROR joining customer thread: " << j << endl;  
			exit(1);  
		}
	}

	// Destroy Semaphore, Mutex
	sem_close(sem_customers);
	sem_close(sem_servers);
	
	pthread_mutex_destroy(&mutex_customers);
	pthread_mutex_destroy(&mutex_servers);
	pthread_mutex_destroy(&mutex_cout);
	
	cout << "All done!" << endl;

	// output result
	cout << "ID\tEnter\tStart\tLeave\tServerNo." << endl;
	for (int i = 0; i < n; ++i)
	{
		cout << Customers[i].id << "\t"
			 << Customers[i].enter_time << "\t"
			 << Customers[i].start_serve_time << "\t" 
			 << Customers[i].leave_time << "\t" 
			 << Customers[i].server_No << endl;
	}



	return 0;
}






