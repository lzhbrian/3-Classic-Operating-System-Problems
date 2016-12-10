# include <pthread.h>
# include <semaphore.h>  
# include <stdio.h>  
# include <stdlib.h> 
# include <iostream> 
# define NITER 1000000  
int count = 0; //两个线程共享同一个资源：count  

sem_t *customers; 	// 顾客同步信号量
sem_t *server;		// 柜员同步信号量



# define N 100 // 柜员数目
struct Customer
{
	int number;
	int enter_time;
	int serve_time;
};

// 柜员服务
void *Serve() {
	while(1) {
		// pop_customer
		// Sleep(customer.serve_time)
		// print customer served
			// print 
	}
}



void *ThreadAdd(void * a)  
{  
    int i, tmp;  
     
    for(i = 0; i < NITER; i++)  
    {  
        sem_wait(s); 
        //sem_wait(&sem_for_linux);  
        //++count; //把自增的过程放大给你看  
        tmp = count;      /* copy the global count locally */  
        tmp = tmp+1;      /* increment the local copy */  
        count = tmp;      /* store the local value into the global count */  
        sem_post(s);
        //sem_post(&sem_for_linux);  
    }  
        return NULL;
}  
  
int main(int argc, char * argv[])  
{  

	// Read File
	customers[];


    pthread_t tid1, tid2;  
  
    sem_unlink("s");  
    s = sem_open("s", O_CREAT, 0, N);  
    //sem_init(&sem_for_linux, 0, 1);
     
     
    // 创建两个线程  
    if(pthread_create(&tid1, NULL, ThreadAdd, NULL))  
    {  
        printf("\n ERROR creating thread 1");  
        exit(1);  
    }  

    printf("%d\n", count);

    if(pthread_create(&tid2, NULL, ThreadAdd, NULL))  
    {  
        printf("\n ERROR creating thread 2");  
        exit(1);  
    }  
     
    // 等待两个线程都结束  
    if(pthread_join(tid1, NULL))     /* wait for the thread 1 to finish */  
    {  
        printf("\n ERROR joining thread");  
        exit(1);  
    }  

    if(pthread_join(tid2, NULL))        /* wait for the thread 2 to finish */  
    {  
        printf("\n ERROR joining thread");  
        exit(1);  
    }  
     
    if (count < 2 * NITER)  
        printf("\n BOOM! count is [%d], should be %d\n", count, 2*NITER);  
    else  
        printf("\n OK! count is [%d]\n", count);  
     
    //sem_destroy(&sem_for_liux);  
    sem_close(s);  
     
    pthread_exit(NULL);  
}






