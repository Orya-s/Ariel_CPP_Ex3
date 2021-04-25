#include "NumberWithUnits.hpp"
#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

constexpr double EPS = 0.0001;
using namespace std;

namespace ariel
{
    map<string, map<string, double>> NumberWithUnits::units;
    

    void NumberWithUnits::read_units(ifstream &units_file)
    {
        string line;
        int one = 0;
        string firstUnit;
        string split = "=" ;
        double val = 0;
        string secondUnit;
            
        while (getline(units_file, line))
        {
            firstUnit = " ";
            secondUnit = " ";
            val = 1;
            istringstream iss(line);
            iss >> one >> firstUnit >> split >> val >> secondUnit;

            if (units.find(firstUnit) == units.end())  // if the unit isn't already in the map
            {
                map<string,double> inMap;   // creating the inside convertion map
                units.insert(pair<string, map<string, double>>(firstUnit,inMap));
            }

            if (units.find(secondUnit) == units.end())  // if the unit isn't already in the map
            {
                map<string,double> inMap;   // creating the inside convertion map
                units.insert(pair<string, map<string, double>>(secondUnit,inMap));                
            }
            
            for (const auto &idx : units.at(firstUnit))  // adding convertion to all the matching units
            {
                units.at(idx.first).insert(pair<string,double>(secondUnit, (1/idx.second) * val));
                units.at(secondUnit).insert(pair<string,double>(idx.first, idx.second * (1/val)));
            }    

            for (const auto &idx : units.at(secondUnit))  // adding convertion to all the matching units
            {
                units.at(idx.first).insert(pair<string,double>(firstUnit, 1/(idx.second * val)));
                units.at(firstUnit).insert(pair<string,double>(idx.first, idx.second * val));
            } 

            units.at(firstUnit).insert(pair<string,double>(secondUnit, val));
            units.at(secondUnit).insert(pair<string,double>(firstUnit, 1 / val));

        }  // end of while
        units_file.close();

        // map<string, map<string, double>>::iterator itr;
        // map<string, double>::iterator ptr;
        // for (itr = units.begin(); itr != units.end(); itr++) {
        //     for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++) {
        //         cout << "First key is " << itr->first << " And second key is " << ptr->first
        //          << " And value is " << ptr->second << endl;
        //     }
        // }
         
    }  // end of read_units


    bool canConvert(const string& firstUnit, const string& secondUnit)
    {
        return NumberWithUnits::units.at(firstUnit).find(secondUnit) != 
            NumberWithUnits::units.at(firstUnit).end(); 
    }

    // binary operators 
    NumberWithUnits NumberWithUnits::operator -(const NumberWithUnits& other) const
    {
        if (unit == other.unit)
        {
            return NumberWithUnits(num-other.num, unit);
        }
        
        if (!canConvert(unit,other.unit))
        {
            throw invalid_argument(unit + " cannot be converted to " + other.unit);
        }
        
        double conv = units.at(other.unit).at(unit);
        double ans = num - (other.num * conv);
        return NumberWithUnits(ans,unit);
    }

    NumberWithUnits NumberWithUnits::operator +(const NumberWithUnits& other) const
    {
        if (unit == other.unit)
        {
            return NumberWithUnits(num+other.num, unit);
        }
          
        if (!canConvert(unit,other.unit))
        {
            throw invalid_argument(unit + " cannot be converted to " + other.unit);
        }
            
        double conv = units.at(other.unit).at(unit);
        double ans = num + (other.num * conv);
        return NumberWithUnits(ans,unit);        
    }

    NumberWithUnits& NumberWithUnits::operator -=(const NumberWithUnits& other)
    {
        *this = *this - other;
        return *this;
    }

    NumberWithUnits& NumberWithUnits::operator +=(const NumberWithUnits& other)
    {
        *this = *this + other;
        return *this;
    }


    // friend global binary operators
    bool operator ==(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        if (n1.unit == n2.unit)
        {
            return ((abs(n1.num - n2.num)) < EPS);
        }
       
        if (!canConvert(n1.unit, n2.unit))
        {
            throw invalid_argument(n1.unit + " cannot be converted to " + n2.unit);
        }
            
        double conv = NumberWithUnits::units.at(n1.unit).at(n2.unit);
        double ans = n1.num * conv;
        return ((abs(ans - n2.num)) < EPS);
    }

    bool operator !=(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return !(n1 == n2);
    }

    bool operator >=(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return !(n1 < n2);
    }

    bool operator <=(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return !(n1 > n2);
    }

    bool operator <(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        if (n1.unit == n2.unit)
        {
            return (n1.num < n2.num);
        }

        if (!canConvert(n1.unit, n2.unit))
        {
            throw invalid_argument(n1.unit + " cannot be converted to " + n2.unit);
        }
            
        double conv = NumberWithUnits::units.at(n1.unit).at(n2.unit);
        double ans = n1.num * conv;
        return (ans < n2.num);
    }

    bool operator >(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        if (n1.unit == n2.unit)
        {
            return (n1.num > n2.num);
        }
        
        if (!canConvert(n1.unit, n2.unit))
        {
            throw invalid_argument(n1.unit + " cannot be converted to " + n2.unit);
        }
            
        double conv = NumberWithUnits::units.at(n1.unit).at(n2.unit);
        double ans = n1.num * conv;
        return (ans > n2.num);
    }


    // friend global IO operator
    istream& operator >>(istream& is, NumberWithUnits& n)
    {
        double newNum = 0;
        string newUnit = " ";
        char b1 = '\0';
        char b2 = '\0';

        is >> newNum >> b1 >> newUnit;
        if (newUnit.at(newUnit.length()-1) == ']')
        {
            newUnit = newUnit.substr(0, newUnit.length()-1);
        }
        else{
            is >> b2;
        }
        if (NumberWithUnits::units.find(newUnit) == NumberWithUnits::units.end())
        {
            throw invalid_argument("Cannot construct a NumberWithUnits from input, please try again");
        }

        n.num = newNum;
        n.unit = newUnit;
        return is;
    }

};
