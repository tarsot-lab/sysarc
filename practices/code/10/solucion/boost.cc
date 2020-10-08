//---------------------------------------------------------
// boost.cc
//---------------------------------------------------------

#include <unistd.h>  // alarm
#include <iostream>  // cout endl
#include <mutex>     // mutex
#include <thread>    // thread
#include <boost/thread/barrier.hpp> // barrier

//---------------------------------------------------------

const int N = 32;

//---------------------------------------------------------

boost::barrier barrera(N);

void hebra()
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	std::string antes = ss.str() + ": antes\n", despues = ss.str() + ": después\n";
	
	while(true)
	{
		std::cout << antes;
		barrera.wait();
		std::cout << despues;
	}
}

//---------------------------------------------------------

int main()
{
	alarm(1);
	std::thread t[N];
	for (auto& i: t) i = std::thread(hebra);
	for (auto& i: t) i.join();
}

//---------------------------------------------------------
