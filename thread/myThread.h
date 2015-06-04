
#ifndef MY_THREAD_HEADER
#define MY_THREAD_HEADER

#include <pthread.h>
#include <map>
#include <string>

/*
 * Обгортка для класу Thread і супутніх класів.
 */
namespace concurrent {

        class Lockable {
            public:
                virtual bool tryLock() = 0;
                virtual void unlock() = 0;
                virtual void lock() = 0;
                virtual ~Lockable() { } 
        };

        class LockGuard {
            public:
                LockGuard(Lockable& whatToLock);
                ~LockGuard();
                
                LockGuard() = delete;
                LockGuard(const LockGuard& ) = delete;
                LockGuard operator=(const LockGuard& ) = delete;
            private:
                Lockable* locker;                
        };
        
        class MyMutex : public Lockable {
            public:
                MyMutex(void);
                virtual ~MyMutex(void);
                virtual bool tryLock();
                virtual void unlock();
                virtual void lock();
                
                MyMutex(const MyMutex& ) = delete; 
                MyMutex& operator=(const MyMutex& ) = delete;
            private:
                pthread_mutex_t Pmutex;
        };

        class Thread {
            public:
                typedef void* (*threadFuncT)(void*);
                
                Thread(threadFuncT func, void* param = nullptr, std::string name = std::string(""));

                void join();
                void deatch();
                unsigned long getId();
                void setName (std::string name);
                static std::string getCurrentThreadName();
                
                Thread() = delete;
            private:

                static std::map<unsigned long,std::string> threadsIdName;
                
                pthread_t innerThread;
        };

}


#endif
