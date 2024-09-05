#include <iostream>
#include "SQLAPI/include/SQLAPI.h"
#include "connect.h"
#include "user.h"
#include <stdio.h>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream> 
#include "logged.h"

using namespace std;


// Function to run a system command and check if it succeeded
bool runCommand(const string& command) {
    int result = system(command.c_str());
    return result == 0;
}

bool backupDatabasePostgreSQL(string newDbName) {

    // Step 1: Create Backup

    UserConnection user = Logged::getInstance().getUser();
    // Get the current time
    time_t rawTime;
    time(&rawTime);

    // Convert the time to a format suitable for a file name
    struct tm* timeInfo;
    timeInfo = localtime(&rawTime);

    stringstream ss;
    ss << user.db_name << "-" << put_time(timeInfo, "%Y%m%d_%H%M%S") << ".dump";
    string backupFile = ss.str();
    string dumpCommand = "pg_dump -U " + user.username + " -h " + user.host + " -p " + user.port + " -F c -b -v -f " + backupFile + " " + user.db_name;

    cout << "Creating backup of database '" << user.db_name << "'..." << endl;
    if (!runCommand(dumpCommand)) {
        cerr << "Failed to create backup of database '" << user.db_name << "'." << endl;
        return false;
    }

    // Step 2: Create a new database
    string createDbCommand = "createdb -U " + user.username + " -h " + user.host + " -p " + user.port + " " + newDbName;
    cout << "Creating new database '" << newDbName << "'..." << endl;
    if (!runCommand(createDbCommand)) {
        cerr << "Failed to create new database '" << newDbName << "'." << endl;
        return false;
    }

    // Step 3: Restore the backup to the new database
    string restoreCommand = "pg_restore -U " + user.username + " -h " + user.host + " -p " + user.port + " -d " + newDbName + " -v " + backupFile;

    cout << "Restoring backup to new database '" << newDbName << "'..." << endl;
    if (!runCommand(restoreCommand)) {
        cerr << "Failed to restore backup to database '" << newDbName << "'." << endl;
        return false;
    }

    cout << "Database '" << user.db_name << "' has been successfully replicated to '" << newDbName << "'." << endl;
    return true;
}