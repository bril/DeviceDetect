#ifndef _C_MODEMDEVICECHECKER_H_
#define _C_MODEMDEVICECHECKER_H_


namespace DeviceDetectLibrary
{

    class ModemDeviceChecker
    {
    public:
        ModemDeviceChecker(ICollector& collector, const std::vector<std::wstring>& patternNames);
        virtual ~ModemDeviceChecker();

        void virtual Collect(const DeviceInfo& deviceInfo, const std::wstring& pluginID);

    protected:
        bool virtual Check( const DeviceInfo& deviceInfo );   
        

    protected: 
        ICollector& collector_;
        std::vector<std::wstring> _patternNames;

    };
    typedef boost::shared_ptr<ModemDeviceChecker> ModemDeviceNameParserPtr;
    typedef std::vector<ModemDeviceNameParserPtr> ModemDeviceNameParsers;

}


#endif // _C_MODEMDEVICECHECKER_H_
