#include "StdAfx.h"
#include "Win_Thread.h"

using namespace DeviceDetectLibrary;

Thread::Thread( ThreadFunction routine )
    : WaitObject(INVALID_HANDLE_VALUE, AutoHandle::Nothrow())
    , routine_(routine)
    , currentState_(NULL)
{
}

Thread::~Thread()
{
    Wait(INFINITE);    
}

void Thread::Start( void * state )
{
    if ((HANDLE)(*this) != INVALID_HANDLE_VALUE)
    {
        throw AutoHandleException("Thread is already started"); 
    }
    currentState_ = state;
    Reset((HANDLE)_beginthreadex(NULL, 0, Thread::Routine, this, 0, NULL));
    if (((HANDLE)(*this) == INVALID_HANDLE_VALUE) || ((HANDLE)(*this) == 0))
    {
        Reset();
        throw AutoHandleException("Cannot create thread");
    }
}

unsigned int __stdcall Thread::Routine( void * state )
{
    Thread* pThis = static_cast<Thread*>(state);
    pThis->routine_(pThis->currentState_);
    return 0;
}

bool Thread::Wait( unsigned int milliseconds )
{
    bool result = WaitObject::Wait(milliseconds);
    if (result)
    {
        Reset();
    }
    return result;
}