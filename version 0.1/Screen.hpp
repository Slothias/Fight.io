#ifndef SCREEN_H
#define SCREEN_H
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
enum State
{
     main_menu=0,play
};
class Screen
{
protected:
    bool change=false;
 public:
     virtual void draw ()=0;
     virtual void handle(sf::Event event)=0;
     bool change_me() { return change;};
     virtual sf::Music& getMusic()=0;;
};

#endif // SCREEN_H
