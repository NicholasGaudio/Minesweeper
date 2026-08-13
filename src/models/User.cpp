#include 'User.h'

class User{
private:
    string username;
    int hours;
    int minutes;
public:
    User(string username, int hours, int minutes){
        this -> username = username;
        this -> hours = hours;
        this -> minutes = minutes;
    }

    string getName(){
        return username;
    }

    string getTime() const {
        ostringstream oss;
        oss << setw(2) << setfill('0') << hours << ":" << setw(2) << setfill('0') << minutes;
        return oss.str();
    }

    bool operator<(const User& other) const {
        if (hours != other.hours) {
            return hours < other.hours; // Compare hours first
        } else {
            return minutes < other.minutes; // If hours are equal, compare minutes
        }
    }


};