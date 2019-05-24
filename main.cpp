/// @file main.cpp
/// @brief This program displays the products that are produced and the amount of each for a media player production facility.
/// @author Stephen Cline
/// @bug The menu does not show again after an invalid choice when selecting from catalog.

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

void printMenu();

void selectMenu(int);

void newProduct();

void productionView();

int main() {
    cout << "Production Line Tracker\n\n";
    int option;
    do {
        printMenu();
        cin >> option;
        selectMenu(option);
    } while (option != 5);

    return 0;
}

/// @todo  merge printMenu and selectMenu
/// @brief This function prints the main menu. endl is used to flush.
void printMenu() {
    cout << "1. Produce Items\n"
            "2. Add Employee Account\n"
            "3. Add New Product\n"
            "4. Display Production Statistics\n"
            "5. Exit" << endl;
}

/// @brief This function allows the user to select from the main menu.
void selectMenu(int option) {
    switch (option) {
        case 1:
            cout << "Produce Items Stub\n";
            break;
        case 2:
            cout << "Add Employee Account Stub\n";
            break;
        case 3:
            cout << "\n";
            newProduct();
            break;
        case 4:
            cout << "\n";
            productionView();
            break;
        case 5:
            break;
        default:
            cout << "Not a valid selection\n";
    }
}

/// @brief This function allows the user to enter a new product into the catalog.
void newProduct() {
    int itemTypeChoice;
    string itemTypeCode;
    cout << "Enter the Manufacturer\n";
    string manufacturer;
    cin >> manufacturer;
    cout << "Enter the Product Name\n";
    string prodName;
    cin >> prodName;
    ofstream catalog;
    catalog.open("catalog.txt");
    catalog << manufacturer << " " << prodName << "\n";
    catalog.close();

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

/// @brief This function grabs the catalog and displays information according to the user's choice.
void productionView() {
    int catalogOption;
    ifstream catalog;
    int catalogAmount = 0;
    catalog.open("catalog.txt");
    cout << "Select a Product\n";
    string line;
    string catalogArray[255];

    for (catalogAmount; getline(catalog, line); catalogAmount++) {
        cout << (catalogAmount + 1) << ". " << line << "\n";
        catalogArray[catalogAmount] = line;
    }
    catalog.close();

    do {
        cin >> catalogOption;
        if (catalogAmount < catalogOption || catalogOption < 1)
            cout << "Not a valid selection\n"; /// @todo   show menu again after invalid selection
    } while (catalogAmount < catalogOption || catalogOption < 1);

    ifstream prodLog;
    prodLog.open("production.txt");
    while (getline(prodLog, line)) {
        if (string::npos != line.find(catalogArray[catalogOption].substr(0, 3))) {
            cout << line << "\n";
        }
    }

}