#ifndef _IENUMERATOR_H_
#define _IENUMERATOR_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <vector>

namespace DeviceDetectLibrary
{
    class ICollector;
    struct DeviceInfo;


    class IEnumerator : public boost::noncopyable
    {
    public:
        virtual void Collect(const DeviceInfo& ) = 0;
        virtual ~IEnumerator() {}
    };

    typedef boost::shared_ptr<IEnumerator> IEnumeratorPtr;
    typedef std::vector<IEnumeratorPtr> IEnumerators;

}

#endif // _IENUMERATOR_H_