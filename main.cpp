/// @file main.cpp
/// @brief This program displays the products that are produced and the amount of each for a media player production facility.
/// @author Stephen Cline
/// @bug Produce items still asks for number of items produced when there are no products to select
/// @bug If a product is typed in with a space it creates an infinite loop

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

/// struct for Product containing strings for manufacturer, name, and type
struct Product {
    string manufacturer;
    string name;
    string type;
};

struct Record {
    int orderNumber;
    string serial;
};

struct Statistic {
    int total;
    string type;
};

int selectMenu(vector<Product> &, vector<Record> &, vector<Statistic> &);

void newProduct(vector<Product> &);

void production(const string &, const string &, vector<Record> &);

void productionView(const string &);

string catalogMenu(string &, const vector<Product> &);

void serialSearch();

void accountCreation();

string encrypt_string(string);

string decrypt_string(string);

Product make_product(string, string, string);

Record make_record(int, string);

Statistic make_statistic(int, string);

void loadProductLine(vector<Product> &);

void loadProductRecords(vector<Record> &);

void loadProductStats(vector<Statistic> &, const vector<Record> &);

int main() {
    vector<Product> product_line;
    vector<Record> product_records;
    vector<Statistic> product_stats;
    int option;
    loadProductLine(product_line);
    loadProductRecords(product_records);
    loadProductStats(product_stats, product_records);
    cout << "Production Line Tracker\n\n";
    do {
        option = selectMenu(product_line, product_records, product_stats);
    } while (option != 6);

    return 0;
}

/// @brief This function allows the user to select from the main menu.
/// @param option
int selectMenu(vector<Product> &product_line, vector<Record> &product_records, vector<Statistic> &product_stats) {
    string itemTypeCode;
    string catalogOption;
    int option;
    cout << "1. Produce Items\n"
            "2. Add Employee Account\n"
            "3. Add New Product\n"
            "4. Display Production Statistics\n"
            "5. Serial Number Search\n"
            "6. Exit" << endl;
    cin >> option;
    switch (option) {
        case 1:
            catalogOption = catalogMenu(itemTypeCode, product_line);
            production(catalogOption, itemTypeCode, product_records);
            break;
        case 2:
            accountCreation();
            break;
        case 3:
            cout << "\n";
            newProduct(product_line);
            break;
        case 4:
            cout << "\n";
            catalogOption = catalogMenu(itemTypeCode, product_line);
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
    return option;
}

/// @brief This function allows the user to enter a new product into the catalog.
void newProduct(vector<Product> &product_line) {
    cout << "Enter the Manufacturer\n";
    string manufacturer;
    cin >> manufacturer;
    cout << "Enter the Product Name\n";
    string prodName;
    cin >> prodName;
    int itemTypeChoice;
    string itemTypeCode;
    do {
        cout << "Enter the item type\n" <<
             "1. Audio\n" <<
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
    catalog.open("ProductLine.csv");
    catalog << manufacturer << "," << prodName << "," << itemTypeCode << "\n";
    catalog.close();
    product_line.push_back(make_product(manufacturer, prodName, itemTypeCode));
}

/// @brief This function allows the user to add new production logs.
/// @param manufacturer
void production(const string &manufacturer, const string &itemTypeCode, vector<Record> &product_record) {
    int numProduced = 1;
    cout << "Enter the number of items that were produced\n";
    cin >> numProduced;

    ofstream prodLog;
    prodLog.open("ProductionLog.csv");
    string tmpProdLog;

    int i = 1;
    for (const auto &x : product_record) {
        if (string::npos != x.serial.find(manufacturer.substr(0, 3)))
            i++;
    }
    numProduced = i + numProduced;
    for (i; i <= numProduced; i++) {
        if (i < 10)
            tmpProdLog = to_string(i) + "," + manufacturer.substr(0, 3) + itemTypeCode + "0000" + to_string(i);
        else if (10 < i && i < 100)
            tmpProdLog = to_string(i) + "," + manufacturer.substr(0, 3) + itemTypeCode + "000" + to_string(i);
        else if (100 < i && i < 1000)
            tmpProdLog = to_string(i) + "," + manufacturer.substr(0, 3) + itemTypeCode + "00" + to_string(i);
        else if (1000 < i && i < 10000)
            tmpProdLog = to_string(i) + "," + manufacturer.substr(0, 3) + itemTypeCode + "0" + to_string(i);
        else
            tmpProdLog = to_string(i) + "," + manufacturer.substr(0, 3) + itemTypeCode + to_string(i);
        prodLog << tmpProdLog << "\n";
        product_record.push_back(make_record(i, tmpProdLog));
    }
    prodLog.close();
}

/// @brief This function displays production logs according to the user's choice.
/// @param catalogOption
void productionView(const string &catalogOption) {
    string line;
    ifstream prodLog;
    prodLog.open("ProductionLog.csv");
    while (getline(prodLog, line)) {
        if (string::npos != line.find(catalogOption.substr(0, 3))) {
            cout << line << "\n";
        }
    }
}

/// @brief This function displays the catalog menu.
/// @return catalog option from user input
string catalogMenu(string &itemTypeCode, const vector<Product> &product_line) {
    bool catalogMatch = false;
    string catalogOption;
    ifstream catalog;
    catalog.open("ProductLine.csv");
    if (!catalog.good()) {
        cout << "No available products\n";
        return "00000";
    }
    catalog.close();

    cout << "Select a Product\nDo not use spaces\n";

    while (!catalogMatch) {
        for (const auto &x : product_line)
            cout << x.manufacturer << " " << x.name << "\n";
        cin >> catalogOption;
        for (auto &i : product_line) {
            if (string::npos != i.manufacturer.find(catalogOption.substr(0, 3)) ||
                string::npos != i.name.find(catalogOption.substr(0, 3))) {
                catalogMatch = true;
                catalogOption = i.manufacturer;
                itemTypeCode = i.type;
            }
        }
        if (!catalogMatch)
            cout << "Not a valid selection\n";
    }

    return catalogOption;
}

/// @brief This function searches for a product by serial.
void serialSearch() {
    ifstream catalog;
    catalog.open("ProductLine.csv");
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
    accounts.open("Users.txt");
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

/// @brief This function creates and returns a new product in the struct format
/// @param manufacturer
/// @param name
/// @param type
Product make_product(string manufacturer, string name, string type) {
    Product product_line = {manufacturer, name, type};
    return product_line;
}

/// @brief This function creates and returns a new record in the struct format
/// @param total
/// @param serial
Record make_record(int total, string serial) {
    Record product_record = {total, serial};
    return product_record;
}

/// @brief This function creates and returns a new statistic in the struct format
/// @param total
/// @param type
Statistic make_statistic(int total, string type) {
    Statistic product_stat = {total, type};
    return product_stat;
}

/// @brief This function loads the production line
/// @param product_line
void loadProductLine(vector<Product> &product_line) {
    ifstream catalog;
    catalog.open("ProductLine.csv");
    if (!catalog.good()) {
        return;
    }

    cout << "Select a Product\n";
    string line;
    string manufacturer;
    string name;
    string type;

    while (getline(catalog, line)) {
        stringstream tmpStream(line);
        getline(tmpStream, manufacturer, ',');
        getline(tmpStream, name, ',');
        getline(tmpStream, type, ',');
        product_line.push_back(make_product(manufacturer, name, type));
    }
    catalog.close();
}

/// @brief This function loads the product records
/// @param product_records
void loadProductRecords(vector<Record> &product_records) {
    string total;
    string serial;
    string line;
    ifstream prodLog;
    prodLog.open("ProductionLog.csv");
    while (getline(prodLog, line)) {
        stringstream tmpStream(line);
        getline(tmpStream, total, ',');
        getline(tmpStream, serial, ',');
        product_records.push_back(make_record(stoi(total), serial));
    }
    prodLog.close();
}

/// @brief This function loads the product stats of total items produced by type
/// @param product_stats
void loadProductStats(vector<Statistic> &product_stats, const vector<Record> &product_records) {
    int All = 0;
    int MM = 0;
    int VI = 0;
    int AM = 0;
    int VM = 0;

    for (const auto &x : product_records) {
        if (string::npos != x.serial.find("MM"))
            MM++;
        if (string::npos != x.serial.find("VI"))
            VI++;
        if (string::npos != x.serial.find("AM"))
            AM++;
        if (string::npos != x.serial.find("VM"))
            VM++;
        All++;
    }

    product_stats.push_back(make_statistic(All, "All"));
    product_stats.push_back(make_statistic(MM, "MM"));
    product_stats.push_back(make_statistic(VI, "VI"));
    product_stats.push_back(make_statistic(AM, "AM"));
    product_stats.push_back(make_statistic(VM, "VM"));
}