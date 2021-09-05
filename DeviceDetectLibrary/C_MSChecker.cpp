#include "StdAfx.h"
#include "C_MSChecker.h"

using namespace DeviceDetectLibrary;
using namespace Disk::Types;
using namespace Disk::Functions;
using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;
using namespace DeviceDetectLibrary;


MSChecker::MSChecker( ICollector& collector, const DeviceInfo::DeviceId& devicePath )
    : collector_(collector)
    , devicePath_(devicePath)
    , thread_(ThreadFunction)
{

}

MSChecker::~MSChecker()
{
    Stop();
}

void MSChecker::Start()
{
    stopEvent_.Reset();
    thread_.Start(this);
}

void MSChecker::Stop()
{
    stopEvent_.Set();
    thread_.Wait(INFINITE);
}

void MSChecker::ThreadFunction( void* state )
{
    MSChecker* This = static_cast<MSChecker*>(state);
    This->ThreadFunction();
}

void MSChecker::ThreadFunction()
{
    int tries = 60;
    const int sleepTimeout = 1000;
    RemovableDeviceInfo_vt disks; 
    while ((tries--) > 0)
    {
        try
        {
            disks = SearchRemovalDisks();
            break;
        }
        catch (const std::exception& /*ex*/)
        {
            // EXCEPTION: Cannot found drives for (%S)", devicePath_.c_str());
            if (stopEvent_.Wait(sleepTimeout))
            {
                return;
            }
        }
    }
    
    for(RemovableDeviceInfo_vt_cit iter = disks.begin(); iter != disks.end(); ++iter )
    {
        if (devicePath_ == iter->wsPath)
        {
                ConnectionInfo info = {
                    TypeUnknown,
                    iter->wsFriendlyName,
                    iter->wsPath,
                    L"",
                    iter->wsPath};
                    collector_.Found(DeviceInfo(iter->wsPath, iter->wsFriendlyName, L"Mass Storage device", info));
        }
    }
}