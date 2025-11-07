#include "GamesEngineeringBase.h"

// CREATE ONE TILE
class tile
{
    GamesEngineeringBase::Image sprite;

public:
    tile()
    {

    }

    void load(std::string filename)
    {
        sprite.load(filename);
    }

    void draw(GamesEngineeringBase::Window& canvas, int x, int y)
    {
        for (unsigned int i = 0; i < sprite.height; i++)
        {
            // bounds checking for Y
            if (y + i > 0 && (y + i) < canvas.getHeight())
            {
                for (unsigned int j = 0; j < sprite.width; j++)
                {
                    // bounds checking for X
                    if (x + j > 0 && (x + j) < canvas.getWidth())
                    {
                        canvas.draw(x + j, y + i, sprite.atUnchecked(j, i));
                    }
                }
            }
        }
    }
};