#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
enum State
{
     main_menu=0,connecting,play
/* egy másik komment */
};
class Screen
{
protected:
    bool change;
 public:
     Screen() {change=false;};
     virtual void draw () =0;
     virtual void handle(sf::Event& event)=0;
     bool change_me() { return change;};
     static const int FRAMERATE=120;
};

#endif // SCREEN_H
