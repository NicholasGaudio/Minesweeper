#include "WelcomeScreen.h"
#include "../utils/UIUtils.h"

#include <vector>
#include <cctype>

string WelcomeScreen(int width, int height, sf::Font font){
    sf::RenderWindow window(sf::VideoMode(width, height), "Welcome Screen");
    window.clear(sf::Color::Blue);
    vector<sf::Drawable*> drawings;

    //Creating texts

    sf::Text welcome;
    welcome.setFont(font);
    welcome.setString("WELCOME TO MINESWEEPER!");
    welcome.setCharacterSize(24);
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcome.setFillColor(sf::Color::White);
    setText(welcome, width/2, height/2 -150);
    drawings.push_back(&welcome);

    sf::Text enter;
    enter.setFont(font);
    enter.setString("Enter your name:");
    enter.setCharacterSize(20);
    enter.setStyle(sf::Text::Bold);
    enter.setFillColor(sf::Color::White);
    setText(enter, width/2, height/2 -75);
    drawings.push_back(&enter);

    sf::Text username;
    username.setFont(font);
    string User = "";
    username.setString(User + "|");
    username.setCharacterSize(18);
    username.setStyle(sf::Text::Bold);
    username.setFillColor(sf::Color::Yellow);
    setText(username, width/2, height/2 -45);
    drawings.push_back(&username);



    //Window Open/Events
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
                return "";
            }
            if (event.type == sf::Event::TextEntered){
                if ((event.text.unicode == 8) ||(event.text.unicode >= 97 && event.text.unicode <= 122) || (event.text.unicode >= 65 && event.text.unicode <=90)){
                    if (event.text.unicode == 8){
                        if (!User.empty()){
                            User.pop_back();
                        }
                    } else {
                        if (User.size() < 10){
                            if (User.size() == 0){
                                User+= static_cast<char>(toupper(static_cast<unsigned char>(event.text.unicode)));

                            } else {
                                User+= static_cast<char>(tolower(static_cast<unsigned char>(event.text.unicode)));
                            }
                        }
                    }
                    username.setString(User + "|");
                    setText(username, width/2, height/2 -45);
                }
            } else if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf:: Keyboard:: Enter){
                    if ((!User.empty()) && (User != "|")){
                        return User;
                    }
                }
            }

            //Drawing
            window.clear(sf::Color::Blue);
            for (int i =0; i<drawings.size(); i++){
                window.draw(*drawings.at(i));
            }
            window.display();
        }
    }
    return "";
}