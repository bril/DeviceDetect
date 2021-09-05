#include "StdAfx.h"
#include "Win_WaitObject.h"

using namespace DeviceDetectLibrary;

WaitObject::WaitObject( Handle handle )
    : AutoHandle(handle)
{
}

WaitObject::WaitObject(Handle handle, AutoHandle::Nothrow nothrow)
    : AutoHandle(handle, nothrow)
{
}

bool WaitObject::Wait(unsigned int milliseconds)
{
    if ((HANDLE)(*this) == INVALID_HANDLE_VALUE)
    {
        return true;    
    }

    int result = ::WaitForSingleObject(*this, milliseconds);
    if (result == WAIT_OBJECT_0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

