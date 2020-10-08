//---------------------------------------------------------
// barrera-ceo.cc: barrera con espera ocupada
//---------------------------------------------------------

#include <unistd.h>  // alarm
#include <iostream>  // cout endl
#include <mutex>     // mutex
#include <sstream>   // stringstream
#include <thread>    // thread

//---------------------------------------------------------

const int N = 32;

//---------------------------------------------------------

class barrera_t
{
public:
	barrera_t(unsigned _limite): en_espera{0, 0}, uso(0), limite(_limite) {}

	void esperar()
	{
		unsigned uso_local = uso;

		m.lock();
		++en_espera[uso_local];
		m.unlock();

		if (en_espera[uso_local] == limite)
		{
			uso = 1 - uso;
			en_espera[uso_local] = 0;
		}
		else
		{
			while(en_espera[uso_local] > 0);
		}
	}

private:
	std::mutex m;
	volatile unsigned en_espera[2], uso;
	unsigned limite;
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
