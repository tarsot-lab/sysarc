//---------------------------------------------------------
// ballhausen.cc
//---------------------------------------------------------

#include <semaphore.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

//---------------------------------------------------------

using namespace std;

//---------------------------------------------------------

const unsigned N = 128;

atomic<bool> run(true);
mutex em;               // exclusión mutua
sem_t justicia;

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
		sem_wait(&justicia);
		seccion_critica('0');
		sem_post(&justicia);
	}
}

//---------------------------------------------------------

void escritor()
{
	while (run)
	{
		em.lock();
		for (unsigned i = 0; i < N; ++i)
			sem_wait(&justicia);
		em.unlock();
		seccion_critica('a');
		for (unsigned i = 0; i < N; ++i)
			sem_post(&justicia);
	}
}

//---------------------------------------------------------

int main()
{
	thread lectores[N], escritores[N];
	
	sem_init(&justicia, 0, N); // N = número de lectores
	
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
