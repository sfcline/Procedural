#include <iostream>

using namespace std;

int main() {
    int option;
    cout << "Production Line Tracker\n\n"
            "1. Add Employee Account\n"
            "2. Add Music Player\n"
            "3. Add Movie Player\n"
            "4. Display Production Statistics\n"
            "5. Exit\n" << endl;
    cin >> option;
    if (0 < option && option < 6 )
        cout << "Hello World!" << endl;
    else
        cout << "A number 1 through five was not entered." << endl;
    return 0;
}