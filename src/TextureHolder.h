#pragma once
#ifndef TEXTURE_HOLLDER_H
#define TEXTURE_HOLDER_H
#include <SFML/Graphics.hpp>
#include "map"

using namespace std;
using namespace sf;

class TextureHolder
{
    //  private members
    map<string, Texture> m_Textures;        // the map to hold string and textures

    static TextureHolder* m_s_Instance;     // a pointer to this instance


    public:
    TextureHolder();
                    //         string const& == same
    static Texture& getTexture(const string& filename);    // return the instance of the texture holder

};
#endif