#include "StdAfx.h"
#include "C_MSEnumerator.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;
using namespace Disk::Types;
using namespace Disk::Functions;

MSEnumerator::MSEnumerator( ICollector& collector )
    : collector_(collector)
{
}

MSEnumerator::~MSEnumerator()
{
}

void MSEnumerator::TryThis( const DeviceInfo::DeviceId& devicePath )
{
    AutoCriticalSection lock(sec_);
    if (checkers_.count(devicePath) == 0)
    {
        checkers_[devicePath] = MSCheckerPtr(new MSChecker(collector_, devicePath));
    }
    // Cancel previous process
    checkers_[devicePath]->Stop();
    // Begin new process
    checkers_[devicePath]->Start();

}

void MSEnumerator::RemoveThis( const DeviceInfo::DeviceId& devicePath )
{
    AutoCriticalSection lock(sec_);
    if (checkers_.count(devicePath) != 0)
    {
        checkers_.erase(devicePath);
    }
}

void MSEnumerator::Collect(const DeviceInfo& deviceInfo)
{
    try
    {
        RemovableDeviceInfo_vt disks = SearchRemovalDisks();
        for(RemovableDeviceInfo_vt_cit iter = disks.begin(); iter != disks.end(); ++iter )
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
    catch (const std::exception& /*ex*/)
    {
        // EXCEPTION: Cannot do SearchRemovalDisks(). Error = %s", ex.what());        
    }    

}
