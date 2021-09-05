#include "StdAfx.h"
#include "EnumDevices.h"

using namespace DeviceDetectLibrary;

DeviceType GetDeviceType( const wchar_t* sPath );
std::wstring GetDeviceRegistryProperty( HDEVINFO & hDevInfo, PSP_DEVINFO_DATA pdevInfoData, DWORD dwProperty );
DeviceType GetDeviceType( const wchar_t* sPath )
{
    COMMPROP comProp;
    DeviceType type = TypeUnknown;

    HANDLE hFile = CreateFile(
        sPath,
        FILE_GENERIC_READ | FILE_GENERIC_WRITE, 
        0, 
        NULL, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL);

    if ( (INVALID_HANDLE_VALUE != hFile) && (TRUE == GetCommProperties(hFile, &comProp)) )
    {
        switch( comProp.dwProvSubType )
        {
        case PST_MODEM:
            {
                type = TypeUsb;
            } break;
        case PST_RS232:
            {
                type = TypeSerial;
            } break;
        default:
            {
                type= TypeUnknown;
            } break;
        }
    }

    CloseHandle(hFile);

    return type;
}

std::wstring GetDeviceRegistryProperty( HDEVINFO & hDevInfo, PSP_DEVINFO_DATA pdevInfoData, DWORD dwProperty )
{
    DWORD dwSize = 0;
    DWORD dwDataType = 0;

    if ( NULL == pdevInfoData )
    {
        // Wrong PSP_DEVINFO_DATA parameter");
        throw std::runtime_error("Wrong PSP_DEVINFO_DATA parameter");
    }

    if ( FALSE == ::SetupDiGetDeviceRegistryProperty(hDevInfo, pdevInfoData, dwProperty, &dwDataType, NULL, 0, &dwSize) )
    {
        DWORD error = ::GetLastError();
        if ( ERROR_INVALID_DATA == error )
        {
            return std::wstring();
        }
        if ( ERROR_INSUFFICIENT_BUFFER != error )
        {
            // Can't get Device Registry Property");
            throw std::runtime_error("Can't get Device Registry Property");
        }
    }

    std::vector<TCHAR> sResult(dwSize, 0x00);
    if ( sResult.empty() )
    {
        // Can't get Device Registry Property is empty");
        throw std::runtime_error("Can't get Device Registry Property is empty");
    }

    if ( FALSE == ::SetupDiGetDeviceRegistryProperty(hDevInfo, pdevInfoData, dwProperty, &dwDataType, (PBYTE) &sResult.front(), (DWORD)sResult.size(), &dwSize) )
    {
        // Can't get Device Registry Property");
        throw std::runtime_error("Can't get Device Registry Property");
    }

    sResult.resize(dwSize);
    if ( sResult.empty() )
    {
        // Device Registry Property is empty");
        throw std::runtime_error("Device Registry Property is empty");
    }

    return std::wstring(&sResult.front());
}


template <class TYPECHECKER>
ConnectionInfo_vt GetDevicesByGuidCheck( GUID* guid, const std::wstring & sPattern, TYPECHECKER checker)
{
    ConnectionInfo_vt result;
    HDEVINFO hDevInfo = ::SetupDiGetClassDevs(guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

    if ( INVALID_HANDLE_VALUE == hDevInfo ) 
    {
        // Can't Find Class Devises");
        return result;
    }


    SP_DEVICE_INTERFACE_DATA devInterfaceData;
    ZeroMemory(&devInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
    devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    for ( DWORD dwCount = 0; ::SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guid, dwCount, &devInterfaceData); ++dwCount )
    {
        try
        {
            ConnectionInfo info = checker(hDevInfo, &devInterfaceData, sPattern);

            result.push_back(info);
        }
        catch ( const std::exception & /*ex*/ )
        {
        }
    }

    ::SetupDiDestroyDeviceInfoList( hDevInfo );

    return result;
}

ConnectionInfo PureChecker(HDEVINFO hDevInfo, SP_DEVICE_INTERFACE_DATA* devInterfaceData, const std::wstring & sPattern)
{
    TCHAR tcBuffer[0x400] = {0x00};
    PSP_DEVICE_INTERFACE_DETAIL_DATA pDevInfoDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA) tcBuffer;
    pDevInfoDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    SP_DEVINFO_DATA devInfoData;
    ZeroMemory(&devInfoData, sizeof(SP_DEVINFO_DATA));
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    DWORD dwReqSize = 0;
    if ( TRUE == ::SetupDiGetDeviceInterfaceDetail(hDevInfo, devInterfaceData, pDevInfoDetail, sizeof(tcBuffer)/sizeof(TCHAR), &dwReqSize, &devInfoData) )
    {
        ConnectionInfo info;
        info.Type = TypeUsb;
        info.FriendlyName = GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME); 
        info.DeviceDescription = GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_DEVICEDESC);
        info.HardwareID = GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_HARDWAREID);
        info.DevicePath = pDevInfoDetail->DevicePath;

        return info;
    }
    throw std::runtime_error("Can't SetupDiGetInterfaceDeviceDetail");
}

ConnectionInfo_vt DeviceDetectLibrary::GetDevicesByGuid( GUID guid )
{
    return GetDevicesByGuidCheck(&guid, L"", &PureChecker);
}

HDEVNOTIFY DeviceDetectLibrary::RegisterUSBNotify( const GUID& guid, Handle hWnd )
{
    DEV_BROADCAST_DEVICEINTERFACE notificationFilter;

    ZeroMemory( &notificationFilter, sizeof(notificationFilter) );
    notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    notificationFilter.dbcc_classguid = guid;

    HDEVNOTIFY hDevNotify = RegisterDeviceNotification( hWnd, 
        &notificationFilter,
        DEVICE_NOTIFY_WINDOW_HANDLE 
        );

    if(hDevNotify == NULL) 
    {
        int error = GetLastError();
        throw std::runtime_error("Can not register device notify");
    }

    return hDevNotify;
}

void DeviceDetectLibrary::UnRegisterUSBNotify( HDEVNOTIFY hNotyfy )
{
    if (FALSE == UnregisterDeviceNotification(hNotyfy))
    {
        throw std::runtime_error("Can not unregister device notify");
    }
}

