#include <C11Units.h>


int main()
{
    auto a = 12N * (1.2km + 12m) / 70min / 0.5m;
    std::cout << a;
}
