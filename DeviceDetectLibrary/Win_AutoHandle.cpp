#include "stdafx.h"
#include "Win_AutoHandle.h"


AutoHandle::AutoHandle(Handle handle) 
:handle_(handle)
{
    if (handle == INVALID_HANDLE_VALUE)
    {
        throw AutoHandleException("[AutoHandle]: Handle is invalid");
    }
}

AutoHandle::AutoHandle(Handle handle, Nothrow) 
:handle_(handle)
{
}

AutoHandle::~AutoHandle()
{
    Reset();
}

AutoHandle::operator Handle() const 
{
    return handle_;
}

Handle AutoHandle::GetHandle() const
{
    return handle_;
}

Handle AutoHandle::Release()
{
    Handle result = handle_;
    handle_ = INVALID_HANDLE_VALUE;
    return result;
}

void AutoHandle::Reset()
{
    Reset(INVALID_HANDLE_VALUE);
}

void AutoHandle::Reset(Handle newHandle/* = INVALID_HANDLE_VALUE*/)
{
    if (handle_ != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(handle_);
    }
    handle_ = newHandle;
}
