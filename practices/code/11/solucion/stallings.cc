//---------------------------------------------------------
// stallings.cc: prioridad a los lectores
//---------------------------------------------------------

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

//---------------------------------------------------------

using namespace std;

//---------------------------------------------------------

atomic<bool> run(true);
mutex em_leyendo;               // exclusión mutua leyendo
volatile unsigned leyendo = 0;  // nº lectores
mutex no_escritor;              // exclusion mutua l/e

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
		em_leyendo.lock();
		if (++leyendo == 1)
			no_escritor.lock();
		em_leyendo.unlock();
		seccion_critica('0');
		em_leyendo.lock();
		if (--leyendo == 0)
			no_escritor.unlock();
		em_leyendo.unlock();
	}
}

//---------------------------------------------------------

void escritor()
{
	while (run)
	{
		no_escritor.lock();
		seccion_critica('a');
		no_escritor.unlock();
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
