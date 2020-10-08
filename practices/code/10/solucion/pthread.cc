//-----------------------------------------------------------------------------
// pthread.cc
//-----------------------------------------------------------------------------

#include <pthread.h>  // pthread_barrier
#include <unistd.h>   // alarm
#include <algorithm>  // for_each
#include <iostream>   // cout endl
#include <mutex>      // mutex
#include <sstream>    // stringstream
#include <string>     // string
#include <thread>     // thread

//-----------------------------------------------------------------------------

const int N = 32;

//---------------------------------------------------------

pthread_barrier_t barrera;

void hebra()
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	std::string antes = ss.str() + ": antes\n", despues = ss.str() + ": después\n";
	
	while(true)
	{
		std::cout << antes;
		pthread_barrier_wait(&barrera);
		std::cout << despues;
	}
}

//-----------------------------------------------------------------------------

int main()
{
	alarm(1);
	pthread_barrier_init(&barrera, NULL, N);
	std::thread t[N];
	for (auto& i: t) i = std::thread(hebra);
	for (auto& i: t) i.join();

}

//-----------------------------------------------------------------------------
