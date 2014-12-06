#include <C11Units.h>

int main()
{
    auto a = 12_N * (1.2_km + 12_m) / 70_min / (0.5_m^2);
    std::cout << a << std::endl;                        /// 6.92571 kg / s^3

    /// You can do all kinds of arithmetic operations
    /// (+, -, *, /, nth-power, nth-root; n must be integral)
    auto b = a * 3;
    std::cout << b << std::endl;                        /// 20.7771 kg / s^3
    auto x = b.pow(3);
    auto y = x.root(3);
    std::cout << x << std::endl;                        /// 8969.28 kg^3 / s^9
    std::cout << y << std::endl;                        /// 20.7771 kg / s^3
    auto c = a + b + ((2_mg^2) / (3_ns^6)).root(2);
    std::cout << c << std::endl;                        /// 53.5227 kg / s^3

    /// C11Units recognizes known units
    auto d = (120_degSec + 3_rad) / 1_s;
    std::cout << d << std::endl;                        /// 3.00058 Hz
    auto mu_r = 1;
    auto l = 12._cm;
    auto n = 5;
    auto magnetic_flux_density = 10_A * mu_r * mu_0 * n/l;

    std::cout << magnetic_flux_density << std::endl;    /// 0.000523599 T

    /// speed of light:
    std::cout << (1_ly / 1_a) << std::endl;             /// 2.99792e+08 m / s

    /// C11Units also throws an error
    /// if the arithmetic operation would generate an ill-formed unit.
    try
    {
        volatile auto e = a + 20_N;
        volatile auto f = a.root(13);
    } catch(std::exception)
    {
        std::cout << "Adding different types gives an error" << std::endl;
        std::cout << "Also you cannot generate exponentials"
                     " of non-integral type" << std::endl;
    }
}
