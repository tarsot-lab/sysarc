//---------------------------------------------------------
// le-interruptor.cc:
//---------------------------------------------------------

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <interruptorm.h>

//---------------------------------------------------------

using namespace std;

//---------------------------------------------------------

atomic<bool> run(true);
interruptorm rwl;

//---------------------------------------------------------

void lector()
{
	while (run)
	{
		rwl.adquirirLectura();
        	rwl.liberarLectura();
	}
}

//---------------------------------------------------------

void escritor()
{
	while (run)
	{
		rwl.adquirirEscritura();
        	rwl.liberarEscritura();
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
