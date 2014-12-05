C11Units
========

C11Units is a short code sample of how to work with physical units in C++11. main.cpp shows an example. Adding new Units is quite simple: Just look at the bottom of C11Units.h

Example:

    #include <C11Units.h>
    
    
    int main()
    {
        auto a = 12N * (1.2km + 12m) / 70min / 0.5m;
        std::cout << a; // gives: 6.92571 kg m / s^-3
    }
