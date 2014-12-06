#include "C11Units.h"

BasicUnit::BasicUnit(std::string s, signed i) : unit(s), exp(i) {}

bool BasicUnit::operator<(BasicUnit a) const
{
    if(unit.compare(a.unit) == 0)
        return exp < a.exp;
    else
        return unit.compare(a.unit) < 0;
}

bool BasicUnit::operator==(BasicUnit a) const
{
    return (unit == a.unit && exp == a.exp);
}

Unit::Unit(std::set<BasicUnit> u) : units(u) {}

Unit Unit::operator+(Unit u) const
{
    bool equal = true;
    auto it2 = u.units.begin();
    for(auto it1 = units.begin(); it1 != units.end() && it2 != u.units.end(); ++it1, ++it2)
    {
        equal &= *it1 == *it2;
        if(!equal)
            break;
    }
    if(equal)
        return *this;
    else
        throw unitErrorException;
}

Unit Unit::operator*(Unit u) const
{
    auto units1 = units;
    auto units2 = u.units;
    std::set<BasicUnit> newUnits;
    for(auto it1 = units1.begin(); it1 != units1.end(); ++it1)
    {
        for(auto it2 = units2.begin(); it2 != units2.end() && it1 != units1.end(); ++it2)
        {
            if(it1->unit == it2->unit)
            {
                if(it1->exp + it2->exp != 0)
                    newUnits.insert(BasicUnit(it1->unit, it1->exp + it2->exp));
                it2 = units2.erase(it2);
                it1 = units1.erase(it1);
            }
            if(it2 == units2.end() || it1 == units1.end())
                break;
        }
        if(it1 == units1.end())
            break;
    }
    newUnits.insert(units1.begin(), units1.end());
    newUnits.insert(units2.begin(), units2.end());
    return Unit(newUnits);
}

Unit Unit::reciprocal() const
{
    std::set<BasicUnit> newUnits;
    for(auto it = units.begin(); it != units.end(); ++it)
    {
        BasicUnit b = *it;
        b.exp *= -1;
        newUnits.insert(b);
    }
    return Unit(newUnits);
}

Unit Unit::pow(signed exponent) const
{
    std::set<BasicUnit> newUnits;
    for(auto it = units.begin(); it != units.end(); ++it)
    {
        BasicUnit b = *it;
        b.exp *= exponent;
        newUnits.insert(b);
    }
    return Unit(newUnits);
}

Unit Unit::root(signed exponent) const
{
    std::set<BasicUnit> newUnits;
    for(auto it = units.begin(); it != units.end(); ++it)
    {
        BasicUnit b = *it;
        if((b.exp % exponent)!=0)
            throw unitErrorException;
        b.exp /= exponent;
        newUnits.insert(b);
    }
    return Unit(newUnits);
}

std::string Unit::hash() const
{
    std::string out("");
    for(auto it = units.begin(); it != units.end(); ++it)
    {
        out += it->unit + std::to_string(it->exp);
    }
    return out;
}

const std::map<std::string, std::string> Unit::mappedUnits =
{
    {"s-1",         "Hz"},
    {"kg1m1s-2",    "N"},
    {"kg1m-1s-2",   "Pa"},
    {"kg1m2s-2",    "J"},
    {"A1s1",        "C"},
    {"A-1kg1m2s-3", "V"},
    {"A2kg-1m-2s4", "F"},
    {"A-2kg1m2s-3", "Ohm"},
    {"A2kg-1m-2s3", "S"},
    {"A-1kg1m2s-2", "Wb"},
    {"A-1kg1s-2",   "T"},
    {"A-2kg1m2s-2", "H"},
};


std::ostream& operator<< (std::ostream& out, const Unit& u)
{
    auto mappedUnit = Unit::mappedUnits.find(u.hash());
    if(mappedUnit !=  Unit::mappedUnits.end())
    {
        out << mappedUnit->second;
        return out;
    }
    std::string denominator;
    std::string numerator;
    for(auto it = u.units.begin(); it != u.units.end(); ++it)
    {
        std::string exponent("");

        if(abs(it->exp) != 1)
            exponent = std::string("^") + std::to_string(abs(it->exp));

        if(it->exp > 0)
            numerator += it->unit + exponent + " ";
        else
            denominator += it->unit + exponent + " ";
    }
    if(numerator == "")
        numerator = "1 ";
    if(denominator != "")
        denominator = std::string("/ ") + denominator;
    out << numerator << denominator;
    return out;
}

PhysicalVariable::PhysicalVariable(double v, Unit u) : value(v), unit(u) {}

PhysicalVariable PhysicalVariable::operator+(const PhysicalVariable v) const
{
    return PhysicalVariable(this->value + v.value, unit + v.unit);
}

PhysicalVariable PhysicalVariable::operator-(const PhysicalVariable v) const
{
    return PhysicalVariable(this->value - v.value, unit + v.unit);
}

PhysicalVariable PhysicalVariable::operator/(const PhysicalVariable v) const
{
    return PhysicalVariable(this->value / v.value, unit * v.unit.reciprocal());
}

PhysicalVariable PhysicalVariable::operator*(const PhysicalVariable v) const
{
    return PhysicalVariable(this->value * v.value, unit * v.unit);
}

PhysicalVariable PhysicalVariable::operator*(const double v) const
{
    return PhysicalVariable(this->value * v, unit);
}

PhysicalVariable PhysicalVariable::operator/(const double v) const
{
    return PhysicalVariable(this->value / v, unit);
}

PhysicalVariable PhysicalVariable::operator+(const double v) const
{
    return PhysicalVariable(this->value + v, unit + Unit({}));
}

PhysicalVariable PhysicalVariable::operator-(const double v) const
{
    return PhysicalVariable(this->value - v, unit + Unit({}));
}

PhysicalVariable PhysicalVariable::operator^(const signed v) const
{
    return PhysicalVariable(value, unit.pow(v));
}

PhysicalVariable PhysicalVariable::pow(const signed v) const
{
    return PhysicalVariable(std::pow(value, v), unit.pow(v));
}

PhysicalVariable PhysicalVariable::root(const signed v) const
{
    return PhysicalVariable(std::pow(value, 1./v), unit.root(v));
}

std::ostream& operator<< (std::ostream& out, const PhysicalVariable& pv)
{
    out << pv.value << " " << pv.unit;
    return out;
}

PhysicalVariable operator "" _pc(long double a)
{
    return PhysicalVariable(a*3.0856776e16, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _pc(unsigned long long int a)
{
    return PhysicalVariable(a*3.0856776e16, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _ly(long double a)
{
    return PhysicalVariable(a*9460730472580800, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _ly(unsigned long long int a)
{
    return PhysicalVariable(a*9460730472580800, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _km(long double a)
{
    return PhysicalVariable(a*1000, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _km(unsigned long long int a)
{
    return PhysicalVariable(a*1000, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _m(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _m(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _dm(long double a)
{
    return PhysicalVariable(a/10, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _dm(unsigned long long int a)
{
    return PhysicalVariable(a/10, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _cm(long double a)
{
    return PhysicalVariable(a/100, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _cm(unsigned long long int a)
{
    return PhysicalVariable(a/100, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _mm(long double a)
{
    return PhysicalVariable(a/1000, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _mm(unsigned long long int a)
{
    return PhysicalVariable(a/1000, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _mum(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _mum(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _nm(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _nm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _pm(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _pm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _fm(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _fm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _am(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _am(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _zm(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _zm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _ym(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _ym(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 1)}));
}

PhysicalVariable operator "" _a(long double a)
{
    return PhysicalVariable(a*31557600, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _a(unsigned long long int a)
{
    return PhysicalVariable(a*31557600, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _d(long double a)
{
    return PhysicalVariable(a*86400, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _d(unsigned long long int a)
{
    return PhysicalVariable(a*86400, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _h(long double a)
{
    return PhysicalVariable(a*3600, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _h(unsigned long long int a)
{
    return PhysicalVariable(a*3600, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _min(long double a)
{
    return PhysicalVariable(a*60, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _min(unsigned long long int a)
{
    return PhysicalVariable(a*60, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _s(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _s(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _ms(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _ms(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _mus(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _mus(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _ns(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _ns(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _ps(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _ps(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _fs(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _fs(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _as(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _as(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _zs(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _zs(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _ys(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _ys(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _kg(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _kg(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _g(unsigned long long int a)
{
    return PhysicalVariable(a/1000, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _g(long double a)
{
    return PhysicalVariable(a/1000, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _mg(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _mg(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _mug(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _mug(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _ng(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _ng(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _pg(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _pg(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _fg(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _fg(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _ag(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _ag(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _zg(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _zg(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _yg(unsigned long long int a)
{
    return PhysicalVariable(a*1e-27, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _yg(long double a)
{
    return PhysicalVariable(a*1e-27, Unit({BasicUnit("kg", 1)}));
}


PhysicalVariable operator "" _deg(long double a)
{
    return PhysicalVariable(M_PI*a/180, Unit({}));
}

PhysicalVariable operator "" _deg(unsigned long long int a)
{
    return PhysicalVariable(M_PI*a/180, Unit({}));
}

PhysicalVariable operator "" _degMin(long double a)
{
    return PhysicalVariable(M_PI*a/10800, Unit({}));
}

PhysicalVariable operator "" _degMin(unsigned long long int a)
{
    return PhysicalVariable(M_PI*a/10800, Unit({}));
}

PhysicalVariable operator "" _degSec(long double a)
{
    return PhysicalVariable(M_PI*a/648000, Unit({}));
}

PhysicalVariable operator "" _degSec(unsigned long long int a)
{
    return PhysicalVariable(M_PI*a/648000, Unit({}));
}

PhysicalVariable operator "" _kl(long double a)
{
    return PhysicalVariable(a*1000, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _kl(unsigned long long int a)
{
    return PhysicalVariable(a*1000, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _hl(long double a)
{
    return PhysicalVariable(a*100, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _hl(unsigned long long int a)
{
    return PhysicalVariable(a*100, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _l(long double a)
{
    return PhysicalVariable(a/1000, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _l(unsigned long long int a)
{
    return PhysicalVariable(a/1000, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _ml(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _ml(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _mul(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _mul(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _nl(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _nl(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", -3)}));
}

PhysicalVariable operator "" _t(long double a)
{
    return PhysicalVariable(a*1000, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _t(unsigned long long int a)
{
    return PhysicalVariable(a*1000, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _meV(long double a)
{
    return PhysicalVariable(a*1.602176565e-22, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _meV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e-22, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _eV(long double a)
{
    return PhysicalVariable(a*1.602176565e-19, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _eV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e-19, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _keV(long double a)
{
    return PhysicalVariable(a*1.602176565e-16, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _keV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e-16, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _MeV(long double a)
{
    return PhysicalVariable(a*1.602176565e-13, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _MeV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e-13, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _GeV(long double a)
{
    return PhysicalVariable(a*1.602176565e-10, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _GeV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e-10, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _TeV(long double a)
{
    return PhysicalVariable(a*1.602176565e-7, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _TeV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e-7, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _PeV(long double a)
{
    return PhysicalVariable(a*1.602176565e-4, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _PeV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e-4, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _EeV(long double a)
{
    return PhysicalVariable(a*1.602176565e-1, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _EeV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e-1, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _ZeV(long double a)
{
    return PhysicalVariable(a*1.602176565e2, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _ZeV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e2, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _YeV(long double a)
{
    return PhysicalVariable(a*1.602176565e5, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _YeV(unsigned long long int a)
{
    return PhysicalVariable(a*1.602176565e5, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _u(long double a)
{
    return PhysicalVariable(a*1.660538921e-27, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _u(unsigned long long int a)
{
    return PhysicalVariable(a*1.660538921e-27, Unit({BasicUnit("kg", 1)}));
}

PhysicalVariable operator "" _yN(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _yN(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _zN(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _zN(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _aN(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _aN(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _fN(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _fN(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _pN(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _pN(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _nN(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _nN(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _muN(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _muN(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _mN(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _mN(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _N(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _N(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _kN(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _kN(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _MN(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _MN(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _GN(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _GN(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _TN(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _TN(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _PN(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _PN(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _EN(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _EN(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _ZN(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _ZN(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _YN(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _YN(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _yA(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _yA(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _zA(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _zA(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _aA(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _aA(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _fA(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _fA(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _pA(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _pA(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _nA(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _nA(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _muA(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _muA(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _mA(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _mA(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _A(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _A(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _kA(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _kA(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _MA(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _MA(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _GA(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _GA(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _TA(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _TA(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _PA(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _PA(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _EA(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _EA(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _ZA(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _ZA(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _YA(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _YA(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("A", 1)}));
}

PhysicalVariable operator "" _K(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("K", 1)}));
}

PhysicalVariable operator "" _K(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("K", 1)}));
}

PhysicalVariable operator "" _mmol(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("mol", 1)}));
}

PhysicalVariable operator "" _mmol(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("mol", 1)}));
}

PhysicalVariable operator "" _mol(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("mol", 1)}));
}

PhysicalVariable operator "" _mol(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("mol", 1)}));
}

PhysicalVariable operator "" _kmol(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("mol", 1)}));
}

PhysicalVariable operator "" _kmol(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("mol", 1)}));
}

PhysicalVariable operator "" _Cd(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("Cd", 1)}));
}

PhysicalVariable operator "" _Cd(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("Cd", 1)}));
}

PhysicalVariable operator "" _rad(long double a)
{
    return PhysicalVariable(a, Unit({}));
}

PhysicalVariable operator "" _rad(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({}));
}

PhysicalVariable operator "" _yHz(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _yHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _zHz(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _zHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _aHz(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _aHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _fHz(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _fHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _pHz(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _pHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _nHz(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _nHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _muHz(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _muHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _mHz(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _mHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _Hz(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _Hz(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _kHz(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _kHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _MHz(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _MHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _GHz(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _GHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _THz(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _THz(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _PHz(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _PHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _EHz(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _EHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _ZHz(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _ZHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _YHz(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _YHz(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _yPa(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _yPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _zPa(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _zPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _aPa(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _aPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _fPa(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _fPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _pPa(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _pPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _nPa(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _nPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _muPa(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _muPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _mPa(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _mPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _Pa(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _Pa(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _hPa(long double a)
{
    return PhysicalVariable(a*1e2, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _hPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e2, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _kPa(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _kPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _MPa(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _MPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _GPa(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _GPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _TPa(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _TPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _PPa(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _PPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _EPa(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _EPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _ZPa(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _ZPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _YPa(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _YPa(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", -1), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _yJ(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _yJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _zJ(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _zJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _aJ(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _aJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _fJ(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _fJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _pJ(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _pJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _nJ(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _nJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _muJ(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _muJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _mJ(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _mJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _J(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _J(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _kJ(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _kJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _MJ(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _MJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _GJ(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _GJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _TJ(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _TJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _PJ(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _PJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _EJ(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _EJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _ZJ(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _ZJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _YJ(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _YJ(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2)}));
}

PhysicalVariable operator "" _yW(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _yW(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _zW(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _zW(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _aW(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _aW(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _fW(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _fW(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _pW(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _pW(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _nW(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _nW(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _muW(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _muW(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _mW(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _mW(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _W(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _W(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _kW(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _kW(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _MW(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _MW(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _GW(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _GW(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _TW(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _TW(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _PW(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _PW(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _EW(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _EW(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _ZW(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _ZW(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _YW(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _YW(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3)}));
}

PhysicalVariable operator "" _mC(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("A", 1), BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _mC(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("A", 1), BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _C(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("A", 1), BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _C(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("A", 1), BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _kC(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("A", 1), BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _kC(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("A", 1), BasicUnit("s", 1)}));
}

PhysicalVariable operator "" _yV(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _yV(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _zV(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _zV(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _aV(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _aV(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _fV(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _fV(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _pV(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _pV(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _nV(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _nV(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _muV(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _muV(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _mV(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _mV(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _V(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _V(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _kV(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _kV(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _MV(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _MV(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _GV(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _GV(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _TV(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _TV(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _PV(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _PV(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _EV(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _EV(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _ZV(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _ZV(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _YV(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _YV(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _yF(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _yF(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _zF(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _zF(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _aF(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _aF(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _fF(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _fF(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _pF(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _pF(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _nF(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _nF(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _muF(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _muF(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _mF(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _mF(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _F(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _F(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _kF(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _kF(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _MF(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _MF(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 4), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _yohm(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _yohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _zohm(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _zohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _aohm(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _aohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _fohm(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _fohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _pohm(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _pohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _nohm(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _nohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _muohm(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _muohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _mohm(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _mohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _ohm(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _ohm(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _kohm(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _kohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Mohm(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Mohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Gohm(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Gohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Tohm(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Tohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Pohm(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Pohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Eohm(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Eohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Zohm(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Zohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Yohm(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _Yohm(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -3), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _S(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 3), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _S(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", -2), BasicUnit("kg", -1), BasicUnit("s", 3), BasicUnit("A", 2)}));
}

PhysicalVariable operator "" _Wb(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _Wb(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _yT(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _yT(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _zT(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _zT(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _aT(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _aT(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _fT(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _fT(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _pT(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _pT(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _nT(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _nT(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _muT(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _muT(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _mT(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _mT(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _T(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _T(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _kT(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _kT(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _MT(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _MT(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _GT(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _GT(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _TT(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _TT(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _PT(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _PT(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _ET(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _ET(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _ZT(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _ZT(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _YT(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _YT(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -1)}));
}

PhysicalVariable operator "" _yH(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _yH(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _zH(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _zH(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _aH(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _aH(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _fH(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _fH(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _pH(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _pH(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _nH(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _nH(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _muH(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _muH(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _mH(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _mH(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _H(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _H(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _kH(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _kH(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _MH(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _MH(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _GH(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _GH(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _TH(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _TH(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _PH(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _PH(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _EH(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _EH(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _ZH(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _ZH(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _YH(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _YH(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("m", 2), BasicUnit("kg", 1), BasicUnit("s", -2), BasicUnit("A", -2)}));
}

PhysicalVariable operator "" _degC(long double a)
{
    return PhysicalVariable(a-273.15, Unit({BasicUnit("K", 1)}));
}

PhysicalVariable operator "" _degC(unsigned long long int a)
{
    return PhysicalVariable(a-273.15, Unit({BasicUnit("K", 1)}));
}

PhysicalVariable operator "" _yBc(long double a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _yBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e-24, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _zBc(long double a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _zBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e-21, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _aBc(long double a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _aBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e-18, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _fBc(long double a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _fBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e-15, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _pBc(long double a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _pBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e-12, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _nBc(long double a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _nBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e-9, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _muBc(long double a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _muBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e-6, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _mBc(long double a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _mBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e-3, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _Bc(long double a)
{
    return PhysicalVariable(a, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _Bc(unsigned long long int a)
{
    return PhysicalVariable(a, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _kBc(long double a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _kBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e3, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _MBc(long double a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _MBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e6, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _GBc(long double a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _GBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e9, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _TBc(long double a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _TBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e12, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _PBc(long double a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _PBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e15, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _EBc(long double a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _EBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e18, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _ZBc(long double a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _ZBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e21, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _YBc(long double a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("s", -1)}));
}

PhysicalVariable operator "" _YBc(unsigned long long int a)
{
    return PhysicalVariable(a*1e24, Unit({BasicUnit("s", -1)}));
}
