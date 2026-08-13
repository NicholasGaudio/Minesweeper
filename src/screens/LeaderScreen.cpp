#include "LeaderScreen.h"

#include "../models/User.h"
#include "../utils/UIUtils.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

bool LeaderScreen(string username, int minutes, int seconds, int columns, int rows, sf::Font font) {
    sf::RenderWindow leaderboardWindow(sf::VideoMode(columns * 16, rows * 16 + 50), "Leaderboard Window");
    vector<sf::Drawable*> leaderDrawing;

    //Creating text
    sf::Text leaderTitle;
    leaderTitle.setFont(font);
    leaderTitle.setString("LEADERBOARD");
    leaderTitle.setCharacterSize(20);
    leaderTitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
    leaderTitle.setFillColor(sf::Color::White);
    setText(leaderTitle, (columns * 16) / 2, (rows * 16 + 50) / 2 - 120);
    leaderDrawing.push_back(&leaderTitle);

    //Reading file data
    string leaderboardData;
    ifstream leaderboardFile("files/leaderboard.txt");
    string line;
    vector<User> users;
    users.emplace_back(username, minutes, seconds);
    while (getline(leaderboardFile, line)) {

        stringstream ss(line);
        string timeString, name;
        getline(ss, timeString, ',');
        getline(ss, name);

        if (name.back() == '*') {
            // Remove the asterisk from the username
            name.pop_back();
        }

        stringstream timeSS(timeString);
        string hourString, minuteString;
        getline(timeSS, hourString, ':');
        getline(timeSS, minuteString);

        int hours = stoi(hourString);
        int minutes = stoi(minuteString);

        users.emplace_back(name, hours, minutes);
    }
    // Write the updated leaderboard data to the file
    sort(users.begin(), users.end());
    string megaString;
    for (int i = 0; i < 5; i++) {
        string name = users[i].getName();
        string correctUsername = " " + username;
        if ((i < 5) && (correctUsername == name)) {
            name += "*"; // Add asterisk to the username if it's in the top 5
        }
        megaString += to_string(i + 1) + "\t" + users[i].getTime() + "\t" + name + "\n\n";
    }
    sf::Text mega;
    mega.setFont(font);
    mega.setString(megaString);
    mega.setCharacterSize(18);
    mega.setStyle(sf::Text::Bold);
    mega.setFillColor(sf::Color::White);
    setText(mega, (columns * 16) / 2, (rows * 16 + 50) / 2 + 20);
    leaderDrawing.push_back(&mega);

    //While Screen open
    while (leaderboardWindow.isOpen()) {
        sf::Event leaderEvents;
        while (leaderboardWindow.pollEvent(leaderEvents)) {
            if (leaderEvents.type == sf::Event::Closed) {
                leaderboardWindow.close();
                return false;
            }
        }

        //Drawing
        leaderboardWindow.clear(sf::Color::Blue);
        for (int i = 0; i < leaderDrawing.size(); i++) {
            leaderboardWindow.draw(*leaderDrawing.at(i));
        }

        leaderboardWindow.display();
    }
}