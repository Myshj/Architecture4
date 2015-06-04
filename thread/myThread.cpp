
#include "myThread.h"
#include <sstream>

/*
 * Обгортка для класу Thread і супутніх класів.
 */
namespace concurrent {


    LockGuard::LockGuard(Lockable& whatToLock) {
        locker = &whatToLock;
        locker->lock();
    }

    LockGuard::~LockGuard() {
        locker->unlock();
    }

    MyMutex::MyMutex(void) {
        pthread_mutex_init(&Pmutex, nullptr);
    }

    MyMutex::~MyMutex(void) {
        pthread_mutex_destroy(&Pmutex);
    }

    bool MyMutex::tryLock() {
        return pthread_mutex_trylock(&Pmutex) == 0;
    }

    void MyMutex::unlock() {
        pthread_mutex_unlock(&Pmutex);
    }

    void MyMutex::lock() {
        pthread_mutex_lock(&Pmutex);
    }

    Thread::Thread(threadFuncT func, void* param, std::string name) {
        pthread_create(&innerThread, nullptr, func, param);
        if (name.length() != 0){
            threadsIdName[(unsigned long) innerThread] = name;
        } else {
            std::ostringstream autoThreadName("thread", std::ios_base::app);
            autoThreadName << (unsigned long) innerThread;
            threadsIdName[(unsigned long) innerThread] = autoThreadName.str();
        }
    }

    void Thread::join() {
        pthread_join(innerThread, nullptr);
    }

    void Thread::deatch() {
        pthread_detach(innerThread);
    }

    unsigned long Thread::getId() {
        return (unsigned long) innerThread;
    }

    void Thread::setName (std::string name) {
        threadsIdName[this->getId()] = name;
    }

    std::string Thread::getCurrentThreadName() {
        try {
            return threadsIdName.at((unsigned long) pthread_self());
        } catch (std::out_of_range) {
            return std::string("Undefined thread.");
        }
    }


    std::map<unsigned long, std::string> concurrent::Thread::threadsIdName;
}

