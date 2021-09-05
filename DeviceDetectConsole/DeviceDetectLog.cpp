#include "Stdafx.h"
#include "DeviceDetectLog.h"

void LogMessage( char *format, ... )
{
    va_list args;
    va_start( args, format );

    std::vector<char> buffer(0xffff);
    _vsnprintf_s(&buffer.front(), buffer.size(), buffer.size(), format, args);

    va_end( args );

    printf( &buffer.front() );
    printf( "\n" );
}

