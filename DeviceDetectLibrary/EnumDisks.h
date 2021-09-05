#ifndef _ENUM_DISKS_H_
#define _ENUM_DISKS_H_

#include <string>
#include <vector>
#include <Windows.h>
#include <WinIoCtl.h>

namespace Disk
{
    namespace Types
    {
        typedef DISK_GEOMETRY DiskGeometry;

        enum RemovableDeviceType
        {
            rdtUnknown  = 0,
            rdtFlash    = 1,
            rdtHardDisk = 2,
        };
        
        struct RemovableDeviceInfo
        {
            std::wstring                wsDevInterfaceVolume; // REG_SZ string that contains the interface volume name of a device
            std::wstring                wsDeviceDesc        ; // REG_SZ string containing the description of a device
            std::wstring                wsEnumeratorName    ; // REG_SZ string string containing the name of the device's enumerator
            std::wstring                wsCompatibleIDs     ; // REG_MULTI_SZ string string containing the list of compatible IDs for a device
            std::wstring                wsHardwareID        ; // REG_MULTI_SZ string string containing the list of hardware IDs for a device
            std::wstring                wsMFG               ; // REG_SZ string that contains the name of the device manufacturer
            std::wstring                wsFriendlyName      ; // REG_SZ string that contains the friendly name of a device
            std::wstring                wsPath              ; // REG_SZ string that contains the disk letter of a device
            std::vector<unsigned int>   vHarddiskIndexes    ; // vector hard disk indexes
            DiskGeometry                diskGeometry        ; // structure describes the geometry of disk devices and media
            RemovableDeviceType         deviceType          ;
        };
        typedef std::vector<RemovableDeviceInfo>                 RemovableDeviceInfo_vt;
        typedef std::vector<RemovableDeviceInfo>::iterator       RemovableDeviceInfo_vt_it;
        typedef std::vector<RemovableDeviceInfo>::const_iterator RemovableDeviceInfo_vt_cit;
    }

    namespace Functions
    {        
        Types::RemovableDeviceInfo_vt SearchRemovalDisks();
    }
}

#endif // _ENUM_DISKS_H_