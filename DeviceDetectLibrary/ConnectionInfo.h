#ifndef _CONNECTION_INFO_H_
#define _CONNECTION_INFO_H_

#include <string>
#include <vector>

namespace DeviceDetectLibrary
{
    enum DeviceType
    {
        TypeUnknown,
        TypeSerial,
        TypeUsb,
        TypePureUsb
    };

    struct ConnectionInfo
    {
        DeviceType   Type;
        std::wstring FriendlyName;
        std::wstring DevicePath;
        std::wstring HardwareID;
        std::wstring PhysicalDeviceName;
        std::wstring DeviceDescription;
    };


    bool operator==(const ConnectionInfo & left, const ConnectionInfo & right);
    bool operator<(const ConnectionInfo & left, const ConnectionInfo & right);

    typedef std::vector<ConnectionInfo>  ConnectionInfo_vt;
    
}

#endif // _CONNECTION_INFO_H_