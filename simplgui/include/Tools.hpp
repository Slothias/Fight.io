#ifndef SIMPLGUI_TOOLS_HPP
#define SIMPLGUI_TOOLS_HPP

namespace simplgui
{

namespace tools
{

static sf::String getSfString(const std::u32string &str)
{
    if(str.empty())
        return sf::String();
    else
        return sf::String(reinterpret_cast<const sf::Uint32*>(str.c_str()));
}

}

}

#endif
