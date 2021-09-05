#ifndef _WIN_CRITICAL_SECTION_H_
#define _WIN_CRITICAL_SECTION_H_

#include "Win32Types.h"

namespace DeviceDetectLibrary
{
	class CriticalSection
	{
	public:
		CriticalSection();
		virtual ~CriticalSection() ;


		void Enter();
		void Leave();


	private:
		CriticalSection(const CriticalSection&);
		CriticalSection& operator=(const CriticalSection&);

	private:
		CRITICAL_SECTION criticalSection_;
	}; 

} 

#endif // _WIN_CRITICAL_SECTION_H_
