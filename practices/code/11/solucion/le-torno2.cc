//---------------------------------------------------------
// le-torno2.cc // interruptor + torno = libre de inanición
//---------------------------------------------------------

#include <atomic>
#include <iostream>
#include <thread>
#include "interruptor.h"

//---------------------------------------------------------

using namespace std;

//---------------------------------------------------------

atomic<bool> run(true);
interruptor i;
mutex nadie, torno;

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
		torno.lock();
		torno.unlock();
		i.lock(nadie);
		seccion_critica('0');
		i.unlock(nadie);
	}
}

//---------------------------------------------------------

void escritor()
{
	while (run)
	{
		torno.lock();
		nadie.lock();
		seccion_critica('a');
		nadie.unlock();
		torno.unlock();
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
