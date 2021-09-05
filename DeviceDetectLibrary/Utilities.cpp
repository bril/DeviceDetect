#include "stdafx.h"
#include "Utilities.h"

using namespace Utilities;


std::string Utilities::StringUpper(const std::string& src)
{
    if( src.empty() )
    {
        return src;
    }

    std::vector<char> buffer(src.begin(), src.end());
    buffer.push_back(0);

    ::CharUpperBuffA(&buffer.at(0), static_cast<DWORD>(src.size()));

    return (&buffer.front());
}

std::wstring Utilities::StringUpper(const std::wstring& src)
{
    if( src.empty() ) return src;

    std::vector<wchar_t> buffer(src.begin(), src.end());
    buffer.push_back(0);

    ::CharUpperBuffW(&buffer.at(0), static_cast<DWORD>(src.size()));

    return (&buffer.front());
}

