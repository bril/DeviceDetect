#ifndef _DEVICE_INFO_H
#define _DEVICE_INFO_H

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

#include "ConnectionInfo.h"

namespace DeviceDetectLibrary
{
    struct DeviceInfo
    {
    public:

        typedef std::wstring DeviceId;
       
        DeviceInfo();
        DeviceInfo(
            const DeviceId& id, 
            const std::wstring& name, 
            std::wstring deviceDisplayName,
            const ConnectionInfo& connInfo);
            
        ~DeviceInfo();

    public:

        inline const DeviceId& GetId() const
        {
            return id_;
        }

        inline const std::wstring& GetName() const 
        {
            return name_;
        }

        inline void SetName(const std::wstring& name)
        {
            name_ = name;
        }

        inline std::wstring GetDeviceDisplayName() const
        {
            return deviceDisplayName_;
        }

        inline void SetDeviceDisplayName(const std::wstring& deviceDisplayName)
        {
            deviceDisplayName_ = deviceDisplayName;
        }

        inline const ConnectionInfo& GetConnectionInfo() const 
        {
            return connInfo_;
        }

        inline void SetConnectionInfo(const ConnectionInfo& connInfo)
        {
            connInfo_ = connInfo;
        }

    private:

        DeviceId id_;
        std::wstring deviceDisplayName_; 
        std::wstring name_;
        ConnectionInfo connInfo_;

    };

    typedef std::map<DeviceInfo::DeviceId, DeviceInfo> Devices;
    typedef boost::shared_ptr<DeviceInfo> DeviceInfoPtr;


}

#endif // _DEVICE_INFO_H