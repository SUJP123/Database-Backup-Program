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

int backupToS3(string newDbName) {

    UserConnection user = Logged::getInstance().getUser();
    string S3name = Logged::getInstance().getS3Name();
    //string s3path = Logged::getInstance().getS3Path();

    time_t rawTime;
    time(&rawTime);
    char timeBuffer[80];
    strftime(timeBuffer, 80, "%Y%m%d_%H%M%S", localtime(&rawTime));

    string backupFileName = user.db_name + "_backup_" + timeBuffer + ".sql";

    // Step 2: Use pg_dump to create a backup of the PostgreSQL database
    string pg_dump_cmd = "pg_dump -U " + user.username + " -F c " + user.db_name + " > " + backupFileName;
    cout << "Executing: " << pg_dump_cmd << endl;
    
    int dumpResult = system(pg_dump_cmd.c_str());
    if (dumpResult != 0) {
        cerr << "Error: pg_dump failed with code " << dumpResult << endl;
        return dumpResult;
    }

    // Step 3: Use AWS CLI to upload the backup file to S3
    string s3_path = "s3://" + S3name + "/" + backupFileName;
    string aws_s3_cmd = "aws s3 cp " + backupFileName + " " + s3_path;
    cout << "Uploading backup to: " << s3_path << endl;

    int s3UploadResult = system(aws_s3_cmd.c_str());
    if (s3UploadResult != 0) {
        cerr << "Error: Upload to S3 failed with code " << s3UploadResult << endl;
        return s3UploadResult;
    }

    // Step 4: Clean up the local backup file after successful upload
    string remove_cmd = "rm " + backupFileName;
    system(remove_cmd.c_str());

    cout << "Database backup successfully uploaded to S3: " << s3_path << endl;
    return 0;
}


int logS3crediantials() {

    Logged& instance = Logged::getInstance();
    
    int choice;
    std::cout <<"Would you like to change your sessions S3 credentials (Type the #)\n";
    std::cout <<"------------------------------------------------------------------\n";
    std::cout <<"1. Yes\n";
    std::cout <<"2. No\n";

    if (choice == 2) {
        return 1;
    } else if (choice == 1) {
        string S3name;
        string S3path;

        std::cout<<"Paste your S3 bucket name here: ";
        std::cin >> S3name;
        std::cout<< "\n";
        // std::cout<<"Paste your S3 bucket path here: ";
        // std::cin >> S3path;
        // std::cout<<"\n";
        std::cout<<"Succesfully Logged S3 credentials";
    }

    return 1;
}

// Provide different choices
int cloudBackupPostgreSQL(string newDbName) {

    int backupChoice;
    std::cout <<"Select an Option Below (Type the #)\n";
    std::cout <<"-------------------------------------\n";
    std::cout <<"1. Backup on S3 Bucket\n";
    std::cout <<"2. Backup on Amazon RDS\n";
    std::cin >> backupChoice;
    if (backupChoice == 1) {
        logS3crediantials();
        backupToS3(newDbName);
    }
    return 0;
}