#ifndef _AUTO_HANDLE_H_
#define _AUTO_HANDLE_H_

#include <stdexcept>
#include "Win32Types.h"

class AutoHandleException : public std::runtime_error
{
public:
    typedef std::runtime_error base;

public:
    AutoHandleException(const std::string& message) : base(message)
    {
    }
};

class AutoHandle 
{
public:
    struct Nothrow {};

public:
    AutoHandle(Handle handle);
    AutoHandle(Handle handle, Nothrow);
    virtual ~AutoHandle();

public:
    Handle GetHandle() const;
    operator Handle() const;

public:
	Handle Release();
    void Reset();
    void Reset(Handle newHandle);

private:
    AutoHandle(const AutoHandle&);
    AutoHandle& operator=(const AutoHandle&);

private:
    Handle handle_;
};

#endif //_AUTO_HANDLE_H_