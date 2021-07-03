/*
Group ID 1
2019-1-60-089
2019-1-60-204
*/

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include<stdlib.h>
#include<unistd.h>
using namespace std;

sem_t totalCustomerFull,totalCustomerEmpty;
sem_t sofaFull,sofaEmpty;
sem_t barberChairFull,barberChairEmpty;
sem_t totalBarberFull,totalBarberEmpty;
sem_t payingFull,payingEmpty;
sem_t regFull,regEmpty;
sem_t exitFull;
queue<int>q1;
queue<int>q2;
queue<int>q3;
queue<int>q4;
queue<int>q5;
queue<int>q6;
queue<int>q7;

pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_mutex_t lock3;
pthread_mutex_t lock4;
pthread_mutex_t lock5;
pthread_mutex_t lock6;

void init_semaphore(){
	sem_init(&totalCustomerEmpty,0,20);
	sem_init(&totalCustomerFull,0,0);
    sem_init(&sofaEmpty,0,4);
	sem_init(&sofaFull,0,0);
    sem_init(&barberChairEmpty,0,3);
	sem_init(&barberChairFull,0,0);
    sem_init(&totalBarberEmpty,0,3);
	sem_init(&totalBarberFull,0,0);
	sem_init(&regEmpty,0,1);
	sem_init(&regFull,0,0);
	sem_init(&payingEmpty,0,1);
	sem_init(&payingFull,0,0);
	sem_init(&exitFull,0,0);

	pthread_mutex_init(&lock1,0);
    pthread_mutex_init(&lock2,0);
    pthread_mutex_init(&lock3,0);
    pthread_mutex_init(&lock4,0);
	pthread_mutex_init(&lock5,0);
	pthread_mutex_init(&lock6,0);
}

void *customer(void *arg){
	sem_wait(&totalCustomerEmpty);
	int enteringCustomer=(intptr_t)arg;
	sleep(1);
	printf("Customer %d entered the barber shop\n",enteringCustomer);
	q1.push(enteringCustomer);
	sem_post(&totalCustomerFull);

	sem_wait(&sofaEmpty);
	pthread_mutex_lock(&lock1);
	int sofaCustomer=q1.front();
	//sleep(1);
	printf("Customer %d sit in sofa\n",sofaCustomer);
	q2.push(sofaCustomer);
	q1.pop();
	pthread_mutex_unlock(&lock1);
	sem_post(&sofaFull);

	sem_wait(&sofaFull);
	sem_wait(&barberChairEmpty);
	pthread_mutex_lock(&lock2);
	int barberChairCustomer=q2.front();
	//sleep(1);
	printf("Customer %d sit in barber chair\n",barberChairCustomer);
	q3.push(barberChairCustomer);
	q2.pop();
	pthread_mutex_unlock(&lock2);
	sem_post(&barberChairFull);
	sem_post(&sofaEmpty);

	sem_wait(&payingFull);
	pthread_mutex_lock(&lock6);
	int payingCustomer=q5.front();
	printf("Customer %d is paying the bill ->->->->->->->->->->->->->->->->->\n",payingCustomer);
	q6.push(payingCustomer);
	q5.pop();
	pthread_mutex_unlock(&lock6);
	sem_post(&payingEmpty);

	sem_wait(&regEmpty);
	sem_post(&regFull);

	sem_wait(&exitFull);
	sem_wait(&totalCustomerFull);
	//sleep(1);
	printf("Customer %d exit the shop ........................................\n",q7.front());
	q7.pop();
	sem_post(&totalCustomerEmpty);

	return 0;
}

void *barber(void *arg){
     int barber=(intptr_t)arg;
	sem_wait(&barberChairFull);
	pthread_mutex_lock(&lock3);
	sem_wait(&totalBarberEmpty);
	int barberCustomer=q3.front();
	//sleep(1);
	printf("Barber %d started cutting hair of Customer %d\n",barber,barberCustomer);
	q4.push(barberCustomer);
	q3.pop();
	sem_post(&totalBarberFull);
	pthread_mutex_unlock(&lock3);


	sem_wait(&totalBarberFull);
	pthread_mutex_lock(&lock4);
	int finishedCustomer=q4.front();
	//sleep(1);
	printf("Barber %d finished cutting hair of Customer %d\n",(intptr_t)arg,finishedCustomer);
	q4.pop();
	pthread_mutex_unlock(&lock4);
	sem_post(&totalBarberEmpty);

	sem_wait(&payingEmpty);
	pthread_mutex_lock(&lock5);
	q5.push(finishedCustomer);
	pthread_mutex_unlock(&lock5);
	sem_post(&payingFull);


	sem_wait(&regFull);
	sem_post(&barberChairEmpty);
	int paidcustomer=q6.front();
	//sleep(1);
	pthread_mutex_lock(&lock6);
	printf("Barber %d accepted the payment of Customer %d <-<-<-<-<--<-<-<-<-<-<-<\n",(intptr_t)arg,paidcustomer);
	q7.push(paidcustomer);
	q6.pop();
	pthread_mutex_unlock(&lock6);
	sem_post(&regEmpty);

	sem_post(&exitFull);



}

int main(){
	pthread_t Customer[50];
	pthread_t Barber1,Barber2,Barber3;
	init_semaphore();
	for(int i=1;i<=50;i++){
        pthread_create(&Customer[i],NULL,customer,(void*)i);
	}
	while(1){
        pthread_create(&Barber1,NULL,barber,(void*)1);
		pthread_create(&Barber2,NULL,barber,(void*)2);
		pthread_create(&Barber3,NULL,barber,(void*)3);
	}
	return 0;
}

