#include "StdAfx.h"
#include "DeviceWatcher.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;

DeviceWatcher::DeviceWatcher(IDeviceWatcherObserver* observer) 
: Base(observer)
{    
}

DeviceWatcher::~DeviceWatcher()
{
    Stop();
}

void DeviceWatcher::InterfaceArrival( const GUID& guid )
{
    usbEnumerator_->TryThis(guid);
}

void DeviceWatcher::InterfaceRemoved( const DeviceInfo::DeviceId& devId )
{
    Lost(devId);
}

void DeviceWatcher::VolumeArrival( const DeviceInfo::DeviceId& devId )
{

    if (NULL != msEnumerator_)
    {
        msEnumerator_->TryThis(devId);
    }
}

void DeviceWatcher::VolumeRemoved( const DeviceInfo::DeviceId& devId )
{

    if (NULL != msEnumerator_)
    {
        msEnumerator_->RemoveThis(devId);
        Lost(devId);
    }
}

void DeviceWatcher::StartEnumerators()
{   

    NotifyWindow& window = CreateNotifyWindow();

    // Create Enumerators
    usbEnumerator_.reset(new UsbEnumerator(window, *this));
    usbEnumerator_->Collect(DeviceInfo());

    msEnumerator_.reset(new MSEnumerator(*this));
    msEnumerator_->Collect(DeviceInfo());
  
}

void DeviceWatcher::StopEnumerators()
{
    
    RemoveNotifyWindow();

    // Remove Enumerators
    usbEnumerator_.reset();
    msEnumerator_.reset();
}