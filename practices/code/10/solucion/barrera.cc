//---------------------------------------------------------
// barrera.cc: esqueleto para completar
//---------------------------------------------------------

#include <unistd.h>  // alarm
#include <iostream>  // cout endl
#include <sstream>   // stringstream
#include <thread>    // thread

//---------------------------------------------------------

const int N = 32;

//---------------------------------------------------------

class barrera_t
{
public:
	barrera_t(unsigned _limite) {}
	void esperar() {}
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
