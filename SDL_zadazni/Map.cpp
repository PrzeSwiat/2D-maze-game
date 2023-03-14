#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL.h>
#include "MapElement.cpp"
#include "Players.cpp"
#include "VectorLib.cpp"
#include <random>

using namespace std;
class Map {

public:
    vector<Element> elementContainer;
    vector<vector<int>> coordinatesContainer;
    vector<string> mapInString;
    VectorsLib vectors;
    
    void load_map(vector<SDL_Texture*> textureContainer)
    {
        srand(time(NULL));
        fstream newfile;
        string line;
        newfile.open("map.txt", ios::in); //open a file to perform read operation using file object
        if (newfile.is_open()) { //checking whether the file is open
            while (getline(newfile, line, '\n')) { //read data from file object and put it into string.
                mapInString.push_back(line);
            }
            newfile.close(); //close the file object.
        }
        float mapSizeHeight = mapInString.size();
        float mapSizeWidth = mapInString.at(0).size();
        float sizeOfBlock = 50;
        SDL_Color RecColor;
        SDL_Texture* InTexture = NULL;
        bool iswall;
        bool isWin = false;
        for (int i = 0; i < mapSizeHeight; i++)
        {
            for (int j = 0; j < mapInString.at(i).size(); j++)
            {
                if (mapInString.at(i).at(j) == ' ')
                {
                    RecColor = { 145, 177, 227 ,255 };
                    InTexture = textureContainer.at(1);
                    iswall = false;
                    isWin = false;
                }
                if (mapInString.at(i).at(j) == '#')
                {
                    RecColor = { 179, 135, 14, 255 };
                    InTexture = textureContainer.at(0);
                    iswall = true;
                    isWin = false;
                }
                if (mapInString.at(i).at(j) == 'U')
                {
                    RecColor = { 15, 191, 15 ,255 };
                    InTexture = NULL;
                    iswall = false;
                    isWin = true;
                }

                Element one = Element(sizeOfBlock * j + 1, sizeOfBlock * i + 1, sizeOfBlock, sizeOfBlock, RecColor, InTexture, iswall, isWin);
              
                elementContainer.push_back(one);
                vector<int> coordinates = {j,i};
                coordinatesContainer.push_back(coordinates);
            }

        }
    }

    void moveMap(float camX, float camY)
    {
        for (int i = 0; i < elementContainer.size(); i++)
        {
            elementContainer.at(i).move(camX, camY, coordinatesContainer.at(i));

        }
    }

    void showMap(SDL_Renderer* renderer)
    {
        for (int i = 0; i < elementContainer.size(); i++)
        {
            elementContainer.at(i).draw_rectangle(renderer);
        }
    }

    vector<Element> getEmelents()
    {
        return elementContainer;
    }

    vector<vector<int>> getCoordinates()
    {
        return coordinatesContainer;
    }

    float clamp(float min, float max, float value)
    {
        if (value < min) {
            return min;
        }
        else if (value > max) {
            return max;
        }
        else {
            return value;
        }
    }

    bool ch(SquarePlayer* player) {
        int type = 0;
        int counter = 0;
        float mapSizeHeight = mapInString.size();
        float mapSizeWidth = mapInString.at(0).size();

        for (int row = 0; row < mapSizeHeight; row++) {
            for (int col = 0; col < mapSizeWidth; col++) {
                float playerX = player->get_x();
                float playerY = player->get_y();
                float tileX = col * 50;
                float tileY = row * 50;
                
                    float left = playerX + player->get_width() - tileX;
                    float right = tileX + 50 - playerX;
                    float top = playerY + player->get_height() - tileY;
                    float bottom = tileY + 50 - playerY;
                    if (left > 0 && right > 0 && top > 0 && bottom > 0) {
                        if (elementContainer.at(counter).checkWin()) {
                            return true;
                        }
                        if (elementContainer.at(counter).checkWall())
                        {

                            float separatedX;
                            float separatedY;

                            if (left < right) {
                                separatedX = -left;
                            }
                            else {
                                separatedX = right;
                            }

                            if (top < bottom) {
                                separatedY = -top;
                            }
                            else {
                                separatedY = bottom;
                            }

                            if (abs(separatedX) < abs(separatedY)) {
                                separatedY = 0;
                            }
                            else separatedX = 0;

                            std::vector<float> wektorek = { separatedX, separatedY };
                            player->set_x(player->get_x() + separatedX);
                            player->set_y(player->get_y() + separatedY);
                        }
                    }
                    counter++;
            }
        }
        return false;
    }

    bool checkCollision(BallPlayer* player) {
        int type = 0;
        int counter = 0;
        float mapSizeHeight = mapInString.size();
        float mapSizeWidth = mapInString.at(0).size();


        for (int row = 0; row < mapSizeHeight; row++) {
            for (int col = 0; col < mapSizeWidth; col++) {
                float playerX = player->get_x();
                float playerY = player->get_y();
                float tileX = col * 50;
                float tileY = row * 50;

                    float radius = player->get_radius();
                    float centerX = playerX;
                    float centerY = playerY;

                    std::vector<float> playerWektor = { centerX, centerY };
                    std::vector<float> rectWektor = { clamp(tileX, tileX + 50, centerX),
                        clamp(tileY, tileY + 50, centerY) };

                    std::vector<float> distancevec = vectors.subbtractVectors(playerWektor, rectWektor);
                    float distance = vectors.lengthVector(distancevec);
                    if (distance < radius) {
                        if (elementContainer.at(counter).checkWin()) {
                            return true;
                        }
                        if (elementContainer.at(counter).checkWall())
                        {
                        
                            
                                if (vectors.equals(playerWektor, rectWektor)) {

                                }
                                else
                                {
                                    std::vector<float> wektor = vectors.subbtractVectors(playerWektor, rectWektor);
                                    vectors.divideScalarVectors(wektor, distance);
                                    vectors.multipyScalarVectors(wektor, radius - distance);
                                    player->set_x(player->get_x() + wektor.at(0));
                                    player->set_y(player->get_y() + wektor.at(1));
                                }

                        }
                    }
                    counter++;
            }
        }
        return false;
    }

    vector<float> saulGoodman()
    {
        while (true)
        {
            int random = 0 + rand() % (elementContainer.size() - 0 + 1);  //from 0 to elementContainer.size()
            if (!elementContainer.at(random).checkWall() && !elementContainer.at(random).checkWin() && 
                elementContainer.at(random).get_x()>50 && elementContainer.at(random).get_y()>50)
            {
                vector<float> pos = { elementContainer.at(random).get_x(), elementContainer.at(random).get_y()};
                return pos;
            }
        }
        
    }
};
