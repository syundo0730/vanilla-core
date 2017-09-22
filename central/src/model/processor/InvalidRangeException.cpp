#include "InvalidRangeException.h"

InvalidRangeException::InvalidRangeException(const int i) : index(i) {}

const int InvalidRangeException::getIndex() const noexcept
{
    return index;
}

const char *InvalidRangeException::what() const noexcept
{
    return "InvalidRangeException";
}