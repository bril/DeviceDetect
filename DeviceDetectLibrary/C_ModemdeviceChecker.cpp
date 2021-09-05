#include "StdAfx.h"

#include "C_ModemdeviceChecker.h"

using namespace DeviceDetectLibrary;

ModemDeviceChecker::ModemDeviceChecker(ICollector& collector, const std::vector<std::wstring>& patternNames )
: collector_(collector)
{
    _patternNames = patternNames;
}

ModemDeviceChecker::~ModemDeviceChecker()
{
   
}


bool ModemDeviceChecker::Check( const DeviceInfo& deviceInfo )
{
    int searchCount = 0;
    std::wstring name = deviceInfo.GetName();
    for (std::vector<std::wstring>::const_iterator iter = _patternNames.begin(); iter != _patternNames.end(); ++iter)
    {

        if (name.end() != std::search(
            name.begin(), 
            name.end(), 
            (*iter).begin(),
            (*iter).end()))
        {
            searchCount++;
        }
    }

    if (searchCount == _patternNames.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}


void ModemDeviceChecker::Collect( const DeviceInfo& deviceInfo, const std::wstring& pluginID)
{
    if (Check(deviceInfo))
    {
        DeviceInfo info(deviceInfo);
        info.SetDeviceDisplayName(pluginID);
        collector_.Found(info);
    }
}
