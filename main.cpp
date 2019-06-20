/// @file main.cpp
/// @brief This program displays the products that are produced and the amount of each for a media player production facility.
/// @author Stephen Cline

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

void production(const int &, const string &, const vector<Product> &, vector<Record> &, vector<Statistic> &);

void productionView(const string &, const vector<Record> &);

void productionView(const vector<Record> &, const vector<Product> &);

int catalogMenu(string &, const vector<Product> &);

void serialSearch();

void accountCreation();

string encryptString(string);

string decryptString(string);

Product makeProduct(const string &, const string &, const string &);

Record makeRecord(const int &, const string &);

Statistic makeStatistic(const int &, const string &);

void loadProductLine(vector<Product> &);

void loadProductRecords(vector<Record> &);

void loadProductStats(vector<Statistic> &, const vector<Record> &);

void printProductStats(const vector<Statistic> &, const vector<Product> &, const vector<Record> &);

int main() {
    vector<Product> productLine;
    vector<Record> productRecords;
    vector<Statistic> productStats;
    int option;
    loadProductLine(productLine);
    loadProductRecords(productRecords);
    loadProductStats(productStats, productRecords);
    cout << "Production Line Tracker\n\n";
    do {
        option = selectMenu(productLine, productRecords, productStats);
    } while (option != 6);

    return 0;
}

/// @brief This function allows the user to select from the main menu.
/// @param productLine
/// @param productRecords
/// @param productStats
int selectMenu(vector<Product> &productLine, vector<Record> &productRecords, vector<Statistic> &productStats) {
    string itemTypeCode;
    int catalogOption;
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
            catalogOption = catalogMenu(itemTypeCode, productLine);
            production(catalogOption, itemTypeCode, productLine, productRecords, productStats);
            break;
        case 2:
            accountCreation();
            break;
        case 3:
            cout << "\n";
            newProduct(productLine);
            break;
        case 4:
            cout << "\n";
            printProductStats(productStats, productLine, productRecords);
            break;
        case 5:
            serialSearch();
            break;
        case 6:
            break;
        default:
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Not a valid selection\n";
    }
    return option;
}

/// @brief This function allows the user to enter a new product into the catalog.
/// @param productLine
void newProduct(vector<Product> &productLine) {
    cout << "Enter the Manufacturer\n";
    string manufacturer;
    cin >> manufacturer;
    cout << "Enter the Product Name\n";
    string prodName;
    cin >> prodName;
    int itemTypeChoice = 0;
    string itemTypeCode;
    while (1 > itemTypeChoice || itemTypeChoice > 4) {
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
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Not a valid selection\n";
        }
    }
    ofstream catalog;
    catalog.open("ProductLine.csv", fstream::app);
    catalog << manufacturer << "," << prodName << "," << itemTypeCode << "\n";
    catalog.close();
    productLine.push_back(makeProduct(manufacturer, prodName, itemTypeCode));
}

/// @brief This function allows the user to add new production logs.
/// @param catalogOption
/// @param itemTypeCode
/// @param productLine
/// @param productRecord
/// @param productStats
void production(const int &catalogOption, const string &itemTypeCode, const vector<Product> &productLine,
                vector<Record> &productRecord, vector<Statistic> &productStats) {
    if (catalogOption == -1)
        return;
    string manufacturer = productLine[catalogOption].manufacturer;
    int numProduced = 1;
    string tmpProdLog;
    ofstream prodLog;
    prodLog.open("ProductionLog.csv", fstream::app);

    cout << "Enter the number of items that were produced\n";
    cin >> numProduced;

    int count = 1;
    for (const auto &x : productRecord) {
        if (string::npos != x.serial.find(manufacturer.substr(0, 3)))
            count++;
    }
    numProduced = count + numProduced;
    for (int i = count; i < numProduced; i++) {
        if (i < 10)
            tmpProdLog = to_string(++productStats[0].total) + "," + manufacturer.substr(0, 3) + itemTypeCode + "0000" +
                         to_string(i);
        else if (10 <= i && i < 100)
            tmpProdLog = to_string(++productStats[0].total) + "," + manufacturer.substr(0, 3) + itemTypeCode + "000" +
                         to_string(i);
        else if (100 <= i && i < 1000)
            tmpProdLog = to_string(++productStats[0].total) + "," + manufacturer.substr(0, 3) + itemTypeCode + "00" +
                         to_string(i);
        else if (1000 <= i && i < 10000)
            tmpProdLog = to_string(++productStats[0].total) + "," + manufacturer.substr(0, 3) + itemTypeCode + "0" +
                         to_string(i);
        else
            tmpProdLog =
                    to_string(++productStats[0].total) + "," + manufacturer.substr(0, 3) + itemTypeCode + to_string(i);

        prodLog << tmpProdLog << "\n";
        productRecord.push_back(
                makeRecord(productStats[0].total, tmpProdLog.substr(tmpProdLog.find(manufacturer.substr(0, 3)))));
        if (itemTypeCode == "MM")
            ++productStats[1].total;
        else if (itemTypeCode == "VI")
            ++productStats[2].total;
        else if (itemTypeCode == "AM")
            ++productStats[3].total;
        else if (itemTypeCode == "VM")
            ++productStats[4].total;

    }
    prodLog.close();
}

/// @brief This function displays production logs according to the user's choice.
/// @param catalogOption
/// @param productRecords
void productionView(const string &catalogOption, const vector<Record> &productRecords) {
    for (const auto &x : productRecords) {
        if (string::npos != x.serial.find(catalogOption.substr(0, 3)))
            cout << x.orderNumber << ". " << x.serial << "\n";
    }
}

/// @brief This function displays all production logs.
/// @param productRecords
/// @param productLine
void productionView(const vector<Record> &productRecords, const vector<Product> &productLine) {
    string tmp;
    for (const auto &x : productRecords) {
        for (const auto &y : productLine) {
            if (string::npos != x.serial.find(y.manufacturer.substr(0, 3)))
                cout << x.orderNumber << ". " << y.manufacturer << " " << y.name << " " << x.serial << "\n";
        }
    }
}

/// @brief This function displays the catalog menu.
/// @param itemTypeCode
/// @param productLine
/// @return catalog option from user input
int catalogMenu(string &itemTypeCode, const vector<Product> &productLine) {
    int catalogOption = 0;
    int count = 0;
    ifstream catalog;
    catalog.open("ProductLine.csv");
    if (!catalog.good()) {
        catalog.close();
        cout << "No available products\n";
        return -1;
    }
    catalog.close();

    cout << "Select a Product\n";

    while (catalogOption < 1 || catalogOption > count) {
        for (const auto &x : productLine)
            cout << ++count << ". " << x.manufacturer << " " << x.name << "\n";
        cin >> catalogOption;
        if (0 < catalogOption <= count) {
            itemTypeCode = productLine[catalogOption - 1].type;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Not a valid selection\n";
        }
    }

    return catalogOption - 1;
}

/// @brief This function overflows catalogMenu.
/// @param productLine
/// @return catalog option from user input
int catalogMenu(const vector<Product> &productLine) {
    string tmp = "tmp";
    return catalogMenu(tmp, productLine);
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

    string firstName;
    cin >> firstName;
    string lastName;
    cin >> lastName;

    string userName;

    /// create user name in proper format
    lastName[0] = tolower(lastName[0]);
    lastName.insert(lastName.begin(), tolower(firstName[0]));
    userName = lastName;

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
            cout << "User name: " + userName + "\n"
                 << "Password: Valid\n";

        } else
            cout << "Invalid password.\n";
    }
    ofstream accounts;
    accounts.open("Users.txt");
    accounts << userName << "," << encryptString(password) << "\n";
    accounts.close();
}

/// @brief This function encrypts a string with recursion
/// @param str
string encryptString(string str) {
    if (str.length() == 1) {
        return str;
    } else {
        return char((int) str[0] + 3) + encryptString(str.substr(1, str.length() - 1));
    }
}

/// @brief This function decrypts a string with recursion
/// @param str
string decryptString(string str) {
    if (str.length() == 1) {
        return str;
    } else {
        return char((int) str[0] - 3) + decryptString(str.substr(1, str.length() - 1));
    }
}

/// @brief This function creates and returns a new product in the struct format
/// @param manufacturer
/// @param name
/// @param type
Product makeProduct(const string &manufacturer, const string &name, const string &type) {
    Product productLine = {manufacturer, name, type};
    return productLine;
}

/// @brief This function creates and returns a new record in the struct format
/// @param total
/// @param serial
Record makeRecord(const int &total, const string &serial) {
    Record productRecord = {total, serial};
    return productRecord;
}

/// @brief This function creates and returns a new statistic in the struct format
/// @param total
/// @param type
Statistic makeStatistic(const int &total, const string &type) {
    Statistic productStat = {total, type};
    return productStat;
}

/// @brief This function loads the production line
/// @param productLine
void loadProductLine(vector<Product> &productLine) {
    ifstream catalog;
    catalog.open("ProductLine.csv");
    if (!catalog.good()) {
        return;
    }

    string line;
    string manufacturer;
    string name;
    string type;

    while (getline(catalog, line)) {
        stringstream tmpStream(line);
        getline(tmpStream, manufacturer, ',');
        getline(tmpStream, name, ',');
        getline(tmpStream, type, ',');
        productLine.push_back(makeProduct(manufacturer, name, type));
    }
    catalog.close();
}

/// @brief This function loads the product records
/// @param productRecords
void loadProductRecords(vector<Record> &productRecords) {
    string total;
    string serial;
    string line;
    ifstream prodLog;
    prodLog.open("ProductionLog.csv");
    while (getline(prodLog, line)) {
        stringstream tmpStream(line);
        getline(tmpStream, total, ',');
        getline(tmpStream, serial, ',');
        productRecords.push_back(makeRecord(stoi(total), serial));
    }
    prodLog.close();
}

/// @brief This function loads the product stats of total items produced by type
/// @param productStats
/// @param productRecords
void loadProductStats(vector<Statistic> &productStats, const vector<Record> &productRecords) {
    int All = 0;
    int MM = 0;
    int VI = 0;
    int AM = 0;
    int VM = 0;

    for (const auto &x : productRecords) {
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

    productStats.push_back(makeStatistic(All, "Total Produced"));
    productStats.push_back(makeStatistic(MM, "Total MM Produced"));
    productStats.push_back(makeStatistic(VI, "Total VI Produced"));
    productStats.push_back(makeStatistic(AM, "Total AM Produced"));
    productStats.push_back(makeStatistic(VM, "Total VM Produced"));
    productStats.push_back(makeStatistic(0, "Specific Production Logs"));
    productStats.push_back(makeStatistic(0, "All Production Logs"));
}

/// @brief This function displays statistics according to the user's choice
/// @param productStats
/// @param productLine
/// @param productRecords
void printProductStats(const vector<Statistic> &productStats, const vector<Product> &productLine,
                       const vector<Record> &productRecords) {
    int count = 0;
    int choice = -1;
    int catalogOption = 0;
    while (choice < 0 || choice > 7) {
        cout << "Select a Type of Statistic\n";
        for (const auto &x : productStats)
            cout << ++count << ". " << x.type << "\n";
        cin >> choice;
        choice--;
        switch (choice) {
            case 0:
                cout << "The total number of products produced is " << productStats[choice].total << "\n";
                break;
            case 1:
                cout << "The total number of Audio products produced is " << productStats[choice].total << "\n";
                break;
            case 2:
                cout << "The total number of Visual products produced is " << productStats[choice].total << "\n";
                break;
            case 3:
                cout << "The total number of Audio Mobile products produced is " << productStats[choice].total << "\n";
                break;
            case 4:
                cout << "The total number of Visual Mobile products produced is " << productStats[choice].total << "\n";
                break;
            case 5:
                catalogOption = catalogMenu(productLine);
                if (catalogOption > -1)
                    productionView(productLine[catalogOption].manufacturer, productRecords);
                break;
            case 6:
                productionView(productRecords, productLine);
                break;
            default:
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Not a valid selection\n";
        }
    }
}