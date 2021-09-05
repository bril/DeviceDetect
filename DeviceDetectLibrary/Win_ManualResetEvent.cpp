#include "StdAfx.h"
#include "Win_ManualResetEvent.h"

using namespace DeviceDetectLibrary;

ManualResetEvent::ManualResetEvent()
    : WaitObject(::CreateEvent(0, true, false, 0))
{
}

void ManualResetEvent::Set()
{
    ::SetEvent(*this);
}

void ManualResetEvent::Reset()
{
    ::ResetEvent(*this);
}