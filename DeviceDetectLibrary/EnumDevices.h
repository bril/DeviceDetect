#ifndef _ENUM_DEVICES_H_
#define _ENUM_DEVICES_H_

#include <vector>
#include <string>
#include <Windows.h>
#include <dbt.h>

#include "ConnectionInfo.h"

namespace DeviceDetectLibrary
{
    
    ConnectionInfo_vt GetDevicesByGuid( GUID guid );      
    HDEVNOTIFY RegisterUSBNotify( const GUID& guid, HANDLE hWnd);
    void UnRegisterUSBNotify(HDEVNOTIFY hNotyfy);


}

#endif // _ENUM_DEVICES_H_