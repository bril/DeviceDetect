#ifndef _I_DEVICE_WATCHER_OBSERVER_H_
#define _I_DEVICE_WATCHER_OBSERVER_H_

#include "DeviceInfo.h"

namespace DeviceDetectLibrary
{

    struct IDeviceWatcherObserver
    {
    public:
        virtual void AppearedDevice(const DeviceInfo& deviceInfo) = 0;
        virtual void DisappearedDevice(const DeviceInfo& deviceInfo) = 0;
        virtual ~IDeviceWatcherObserver() {}
    };
}


#endif // _I_DEVICE_WATCHER_OBSERVER_H_