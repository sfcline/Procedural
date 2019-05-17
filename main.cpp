#include <iostream>

using namespace std;

int main() {
    int option;
    cout << "Please enter a number 1~5 for the following options.\n"
            "1. Make an employee account\n"
            "2. Make a Music Player\n"
            "3. Make a Movie Player\n"
            "4. Add a new product\n"
            "5. Display collection\n" << endl;
    cin >> option;
    if (0 < option && option < 6 )
        cout << "Hello World!" << endl;
    else
        cout << "A number 1 through five was not entered." << endl;
    return 0;
}