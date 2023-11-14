#ifndef MYCSV_H
#define MYCSV_H

#include <map>
#include <set>
#include <string>
#include <iostream>

//v 0.1

using namespace std;

namespace myCSV_ns{
using cell_address_t = string;
using cell_data_t    = string;
using table_t        = map<cell_address_t, cell_data_t>;

struct MY_CSV_EXCEPTION{
        string exep_type;
        string place;
        string args;

        MY_CSV_EXCEPTION(const string& ntype, const string& nplace, const string& nargs):
            exep_type(ntype),place(nplace),args(nargs)
        {}

    };


bool isAddr(const cell_data_t &data);
bool isExpr(const cell_data_t &data);

void splitExpr(const cell_data_t &data, cell_data_t &op1, cell_data_t & op2, char &opr);
void splitAddr(const cell_address_t &addr, string &column, string& row);

class myCSV
{
    friend istream& operator>>(istream& is, myCSV& csv);
    friend ostream& operator<<(ostream& os, const myCSV& csv);

private:
    table_t _table;
    set<string> _rows;
    set<string> _columns;
    long long _calculate (const cell_address_t & addr, set<cell_address_t> visited)const;

public:
    myCSV() = default;
    myCSV(const table_t & table);
    myCSV(const string & file);
    myCSV(const myCSV & orig):
        _table(orig._table),_rows(orig._rows),_columns(orig._columns){};


    long long calculate (const cell_address_t & addr)const;
    long long cellConvert(const cell_data_t& data) const;

    table_t::const_iterator begin() const {return _table.begin();}
    table_t::const_iterator end() const {return _table.end();}
    table_t::iterator begin() {return _table.begin();}
    table_t::iterator end() {return _table.end();}

    cell_data_t& operator [] (const cell_address_t & addr){return _table[addr];};
    const cell_data_t& operator [] (const cell_address_t & addr) const{return _table.at(addr);};

    ~myCSV(){};
};
}

#endif // MYCSV_H
