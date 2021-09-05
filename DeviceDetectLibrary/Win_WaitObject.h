#ifndef _WIN_WAITOBJECT_H_
#define _WIN_WAITOBJECT_H_

#include "Win_AutoHandle.h"
#include "Win32Types.h"

namespace DeviceDetectLibrary
{
    class WaitObject : public AutoHandle
    {
    public:

        WaitObject(Handle handle);
        WaitObject(Handle handle, AutoHandle::Nothrow);
        virtual bool Wait(unsigned int milliseconds);

    };
}

#endif // _WIN_WAITOBJECT_H_