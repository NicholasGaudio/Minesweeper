#pragma once

#include <string>

class User
{
private:
    std::string username;
    int hours;
    int minutes;

public:
    User(std::string username, int hours, int minutes);

    std::string getName();
    std::string getTime() const;

    bool operator<(const User& other) const;
};