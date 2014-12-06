#ifndef C11UNITS_CPP_INCLUDED
#define C11UNITS_CPP_INCLUDED

#include <iostream>
#include <exception>
#include <set>
#include <map>
#include <cstdlib>
#include <cmath>

class UnitErrorException: public std::exception
{
    virtual const char* what() const throw()
    {
        return "An Unit error occurred. You cannot add different types or generate units with non-integral exponentials!";
    }
} const unitErrorException;

class BasicUnit
{
public:
    std::string unit;
    signed exp;

    BasicUnit(std::string s, signed i);
    bool operator<(BasicUnit a) const;
    bool operator==(BasicUnit a) const;
};

class Unit
{
public:
    Unit(std::set<BasicUnit> u);

    Unit operator+(Unit u) const;
    Unit operator*(Unit u) const;
    Unit reciprocal() const;
    Unit pow(signed exponent) const;
    Unit root(signed exponent) const;
    std::string hash() const;
    friend std::ostream& operator<< (std::ostream& out, const Unit& u);

private:
    std::set<BasicUnit> units;
    static const std::map<std::string, std::string> mappedUnits;
};

std::ostream& operator<< (std::ostream& out, const Unit& u);

class PhysicalVariable
{
public:
    PhysicalVariable(double v, Unit u);

    PhysicalVariable operator+(const PhysicalVariable v) const;
    PhysicalVariable operator-(const PhysicalVariable v) const;
    PhysicalVariable operator/(const PhysicalVariable v) const;
    PhysicalVariable operator*(const PhysicalVariable v) const;
    PhysicalVariable operator*(const double v) const;
    PhysicalVariable operator/(const double v) const;
    PhysicalVariable operator+(const double v) const;
    PhysicalVariable operator-(const double v) const;
    PhysicalVariable operator^(const signed v) const;
    PhysicalVariable pow(const signed v) const;
    PhysicalVariable root(const signed v) const;
    friend std::ostream& operator<< (std::ostream& out, const PhysicalVariable& pv);

private:
    double value;
    Unit unit;
};

std::ostream& operator<< (std::ostream& out, const PhysicalVariable& pv);

PhysicalVariable operator "" _pc(long double a);
PhysicalVariable operator "" _pc(unsigned long long int a);

PhysicalVariable operator "" _ly(long double a);
PhysicalVariable operator "" _ly(unsigned long long int a);

PhysicalVariable operator "" _km(long double a);
PhysicalVariable operator "" _km(unsigned long long int a);
PhysicalVariable operator "" _m(long double a);
PhysicalVariable operator "" _m(unsigned long long int a);
PhysicalVariable operator "" _dm(long double a);
PhysicalVariable operator "" _dm(unsigned long long int a);
PhysicalVariable operator "" _cm(long double a);
PhysicalVariable operator "" _cm(unsigned long long int a);
PhysicalVariable operator "" _mm(long double a);
PhysicalVariable operator "" _mm(unsigned long long int a);
PhysicalVariable operator "" _mum(long double a);
PhysicalVariable operator "" _mum(unsigned long long int a);
PhysicalVariable operator "" _nm(long double a);
PhysicalVariable operator "" _nm(unsigned long long int a);
PhysicalVariable operator "" _pm(long double a);
PhysicalVariable operator "" _pm(unsigned long long int a);
PhysicalVariable operator "" _fm(long double a);
PhysicalVariable operator "" _fm(unsigned long long int a);
PhysicalVariable operator "" _am(long double a);
PhysicalVariable operator "" _am(unsigned long long int a);
PhysicalVariable operator "" _zm(long double a);
PhysicalVariable operator "" _zm(unsigned long long int a);
PhysicalVariable operator "" _ym(long double a);
PhysicalVariable operator "" _ym(unsigned long long int a);

PhysicalVariable operator "" _a(long double a);
PhysicalVariable operator "" _a(unsigned long long int a);
PhysicalVariable operator "" _d(long double a);
PhysicalVariable operator "" _d(unsigned long long int a);
PhysicalVariable operator "" _h(long double a);
PhysicalVariable operator "" _h(unsigned long long int a);
PhysicalVariable operator "" _min(long double a);
PhysicalVariable operator "" _min(unsigned long long int a);
PhysicalVariable operator "" _s(long double a);
PhysicalVariable operator "" _s(unsigned long long int a);
PhysicalVariable operator "" _ms(long double a);
PhysicalVariable operator "" _ms(unsigned long long int a);
PhysicalVariable operator "" _mus(long double a);
PhysicalVariable operator "" _mus(unsigned long long int a);
PhysicalVariable operator "" _ns(long double a);
PhysicalVariable operator "" _ns(unsigned long long int a);
PhysicalVariable operator "" _ps(long double a);
PhysicalVariable operator "" _ps(unsigned long long int a);
PhysicalVariable operator "" _fs(long double a);
PhysicalVariable operator "" _fs(unsigned long long int a);
PhysicalVariable operator "" _as(long double a);
PhysicalVariable operator "" _as(unsigned long long int a);
PhysicalVariable operator "" _zs(long double a);
PhysicalVariable operator "" _zs(unsigned long long int a);
PhysicalVariable operator "" _ys(long double a);
PhysicalVariable operator "" _ys(unsigned long long int a);

PhysicalVariable operator "" _kg(long double a);
PhysicalVariable operator "" _kg(unsigned long long int a);
PhysicalVariable operator "" _g(unsigned long long int a);
PhysicalVariable operator "" _g(long double a);
PhysicalVariable operator "" _mg(unsigned long long int a);
PhysicalVariable operator "" _mg(long double a);
PhysicalVariable operator "" _mug(unsigned long long int a);
PhysicalVariable operator "" _mug(long double a);
PhysicalVariable operator "" _ng(unsigned long long int a);
PhysicalVariable operator "" _ng(long double a);
PhysicalVariable operator "" _pg(unsigned long long int a);
PhysicalVariable operator "" _pg(long double a);
PhysicalVariable operator "" _fg(unsigned long long int a);
PhysicalVariable operator "" _fg(long double a);
PhysicalVariable operator "" _ag(unsigned long long int a);
PhysicalVariable operator "" _ag(long double a);
PhysicalVariable operator "" _zg(unsigned long long int a);
PhysicalVariable operator "" _zg(long double a);
PhysicalVariable operator "" _yg(unsigned long long int a);
PhysicalVariable operator "" _yg(long double a);

PhysicalVariable operator "" _deg(long double a);
PhysicalVariable operator "" _deg(unsigned long long int a);
PhysicalVariable operator "" _degMin(long double a);
PhysicalVariable operator "" _degMin(unsigned long long int a);
PhysicalVariable operator "" _degSec(long double a);
PhysicalVariable operator "" _degSec(unsigned long long int a);

PhysicalVariable operator "" _kl(long double a);
PhysicalVariable operator "" _kl(unsigned long long int a);
PhysicalVariable operator "" _hl(long double a);
PhysicalVariable operator "" _hl(unsigned long long int a);
PhysicalVariable operator "" _l(long double a);
PhysicalVariable operator "" _l(unsigned long long int a);
PhysicalVariable operator "" _ml(long double a);
PhysicalVariable operator "" _ml(unsigned long long int a);
PhysicalVariable operator "" _mul(long double a);
PhysicalVariable operator "" _mul(unsigned long long int a);
PhysicalVariable operator "" _nl(long double a);
PhysicalVariable operator "" _nl(unsigned long long int a);

PhysicalVariable operator "" _t(long double a);
PhysicalVariable operator "" _t(unsigned long long int a);

PhysicalVariable operator "" _meV(long double a);
PhysicalVariable operator "" _meV(unsigned long long int a);
PhysicalVariable operator "" _eV(long double a);
PhysicalVariable operator "" _eV(unsigned long long int a);
PhysicalVariable operator "" _keV(long double a);
PhysicalVariable operator "" _keV(unsigned long long int a);
PhysicalVariable operator "" _MeV(long double a);
PhysicalVariable operator "" _MeV(unsigned long long int a);
PhysicalVariable operator "" _GeV(long double a);
PhysicalVariable operator "" _GeV(unsigned long long int a);
PhysicalVariable operator "" _TeV(long double a);
PhysicalVariable operator "" _TeV(unsigned long long int a);
PhysicalVariable operator "" _PeV(long double a);
PhysicalVariable operator "" _PeV(unsigned long long int a);
PhysicalVariable operator "" _EeV(long double a);
PhysicalVariable operator "" _EeV(unsigned long long int a);
PhysicalVariable operator "" _ZeV(long double a);
PhysicalVariable operator "" _ZeV(unsigned long long int a);
PhysicalVariable operator "" _YeV(long double a);
PhysicalVariable operator "" _YeV(unsigned long long int a);

PhysicalVariable operator "" _u(long double a);
PhysicalVariable operator "" _u(unsigned long long int a);

PhysicalVariable operator "" _yN(long double a);
PhysicalVariable operator "" _yN(unsigned long long int a);
PhysicalVariable operator "" _zN(long double a);
PhysicalVariable operator "" _zN(unsigned long long int a);
PhysicalVariable operator "" _aN(long double a);
PhysicalVariable operator "" _aN(unsigned long long int a);
PhysicalVariable operator "" _fN(long double a);
PhysicalVariable operator "" _fN(unsigned long long int a);
PhysicalVariable operator "" _pN(long double a);
PhysicalVariable operator "" _pN(unsigned long long int a);
PhysicalVariable operator "" _nN(long double a);
PhysicalVariable operator "" _nN(unsigned long long int a);
PhysicalVariable operator "" _muN(long double a);
PhysicalVariable operator "" _muN(unsigned long long int a);
PhysicalVariable operator "" _mN(long double a);
PhysicalVariable operator "" _mN(unsigned long long int a);
PhysicalVariable operator "" _N(long double a);
PhysicalVariable operator "" _N(unsigned long long int a);
PhysicalVariable operator "" _kN(long double a);
PhysicalVariable operator "" _kN(unsigned long long int a);
PhysicalVariable operator "" _MN(long double a);
PhysicalVariable operator "" _MN(unsigned long long int a);
PhysicalVariable operator "" _GN(long double a);
PhysicalVariable operator "" _GN(unsigned long long int a);
PhysicalVariable operator "" _TN(long double a);
PhysicalVariable operator "" _TN(unsigned long long int a);
PhysicalVariable operator "" _PN(long double a);
PhysicalVariable operator "" _PN(unsigned long long int a);
PhysicalVariable operator "" _EN(long double a);
PhysicalVariable operator "" _EN(unsigned long long int a);
PhysicalVariable operator "" _ZN(long double a);
PhysicalVariable operator "" _ZN(unsigned long long int a);
PhysicalVariable operator "" _YN(long double a);
PhysicalVariable operator "" _YN(unsigned long long int a);

PhysicalVariable operator "" _yA(long double a);
PhysicalVariable operator "" _yA(unsigned long long int a);
PhysicalVariable operator "" _zA(long double a);
PhysicalVariable operator "" _zA(unsigned long long int a);
PhysicalVariable operator "" _aA(long double a);
PhysicalVariable operator "" _aA(unsigned long long int a);
PhysicalVariable operator "" _fA(long double a);
PhysicalVariable operator "" _fA(unsigned long long int a);
PhysicalVariable operator "" _pA(long double a);
PhysicalVariable operator "" _pA(unsigned long long int a);
PhysicalVariable operator "" _nA(long double a);
PhysicalVariable operator "" _nA(unsigned long long int a);
PhysicalVariable operator "" _muA(long double a);
PhysicalVariable operator "" _muA(unsigned long long int a);
PhysicalVariable operator "" _mA(long double a);
PhysicalVariable operator "" _mA(unsigned long long int a);
PhysicalVariable operator "" _A(long double a);
PhysicalVariable operator "" _A(unsigned long long int a);
PhysicalVariable operator "" _kA(long double a);
PhysicalVariable operator "" _kA(unsigned long long int a);
PhysicalVariable operator "" _MA(long double a);
PhysicalVariable operator "" _MA(unsigned long long int a);
PhysicalVariable operator "" _GA(long double a);
PhysicalVariable operator "" _GA(unsigned long long int a);
PhysicalVariable operator "" _TA(long double a);
PhysicalVariable operator "" _TA(unsigned long long int a);
PhysicalVariable operator "" _PA(long double a);
PhysicalVariable operator "" _PA(unsigned long long int a);
PhysicalVariable operator "" _EA(long double a);
PhysicalVariable operator "" _EA(unsigned long long int a);
PhysicalVariable operator "" _ZA(long double a);
PhysicalVariable operator "" _ZA(unsigned long long int a);
PhysicalVariable operator "" _YA(long double a);
PhysicalVariable operator "" _YA(unsigned long long int a);

PhysicalVariable operator "" _K(long double a);
PhysicalVariable operator "" _K(unsigned long long int a);

PhysicalVariable operator "" _mmol(long double a);
PhysicalVariable operator "" _mmol(unsigned long long int a);
PhysicalVariable operator "" _mol(long double a);
PhysicalVariable operator "" _mol(unsigned long long int a);
PhysicalVariable operator "" _kmol(long double a);
PhysicalVariable operator "" _kmol(unsigned long long int a);

PhysicalVariable operator "" _Cd(long double a);
PhysicalVariable operator "" _Cd(unsigned long long int a);

PhysicalVariable operator "" _rad(long double a);
PhysicalVariable operator "" _rad(unsigned long long int a);

PhysicalVariable operator "" _yHz(long double a);
PhysicalVariable operator "" _yHz(unsigned long long int a);
PhysicalVariable operator "" _zHz(long double a);
PhysicalVariable operator "" _zHz(unsigned long long int a);
PhysicalVariable operator "" _aHz(long double a);
PhysicalVariable operator "" _aHz(unsigned long long int a);
PhysicalVariable operator "" _fHz(long double a);
PhysicalVariable operator "" _fHz(unsigned long long int a);
PhysicalVariable operator "" _pHz(long double a);
PhysicalVariable operator "" _pHz(unsigned long long int a);
PhysicalVariable operator "" _nHz(long double a);
PhysicalVariable operator "" _nHz(unsigned long long int a);
PhysicalVariable operator "" _muHz(long double a);
PhysicalVariable operator "" _muHz(unsigned long long int a);
PhysicalVariable operator "" _mHz(long double a);
PhysicalVariable operator "" _mHz(unsigned long long int a);
PhysicalVariable operator "" _Hz(long double a);
PhysicalVariable operator "" _Hz(unsigned long long int a);
PhysicalVariable operator "" _kHz(long double a);
PhysicalVariable operator "" _kHz(unsigned long long int a);
PhysicalVariable operator "" _MHz(long double a);
PhysicalVariable operator "" _MHz(unsigned long long int a);
PhysicalVariable operator "" _GHz(long double a);
PhysicalVariable operator "" _GHz(unsigned long long int a);
PhysicalVariable operator "" _THz(long double a);
PhysicalVariable operator "" _THz(unsigned long long int a);
PhysicalVariable operator "" _PHz(long double a);
PhysicalVariable operator "" _PHz(unsigned long long int a);
PhysicalVariable operator "" _EHz(long double a);
PhysicalVariable operator "" _EHz(unsigned long long int a);
PhysicalVariable operator "" _ZHz(long double a);
PhysicalVariable operator "" _ZHz(unsigned long long int a);
PhysicalVariable operator "" _YHz(long double a);
PhysicalVariable operator "" _YHz(unsigned long long int a);

PhysicalVariable operator "" _yPa(long double a);
PhysicalVariable operator "" _yPa(unsigned long long int a);
PhysicalVariable operator "" _zPa(long double a);
PhysicalVariable operator "" _zPa(unsigned long long int a);
PhysicalVariable operator "" _aPa(long double a);
PhysicalVariable operator "" _aPa(unsigned long long int a);
PhysicalVariable operator "" _fPa(long double a);
PhysicalVariable operator "" _fPa(unsigned long long int a);
PhysicalVariable operator "" _pPa(long double a);
PhysicalVariable operator "" _pPa(unsigned long long int a);
PhysicalVariable operator "" _nPa(long double a);
PhysicalVariable operator "" _nPa(unsigned long long int a);
PhysicalVariable operator "" _muPa(long double a);
PhysicalVariable operator "" _muPa(unsigned long long int a);
PhysicalVariable operator "" _mPa(long double a);
PhysicalVariable operator "" _mPa(unsigned long long int a);
PhysicalVariable operator "" _Pa(long double a);
PhysicalVariable operator "" _Pa(unsigned long long int a);
PhysicalVariable operator "" _hPa(long double a);
PhysicalVariable operator "" _hPa(unsigned long long int a);
PhysicalVariable operator "" _kPa(long double a);
PhysicalVariable operator "" _kPa(unsigned long long int a);
PhysicalVariable operator "" _MPa(long double a);
PhysicalVariable operator "" _MPa(unsigned long long int a);
PhysicalVariable operator "" _GPa(long double a);
PhysicalVariable operator "" _GPa(unsigned long long int a);
PhysicalVariable operator "" _TPa(long double a);
PhysicalVariable operator "" _TPa(unsigned long long int a);
PhysicalVariable operator "" _PPa(long double a);
PhysicalVariable operator "" _PPa(unsigned long long int a);
PhysicalVariable operator "" _EPa(long double a);
PhysicalVariable operator "" _EPa(unsigned long long int a);
PhysicalVariable operator "" _ZPa(long double a);
PhysicalVariable operator "" _ZPa(unsigned long long int a);
PhysicalVariable operator "" _YPa(long double a);
PhysicalVariable operator "" _YPa(unsigned long long int a);

PhysicalVariable operator "" _yJ(long double a);
PhysicalVariable operator "" _yJ(unsigned long long int a);
PhysicalVariable operator "" _zJ(long double a);
PhysicalVariable operator "" _zJ(unsigned long long int a);
PhysicalVariable operator "" _aJ(long double a);
PhysicalVariable operator "" _aJ(unsigned long long int a);
PhysicalVariable operator "" _fJ(long double a);
PhysicalVariable operator "" _fJ(unsigned long long int a);
PhysicalVariable operator "" _pJ(long double a);
PhysicalVariable operator "" _pJ(unsigned long long int a);
PhysicalVariable operator "" _nJ(long double a);
PhysicalVariable operator "" _nJ(unsigned long long int a);
PhysicalVariable operator "" _muJ(long double a);
PhysicalVariable operator "" _muJ(unsigned long long int a);
PhysicalVariable operator "" _mJ(long double a);
PhysicalVariable operator "" _mJ(unsigned long long int a);
PhysicalVariable operator "" _J(long double a);
PhysicalVariable operator "" _J(unsigned long long int a);
PhysicalVariable operator "" _kJ(long double a);
PhysicalVariable operator "" _kJ(unsigned long long int a);
PhysicalVariable operator "" _MJ(long double a);
PhysicalVariable operator "" _MJ(unsigned long long int a);
PhysicalVariable operator "" _GJ(long double a);
PhysicalVariable operator "" _GJ(unsigned long long int a);
PhysicalVariable operator "" _TJ(long double a);
PhysicalVariable operator "" _TJ(unsigned long long int a);
PhysicalVariable operator "" _PJ(long double a);
PhysicalVariable operator "" _PJ(unsigned long long int a);
PhysicalVariable operator "" _EJ(long double a);
PhysicalVariable operator "" _EJ(unsigned long long int a);
PhysicalVariable operator "" _ZJ(long double a);
PhysicalVariable operator "" _ZJ(unsigned long long int a);
PhysicalVariable operator "" _YJ(long double a);
PhysicalVariable operator "" _YJ(unsigned long long int a);

PhysicalVariable operator "" _yW(long double a);
PhysicalVariable operator "" _yW(unsigned long long int a);
PhysicalVariable operator "" _zW(long double a);
PhysicalVariable operator "" _zW(unsigned long long int a);
PhysicalVariable operator "" _aW(long double a);
PhysicalVariable operator "" _aW(unsigned long long int a);
PhysicalVariable operator "" _fW(long double a);
PhysicalVariable operator "" _fW(unsigned long long int a);
PhysicalVariable operator "" _pW(long double a);
PhysicalVariable operator "" _pW(unsigned long long int a);
PhysicalVariable operator "" _nW(long double a);
PhysicalVariable operator "" _nW(unsigned long long int a);
PhysicalVariable operator "" _muW(long double a);
PhysicalVariable operator "" _muW(unsigned long long int a);
PhysicalVariable operator "" _mW(long double a);
PhysicalVariable operator "" _mW(unsigned long long int a);
PhysicalVariable operator "" _W(long double a);
PhysicalVariable operator "" _W(unsigned long long int a);
PhysicalVariable operator "" _kW(long double a);
PhysicalVariable operator "" _kW(unsigned long long int a);
PhysicalVariable operator "" _MW(long double a);
PhysicalVariable operator "" _MW(unsigned long long int a);
PhysicalVariable operator "" _GW(long double a);
PhysicalVariable operator "" _GW(unsigned long long int a);
PhysicalVariable operator "" _TW(long double a);
PhysicalVariable operator "" _TW(unsigned long long int a);
PhysicalVariable operator "" _PW(long double a);
PhysicalVariable operator "" _PW(unsigned long long int a);
PhysicalVariable operator "" _EW(long double a);
PhysicalVariable operator "" _EW(unsigned long long int a);
PhysicalVariable operator "" _ZW(long double a);
PhysicalVariable operator "" _ZW(unsigned long long int a);
PhysicalVariable operator "" _YW(long double a);
PhysicalVariable operator "" _YW(unsigned long long int a);

PhysicalVariable operator "" _mC(long double a);
PhysicalVariable operator "" _mC(unsigned long long int a);
PhysicalVariable operator "" _C(long double a);
PhysicalVariable operator "" _C(unsigned long long int a);
PhysicalVariable operator "" _kC(long double a);
PhysicalVariable operator "" _kC(unsigned long long int a);

PhysicalVariable operator "" _yV(long double a);
PhysicalVariable operator "" _yV(unsigned long long int a);
PhysicalVariable operator "" _zV(long double a);
PhysicalVariable operator "" _zV(unsigned long long int a);
PhysicalVariable operator "" _aV(long double a);
PhysicalVariable operator "" _aV(unsigned long long int a);
PhysicalVariable operator "" _fV(long double a);
PhysicalVariable operator "" _fV(unsigned long long int a);
PhysicalVariable operator "" _pV(long double a);
PhysicalVariable operator "" _pV(unsigned long long int a);
PhysicalVariable operator "" _nV(long double a);
PhysicalVariable operator "" _nV(unsigned long long int a);
PhysicalVariable operator "" _muV(long double a);
PhysicalVariable operator "" _muV(unsigned long long int a);
PhysicalVariable operator "" _mV(long double a);
PhysicalVariable operator "" _mV(unsigned long long int a);
PhysicalVariable operator "" _V(long double a);
PhysicalVariable operator "" _V(unsigned long long int a);
PhysicalVariable operator "" _kV(long double a);
PhysicalVariable operator "" _kV(unsigned long long int a);
PhysicalVariable operator "" _MV(long double a);
PhysicalVariable operator "" _MV(unsigned long long int a);
PhysicalVariable operator "" _GV(long double a);
PhysicalVariable operator "" _GV(unsigned long long int a);
PhysicalVariable operator "" _TV(long double a);
PhysicalVariable operator "" _TV(unsigned long long int a);
PhysicalVariable operator "" _PV(long double a);
PhysicalVariable operator "" _PV(unsigned long long int a);
PhysicalVariable operator "" _EV(long double a);
PhysicalVariable operator "" _EV(unsigned long long int a);
PhysicalVariable operator "" _ZV(long double a);
PhysicalVariable operator "" _ZV(unsigned long long int a);
PhysicalVariable operator "" _YV(long double a);
PhysicalVariable operator "" _YV(unsigned long long int a);

PhysicalVariable operator "" _yF(long double a);
PhysicalVariable operator "" _yF(unsigned long long int a);
PhysicalVariable operator "" _zF(long double a);
PhysicalVariable operator "" _zF(unsigned long long int a);
PhysicalVariable operator "" _aF(long double a);
PhysicalVariable operator "" _aF(unsigned long long int a);
PhysicalVariable operator "" _fF(long double a);
PhysicalVariable operator "" _fF(unsigned long long int a);
PhysicalVariable operator "" _pF(long double a);
PhysicalVariable operator "" _pF(unsigned long long int a);
PhysicalVariable operator "" _nF(long double a);
PhysicalVariable operator "" _nF(unsigned long long int a);
PhysicalVariable operator "" _muF(long double a);
PhysicalVariable operator "" _muF(unsigned long long int a);
PhysicalVariable operator "" _mF(long double a);
PhysicalVariable operator "" _mF(unsigned long long int a);
PhysicalVariable operator "" _F(long double a);
PhysicalVariable operator "" _F(unsigned long long int a);
PhysicalVariable operator "" _kF(long double a);
PhysicalVariable operator "" _kF(unsigned long long int a);
PhysicalVariable operator "" _MF(long double a);
PhysicalVariable operator "" _MF(unsigned long long int a);

PhysicalVariable operator "" _yohm(long double a);
PhysicalVariable operator "" _yohm(unsigned long long int a);
PhysicalVariable operator "" _zohm(long double a);
PhysicalVariable operator "" _zohm(unsigned long long int a);
PhysicalVariable operator "" _aohm(long double a);
PhysicalVariable operator "" _aohm(unsigned long long int a);
PhysicalVariable operator "" _fohm(long double a);
PhysicalVariable operator "" _fohm(unsigned long long int a);
PhysicalVariable operator "" _pohm(long double a);
PhysicalVariable operator "" _pohm(unsigned long long int a);
PhysicalVariable operator "" _nohm(long double a);
PhysicalVariable operator "" _nohm(unsigned long long int a);
PhysicalVariable operator "" _muohm(long double a);
PhysicalVariable operator "" _muohm(unsigned long long int a);
PhysicalVariable operator "" _mohm(long double a);
PhysicalVariable operator "" _mohm(unsigned long long int a);
PhysicalVariable operator "" _ohm(long double a);
PhysicalVariable operator "" _ohm(unsigned long long int a);
PhysicalVariable operator "" _kohm(long double a);
PhysicalVariable operator "" _kohm(unsigned long long int a);
PhysicalVariable operator "" _Mohm(long double a);
PhysicalVariable operator "" _Mohm(unsigned long long int a);
PhysicalVariable operator "" _Gohm(long double a);
PhysicalVariable operator "" _Gohm(unsigned long long int a);
PhysicalVariable operator "" _Tohm(long double a);
PhysicalVariable operator "" _Tohm(unsigned long long int a);
PhysicalVariable operator "" _Pohm(long double a);
PhysicalVariable operator "" _Pohm(unsigned long long int a);
PhysicalVariable operator "" _Eohm(long double a);
PhysicalVariable operator "" _Eohm(unsigned long long int a);
PhysicalVariable operator "" _Zohm(long double a);
PhysicalVariable operator "" _Zohm(unsigned long long int a);
PhysicalVariable operator "" _Yohm(long double a);
PhysicalVariable operator "" _Yohm(unsigned long long int a);

PhysicalVariable operator "" _S(long double a);
PhysicalVariable operator "" _S(unsigned long long int a);

PhysicalVariable operator "" _Wb(long double a);
PhysicalVariable operator "" _Wb(unsigned long long int a);

PhysicalVariable operator "" _yT(long double a);
PhysicalVariable operator "" _yT(unsigned long long int a);
PhysicalVariable operator "" _zT(long double a);
PhysicalVariable operator "" _zT(unsigned long long int a);
PhysicalVariable operator "" _aT(long double a);
PhysicalVariable operator "" _aT(unsigned long long int a);
PhysicalVariable operator "" _fT(long double a);
PhysicalVariable operator "" _fT(unsigned long long int a);
PhysicalVariable operator "" _pT(long double a);
PhysicalVariable operator "" _pT(unsigned long long int a);
PhysicalVariable operator "" _nT(long double a);
PhysicalVariable operator "" _nT(unsigned long long int a);
PhysicalVariable operator "" _muT(long double a);
PhysicalVariable operator "" _muT(unsigned long long int a);
PhysicalVariable operator "" _mT(long double a);
PhysicalVariable operator "" _mT(unsigned long long int a);
PhysicalVariable operator "" _T(long double a);
PhysicalVariable operator "" _T(unsigned long long int a);
PhysicalVariable operator "" _kT(long double a);
PhysicalVariable operator "" _kT(unsigned long long int a);
PhysicalVariable operator "" _MT(long double a);
PhysicalVariable operator "" _MT(unsigned long long int a);
PhysicalVariable operator "" _GT(long double a);
PhysicalVariable operator "" _GT(unsigned long long int a);
PhysicalVariable operator "" _TT(long double a);
PhysicalVariable operator "" _TT(unsigned long long int a);
PhysicalVariable operator "" _PT(long double a);
PhysicalVariable operator "" _PT(unsigned long long int a);
PhysicalVariable operator "" _ET(long double a);
PhysicalVariable operator "" _ET(unsigned long long int a);
PhysicalVariable operator "" _ZT(long double a);
PhysicalVariable operator "" _ZT(unsigned long long int a);
PhysicalVariable operator "" _YT(long double a);
PhysicalVariable operator "" _YT(unsigned long long int a);

PhysicalVariable operator "" _yH(long double a);
PhysicalVariable operator "" _yH(unsigned long long int a);
PhysicalVariable operator "" _zH(long double a);
PhysicalVariable operator "" _zH(unsigned long long int a);
PhysicalVariable operator "" _aH(long double a);
PhysicalVariable operator "" _aH(unsigned long long int a);
PhysicalVariable operator "" _fH(long double a);
PhysicalVariable operator "" _fH(unsigned long long int a);
PhysicalVariable operator "" _pH(long double a);
PhysicalVariable operator "" _pH(unsigned long long int a);
PhysicalVariable operator "" _nH(long double a);
PhysicalVariable operator "" _nH(unsigned long long int a);
PhysicalVariable operator "" _muH(long double a);
PhysicalVariable operator "" _muH(unsigned long long int a);
PhysicalVariable operator "" _mH(long double a);
PhysicalVariable operator "" _mH(unsigned long long int a);
PhysicalVariable operator "" _H(long double a);
PhysicalVariable operator "" _H(unsigned long long int a);
PhysicalVariable operator "" _kH(long double a);
PhysicalVariable operator "" _kH(unsigned long long int a);
PhysicalVariable operator "" _MH(long double a);
PhysicalVariable operator "" _MH(unsigned long long int a);
PhysicalVariable operator "" _GH(long double a);
PhysicalVariable operator "" _GH(unsigned long long int a);
PhysicalVariable operator "" _TH(long double a);
PhysicalVariable operator "" _TH(unsigned long long int a);
PhysicalVariable operator "" _PH(long double a);
PhysicalVariable operator "" _PH(unsigned long long int a);
PhysicalVariable operator "" _EH(long double a);
PhysicalVariable operator "" _EH(unsigned long long int a);
PhysicalVariable operator "" _ZH(long double a);
PhysicalVariable operator "" _ZH(unsigned long long int a);
PhysicalVariable operator "" _YH(long double a);
PhysicalVariable operator "" _YH(unsigned long long int a);

PhysicalVariable operator "" _degC(long double a);
PhysicalVariable operator "" _degC(unsigned long long int a);

PhysicalVariable operator "" _yBc(long double a);
PhysicalVariable operator "" _yBc(unsigned long long int a);
PhysicalVariable operator "" _zBc(long double a);
PhysicalVariable operator "" _zBc(unsigned long long int a);
PhysicalVariable operator "" _aBc(long double a);
PhysicalVariable operator "" _aBc(unsigned long long int a);
PhysicalVariable operator "" _fBc(long double a);
PhysicalVariable operator "" _fBc(unsigned long long int a);
PhysicalVariable operator "" _pBc(long double a);
PhysicalVariable operator "" _pBc(unsigned long long int a);
PhysicalVariable operator "" _nBc(long double a);
PhysicalVariable operator "" _nBc(unsigned long long int a);
PhysicalVariable operator "" _muBc(long double a);
PhysicalVariable operator "" _muBc(unsigned long long int a);
PhysicalVariable operator "" _mBc(long double a);
PhysicalVariable operator "" _mBc(unsigned long long int a);
PhysicalVariable operator "" _Bc(long double a);
PhysicalVariable operator "" _Bc(unsigned long long int a);
PhysicalVariable operator "" _kBc(long double a);
PhysicalVariable operator "" _kBc(unsigned long long int a);
PhysicalVariable operator "" _MBc(long double a);
PhysicalVariable operator "" _MBc(unsigned long long int a);
PhysicalVariable operator "" _GBc(long double a);
PhysicalVariable operator "" _GBc(unsigned long long int a);
PhysicalVariable operator "" _TBc(long double a);
PhysicalVariable operator "" _TBc(unsigned long long int a);
PhysicalVariable operator "" _PBc(long double a);
PhysicalVariable operator "" _PBc(unsigned long long int a);
PhysicalVariable operator "" _EBc(long double a);
PhysicalVariable operator "" _EBc(unsigned long long int a);
PhysicalVariable operator "" _ZBc(long double a);
PhysicalVariable operator "" _ZBc(unsigned long long int a);
PhysicalVariable operator "" _YBc(long double a);
PhysicalVariable operator "" _YBc(unsigned long long int a);

const auto mu_0 = (1.e-7_H / 1._m) * 4. * M_PI;

#endif // C11UNITS_CPP_INCLUDED
