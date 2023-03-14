#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL.h>
using namespace std;

class Element {
private:
    std::vector<float> position = {0,0};
    std::vector<float> paramiters = {0,0};
    SDL_Color color;
    SDL_Texture* texture;
    bool isWall;
    bool isWin;

public:
	Element(float x, float y, float w, float h, SDL_Color col,SDL_Texture* tex, bool iswallt, bool iswint)
	{
		position.at(0) = x;
		position.at(1) = y;
		paramiters.at(0) = w;
		paramiters.at(1) = h;
        color = col;
        texture = tex;
        isWall = iswallt;
        isWin = iswint;
	}

    void draw_rectangle(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        SDL_Rect r;
        r.x = position.at(0);
        r.y = position.at(1);
        r.w = paramiters.at(0);
        r.h = paramiters.at(1);

        if (texture)
        {
            SDL_RenderCopy(renderer, texture, nullptr, &r);
        }
        else
        {
            SDL_RenderFillRect(renderer, &r);
        }
    }

    void move(float camX, float camY, vector<int> coordinates)
    {
        position.at(0) = coordinates.at(0)*50 - camX;
        position.at(1) = coordinates.at(1)*50 - camY;
    }

    float get_x()
    {
        return position.at(0);
    }
    float get_y()
    {
        return position.at(1);
    }

    bool checkWall()
    {
        return isWall;
    }

    bool checkWin()
    {
        return isWin;
    }

    void set_x(float gx)
    {
        position.at(0) = gx;
    }
    void set_y(float gx)
    {
        position.at(1) = gx;
    }
};