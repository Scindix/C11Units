#ifndef C11UNITS_CPP_INCLUDED
#define C11UNITS_CPP_INCLUDED

#include <iostream>
#include <exception>
#include <set>
#include <cstdlib>

class UnitErrorException: public std::exception
{
    virtual const char* what() const throw()
    {
        return "An Unit error occurred. You cannot add different types!";
    }
} unitErrorException;

class BasicUnit
{
public:
    std::string unit;
    signed exp;

    BasicUnit(std::string s, signed i) : unit(s), exp(i) {}

    bool operator<(BasicUnit a) const
    {
        if(unit.compare(a.unit) == 0)
            return exp < a.exp;
        else
            return unit.compare(a.unit) < 0;
    }

    bool operator==(BasicUnit a) const
    {
        return (unit == a.unit && exp == a.exp);
    }
};

class Unit
{
public:
    Unit(std::set<BasicUnit> u) : units(u) {}

    Unit operator+(Unit u) const
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

    Unit operator*(Unit u) const
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
        }
        newUnits.insert(units1.begin(), units1.end());
        newUnits.insert(units2.begin(), units2.end());
        return Unit(newUnits);
    }

    Unit reciprocal() const
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

    friend std::ostream& operator<< (std::ostream& out, Unit& u);

private:
    std::set<BasicUnit> units;
};

std::ostream& operator<< (std::ostream& out, Unit& u)
{
    std::string denominator;
    std::string numerator;
    for(auto it = u.units.begin(); it != u.units.end(); ++it)
    {
        std::string exponent("");

        if(abs(it->exp) != 1)
            exponent = std::string("^") + std::to_string(it->exp);

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

class PhyscalVariable
{
public:
    PhyscalVariable(double v, Unit u) : value(v), unit(u) {}

    PhyscalVariable operator+(const PhyscalVariable v) const
    {
        return PhyscalVariable(this->value + v.value, unit + v.unit);
    }

    PhyscalVariable operator-(const PhyscalVariable v) const
    {
        return PhyscalVariable(this->value - v.value, unit + v.unit);
    }

    PhyscalVariable operator/(const PhyscalVariable v) const
    {
        return PhyscalVariable(this->value / v.value, unit * v.unit.reciprocal());
    }

    PhyscalVariable operator*(const PhyscalVariable v) const
    {
        return PhyscalVariable(this->value * v.value, unit * v.unit);
    }

    PhyscalVariable operator*(const double v) const
    {
        return PhyscalVariable(this->value * v, unit);
    }

    PhyscalVariable operator/(const double v) const
    {
        return PhyscalVariable(this->value / v, unit);
    }

    PhyscalVariable operator+(const double v) const
    {
        return PhyscalVariable(this->value + v, unit + Unit({}));
    }

    PhyscalVariable operator-(const double v) const
    {
        return PhyscalVariable(this->value - v, unit + Unit({}));
    }

    friend std::ostream& operator<< (std::ostream& out, PhyscalVariable& pv);

private:
    double value;
    Unit unit;
};

std::ostream& operator<< (std::ostream& out, PhyscalVariable& pv)
{
    out << pv.value << " " << pv.unit;
    return out;
}

PhyscalVariable operator "" km(long double a)
{
    return PhyscalVariable(a*1000, Unit({BasicUnit("m", 1)}));
}

PhyscalVariable operator "" km(unsigned long long int a)
{
    return PhyscalVariable(a*1000, Unit({BasicUnit("m", 1)}));
}

PhyscalVariable operator "" m(long double a)
{
    return PhyscalVariable(a, Unit({BasicUnit("m", 1)}));
}

PhyscalVariable operator "" m(unsigned long long int a)
{
    return PhyscalVariable(a, Unit({BasicUnit("m", 1)}));
}

PhyscalVariable operator "" min(long double a)
{
    return PhyscalVariable(a*60, Unit({BasicUnit("s", 1)}));
}

PhyscalVariable operator "" min(unsigned long long int a)
{
    return PhyscalVariable(a*60, Unit({BasicUnit("s", 1)}));
}

PhyscalVariable operator "" N(long double a)
{
    return PhyscalVariable(a, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}

PhyscalVariable operator "" N(unsigned long long int a)
{
    return PhyscalVariable(a, Unit({BasicUnit("kg", 1), BasicUnit("m", 1), BasicUnit("s", -2)}));
}


#endif // C11UNITS_CPP_INCLUDED
