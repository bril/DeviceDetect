#include "StdAfx.h"
#include "EnumDisks.h"

using namespace Disk;
using namespace Disk::Types;

static RemovableDeviceType GetDeviceType( PSP_DEVINFO_DATA pDevInfoData );
static std::wstring GetDevicePath( PSP_DEVINFO_DATA pDevInfoData );
static std::wstring GetDeviceRegistryProperty( HDEVINFO hDevInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty );

static BOOL IsDeviceNumberEq(const std::wstring& sDevicePath, DWORD dwDeviceNumber);
static BOOL GetDrivesDevInst(DWORD dwDeviceNumber, HDEVINFO hDevInfo, DWORD dwIndex, PDEVINST devInst);

static DEVINST FindDrivesDevInstByPath(const std::wstring& wsPath);
static DEVINST FindDrivesDevInstByDeviceNumber(DWORD dwDeviceNumber);

static bool BuildPaths(const std::wstring& sPath, std::wstring& sDevice, std::wstring& sVolumeAccess);
static DWORD GetDeviceNumber(const std::wstring & sVolumeAccess);

static std::vector<unsigned int> GetHarddiskIndexes(const std::wstring& wsPath);
static DiskGeometry GetDeviceGeometry(const std::wstring& wsDevInterfaceVolume);
static std::wstring GetDevInterfaceVolume(PSP_DEVINFO_DATA pdevInfoData);

std::wstring GetDevicePath(PSP_DEVINFO_DATA pDevInfoData)
{
    if ( NULL == pDevInfoData )
    {
        // Wrong PSP_DEVINFO_DATA parameter
        throw std::runtime_error("Wrong PSP_DEVINFO_DATA parameter");
    }

    std::wstring wsPath(L"X:\\");
    for (wchar_t wLeter = L'A'; wLeter <= L'Z'; ++wLeter)
    {
        wsPath[0] = wLeter;
        if ( pDevInfoData->DevInst == FindDrivesDevInstByPath(wsPath) )
        {
            return wsPath;
        }
    }

    // Can't find device path
    throw std::runtime_error("Can't find device path");
}

std::wstring GetDeviceRegistryProperty(HDEVINFO hDevInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty)
{
    DWORD dwSize = 0;
    DWORD dwDataType = 0;

    if ( NULL == pDevInfoData )
    {
        // Wrong PSP_DEVINFO_DATA parameter
        throw std::runtime_error("Wrong PSP_DEVINFO_DATA parameter");
    }

    if ( FALSE == ::SetupDiGetDeviceRegistryPropertyW(hDevInfo, pDevInfoData, dwProperty, &dwDataType, NULL, 0, &dwSize) )
    {
        if ( ERROR_INSUFFICIENT_BUFFER != ::GetLastError() )
        {
            if ( SPDRP_FRIENDLYNAME == dwProperty )
            {
                return L"Removable Disk";
            }

            // Can't get Device Registry Property
            throw std::runtime_error("Can't get Device Registry Property");
        }
    }

    std::vector<wchar_t> sResult(dwSize + sizeof(wchar_t), 0x00);
    if ( FALSE == ::SetupDiGetDeviceRegistryPropertyW(hDevInfo, pDevInfoData, dwProperty, &dwDataType, (PBYTE) &sResult.front(), (DWORD)sResult.size(), &dwSize) )
    {
        // Can't get Device Registry Property
        throw std::runtime_error("Can't get Device Registry Property");
    }

    return std::wstring(&sResult.at(0));
}

BOOL IsDeviceNumberEq(const std::wstring& sDevicePath, DWORD dwDeviceNumber)
{
    BOOL bResult = FALSE;

    HANDLE hDrive = ::CreateFileW(sDevicePath.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
    if ( INVALID_HANDLE_VALUE == hDrive )
    {
        return bResult;
    }

    DWORD dwRead = 0;
    STORAGE_DEVICE_NUMBER sdn;
    ZeroMemory(&sdn, sizeof(sdn));

    if ( ::DeviceIoControl(hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwRead, NULL) )
    {
        bResult = ( dwDeviceNumber == sdn.DeviceNumber );
    }

    ::CloseHandle(hDrive);

    return bResult;
}

BOOL GetDrivesDevInst(DWORD dwDeviceNumber, HDEVINFO hDevInfo, DWORD dwIndex, PDEVINST devInst)
{
    if ( (INVALID_HANDLE_VALUE == hDevInfo) || (0 == devInst) )
    {
        return FALSE;
    }

    *devInst = 0;
    DWORD dwSize = 0;
    BYTE bBuffer[0x1000] = {0};

    SP_DEVICE_INTERFACE_DATA spDevInterfaceData;
    ZeroMemory(&spDevInterfaceData, sizeof(spDevInterfaceData));
    spDevInterfaceData.cbSize = sizeof(spDevInterfaceData);

    if ( FALSE == ::SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &GUID_DEVINTERFACE_DISK, dwIndex, &spDevInterfaceData) )
    {
        return FALSE;
    }

    ::SetupDiGetDeviceInterfaceDetail(hDevInfo, &spDevInterfaceData, NULL, 0, &dwSize, NULL);
    if ( (0 == dwSize) || (sizeof(bBuffer) < dwSize) )
    {
        return FALSE;
    }

    SP_DEVINFO_DATA spDevInfoData;
    ZeroMemory(&spDevInfoData, sizeof(spDevInfoData));
    spDevInfoData.cbSize = sizeof(spDevInfoData);

    PSP_DEVICE_INTERFACE_DETAIL_DATA pspDevInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) bBuffer;
    pspDevInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    if ( FALSE == ::SetupDiGetDeviceInterfaceDetail( hDevInfo, &spDevInterfaceData, pspDevInterfaceDetailData, dwSize, &dwSize, &spDevInfoData) )
    {
        return TRUE;
    }

    if ( IsDeviceNumberEq(pspDevInterfaceDetailData->DevicePath, dwDeviceNumber) )
    {
        *devInst = spDevInfoData.DevInst;
    }

    return TRUE;
}

DEVINST FindDrivesDevInstByPath(const std::wstring& wsPath)
{
    std::wstring sDevice;
    std::wstring sVolumeAccess;

    if ( !BuildPaths(wsPath, sDevice, sVolumeAccess) )
    {
        return 0;
    }

    DWORD dwDeviceNumber = GetDeviceNumber(sVolumeAccess);
    if ( -1 == dwDeviceNumber )
    {
        return 0;
    }

    wchar_t szDosDeviceName[MAX_PATH] = {0};
    if ( FALSE == ::QueryDosDeviceW(sDevice.c_str(), szDosDeviceName, MAX_PATH) )
    {
        return 0;
    }

    const std::wstring patern = L"Harddisk";
    const std::wstring dosDeviceName = szDosDeviceName;
    if ( dosDeviceName.end() == std::search(dosDeviceName.begin(), dosDeviceName.end(), patern.begin(), patern.end()) )
    {
        return 0;
    }

    return FindDrivesDevInstByDeviceNumber(dwDeviceNumber);
}

DEVINST FindDrivesDevInstByDeviceNumber(DWORD dwDeviceNumber)
{
    CONST DEVINST devInstEmpty = 0;

    HDEVINFO hDevInfo = ::SetupDiGetClassDevs(
        &GUID_DEVINTERFACE_DISK, 
        NULL, 
        NULL, 
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    DEVINST devInst = 0;
    BOOL bResult = TRUE;
    for ( DWORD dwIndex = 0; ((dwIndex < 0xFFFFFFFF) && (devInstEmpty == devInst) && bResult); ++dwIndex )
    {
        bResult = GetDrivesDevInst(dwDeviceNumber, hDevInfo, dwIndex, &devInst);
    }

    ::SetupDiDestroyDeviceInfoList(hDevInfo);

    return devInst;
}

bool BuildPaths(const std::wstring& sPath, std::wstring& sDevice, std::wstring& sVolumeAccess)
{
    if ( sPath.empty() )
    {
        return false;
    }

    wchar_t wcDriveLetter = sPath[0];
    wcDriveLetter &= ~0x20;

    if ( (wcDriveLetter < L'A') || (wcDriveLetter > L'Z') ) 
    {
        return false;
    }

    sDevice.assign(L"X:");
    sVolumeAccess.assign(L"\\\\.\\X:");

    sDevice[0] = wcDriveLetter;
    sVolumeAccess[4] = wcDriveLetter;

    return true;
}

DWORD GetDeviceNumber(const std::wstring& sVolumeAccess)
{
    DWORD dwDeviceNumber = (DWORD)(-1);

    HANDLE hVolume = ::CreateFileW(sVolumeAccess.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
    if (hVolume == INVALID_HANDLE_VALUE) 
    {
        return dwDeviceNumber;
    }

    DWORD dwBytesReturned = 0;
    STORAGE_DEVICE_NUMBER sdn;

    if ( ::DeviceIoControl(hVolume, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL) )
    {
        dwDeviceNumber = sdn.DeviceNumber;
    }

    ::CloseHandle(hVolume);

    return dwDeviceNumber;
}

std::vector<unsigned int> GetHarddiskIndexes(const std::wstring& wsPath)
{
    AutoHandle handle( ::CreateFileW(
        wsPath.c_str(), 
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
        NULL) );

    VOLUME_DISK_EXTENTS vde;
    ::ZeroMemory(&vde, sizeof(VOLUME_DISK_EXTENTS));

    DWORD dwBytes = 0;
    BOOL bRes = ::DeviceIoControl(
        handle, 
        IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, 
        NULL, 
        0, 
        &vde, 
        sizeof(vde), 
        &dwBytes, 
        NULL);

    DWORD dwError = ::GetLastError();
    if ( ((FALSE == bRes) && (ERROR_MORE_DATA != dwError)) || (0 == vde.NumberOfDiskExtents) )
    {
        return std::vector<unsigned int>();
    }

    dwBytes = sizeof(VOLUME_DISK_EXTENTS) + (vde.NumberOfDiskExtents - 1) * sizeof(DISK_EXTENT);
    std::vector<BYTE> tmp_buf(dwBytes, 0);
        
    PVOLUME_DISK_EXTENTS pExtVde = (PVOLUME_DISK_EXTENTS)&tmp_buf.at(0);
    bRes = ::DeviceIoControl(handle,
        IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
        NULL,
        0,
        pExtVde,
        dwBytes,
        &dwBytes,
        NULL);

    if ( FALSE == bRes )
    {
        return std::vector<unsigned int>();
    }

    std::vector<unsigned int> result(pExtVde->NumberOfDiskExtents, 0);
    for (DWORD i = 0; i < pExtVde->NumberOfDiskExtents; ++i)
    {
        result[i] = pExtVde->Extents[i].DiskNumber;
    }

    return result;
}

DiskGeometry GetDeviceGeometry(const std::wstring& wsDevInterfaceVolume)
{
    DiskGeometry result;

    AutoHandle hDisk( ::CreateFileW( 
        wsDevInterfaceVolume.c_str(), 
        GENERIC_READ | GENERIC_WRITE, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, 
        NULL, 
        OPEN_EXISTING, 
        NULL, 
        NULL) );

    DWORD dwRead = 0;
    BOOL bResult = ::DeviceIoControl(
        hDisk, 
        IOCTL_DISK_GET_DRIVE_GEOMETRY, 
        NULL, 
        NULL, 
        &result, 
        sizeof(result), 
        &dwRead, 
        NULL);

    if ( FALSE == bResult )
    {
        throw std::runtime_error("Can't get drive geometry");
    }

    return result;
}

RemovableDeviceType GetDeviceType( PSP_DEVINFO_DATA pDevInfoData )
{
    DEVINST childDevInst;
    DWORD res = CM_Get_Child_Ex(&childDevInst, pDevInfoData->DevInst, 0, NULL);
    
    bool isDisk = (CR_NO_SUCH_DEVNODE == res);
    if(isDisk)
    {
        return rdtHardDisk;
    }
    else
    {
        return rdtFlash;
    }
    
    return rdtUnknown;
}

std::wstring GetDevInterfaceVolume(PSP_DEVINFO_DATA pdevInfoData)
{

    ULONG   ulSize = 0;
    DEVINST childDevInst;
    bool isDisk = false;

    if ( NULL == pdevInfoData )
    {
        // Wrong PSP_DEVINFO_DATA parameter
        throw std::runtime_error("Wrong PSP_DEVINFO_DATA parameter");
    }

    DWORD res = CM_Get_Child_Ex(&childDevInst, pdevInfoData->DevInst, 0, NULL);
    isDisk = (CR_NO_SUCH_DEVNODE == res);
    if(isDisk)
    {
        childDevInst = pdevInfoData->DevInst;
    }
    else if ( CR_SUCCESS != res)
    {
        // Can't get Child Device Instance
        throw std::runtime_error("Can't get Child Device Instance");
    }

    TCHAR bDeviceID[MAX_DEVICE_ID_LEN] = {0};
    if ( CR_SUCCESS != CM_Get_Device_ID_Ex(childDevInst, bDeviceID, sizeof(bDeviceID)/sizeof(TCHAR), 0, NULL) ) 
    {
        // Can't get Device ID");
        throw std::runtime_error("Can't get Device ID");
    }

    LPGUID InterfaceClassGuid = isDisk ? (LPGUID)&DiskClassGuid : (LPGUID)&VolumeClassGuid;

    if ( CR_SUCCESS != CM_Get_Device_Interface_List_Size(&ulSize, InterfaceClassGuid, bDeviceID, 0) )
    {
        // Can't get Device Instance List Size");
        throw std::runtime_error("Can't get Device Instance List Size");
    }

    std::vector<TCHAR> sResult(ulSize, 0x00);
    if ( sResult.empty() )
    {
        // Device Interface Volume is empty");
        throw std::runtime_error("Device Interface Volume is empty");
    }

    InterfaceClassGuid = isDisk ? (LPGUID)&GUID_DEVINTERFACE_DISK : (LPGUID)&GUID_DEVINTERFACE_VOLUME;

    if ( CR_SUCCESS != CM_Get_Device_Interface_List(InterfaceClassGuid, bDeviceID, &sResult.front(), ulSize, 0) )
    {
        // Can't get Device Instance List");
        throw std::runtime_error("Can't get Device Instance List");
    }

    sResult.resize(ulSize);

    if ( sResult.empty() )
    {
        // Device Interface Volume is empty
        throw std::runtime_error("Device Interface Volume is empty");
    }

    return std::wstring(&sResult.front());
}



RemovableDeviceInfo_vt Disk::Functions::SearchRemovalDisks()
{
    RemovableDeviceInfo_vt devInfos;

    /*GUID_DEVCLASS_DISKDRIVE*/
    CONST CLSID CLSID_DeviceInstance = { 0x4D36E967, 0xE325, 0x11CE, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } };
    HDEVINFO hDevInfo = ::SetupDiGetClassDevs(&CLSID_DeviceInstance, NULL, NULL, DIGCF_PRESENT);

    if ( INVALID_HANDLE_VALUE == hDevInfo ) 
    {
        // Can't Get Class Devices
        return devInfos;
    }

    try
    {
        std::wstring name;
        RemovableDeviceInfo devInfo;
        SP_DEVINFO_DATA devInfoData;
        devInfoData.cbSize = sizeof(devInfoData);

        for ( DWORD dwCount = 0; ::SetupDiEnumDeviceInfo(hDevInfo, dwCount, &devInfoData); ++dwCount )
        {
            DWORD dwSize = 0;
            DWORD dwDataType = 0;
            DWORD dwRemovalPolicy = 0;

            // [Warning]: only Windows XP and later
            if ( ::SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_REMOVAL_POLICY, &dwDataType, (PBYTE)&dwRemovalPolicy, sizeof(dwRemovalPolicy), &dwSize) )
            {
                if ( CM_REMOVAL_POLICY_EXPECT_NO_REMOVAL != dwRemovalPolicy )
                {
                    RemovableDeviceInfo devInfo;
                    devInfo.wsDeviceDesc         = GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_DEVICEDESC     );
                    devInfo.wsEnumeratorName     = GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_ENUMERATOR_NAME);
                    devInfo.wsCompatibleIDs      = GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_COMPATIBLEIDS  );
                    devInfo.wsHardwareID         = GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_HARDWAREID     );
                    devInfo.wsMFG                = GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_MFG            );
                    devInfo.wsFriendlyName       = GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME   );
                    devInfo.wsDevInterfaceVolume = GetDevInterfaceVolume(&devInfoData);
                    devInfo.wsPath               = GetDevicePath        (&devInfoData);
                    devInfo.deviceType           = GetDeviceType        (&devInfoData);



                    std::wstring diskPath(L"\\\\.\\");
                    diskPath += devInfo.wsPath[0];
                    diskPath += L":";


                    std::vector<unsigned int> divIndexes  = GetHarddiskIndexes(devInfo.wsDevInterfaceVolume);
                    std::vector<unsigned int> pathIndexes = GetHarddiskIndexes(diskPath);

                    if (false == pathIndexes.empty())
                    {
                        devInfo.vHarddiskIndexes = pathIndexes;
                    }
                    else
                    {
                        devInfo.vHarddiskIndexes = divIndexes;
                    }
                                            

                    if ( !devInfo.vHarddiskIndexes.empty() )
                    {
                        try
                        {
                            devInfo.diskGeometry         = GetDeviceGeometry(devInfo.wsDevInterfaceVolume);
                            devInfos.push_back(devInfo);
                        }
                        catch (const std::runtime_error&)
                        {
                            // Suppressing GetDeviceGeometry error 
                        }
                    }
                }
            }
        }

        ::SetupDiDestroyDeviceInfoList( hDevInfo );
        return devInfos;
    }
    catch (...)
    {
        // Catch exception in SearchRemovableDevicePath
        ::SetupDiDestroyDeviceInfoList( hDevInfo );

        throw;
    }
}
