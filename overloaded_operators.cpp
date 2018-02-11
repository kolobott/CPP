/*The task is to implement the CPolynomial class, which will represent polynomials.

The CPolynomial class stores the polynomial with the coefficients and its powers. 
We assume coefficients in the form of double-digit decimal numbers. 
By using the interface (most interfaces are overloaded operators), this polynomial class can work.*/

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <complex>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;


class CPolynomial
{
public:
    vector<double> polynom;   //The indices are powers and the numbers in vector are coefficients 
public:
    CPolynomial()
    {
        polynom.resize(1000, 0); //Polynom: 0 + 0x + 0x^2 + 0x^3 .... + 0x^1000
    }
    friend CPolynomial operator +(const CPolynomial & a, const CPolynomial & b)
    {
        CPolynomial c;
        unsigned int size;
        if((a.polynom.size()) > b.polynom.size())
            size = a.polynom.size();
        else
            size = b.polynom.size();
        c.polynom.resize(size);
        for(unsigned int i = 0; i < size; i++)
        {
            c.polynom[i] = a.polynom[i] + b.polynom[i];
        }
        return c;
    }
    friend CPolynomial operator -(const CPolynomial & a, const CPolynomial & b)
    {
        CPolynomial c;
        unsigned int size;
        if((a.polynom.size()) > b.polynom.size())
            size = a.polynom.size();
        else
            size = b.polynom.size();
        c.polynom.resize(size);
        for(unsigned int i = 0; i < size; i++)
        {
            c.polynom[i] = a.polynom[i] - b.polynom[i];
        }
        return c;
    }   
    friend CPolynomial operator *(const CPolynomial & a, const CPolynomial & b)
    {
        CPolynomial c;
        c.polynom.resize(a.polynom.size()+b.polynom.size()-1);
        for(unsigned int i = 0; i < a.polynom.size(); i++)
        {
            for(unsigned int j = 0; j < b.polynom.size(); j++)
            {
                c.polynom[i + j] += a.polynom[i] * b.polynom[j];
            }
        }
        return c;
    }   
    friend CPolynomial operator *(const CPolynomial & a, const double b)
    {
        CPolynomial c;    
        c.polynom.resize(a.polynom.size());
        for(unsigned int i = 0; i < a.polynom.size(); i++)
        {
            c.polynom[i] =  a.polynom[i] * b;
        }
        return c;
    }   
    friend bool operator == (const CPolynomial & a, const CPolynomial & b )
    {
        CPolynomial c ,d;
        c = a;
        d = b;
        for(unsigned int i = c.polynom.size()-1; i >= 1; i--)
        {
            if(c.polynom[i] != 0)
            {
                c.polynom.resize(i+1);
                break;
            }
        }
        for(unsigned int i = d.polynom.size()-1; i >= 1; i--)
        {
            if(d.polynom[i] != 0)
            {
                d.polynom.resize(i+1);
                break;
            }
        }
        return c.polynom == d.polynom;
    }
    friend bool operator != (const CPolynomial & a, const CPolynomial & b )
    {
        return !(a.polynom == b.polynom);
    }
    double & operator [](int i)
    {
        if(i >= (int)polynom.size())
            polynom.resize(i+1);
        return polynom[i];
    }
    const double & operator [](int i) const
    {
        return polynom[i];
    }
    const double  operator ()(double x) const
    {
        if(polynom.size() == 0)
            return 0;
        double vysledek = polynom[0];
        for(unsigned int i = 1; i < polynom.size(); i++)
        {
            vysledek += polynom[i]*pow(x, i);
        }
        return vysledek;
    }
    const int Degree() const
    {
        if(polynom.size() == 0)
            return 0;
        for(int i = (int)polynom.size()-1; i >= 0; i--)
        {
            if(polynom[i] != 0)
                return i;
        }
        return 0;
    }
    CPolynomial & operator =(const CPolynomial & a)
    {
        polynom = a.polynom;
        return *this;
    }
    /*operator << will allow the object output to the C ++ stream. 
    The listing will be made in compact form:
	the members are shown away from the highest power,
	members with a zero coefficient will not be displayed,
	members with +1 or -1 will not display the number 1, only show the appropriate power x,
	there are no unnecessary signs in the listing - (eg x ^ 1 - 9 or - x ^ 2 + 4 is correct, but x ^ 1 + (-9) is wrong)
	the zero polynomial is displayed as a separate number of 0.
    The default variable in the displayed polynomial will be x.*/
    friend ostream & operator << ( ostream & os,const CPolynomial & x ) 
    {
        unsigned int i;
        
        bool j = 0;
        
        CPolynomial a;
        a = x;
        
        if(a.polynom.size() == 0)
        {
            os<<0;
            return (os);
        }
        for(i = a.polynom.size() - 1; i >= 1; i--)
        {
            if(a.polynom[i] != 0)
            {
                j = 1;
                a.polynom.resize(i+1);
                if(a.polynom[i] > 0)
                {
                    if(a.polynom[i] == 1)
                    {
                        os<<"x^"<<i;
                    }
                    else
                        os<<a.polynom[i]<<"*x^"<<i;
                }
                else
                {
                    if(a.polynom[i] == -1)
                    {
                        os<<"- "<<"x^"<<i;
                    }
                    else 
                        os<<"- "<<abs(a.polynom[i])<<"*x^"<<i;
                }
                break;
            }
        } 
        for(i = a.polynom.size()-2; i>= 1; i--)
        {
            
            if(a.polynom[i] == 0)
                continue;
            if(a.polynom[i] < 0)
            {
                j = 1;
               
                if(a.polynom[i] == -1)
                {
                    
                    os<<" - "<<"x^"<<i;
                }
                else
                    os<<" - "<<abs(a.polynom[i])<<"*"<<"x^"<<i;
            }
            else
            {
                j = 1;
                if(a.polynom[i] == 1)
                {
                    
                    os<<" + "<<"x^"<<i;
                }
                else                       
                    os<<" + "<<a.polynom[i]<<"*"<<"x^"<<i;
            }
        }
        if(j && a.polynom[0] != 0)
            os<<" ";
        if(a.polynom[0] < 0)
        {
            j = 1;
            os<<"- "<<abs(x.polynom[0]);
            return (os);
        }
        else if(a.polynom[0] > 0)
        {
            j = 1;
            if(a.polynom.size() == 1 || a.polynom[1] == 0)
                os<<a.polynom[0];
            else
                os<<"+ "<<a.polynom[0];
            return (os);
        }
        if(!j)
        {
            os<<0;
            return (os);
        }
        return (os);
    }  
  
};



int                main                                    ( void )
{
  CPolynomial a, b, c;
  ostringstream out;

  a[0] = -10;
  a[1] = 3.5;
  a[3] = 1;
  assert ( smallDiff ( a ( 2 ), 5 ) );
  out . str ("");
  out << a;
  assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
  a = a * -2;
  assert ( a . Degree () == 3
           && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

  out . str ("");
  out << a;
  assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
  out . str ("");
  out << b;
  assert ( out . str () == "0" );
  b[5] = -1;
  out . str ("");
  out << b;
  assert ( out . str () == "- x^5" );
  c = a + b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a - b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a * b;
  assert ( c . Degree () == 8
           && dumpMatch ( c, vector<double>{ 0.0, -0.0, 0.0, -0.0, -0.0, -20.0, 7.0, 0.0, 2.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );
  assert ( a != b );
  b[5] = 0;
  assert ( !(a == b) );
  a = a * 0;
  assert ( a . Degree () == 0
           && dumpMatch ( a, vector<double>{ 0.0 } ) );

  assert ( a == b );

  return 0;
}
