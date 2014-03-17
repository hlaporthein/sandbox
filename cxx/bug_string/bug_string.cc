#include <string>
#include <sstream>

namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#include <iostream>

int main()
{
    std::cout << std::to_string(1234) << '\n' << std::to_string(1234.56) << '\n' ;
}