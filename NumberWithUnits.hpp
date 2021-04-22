#pragma once

#include <fstream>
#include <sstream>
#include <map>
#include <string>

namespace ariel {
    class NumberWithUnits {
        private:
            double num;
            std::string unit;

        public:
            static std::map<std::string, std::map<std::string, double> > units;

            NumberWithUnits(double i, const std::string& s) : num(i), unit(s) 
            {
                if (!units.count(s))
                {
                    throw std::runtime_error("Unit is unknown, unable to construct a new NumberWithUnits");
                }
            }
            
            static void read_units(std::ifstream& units);

            // unary operators
            const NumberWithUnits operator -() const {
                return NumberWithUnits(-num, unit);
            }
            const NumberWithUnits operator +() const {
                return *this ;  // NumberWithUnits(num, unit);
            }

            // binary operators
            NumberWithUnits operator -(const NumberWithUnits& other) const;
            NumberWithUnits operator +(const NumberWithUnits& other) const;
            NumberWithUnits& operator -=(const NumberWithUnits& other);
            NumberWithUnits& operator +=(const NumberWithUnits& other);

            // prefix increment
            NumberWithUnits& operator ++(){
                num++;
                return *this;
            }
            // posfic increment
            const NumberWithUnits operator ++(int dummy_flag){
                NumberWithUnits copy = *this;
                num++;
                return copy;
            }
            // prefix decrement
            NumberWithUnits& operator --(){
                num--;
                return *this;
            }
            // posfic decrement
            const NumberWithUnits operator --(int dummy_flag){
                NumberWithUnits copy = *this;
                num--;
                return copy;
            }

            const NumberWithUnits operator *(double d) const{
                return NumberWithUnits(num*d, unit);
            }
            friend const NumberWithUnits operator *(double d, const NumberWithUnits& n){
                return NumberWithUnits(n.num*d, n.unit);
            }

            // friend global binary operators
           friend bool operator ==(const NumberWithUnits& n1, const NumberWithUnits& n2);
           friend bool operator !=(const NumberWithUnits& n1, const NumberWithUnits& n2);
           friend bool operator >=(const NumberWithUnits& n1, const NumberWithUnits& n2);
           friend bool operator <=(const NumberWithUnits& n1, const NumberWithUnits& n2);
           friend bool operator <(const NumberWithUnits& n1, const NumberWithUnits& n2);
           friend bool operator >(const NumberWithUnits& n1, const NumberWithUnits& n2);

            // friend global IO operators
            friend std::ostream& operator <<(std::ostream& os, const NumberWithUnits& n){
                return os << n.num << "[" << n.unit << "]";
            }
            friend std::istream& operator >>(std::istream& is, NumberWithUnits& n);

            ~ NumberWithUnits() {}
    };
};