#ifndef _DEVICE_WATCHER_H_
#define _DEVICE_WATCHER_H_

#include "ADeviceWatcher.h"
#include "IDeviceChanged.h"
#include "DeviceInfo.h"

#include "C_UsbEnumerator.h"
#include "C_MSEnumerator.h"

namespace DeviceDetectLibrary
{
    struct IDeviceWatcherObserver;

    class DeviceWatcher : public ADeviceWatcher
    {
        typedef ADeviceWatcher Base;
    public:
        DeviceWatcher(IDeviceWatcherObserver* observer);
        ~DeviceWatcher();

    protected:
        void StartEnumerators();
        void StopEnumerators();

    private: // IDeviceChanged
        void InterfaceArrival(const GUID& guid);
        void InterfaceRemoved(const DeviceInfo::DeviceId& devId);
        void VolumeArrival(const DeviceInfo::DeviceId& devId);
        void VolumeRemoved(const DeviceInfo::DeviceId& devId);

    private:
        Connection::UsbEnumeratorPtr usbEnumerator_;
        Connection::MSEnumeratorPtr msEnumerator_;
    };
}

#endif // _DEVICE_WATCHER_H_