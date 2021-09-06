#ifndef __EXCEPTION__
#define __EXCEPTION__
#include <exception>

class BusError: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Memory access should be a multiple of four.";
    }
};

class SegmentationFault: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Segmentation falut";
    }
};

#endif
