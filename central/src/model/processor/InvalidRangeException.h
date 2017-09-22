#ifndef INVALID_RENGE_EXCEPTION_H
#define INVALID_RENGE_EXCEPTION_H
#include <stdexcept>

class InvalidRangeException : public std::exception
{
public:
    InvalidRangeException(const int i);
    const int getIndex() const noexcept;
    const char* what() const noexcept override;
private:
    const int index;
};

#endif // INVALID_RENGE_EXCEPTION_H