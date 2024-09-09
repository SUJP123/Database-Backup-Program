#pragma once

#include <iostream>
#include <string>
#include "user.h"

class Logged {

    private:

        Logged() = default;

        Logged(const Logged&) = delete;
        Logged& operator = (const Logged&) = delete;

        UserConnection user;
        string s3name;
        string s3path;
    
    public:
        static Logged& getInstance() {
            static Logged instance;
            return instance;
        }

        string getS3Name() {
            return s3name;
        }

        void setS3Name(string newS3Name) {
            s3name = newS3Name;
        }

        string getS3Path() {
            return s3path;
        }

        void setS3Path(string newS3Path) {
            s3path = newS3Path;
        }

        UserConnection getUser() {
            return user;
        }

        void setUser(UserConnection newUser) {
            user = newUser;
        }
};