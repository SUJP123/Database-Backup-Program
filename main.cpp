#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "SQLAPI/include/SQLAPI.h"
#include "connect.h"
#include "fullBackup.h"
#include "logged.h"
#include "cloudBackup.h"

using namespace std;

// CLI -> g++ -o my_program main.cpp -LSQLAPI/lib -lsqlapi
// export DYLD_LIBRARY_PATH=/Users/sujaypatel/Database-Backup-CLI/SQLAPI/lib:$DYLD_LIBRARY_PATH
// ./my_program


void welcome_message() {
    std::cout <<"\n";
    std::cout <<"\n";
    std::cout <<"\n";
    std::cout <<"Welcome To The Database Backup System\n";
    std::cout <<"-------------------------------------\n";
    std::cout <<"           By: Sujay Patel           \n";
    std::cout <<"\n";
    std::cout <<"\n";
}

string select_type() {
    int choice;
    std::cout <<"Select a database type (Type the #): \n";
    std::cout <<"-------------------------------------\n";
    std::cout <<"1. PostgreSQL\n";

    std::cout <<"number : ";
    std::cin >> choice;
    if (choice == 1) {
        return "PostgreSQL";
    }
    return "N/A";
}

int choice_selection() {
    int choice;
    std::cout << "\n";
    std::cout <<"Select an Option Below (Type the #)\n";
    std::cout <<"-------------------------------------\n";
    std::cout <<"1. Full Backup on Local System\n";
    std::cout <<"2. Full Backup on AWS Cloud\n";

    std::cout <<"number : ";
    std::cin >> choice;
    if (choice == 1) {
        string newDbName;
        std::cout << "\n";
        std::cout << "What do you want to call this new database: ";
        std::cin >> newDbName;
        std::cout << "\n";

        // Full Backup Function
        backupDatabasePostgreSQL(newDbName);
    } else if (choice == 2) {
        string newDbName;
        std::cout << "\n";
        std::cout << "What do you want to call this new database: ";
        std::cin >> newDbName;
        std::cout << "\n";

        cloudBackupPostgreSQL(newDbName);
    }
    return 1;
}


int main() {
    welcome_message();

    // Select DB Type
    string db_type = select_type();

    // connect to DB
    SAConnection* con = connect(db_type);
    if (con != nullptr) {  // Check if connection was successful
        try {
            // Run menu continuously
            int menuStatus = 0;
            while (menuStatus == 0) {
                menuStatus = choice_selection();
            }
            // SQL operations go here, e.g., querying the database
            con->Disconnect();  // Disconnect when done
        } catch (SAException &x) {
            std::cerr << "Error during database operation: " << (const char*)x.ErrText() << std::endl;
        }
        delete con;  // Clean up memory
    } else {
        std::cerr << "Failed to establish a database connection.\n";
    }
    return 0;
}