//---------------------------------------------------------
// shared.cc
//---------------------------------------------------------

#include <atomic>
#include <chrono>
#include <iostream>
#include <shared_mutex>
#include <thread>

//---------------------------------------------------------

using namespace std;

//---------------------------------------------------------

atomic<bool> run(true);
shared_mutex em;         // exclusión mutua

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
		shared_lock<shared_mutex> sl(em);
		seccion_critica('0');
	}
}

//---------------------------------------------------------

void escritor()
{
	while (run)
	{
		unique_lock<shared_mutex> ul(em);
		seccion_critica('a');
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
