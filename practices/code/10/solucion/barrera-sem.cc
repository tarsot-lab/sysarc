//---------------------------------------------------------
// barrera-sem.cc: barrera con semáforos
//---------------------------------------------------------

#include <semaphore.h>  // sem_t
#include <unistd.h>     // alarm
#include <algorithm>    // for_each
#include <iostream>     // cout endl
#include <sstream>      // stringstream
#include <thread>       // thread

//---------------------------------------------------------

const int N = 32;

//---------------------------------------------------------

class barrera_t
{
public:
	explicit barrera_t(unsigned _limite):
		en_espera(0), uso(0), limite(_limite)
	{
		sem_init(&m, 0, 1);
		for (unsigned i = 0; i < 2; ++i)
			sem_init(&sem[i], 0, 0);
	}

	~barrera_t()
	{
		for (unsigned i = 0; i < 2; ++i)
			sem_destroy(&sem[i]);
	}

	void esperar()
	{
		unsigned uso_local = uso;
		sem_wait(&m); 
		if (++en_espera < limite)
		{
			sem_post(&m);
			sem_wait(&sem[uso_local]);
		}
		else
		{
			uso = 1 - uso;
			en_espera = 0;
			sem_post(&m);
			for (int i = 0; i < N - 1; ++i)
				sem_post(&sem[uso_local]);
		}
	}

private:
	volatile unsigned en_espera, uso;
	unsigned limite;
	sem_t m, sem[2];
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
