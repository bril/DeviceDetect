#ifndef _TEST_DEVICE_WATCHER_OBSERVER_H_
#define _TEST_DEVICE_WATCHER_OBSERVER_H_

#include "IDeviceWatcherObserver.h"

class TestDeviceWatcherObserver : public DeviceDetectLibrary::IDeviceWatcherObserver
{
public:
    TestDeviceWatcherObserver();
    ~TestDeviceWatcherObserver();

public: // IDeviceWatcherObserver interface

    void AppearedDevice(const DeviceDetectLibrary::DeviceInfo& deviceInfo);
    void DisappearedDevice(const DeviceDetectLibrary::DeviceInfo& deviceInfo);

private:

    DeviceDetectLibrary::Devices devices_;

};

#endif // _TEST_DEVICE_WATCHER_OBSERVER_H_