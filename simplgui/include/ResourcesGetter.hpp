#ifndef SIMPLGUI_RESOURCESGETTER_HPP
#define SIMPLGUI_RESOURCESGETTER_HPP

#include <memory>
#include <SFML/Graphics/Font.hpp>

namespace simplgui
{

class ResourcesGetter
{
public:
    using Ptr = std::shared_ptr<ResourcesGetter>;

    virtual std::shared_ptr<sf::Font> loadFont(const std::string &name) = 0;
    
protected:
    ResourcesGetter() {}; 
};

}

#endif

