#ifndef _WIN_MANUAL_RESET_EVENT_
#define _WIN_MANUAL_RESET_EVENT_

#include "Win_WaitObject.h"

namespace DeviceDetectLibrary
{
    class ManualResetEvent : public WaitObject
    {
    public:
        
        ManualResetEvent();

        void Set();
        void Reset();

    };
}

#endif //_WIN_MANUAL_RESET_EVENT_
