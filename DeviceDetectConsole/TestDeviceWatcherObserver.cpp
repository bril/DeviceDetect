#include "StdAfx.h"
#include "TestDeviceWatcherObserver.h"

using namespace DeviceDetectLibrary;

TestDeviceWatcherObserver::TestDeviceWatcherObserver()
{
}

TestDeviceWatcherObserver::~TestDeviceWatcherObserver()
{
}

void TestDeviceWatcherObserver::AppearedDevice( const DeviceInfo& deviceInfo )
{
    LogMessage("Appeared new device: %S", deviceInfo.GetName().c_str());
    ConnectionInfo connectionInfo = deviceInfo.GetConnectionInfo();

    LogMessage("    Device description: %S", connectionInfo.DeviceDescription.c_str());
    LogMessage("    Device path: %S", deviceInfo.GetConnectionInfo().DevicePath.c_str());
    LogMessage("    Friendly name: %S", deviceInfo.GetConnectionInfo().FriendlyName.c_str());
    LogMessage("    Hardware ID: %S", deviceInfo.GetConnectionInfo().HardwareID.c_str());
    LogMessage("    Physical device name: %S", deviceInfo.GetConnectionInfo().PhysicalDeviceName.c_str());
    devices_[deviceInfo.GetId()] = deviceInfo;
}

void TestDeviceWatcherObserver::DisappearedDevice( const DeviceInfo& deviceInfo)
{
    LogMessage("Disappeared device: %S", devices_[deviceInfo.GetId()].GetName().c_str());
    devices_.erase(deviceInfo.GetId());
}