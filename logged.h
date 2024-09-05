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
    
    public:
        static Logged& getInstance() {
            static Logged instance;
            return instance;
        }

        UserConnection getUser() {
            return user;
        }

        void setUser(UserConnection newUser) {
            user = newUser;
        }
};