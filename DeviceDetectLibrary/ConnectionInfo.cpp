#include "StdAfx.h"
#include "ConnectionInfo.h"

bool DeviceDetectLibrary::operator<(const DeviceDetectLibrary::ConnectionInfo & left, const DeviceDetectLibrary::ConnectionInfo & right)
{
    return (left.FriendlyName < right.FriendlyName);
}

bool DeviceDetectLibrary::operator==(const DeviceDetectLibrary::ConnectionInfo & left, const DeviceDetectLibrary::ConnectionInfo & right)
{
    return (left.FriendlyName == right.FriendlyName) && (left.PhysicalDeviceName == right.PhysicalDeviceName);
}
