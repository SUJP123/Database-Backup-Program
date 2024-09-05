#pragma once

#include <iostream>
#include <string>
#include "user.h"
#include "logged.h"
#include "SQLAPI/include/SQLAPI.h"

using namespace std;

UserConnection requestParamsPostgreSQL() {
    UserConnection user;
    
    std::cout<< "Please fill out the following params to connect to your desired database.\n";
    std::cout<< "Database Name: ";
    std::cin >> user.db_name;

    std::cout<< "username: ";
    std::cin >> user.username;

    std::cout<< "password: ";
    std::cin >> user.password;

    std::cout<< "host: ";
    std::cin >> user.host;

    std::cout<< "port: ";
    std::cin >> user.port;
    
    Logged::getInstance().setUser(user);
    return user;
}


UserConnection requestConnectionParams(string database) {
    UserConnection user;
    if (database == "PostgreSQL") {
        user = requestParamsPostgreSQL();
    } else {
        std::cout <<"Error processing database request\n";
    }

    return user;
}

SAConnection* connectToPostgreSQL(UserConnection user) {
    std::string db_url = user.host + "," + user.port + "@" + user.db_name;
    SAConnection* connect = new SAConnection();
    try {
        connect->Connect(
            db_url.c_str(),
            user.username.c_str(),
            user.password.c_str(),
            SA_PostgreSQL_Client
        ); 
    } catch (SAException &x) {
        printf("%s\n", (const char*)x.ErrText());
        delete connect;
        return nullptr;
    }
    return connect;
}

SAConnection* connect(string database) {
    UserConnection user = requestConnectionParams(database);

    if (database == "PostgreSQL") {
        SAConnection* con = connectToPostgreSQL(user);
        return con;
    }


    return nullptr;
}