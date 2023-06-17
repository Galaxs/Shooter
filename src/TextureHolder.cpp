#include "TextureHolder.h"
#include "assert.h"

TextureHolder* TextureHolder::m_s_Instance = nullptr;           // acceding the private STATIC member through the class name itself

TextureHolder::TextureHolder ()
{
    // asert the singleton
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;
}

Texture& TextureHolder::getTexture(const string& filename)
{
    // auto aka map<string, texture> -- GET A REFERENCE of textures THROUGH IT OWN INSTANCE CLASS
    map<string, Texture>& textures = m_s_Instance->m_Textures;

    // create a iterator to hold the key value pair
    map<string, Texture>::iterator iter = textures.find(filename);

    //  Did we found a match?
    if (iter != textures.end())
    {
        return iter->second;
    }
    else
    {
        //  texture not found / Load a new texture using the filename as string filename path
        Texture& newTexture = textures[filename];
        newTexture.loadFromFile(filename);
        return newTexture;
    }

            /*  VERY CUTE, BUT I THINK THAT iter.find() ALREDY ITERATE THROUGH THE MAP  */
            //  my idea was to loop though the map and find the texture myself to return it
            //  perhaps i should (inside the loop..): iter == Texure??
    // for (iter = textures.begin(); iter != textures.end(); ++iter)
    // {
    //     if (textures.find(filename) != textures.end())
    //     {
    //         /* code */
    //         return iter->second;
    //     }
    // }
    
}