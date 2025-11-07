#include "Tile.cpp"
#include <iostream>
#include "Player.h"

// CREATE A TILESET
const unsigned int tileNum = 5;
class tileSet
{
    tile t[tileNum];

public:
    // create and load tiles here
    tileSet()
    {
        for (unsigned int i = 0; i < tileNum; i++)
        {
            std::string filename;
            filename = "Resources/" + std::to_string(i) + ".png";
            t[i].load(filename);
        }
    }
    // access individual tile here
    tile& operator[](unsigned int index) 
    {
        return t[index];
    }
};