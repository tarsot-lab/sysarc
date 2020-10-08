//---------------------------------------------------------
// rwlock.cc // solución de pthread
//---------------------------------------------------------

#include <pthread.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

//---------------------------------------------------------

using namespace std;

//---------------------------------------------------------

atomic<bool> run(true);
pthread_rwlock_t rwlock;  // cerrojo de lectura/escritura

//---------------------------------------------------------

void seccion_critica(char c)
{
	for (char i = 0; i < 10; ++i)
		cout << c++;
	cout << endl;
}

//---------------------------------------------------------

void lector()
{
	while (run)
	{
		pthread_rwlock_rdlock(&rwlock);
		seccion_critica('0');
		pthread_rwlock_unlock(&rwlock);
	}
}

//---------------------------------------------------------

void escritor()
{
	while (run)
	{
		pthread_rwlock_wrlock(&rwlock);
		seccion_critica('a');
		pthread_rwlock_unlock(&rwlock); 
	}
}

//---------------------------------------------------------

int main()
{
	const unsigned N = 128;
	thread lectores[N], escritores[N];
	
	pthread_rwlock_init(&rwlock, NULL);
	
	for (int i = 0; i < N; ++i)
	{
		  lectores[i] = thread(  lector);
		escritores[i] = thread(escritor);
	}
	
	this_thread::sleep_for(chrono::seconds(1));
	run = false;
	
	for(thread& i:   lectores) i.join();
	for(thread& i: escritores) i.join();
}

//---------------------------------------------------------
