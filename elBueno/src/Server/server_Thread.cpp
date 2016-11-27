#include "server_Thread.h"

Thread::Thread() {
}

void Thread::start() {
	thread = std::thread(&Thread::run, this);
}

void Thread::join() {
	thread.join();
}

Thread::~Thread() {
}

Thread::Thread(Thread&& other) {
	this->thread = std::move(other.thread);
}
