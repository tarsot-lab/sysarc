//---------------------------------------------------------
// le-lg.cc // problema lectores/escritores: s + i
//---------------------------------------------------------

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

//---------------------------------------------------------

using namespace std;

//---------------------------------------------------------

mutex m;

void seccion_critica(char c)
{
	lock_guard<mutex> lg(m);
	for (char i = 0; i < 10; ++i)
		cout << c++;
	cout << endl;
}

//---------------------------------------------------------

int main()
{
	const unsigned N = thread::hardware_concurrency();
	atomic<bool> run(true);
	
	thread lectores[N], escritores[N];

	for (thread& i: lectores)
		i = thread([&]
		{
			while(run)
				seccion_critica('0');
		});

	for (thread& i: escritores)
		i = thread([&]
		{
			while(run)
				seccion_critica('a');
		});

	this_thread::sleep_for(chrono::seconds(1));
	run = false;

	for(thread& i: lectores)
		i.join();

	for(thread& i: escritores)
		i.join();
}

//---------------------------------------------------------
