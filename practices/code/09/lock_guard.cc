//----------------------------------------------------
// lock_guard.cc
//----------------------------------------------------

#include <unistd.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

//----------------------------------------------------

using namespace std;

//----------------------------------------------------

const int N = 16;

//----------------------------------------------------

void seccion_critica()
{
	cout << "[" << this_thread::get_id() << "]: ";
	for(int i = 0; i < 10; ++i)
		cout << i;
	cout << endl;
}

//----------------------------------------------------

void hebra()
{
	while(true)
	{
		seccion_critica();
	}
}

//----------------------------------------------------

int main()
{
	alarm(1);

	thread t[N];
	
	for(auto& i: t)
		i = thread(hebra);

	for(auto& i: t)
		i.join();
}

//----------------------------------------------------
