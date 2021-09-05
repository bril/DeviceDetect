#include "StdAfx.h"
#include "ResultEnumerator.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary;

ResultEnumerator::ResultEnumerator(ICollector& collector, std::wstring deviceDisplayName, const std::wstring& friendlyName) 
    : deviceID_(deviceDisplayName)
    , friendlyName_(friendlyName)
    , collector_(collector)
{
}

ResultEnumerator::~ResultEnumerator()
{
}

void ResultEnumerator::Collect(const DeviceInfo& deviceInfo)
{
    DeviceInfo info(deviceInfo);
    info.SetName(friendlyName_);
    info.SetDeviceDisplayName(deviceID_);
    collector_.Found(info);
}
