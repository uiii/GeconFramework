#ifndef GECON_CONFIG_VARIABLE_HPP
#define GECON_CONFIG_VARIABLE_HPP

#include <boost/thread.hpp>

namespace Gecon
{
    template< typename T >
    class config_variable
    {
    public:
        config_variable(const T& value);
        config_variable(const config_variable<T>& another);

        config_variable<T>& operator=(const T& value);
        config_variable<T>& operator=(const config_variable<T>& another);

        operator const T&() const;

    private:
        T value_;

        mutable boost::mutex mutex_;
    };
}

#include "private/config_variable.tpp"

#endif // GECON_CONFIG_VARIABLE_HPP
