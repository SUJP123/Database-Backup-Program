#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "SQLAPI/include/SQLAPI.h"
#include "connect.h"

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


int main() {
    welcome_message();

    // Select DB Type
    string db_type = select_type();

    // connect to DB
    SAConnection* con = connect(db_type);
    if (con != nullptr) {  // Check if connection was successful
        try {
            SACommand cmd(con, "SELECT * FROM exercise");
            cmd.Execute();
            while (cmd.FetchNext()) {
                std::cout << (const char*)cmd.Field("workout_id").asString() << std::endl;
            }
            // Your SQL operations go here, e.g., querying the database
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