#pragma once

#include <iostream>
#include <string>

using namespace std;

// user connection
struct UserConnection {
    public:
        string db_name;
        string username;
        string password;
        string host;
        string port;
};