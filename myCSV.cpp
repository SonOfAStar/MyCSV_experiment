#include <cctype>

#include "myCSV.h"

//v 0.1

namespace myCSV_ns{

bool isAddr(const cell_data_t &data){
        if (data.size() == 0){
            return false;
        }

        return isalpha((unsigned char) data[0]);
    }

bool isExpr(const cell_data_t &data){
    if (data.size() == 0) {
        return false;
    }
    return data[0] == '=';
}

void splitExpr(const cell_data_t &data, cell_data_t &op1, cell_data_t & op2, char &opr){

    size_t operation_pos = 0;
    for(size_t i = 1; i < data.size(); i++){
        if((data[i] == '+') || (data[i] == '-') ||
           (data[i] == '*') || (data[i] == '/')){
            opr = data[i];
            operation_pos = i;
           }
    }

    if(operation_pos > 1){
        //expression has two operands
        op1 = data.substr(1, operation_pos - 1);
        op2 = data.substr(operation_pos + 1, data.size() - operation_pos - 1);
    } else {
        op1 = data.substr(1, data.size() - 1);
    }
}

void splitAddr(const cell_address_t &addr, string &column, string& row){
    size_t split_pos = 0;

    while(isalpha((unsigned char)addr[split_pos])){
        split_pos ++;
    }
    column = addr.substr(0, split_pos);
    row = addr.substr(split_pos, addr.size() - split_pos);

}


myCSV::myCSV(const table_t & table):_table(table){
    string row, column;
    _rows.clear();
    _columns.clear();

    for (const auto& [addr, data]: _table){
        splitAddr(addr, column, row);

        if(_columns.find(column) == _columns.end()){
            _columns.insert(column);
        }
        if(_rows.find(row) == _rows.end()){
            _rows.insert(row);
        }
    }

}


myCSV::myCSV(const string & file){}


long long myCSV::cellConvert(const cell_data_t& data) const{
    long long result;
    try{
        result = stoi(data);
    }catch(const invalid_argument& ex){
        throw MY_CSV_EXCEPTION("Invalid data", "myCSV.cellConvert", data);
    } catch(const out_of_range& ex){
        throw MY_CSV_EXCEPTION("Out of range exception", "myCSV.cellConvert", data);
    }
    return result;
};


long long myCSV::_calculate(const cell_address_t & addr, set<cell_address_t> visited)const{
    long long result;

        if(visited.contains(addr)){
            throw MY_CSV_EXCEPTION("Loop exception", "myCSV.calculate", addr);
        }

        //check validity of the address
        if(_table.contains(addr)){
            visited.insert(addr);

            const cell_data_t data = _table.at(addr);

            //check cell data type
            if(isExpr(data)){
                cell_data_t operand1 = "", operand2 = "";
                char operation;
                splitExpr(data, operand1, operand2, operation);

                if(operand2.size() > 0){
                //expression has two operands

                    //calculate operands
                    long long res1;
                    long long res2;

                    if (isAddr(operand1)){
                        res1 = _calculate(operand1, visited);
                    } else {
                        try{
                            res1 = cellConvert(operand1);
                        } catch (MY_CSV_EXCEPTION& ex){
                            ex.args += " at:" + addr;
                            throw ex;
                        }
                    }

                    if (isAddr(operand2)){
                        res2 = _calculate(operand2, visited);
                    } else {
                        try{
                            res2 = cellConvert(operand2);
                        } catch (MY_CSV_EXCEPTION& ex){
                            ex.args += " at:" + addr;
                            throw ex;
                        }
                    }


                    //calculate operation
                    if (operation == '+'){
                        result = res1 + res2;
                    } else if (operation == '-'){
                        result = res1 - res2;
                    } else if (operation == '*'){
                        result = res1 * res2;
                    } else if (operation == '/'){
                        result = res1 / res2;
                    }
                } else {
                    //expression is a link or a single operand
                    if (isAddr(operand1)){
                        result = _calculate(operand1, visited);
                    } else {
                        try{
                            result = cellConvert(operand1);
                        } catch (MY_CSV_EXCEPTION& ex){
                            ex.args += " at:" + addr;
                            throw ex;
                        }
                    }
                }

            } else {
                //cell contains a number
                try{
                    result = cellConvert(data);
                } catch (MY_CSV_EXCEPTION& ex){
                    ex.args += " at:" + addr;
                    throw ex;
                }
            }


        } else {
            throw MY_CSV_EXCEPTION("Incorrect address exception", "myCSV.calculate", addr);
        }

        return result;
}


long long myCSV::calculate(const cell_address_t & addr)const{
    set <cell_address_t> visited;
        long long result;
        try{
           result = _calculate(addr, visited);

        } catch( const MY_CSV_EXCEPTION& ex){
            std::cerr << "Exception detected: " << ex.exep_type << ".\n";
            std::cerr << "In function: " << ex.place << "\n" << "While calculating address " << ex.args << endl;
            return 0;
        }
        return result;
}


istream& operator>>(istream& is, myCSV& csv){

    return is;
}


ostream& operator<<(ostream& os, const myCSV& csv){

    os << " ";

    for (const auto& elem : csv._columns){
        os << ',' << elem;
    }

    os << '\n';

    for (const auto& row : csv._rows){
        os << row;
        for (const auto& column : csv._columns){
            os << ',' << csv[column+row];
        }
        os << '\n';

    }
    return os;
}
}// end of namespace
