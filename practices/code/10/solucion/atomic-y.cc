//---------------------------------------------------------
// atomic-y.cc
//---------------------------------------------------------

#include <unistd.h>  // alarm
#include <atomic>    // atomic
#include <iostream>  // cout endl
#include <sstream>   // stringstream
#include <thread>    // thread

//---------------------------------------------------------

const int N = 32;

//---------------------------------------------------------

class barrera_t
{
public:
	explicit barrera_t(unsigned count_):
		count(count_), spaces(count), generation(0) {}

	void esperar()
	{
		unsigned my_generation = generation;
		if(!--spaces)
		{
			spaces = count;
			++generation;
		}
		else
		{
			while(generation == my_generation)
				std::this_thread::yield();
		}
	}

private:
	unsigned const count;
	std::atomic<unsigned> spaces;
	std::atomic<unsigned> generation;
} barrera(N);

//---------------------------------------------------------

void hebra()
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	std::string antes = ss.str() + ": antes\n", despues = ss.str() + ": después\n";
	
	while(true)
	{
		std::cout << antes;
		barrera.esperar();
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
