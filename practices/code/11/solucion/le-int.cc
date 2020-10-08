//---------------------------------------------------------
// le-int.cc // interruptor --> inanición
//---------------------------------------------------------

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include "interruptor.h"

//---------------------------------------------------------

using namespace std;

//---------------------------------------------------------

atomic<bool> run(true);
interruptor i;
mutex em;               // exclusión mutua

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
		i.lock(em);
		seccion_critica('0');
		i.unlock(em);
	}
}

//---------------------------------------------------------

void escritor()
{
	while (run)
	{
		em.lock();
		seccion_critica('a');
		em.unlock();
	}
}

//---------------------------------------------------------

int main()
{
	const unsigned N = 128;
	thread lectores[N], escritores[N];
	
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
