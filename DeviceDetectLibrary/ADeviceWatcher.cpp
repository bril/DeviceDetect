#include "StdAfx.h"
#include "ADeviceWatcher.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary;

ADeviceWatcher::ADeviceWatcher(IDeviceWatcherObserver* observer)
: observer_(observer)
{
}

ADeviceWatcher::~ADeviceWatcher()
{
}

void ADeviceWatcher::Start()
{
    StartEnumerators();
    tempDevices_.clear();
}

void ADeviceWatcher::Stop()
{
    StopEnumerators();
    actualDevices_.clear();
}

NotifyWindow& ADeviceWatcher::CreateNotifyWindow()
{
    if (0 == window_.get())
    {
        // Create NotifyWindow
        window_.reset(new NotifyWindow(*this));        
    }
    return *window_;
}

void ADeviceWatcher::RemoveNotifyWindow()
{
    // Remove NotifyWindow");
    window_.reset(0);
}

void ADeviceWatcher::Found(const DeviceInfo& deviceInfo)
{
    AutoCriticalSection lock(section_);
    tempDevices_[deviceInfo.GetId()] = deviceInfo;
    if (actualDevices_.count(deviceInfo.GetId()) == 0)
    {
        actualDevices_[deviceInfo.GetId()] = deviceInfo;
        observer_->AppearedDevice(deviceInfo);
    }
}

void ADeviceWatcher::Lost(const DeviceInfo::DeviceId& deviceId)
{
    AutoCriticalSection lock(section_);
    if (actualDevices_.count(deviceId) != 0)
    {
        observer_->DisappearedDevice(actualDevices_[deviceId]);
        actualDevices_.erase(deviceId);
    }
}
