#ifndef Thread_h
#define Thread_h

#include <thread>

class Thread {
private:
	std::thread thread;

public:
	Thread();

	void start();

	void join();

	virtual void run()=0;
	
	virtual ~Thread();

private:
	Thread(const Thread&) = delete;

	Thread& operator=(const Thread&) = delete;

	Thread(Thread&& other);

	Thread& operator=(Thread&& other) {
		this->thread = std::move(other.thread);
		return *this;
	}
};

#endif
