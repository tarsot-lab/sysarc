//---------------------------------------------------------
// barrera_cv.cc
//---------------------------------------------------------

#include <unistd.h>            // alarm
#include <algorithm>           // for_each
#include <condition_variable>  // condition_variable
#include <iostream>            // cout endl
#include <mutex>               // mutex lock_guard unique_lock
#include <sstream>             // stringstream
#include <string>              // string
#include <thread>              // thread

//---------------------------------------------------------

const int N = 32;

//---------------------------------------------------------

class barrera_t
{
public:
	explicit barrera_t(unsigned _limite):
		en_espera{0, 0}, uso(0), limite(_limite) {}

	void esperar()
	{
		std::unique_lock<std::mutex> lock(m);
		unsigned uso_local = uso;
		if (++en_espera[uso_local] < limite)
		{
			restart.wait(lock, [&]{ return en_espera[uso_local] == 0; });
		}
		else
		{
			uso = 1 - uso;
			en_espera[uso_local] = 0;
			restart.notify_all();
		}
	}

private:
	volatile unsigned en_espera[2], uso;
	unsigned limite;
	std::mutex m;
	std::condition_variable restart;
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
