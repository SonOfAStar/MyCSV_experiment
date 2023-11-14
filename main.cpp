#include <iostream>
#include <string>
#include <map>
#include <set>

#include "myCSV.h"


using namespace std;
using namespace myCSV_ns;

int main()
{

    table_t table {{"A1", "=A2"}, {"A2", "2"},{"B1", "12"},{"B2","=A2+B1"}};

    myCSV csv_var (table);

    cout << csv_var << endl;

    cout << "A1 = " << csv_var.calculate("A1") << endl;
    cout << "A2 = " << csv_var.calculate("A2") << endl;
    cout << "B1 = " << csv_var.calculate("B1") << endl;
    cout << "B2 = " << csv_var.calculate("B2") << endl;

    return 0;
}
