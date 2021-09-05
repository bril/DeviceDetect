#ifndef _WIN_THREAD_H_
#define _WIN_THREAD_H_

#include "Win_WaitObject.h"

namespace DeviceDetectLibrary
{
    class Thread : public WaitObject
    {
    public:

        typedef void (*ThreadFunction)(void * state);

        Thread(ThreadFunction routine);
        ~Thread();

    public:

        void Start(void * state);
        bool Wait(unsigned int milliseconds);

    private:

        static unsigned int __stdcall Routine(void * state);

    private:

        ThreadFunction routine_;
        void * currentState_;

    };
}

#endif // _WIN_THREAD_H_