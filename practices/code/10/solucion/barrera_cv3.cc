//---------------------------------------------------------
// barrera_cv3.cc
//---------------------------------------------------------

#include <unistd.h>            // alarm
#include <condition_variable>  // condition_variable
#include <iostream>
#include <mutex>
#include <sstream>             // stringstream
#include <thread>

//---------------------------------------------------------

const int N = 32;

//---------------------------------------------------------

class barrera_t
{
public:
	barrera_t(unsigned _limite): 
		contador(_limite), generacion(0), limite(_limite) {}

	void esperar()
	{
		std::unique_lock<std::mutex> lock(mutex);
		unsigned copia = generacion;
		if (--contador == 0)
		{
			++generacion;
			contador = limite;
			bloqueadas.notify_all();
		}
		else
		{
//			while(copia == generacion)
//				bloqueadas.wait(lock);
			bloqueadas.wait(lock, [&]{ return copia != generacion; });
		}
	}

private:
	std::mutex mutex;
	std::condition_variable bloqueadas;
	unsigned contador, generacion, limite;
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
