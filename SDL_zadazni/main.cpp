#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "Players.cpp"
#include "Map.cpp"
#include <typeinfo>

using namespace std;

//const int WIDTH = 1900, HEIGHT = 1100;
const int WIDTH = 900, HEIGHT = 800;
int get_distance_btw_points(float , float, float, float);
void colision(SquarePlayer* , Map*);
void circleColision(BallPlayer*, Map*);
void startAgain();

BallPlayer circle = BallPlayer(0, 0, 0);
SquarePlayer player = SquarePlayer(0,0, 0, 0);
bool win = false;
Map map;

int main(int argc, char* argv[])
{

    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if (NULL == window)
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }
    // call SDL_CreateRenderer in order for draw calls to affect this window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Rect camera;
    camera.x = 0;
    camera.y = 0;
    camera.w = WIDTH;
    camera.h = HEIGHT;
    
    

    SDL_Surface* surface = IMG_Load("lava.jpg");
    SDL_Texture* image_surface = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Surface* surface2 = IMG_Load("wall.jpg");
    SDL_Texture* wall_tex = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Surface* surface3 = IMG_Load("dirt.png");
    SDL_Texture* dirt_tex = SDL_CreateTextureFromSurface(renderer, surface3);

    vector<SDL_Texture*> TextureContener;
    SDL_Event windowEvent;
    SDL_Event event;
    SDL_Color color_circle = {245, 220, 130 ,100};
    const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);

    bool up = false;
    bool down = false;
    bool right = false;
    bool left = false;

    bool p1up = false;
    bool p1down = false;
    bool p1right = false;
    bool p1left = false;

    
    bool cam_lock = false;

    TextureContener.push_back(wall_tex);
    TextureContener.push_back(dirt_tex);
    map.load_map(TextureContener);
    vector<float> cirPos = map.saulGoodman();
    vector<float> plaPos = map.saulGoodman();
    circle = BallPlayer(cirPos.at(0), cirPos.at(1), 10);
    player = SquarePlayer(plaPos.at(0), plaPos.at(1), 20, 20);
    //SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);

    while (window)
    {
        if (win)
        {
            SDL_Delay(1000);
            startAgain();
            
        }

        SDL_ShowCursor(SDL_DISABLE);
        SDL_SetRenderDrawColor(renderer, 112, 112, 112, 255);
        SDL_RenderClear(renderer);

        map.showMap(renderer);
        map.moveMap(camera.x, camera.y);

        player.displayPlayer(renderer, color_circle, image_surface, camera.x, camera.y);
        player.move();
        circle.displayPlayer(renderer,color_circle, camera.x,camera.y);
        circle.move();
        
        SDL_RenderPresent(renderer);

        
       
        int distancePlayersX = abs(player.get_x() - circle.get_x()) - 40;
        int distancePlayersY = abs(player.get_y() - circle.get_y()) - 40;

        int scaledScreenWidth = (int)(WIDTH / 1);
        int scaledScreenHeight = (int)(HEIGHT / 1);

        if (player.get_x() > circle.get_x()) {
            camera.x = circle.get_x() + distancePlayersX / 2 - scaledScreenWidth / 2;
        }
        else camera.x = player.get_x() + distancePlayersX / 2 - scaledScreenWidth / 2;

        if (player.get_y() > circle.get_y()) {
            camera.y = circle.get_y() + distancePlayersY / 2 - scaledScreenHeight / 2;
        }
        else camera.y = player.get_y() + distancePlayersY / 2 - scaledScreenHeight / 2;


        ////edge snapping

        if (camera.x < 0) camera.x = 0;
        if (camera.y < 0) camera.y = 0;
        if (camera.x > camera.w) camera.x = camera.w;
        if (camera.y > camera.h) camera.y = camera.h;


       
        if (map.checkCollision(&circle) || map.ch(&player))
        {
            win = true;
        }
        



        if (!cam_lock)
        {
            if (up)
            {
                circle.up();
            }
            if (down)
            {
                circle.down();
            }
            if (right)
            {
                circle.right();
            }
            if (left)
            {
                circle.left();
            }

            
                if (p1up)
                {
                    player.up();
                }
                if (p1down)
                {
                    player.down();
                }
                if (p1right)
                {
                    player.right();
                }
                if (p1left)
                {
                    player.left();
                }
            }
        
      

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if ((keyboard_state_array[SDL_SCANCODE_A])) {
                    left = true;
                }
                if ((keyboard_state_array[SDL_SCANCODE_D])) {
                    right = true;
                }
                if ((keyboard_state_array[SDL_SCANCODE_S])) {
                    down = true;
                }
                if ((keyboard_state_array[SDL_SCANCODE_W])) {
                    up = true;
                }

                if ((keyboard_state_array[SDL_SCANCODE_LEFT])) {
                    p1left = true;
                }
                if ((keyboard_state_array[SDL_SCANCODE_RIGHT])) {
                    p1right = true;
                }
                if ((keyboard_state_array[SDL_SCANCODE_DOWN])) {
                    p1down = true;
                }
                if ((keyboard_state_array[SDL_SCANCODE_UP])) {
                    p1up = true;
                }
                break;
            case SDL_KEYUP:
                if (!(keyboard_state_array[SDL_SCANCODE_A])) {
                    left = false;
                }
                if (!(keyboard_state_array[SDL_SCANCODE_D])) {
                    right = false;
                }
                if (!(keyboard_state_array[SDL_SCANCODE_S])) {
                    down = false;
                }
                if (!(keyboard_state_array[SDL_SCANCODE_W])) {
                    up = false;
                }

                if (!(keyboard_state_array[SDL_SCANCODE_LEFT])) {
                    p1left = false;
                }
                if (!(keyboard_state_array[SDL_SCANCODE_RIGHT])) {
                    p1right = false;
                }
                if (!(keyboard_state_array[SDL_SCANCODE_DOWN])) {
                    p1down = false;
                }
                if (!(keyboard_state_array[SDL_SCANCODE_UP])) {
                    p1up = false;
                }
                break;
            }
        }




        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
        }

        player.enableBottom();
        player.enableTop();
        player.enableRight();
        player.enableLeft();

        if (!p1right && !p1left) {
            player.stopx();
        }
        if (!p1up && !p1down) {
            player.stopy();
        }

        if (!right && !left) {
            circle.stopx();
        }
        if (!up && !down) {
            circle.stopy();
        }
    }

    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}


int get_distance_btw_points(float x1, float y1, float x2, float y2)
{
    return (sqrt(pow(double(x2-x1),2)+ pow(double(y2-y1), 2)));
}

void startAgain()
{
    vector<float> cirPos = map.saulGoodman();
    vector<float> plaPos = map.saulGoodman();
    circle = BallPlayer(cirPos.at(0), cirPos.at(1), 10);
    player = SquarePlayer(plaPos.at(0), plaPos.at(1), 20, 20);
    cout << cirPos.at(0) << " " << cirPos.at(1) << endl;
    cout << plaPos.at(0) << " " << plaPos.at(1) << endl;
    win = false;
}