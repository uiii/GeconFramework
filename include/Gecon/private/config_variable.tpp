#include "../config_variable.hpp"

namespace Gecon
{
    template< typename T >
    config_variable<T>::config_variable(const T& value)
    {
        operator=(value);
    }

    template< typename T >
    config_variable<T>::config_variable(const config_variable<T>& another)
    {
        operator=((const T&)another);
    }

    template< typename T >
    config_variable<T>& config_variable<T>::operator=(const T& value)
    {
        boost::lock_guard<boost::mutex> lock(mutex_);
        value_ = value;

        return *this;
    }

    template< typename T >
    config_variable<T>& config_variable<T>::operator=(const config_variable<T> &another)
    {
        return operator=((const T&)another);
    }

    template< typename T >
    config_variable<T>::operator const T&() const
    {
        boost::lock_guard<boost::mutex> lock(mutex_);
        return value_;
    }
}
