/// @file main.cpp
/// @brief This program displays the products that are produced and the amount of each for a media player production facility.
/// @author Stephen Cline
/// @bug The menu does not show again after an invalid choice when selecting from catalog.

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void printMenu();

void selectMenu(int);

void newProduct();

void production(const string &, const string &);

void productionView(const string &);

string catalogMenu(string &);

void serialSearch();

void accountCreation();

string encrypt_string(string);

string decrypt_string(string);

int main() {
    cout << "Production Line Tracker\n\n";
    int option;
    do {
        printMenu();
        cin >> option;
        selectMenu(option);
    } while (option != 6);

    return 0;
}

/// @todo  merge printMenu and selectMenu
/// @brief This function prints the main menu. endl is used to flush.
void printMenu() {
    cout << "1. Produce Items\n"
            "2. Add Employee Account\n"
            "3. Add New Product\n"
            "4. Display Production Statistics\n"
            "5. Serial Number Search\n"
            "6. Exit" << endl;
}

/// @brief This function allows the user to select from the main menu.
/// @param option
void selectMenu(int option) {
    string itemTypeCode;
    string catalogOption;
    switch (option) {
        case 1:
            catalogOption = catalogMenu(itemTypeCode);
            production(catalogOption, itemTypeCode);
            break;
        case 2:
            accountCreation();
            break;
        case 3:
            cout << "\n";
            newProduct();
            break;
        case 4:
            cout << "\n";
            catalogOption = catalogMenu(itemTypeCode);
            productionView(catalogOption);
            break;
        case 5:
            serialSearch();
            break;
        case 6:
            break;
        default:
            cout << "Not a valid selection\n";
    }
}

/// @brief This function allows the user to enter a new product into the catalog.
void newProduct() {
    cout << "Enter the Manufacturer\n";
    string manufacturer;
    cin >> manufacturer;
    cout << "Enter the Product Name\n";
    string prodName;
    cin >> prodName;
    int itemTypeChoice;
    string itemTypeCode;
    do {
        cout << "Enter the item type\n";
        cout << "1. Audio\n" <<
             "2. Visual\n" <<
             "3. AudioMobile\n" <<
             "4. VisualMobile\n";
        cin >> itemTypeChoice;
        switch (itemTypeChoice) {
            case 1:
                itemTypeCode = "MM";
                break;
            case 2:
                itemTypeCode = "VI";
                break;
            case 3:
                itemTypeCode = "AM";
                break;
            case 4:
                itemTypeCode = "VM";
                break;
            default:
                cout << "Not a valid selection\n";
        }
    } while (1 > itemTypeChoice || itemTypeChoice > 4);
    ofstream catalog;
    catalog.open("catalog.txt");
    catalog << manufacturer << "," << prodName << "\n";
    catalog.close();
}

/// @brief This function allows the user to add new production logs.
/// @param manufacturer
void production(const string &manufacturer, const string &itemTypeCode) {
    cout << "Enter the number of items that were produced\n";
    int numProduced;
    cin >> numProduced;

    ofstream prodLog;
    prodLog.open("production.txt");

    for (int i = 1; i <= numProduced; i++) {
        if (i < 10)
            prodLog << "Production Number: " << i << " Serial Number: " << manufacturer.substr(0, 3) << itemTypeCode
                    << "0000" << i << "\n";
        else if (10 < i && i < 100)
            prodLog << "Production Number: " << i << " Serial Number: " << manufacturer.substr(0, 3) << itemTypeCode
                    << "000" << i << "\n";
        else if (100 < i && i < 1000)
            prodLog << "Production Number: " << i << " Serial Number: " << manufacturer.substr(0, 3) << itemTypeCode
                    << "00" << i << "\n";
        else if (1000 < i && i < 10000)
            prodLog << "Production Number: " << i << " Serial Number: " << manufacturer.substr(0, 3) << itemTypeCode
                    << "0" << i << "\n";
        else if (10000 < i)
            prodLog << "Production Number: " << i << " Serial Number: " << manufacturer.substr(0, 3) << itemTypeCode
                    << i << "\n";
    }

    prodLog.close();
}

/// @brief This function displays production logs according to the user's choice.
/// @param catalogOption
void productionView(const string &catalogOption) {
    string line;
    ifstream prodLog;
    prodLog.open("production.txt");
    while (getline(prodLog, line)) {
        if (string::npos != line.find(catalogOption.substr(0, 3))) {
            cout << line << "\n";
        }
    }

}

/// @brief This function displays the catalog menu.
/// @return catalog option from user input
string catalogMenu(string &itemTypeCode) {
    bool catalogMatch = false;
    string catalogOption;
    ifstream catalog;
    catalog.open("catalog.txt");
    if (!catalog.good()) {
        cout << "No available products\n";
        return "00000";
    }

    cout << "Select a Product\n";
    string line;
    vector<string> catalogVec;

    while (getline(catalog, line)) {
        catalogVec.push_back(line);
    }
    catalog.close();

    sort(catalogVec.begin(), catalogVec.end());


    do {
        for (const auto &x : catalogVec)
            cout << x << "\n";
        cin >> catalogOption;
        for (auto &i : catalogVec) {
            if (string::npos != i.find(catalogOption.substr(0, 3))) {
                catalogMatch = true;
                catalogOption = i;
            }
        }
        if (!catalogMatch)
            cout << "Not a valid selection\n";
    } while (!catalogMatch);

    if (string::npos != catalogOption.find("MM")) {
        itemTypeCode = "MM";
    } else if (string::npos != catalogOption.find("VI")) {
        itemTypeCode = "VI";
    } else if (string::npos != catalogOption.find("AM")) {
        itemTypeCode = "AM";
    } else if (string::npos != catalogOption.find("VM")) {
        itemTypeCode = "VM";
    }

    return catalogOption;
}

/// @brief This function searches for a product by serial.
void serialSearch() {
    ifstream catalog;
    catalog.open("catalog.txt");
    string line;
    string serial;
    cout << "Enter a Serial Number\n";
    cin >> serial;

    while (getline(catalog, line)) {
        if (string::npos != line.find(serial.substr(0, 3))) {
            cout << line << "\n";
            catalog.close();
            return;
        }
    }

    cout << "Not a valid Serial Number\n";

    catalog.close();
}

/// @brief This function creates a new employee account.
void accountCreation() {
    bool valid = false;

    /// Create Username
    cout << "Enter employee's full name\n";

    string first_name;
    cin >> first_name;
    string last_name;
    cin >> last_name;

    string user_name;

    // create user name in proper format
    last_name[0] = tolower(last_name[0]);
    last_name.insert(last_name.begin(), tolower(first_name[0]));
    user_name = last_name;

    /// Create password
    cout << "Enter employee's password.\n";

    string password;

    while (!valid) {
        cout << "Must contain a number and lowercase and uppercase letters.\n";
        cin >> password;
        if (any_of(password.begin(), password.end(), ::isdigit)
            && any_of(password.begin(), password.end(), ::isupper)
            && all_of(password.begin(), password.end(), ::isalnum)) {
            valid = true;
            cout << "User name: " + user_name + "\n"
                      << "Password: Valid\n";

        } else
            cout << "Invalid password.\n";
    }
    ofstream accounts;
    accounts.open("accounts.txt");
    accounts << user_name << "," << encrypt_string(password) << "\n";
    accounts.close();
}

/// @brief This function encrypts a string with recursion
/// @param str
string encrypt_string(string str) {
    if (str.length() == 1) {
        return str;
    } else {
        return char((int) str[0] + 3) + encrypt_string(str.substr(1, str.length() - 1));
    }
}

/// @brief This function decrypts a string with recursion
/// @param str
string decrypt_string(string str) {
    if (str.length() == 1) {
        return str;
    } else {
        return char((int) str[0] - 3) + decrypt_string(str.substr(1, str.length() - 1));
    }
}
