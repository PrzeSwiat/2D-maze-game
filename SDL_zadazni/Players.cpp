#ifndef Players
#define Players

#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include "VectorLib.cpp"
#include <SDL.h>
#include <SDL.h>
using namespace std;

class Player {
private:
	VectorsLib vectors;
	std::vector<float> position ;
	std::vector<float> velocity = {0,0};
	std::vector<float> acceleration = {0,0};
	float playerspped = 4.0f;
	bool leftB = true;
	bool rightB = true;
	bool top = true;
	bool bottom = true;

	float acceleration_unit = 2;
	float move_unit = 1;

public:
	Player(float g_x, float g_y)
	{
		position.push_back(g_x);
		position.push_back(g_y);
		velocity.push_back(0);
		velocity.push_back(0);
	}

	float get_x()
	{
		return position.at(0);
	}
	float get_y()
	{
		return position.at(1);
	}
	float get_accelerationX()
	{
		return acceleration.at(0);
	}
	float get_accelerationY()
	{
		return acceleration.at(1);
	}
	float get_velX()
	{
		return velocity.at(0);
	}
	float get_velY()
	{
		return velocity.at(1);
	}

	void set_x(float X)
	{
		position.at(0) = X;
	}
	void set_y(float Y)
	{
		position.at(1) = Y;
	}
	void set_accelerationX(float X)
	{
		acceleration.at(0) = X;
	}
	void set_accelerationY(float Y)
	{
		acceleration.at(1) = Y;
	}
	void set_velX(float x)
	{
		velocity.at(0) = x;
	}
	void set_velY(float y)
	{
		velocity.at(1) = y;
	}

	void move()
	{
		position.at(0) += velocity.at(0);
		position.at(1) += velocity.at(1);
	}

	bool equal(Player player)
	{
		if (this->get_x() == player.get_x() && this->get_y() == player.get_y())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void up()
	{
		if(top)
		//acceleration.at(1) -= acceleration_unit;
		velocity.at(1) = -playerspped;
	}
	void down()
	{
		if (bottom)
		//acceleration.at(1) += acceleration_unit;
		velocity.at(1) = playerspped;
	}
	void left()
	{
		if(leftB)
		//acceleration.at(0) -= acceleration_unit;
		velocity.at(0) = -playerspped;
	}
	void right()
	{
		if(rightB)
		//acceleration.at(0) += acceleration_unit;
		velocity.at(0) = playerspped;
	}
	void stopx() {
		velocity.at(0) = 0;
	}
	void stopy() {
		velocity.at(1) = 0;
	}


	void enableLeft()
	{
		leftB = true;
	}
	void disableLeft()
	{
		leftB = false;
	}

	void enableRight()
	{
		rightB = true;
	}
	void disableRight()
	{
		rightB = false;
	}

	void enableTop()
	{
		top = true;
	}
	void disableTop()
	{
		top = false;
	}
	void enableBottom()
	{
		bottom = true;
	}

	void disableBottom()
	{
		bottom = false;
	}

};

class BallPlayer : public Player
{
	float radius;
public:
	BallPlayer(float g_x, float g_y, float g_radius):Player(g_x, g_y)
	{
		radius = g_radius;
	}

	float get_radius()
	{
		return radius;
	}
	void set_radius(float Radius)
	{
		radius = Radius;
	}

	void displayPlayer(SDL_Renderer* renderer, SDL_Color color , float cameraX, float cameraY)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		for (int w = 0; w < radius * 2; w++)
		{
			for (int h = 0; h < radius * 2; h++)
			{
				float dx = radius  - w; // horizontal offset
				float dy = radius  - h; // vertical offset
				if ((dx * dx + dy * dy) <= (radius * radius))
				{
					SDL_RenderDrawPoint(renderer, this->get_x() + dx - cameraX, this->get_y() + dy - cameraY);
				}
			}
		}
	}
};

class SquarePlayer : public Player
{
	float width;
	float height;
public:
	SquarePlayer(float g_x, float g_y, float w, float h) :Player(g_x, g_y)
	{
		width = w;
		height = h;
	}

	void set_width(float w) {
		width = w;
	}
	void set_height(float h) {
		height = h;
	}

	float get_width()
	{
		return width;
	}
	float get_height()
	{
		return height;
	}

	void displayPlayer(SDL_Renderer* renderer, SDL_Color color, SDL_Texture* texture , float cameraX, float cameraY)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

		SDL_Rect r;
		r.x = this->get_x() - cameraX;
		r.y = this->get_y() - cameraY;
		r.w = width;
		r.h = height;

		if (texture)
		{
			SDL_RenderCopy(renderer, texture, nullptr, &r);
		}
		else
		{
			SDL_RenderFillRect(renderer, &r);
		}
	}

	void separete()
	{

	}
};
#endif // !Players