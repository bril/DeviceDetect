#ifndef _IDEVICECHANGED_H_
#define _IDEVICECHANGED_H_

#include "DeviceInfo.h"

namespace DeviceDetectLibrary
{
    struct IDeviceChanged
    {
    public:

        virtual void InterfaceArrival(const GUID& guid) = 0;
        virtual void InterfaceRemoved(const DeviceInfo::DeviceId& devId) = 0;
        virtual void VolumeArrival(const DeviceInfo::DeviceId& devId) = 0;
        virtual void VolumeRemoved(const DeviceInfo::DeviceId& devId) = 0;
        virtual ~IDeviceChanged() {}

    };

}

#endif // _IDEVICECHANGED_H_
