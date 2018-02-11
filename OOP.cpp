/*The task is to implement the CVATRegister class, which will implement a database of VAT control reports.

A system of control reports is introduced for the planned more rigorous combating of tax evasion. 
There are established individual companies in the database, and the individual invoices issued by the company are recorded in the database. 
Companies can be entered into the database and can be erased. 
The company is identified by its name, address, and tax ID (id).
The tax identifier is unique throughout the database. 
Names and addresses may be repeated, but the pair (name, address) is unique in the database again. 
Thus, many ACME companies can be found in the database, many companies may have the address in Prague, but ACME residing in Prague can only be one in the database. 
When comparing tax identifiers, we distinguish between lower case and upper case letters, and for case of names and addresses, we do not distinguish between upper and lower case.*/

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;


string toLower(const string & s)
{
    string h = s;
    for(unsigned int i = 0; i < h.size(); i++)
    {
        if(h[i] >= 'A' && h[i] <= 'Z')
            h[i] += 32;
    }
    return h;
}

class CVATRegister {
private:
    struct Company
    {
        string nameaddr, id;
        unsigned int sum;
        Company() { sum = 0; nameaddr = ""; }
    };
    vector< Company > idCompany;  //vector of company's ids
    vector<string>::iterator iid, iname, uname, oid; //iterators for name and id for binary search
    vector< Company > nameCompany; //vector of company's names
    vector< string > names;   //sorted vector of company's names
    vector< string > ids;     //sorted vector of company's ids
    vector< int > invoices; //vector of all invoices of companies
    
    
public:
    CVATRegister(void) {};
    ~CVATRegister(void) {};
    /*Method NewCompany(name, addr, id) adds to existing database next record. 
    Parameters name and addr reprezentate name a address, parameter id indicates the tax identifier. 
    Method returns true, if the record has been added, or false, if it hasn't been added. 
    (because there was already a record with the same name and address already in the database, or a record with the same id).*/
    bool NewCompany(const string & name,
            const string & addr,
            const string & taxID)
    {
        string name1 = toLower(name);
        string addr1 = toLower(addr);
        string name2 = name1 + " abdcefgz " + addr1; //I need to separate name from address because some records can have name = name1addr1.
        if(names.size() != 0)
        {
            iname = lower_bound(names.begin(), names.end(), name2); //The lower_bound of name2
            uname = upper_bound(names.begin(), names.end(), name2); //The upper_bound of name2
            iid = lower_bound(ids.begin(), ids.end(), taxID);  
            oid = upper_bound(ids.begin(), ids.end(), taxID);
            if((uname - names.begin()- 1) >= (iname - names.begin()) || (oid - ids.begin() - 1) >= (iid - ids.begin())) //
                    return false;
        }
        iname = lower_bound(names.begin(), names.end(), name2 );
        iid = lower_bound(ids.begin(), ids.end(), taxID );
        names.insert(iname, name2);
        ids.insert(iid, taxID);
        iname = lower_bound(names.begin(), names.end(), name2);
        iid = lower_bound(ids.begin(), ids.end(), taxID);
        Company a;
        a.nameaddr += name1;
        a.nameaddr += " abdcefgz ";
        a.nameaddr += addr1;
        a.id = taxID;
        
        nameCompany.insert(nameCompany.begin()+(iname - names.begin()), a);
        idCompany.insert(idCompany.begin()+(iid - ids.begin()), a);
        return true;
    }
    /*Method CancelCompany (name, addr) deletes the record from the database. 
    The parameter is a unique identification using the name and address. 
    If the record was actually deleted, the method returns true. If the record wasn't deleted (because there was no company with this name and address), the method returns false.*/
    bool CancelCompany(const string & name,
            const string & addr)
    {
        string name1 = toLower(name);
        string addr1 = toLower(addr);
        string name2 = name1 + " abdcefgz "+ addr1;
        iname = lower_bound(names.begin(), names.end(), name2);     
        uname = upper_bound(names.begin(), names.end(), name2);
        if((uname - names.begin() - 1) < (iname-names.begin()))
            return false;
       	idCompany.erase(idCompany.begin() + (iid-ids.begin()));
        nameCompany.erase(nameCompany.begin() + (iname - names.begin()));
        names.erase(names.begin() + (iname - names.begin()));
        ids.erase(ids.begin() + (iid-ids.begin()));
        return true;
    }
    /*Method CancelCompany (id) deletes the record from the database. 
    The parameter is a unique identification using tax identifier. 
    If the record was actually deleted, the method returns true. If the record haven't been deleted (because there was no company with this id), the method returns false.*/
    bool CancelCompany(const string & taxID)
    {
        iid = lower_bound(ids.begin(), ids.end(), taxID);
        oid = upper_bound(ids.begin(), ids.end(), taxID);
        int beg = oid - ids.begin() - 1;
        if(beg < (iid - ids.begin()))
            return false;
        iname = lower_bound(names.begin(), names.end(), idCompany[beg].nameaddr);  
        uname = upper_bound(names.begin(), names.end(), idCompany[beg].nameaddr);
        if((uname - names.begin() - 1) < (iname-names.begin()))
            return false;
        
        if(nameCompany[iname - names.begin()].id == taxID)
        {
                
                idCompany.erase(idCompany.begin() + beg);
                nameCompany.erase(nameCompany.begin() + (iname - names.begin()));
                ids.erase(ids.begin() + beg);
                names.erase(names.begin() + (iname - names.begin()));
                return true;
        }
        return false;
        
    }
    
    /*Method Invoice (id, amount) will record revenue in the amount. The company is identified by the tax identifier. 
    If the method succeeds, returns true, if not, it returns false (non-existent company).*/
    bool Invoice(const string & taxID,
            unsigned int amount)
    {
        iid = lower_bound(ids.begin(), ids.end(), taxID);
        oid = upper_bound(ids.begin(), ids.end(), taxID);
        int beg = oid - ids.begin() - 1;
        if(beg < (iid - ids.begin()))
            return false;
        iname = lower_bound(names.begin(), names.end(), idCompany[beg].nameaddr);  
        uname = upper_bound(names.begin(), names.end(), idCompany[beg].nameaddr);
        if((uname - names.begin() - 1) < (iname-names.begin()))
            return false;
        
        if(nameCompany[iname - names.begin()].id == taxID)
        {
                
                idCompany[beg].sum += amount;
                nameCompany[iname - names.begin()].sum += amount;
                invoices.push_back(amount);
                return true;
        }
        return false;
        
    }
    /*Method Invoice (name, address, amount) will record revenue in the amount. The company is identified by its name and address. 
    If the method succeeds, returns true, if not, it returns false (non-existent company).*/
    bool Invoice(const string & name,
            const string & addr,
            unsigned int amount)
    {
        string name1 = toLower(name);
        string addr1 = toLower(addr);
        string name2 = name1 + " abdcefgz "+addr1;
        iname = lower_bound(names.begin(), names.end(), name2);     
        uname = upper_bound(names.begin(), names.end(), name2);
        if((uname - names.begin() - 1) < (iname-names.begin()))
            return false;
               
        iid = lower_bound(ids.begin(), ids.end(), nameCompany[iname - names.begin()].id);
        if(name2 == idCompany[iid - ids.begin()].nameaddr)
        {
                idCompany[iid - ids.begin()].sum += amount;
                nameCompany[iname - names.begin()].sum += amount;
                invoices.push_back(amount);
                return true;
        }
        return false;
        
    }
    /*The Audit method (name, addr, sum) searches for the sum of revenue for the company with the specified name and address. 
    Found sum method saves to the output parameter sum. The method will return true if it successes, false for failure (non-existent company).*/
    bool Audit(const string & name,
            const string & addr,
            unsigned int & sumIncome) const
    {
        vector<string>::iterator io, uo, ik;
        io = iname;
        uo - uname;
        ik = iid;
        vector<string> doplnName, doplnID;
        doplnName = names;
        doplnID = ids;
        string name1 = toLower(name);
        string addr1 = toLower(addr);
        string name2 = name1 + " abdcefgz "+addr1;
        io = lower_bound(doplnName.begin(), doplnName.end(), name2);     
        uo = upper_bound(doplnName.begin(), doplnName.end(), name2);
        if((uo - doplnName.begin() - 1) < (io-doplnName.begin()))
            return false;
        ik = lower_bound(doplnID.begin(), doplnID.end(), nameCompany[io - doplnName.begin()].id);
        if(name2 == idCompany[ik - doplnID.begin()].nameaddr)
        {
            sumIncome = idCompany[ik - doplnID.begin()].sum;     
            return true;
        }
        return false;
    
    }
    /*Method Audit(id, sum) searches for the sum of revenue for the company identified by the tax identifier. 
    Found sum method saves to the output parameter sum. The method will return true if it successes, false for failure (non-existent company).*/
    bool Audit(const string & taxID,
            unsigned int & sumIncome) const
    {
        vector<string>::iterator io, uo, ik, ok;
        vector<string> doplnName, doplnID;
        io = iname;
        uo = uname;
        ik = iid;
        ok = oid;
        doplnName = names;
        doplnID = ids;
        ik = lower_bound(doplnID.begin(), doplnID.end(), taxID);
        ok = upper_bound(doplnID.begin(), doplnID.end(), taxID);
        int beg = ok - doplnID.begin() - 1;
        if(beg < (ik - doplnID.begin()))
            return false;
        io = lower_bound(doplnName.begin(), doplnName.end(), idCompany[beg].nameaddr);  
        uo = upper_bound(doplnName.begin(), doplnName.end(), idCompany[beg].nameaddr);
        if(nameCompany[io - doplnName.begin()].id == taxID)
        {             
            sumIncome = idCompany[ik - doplnID.begin()].sum;
            return true;
        }
        return false;
    }
    /*Method MedianInvoice () looks for the median of the invoice value. 
    The calculated median counts all successfully processed invoices entered by calling the Invoice. 
    Thus, invoices that could not be assigned (Invoice call failed) are not counted, but all invoices of erased companies are counted
    (when the company is erased, its invoices are not removed from the median calculation). 
    If an even number of invoices is entered in the system, it will take the higher of two middle values. 
    If the system has not yet processed any invoice, the value of 0 will be returned.*/
    unsigned int MedianInvoice(void) const
    {
        if(invoices.size() == 0)
            return 0;
        vector < int > mas;
        mas = invoices;
        sort(mas.begin(), mas.end());
        return mas[ceil(mas.size()/2)];
    }

};


int main(void) {
  unsigned int sumIncome;
  CVATRegister b1;
  assert ( b1 . NewCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . NewCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . NewCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . Invoice ( "666/666", 2000 ) );
  assert ( b1 . MedianInvoice () == 2000 );
  assert ( b1 . Invoice ( "666/666/666", 3000 ) );
  assert ( b1 . MedianInvoice () == 3000 );
  assert ( b1 . Invoice ( "123456", 4000 ) );
  assert ( b1 . MedianInvoice () == 3000 );
  assert ( b1 . Invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . MedianInvoice () == 4000 );
  assert ( b1 . Audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . Audit ( "123456", sumIncome ) && sumIncome == 4000 );
  assert ( b1 . CancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . MedianInvoice () == 4000 );
  assert ( b1 . CancelCompany ( "666/666" ) );
  assert ( b1 . MedianInvoice () == 4000 );
  assert ( b1 . Invoice ( "123456", 100 ) );
  assert ( b1 . MedianInvoice () == 3000 );
  assert ( b1 . Invoice ( "123456", 300 ) );
  assert ( b1 . MedianInvoice () == 3000 );
  assert ( b1 . Invoice ( "123456", 200 ) );
  assert ( b1 . MedianInvoice () == 2000 );
  assert ( b1 . Invoice ( "123456", 230 ) );
  assert ( b1 . MedianInvoice () == 2000 );
  assert ( b1 . Invoice ( "123456", 830 ) );
  assert ( b1 . MedianInvoice () == 830 );
  assert ( b1 . Invoice ( "123456", 1830 ) );
  assert ( b1 . MedianInvoice () == 1830 );
  assert ( b1 . Invoice ( "123456", 2830 ) );
  assert ( b1 . MedianInvoice () == 1830 );
  assert ( b1 . Invoice ( "123456", 2830 ) );
  assert ( b1 . MedianInvoice () == 2000 );
  assert ( b1 . Invoice ( "123456", 3200 ) );
  assert ( b1 . MedianInvoice () == 2000 );

  CVATRegister b2;
  assert ( b2 . NewCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . NewCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . NewCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . NewCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . MedianInvoice () == 0 );
  assert ( b2 . Invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . MedianInvoice () == 1000 );
  assert ( b2 . Invoice ( "abcdef", 2000 ) );
  assert ( b2 . MedianInvoice () == 2000 );
  assert ( b2 . Invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . MedianInvoice () == 2000 );
  assert ( ! b2 . Invoice ( "1234567", 100 ) );
  assert ( ! b2 . Invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . Invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . Audit ( "1234567", sumIncome ) );
  assert ( ! b2 . Audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . Audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . CancelCompany ( "1234567" ) );
  assert ( ! b2 . CancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . CancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . CancelCompany ( "abcdef" ) );
  assert ( b2 . MedianInvoice () == 2000 );
  assert ( ! b2 . CancelCompany ( "abcdef" ) );
  assert ( b2 . NewCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . CancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . CancelCompany ( "ACME", "Kolejni" ) );
  return 0;
}
