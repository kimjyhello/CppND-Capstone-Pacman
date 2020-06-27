#include "pacman.h"
#include <cmath>
#include <iostream>

void Pacman::Update() {
  /*
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
          */
  UpdateHead();
  /*SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.
*/
  // Update all of the body vector items if the snake head has moved to a new
  // cell.
 // if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    //UpdateBody(current_cell, prev_cell);
 // }
}

int Pacman::GetPacmanSize() 
{
  return pacman_size;
}

void Pacman::SetBorderSize(int i)
{
  border_size = i;
}

void Pacman::SetScreenWidth(int i)
{
  screen_width = i;
}

void Pacman::SetScreenHeight(int i) 
{
  screen_height = i;
}


bool Pacman::TouchingBorder(Direction dir)
{  
  int width= screen_width / grid_width;
  int height = screen_height / grid_height;
  switch (direction) {
    case Direction::kUp:
      if ((head_y - speed) * height < border_size) // / grid_height) 
      {
        return true;
      }
      break;
    case Direction::kDown:
      if ((head_y + speed) * height > (screen_height - border_size - 32))
      {
        return true;
      }
      break;
    case Direction::kRight:
      if ((head_x + speed) * width > (screen_width - border_size - 32)) 
        return true;
      break;
    case Direction::kLeft:
      if ((head_x - speed) * width < border_size)
        return true;
      break;
  }
  return false;
}

void Pacman::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      if (!(TouchingBorder(direction)))
        head_y -= speed;
      break;

    case Direction::kDown:
      if (!(TouchingBorder(direction)))
        head_y += speed;
      break;

    case Direction::kLeft:
      if (!(TouchingBorder(direction)))
        head_x -= speed;
      break;

    case Direction::kRight:
      if (!(TouchingBorder(direction)))
        head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  //head_x = fmod(head_x + grid_width, grid_width);
  //head_y = fmod(head_y + grid_height, grid_height);
}
/*
void Pacman::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}*/

//void Pacman::GrowBody() { growing = true; }

bool Pacman::TouchPacman(int x, int y, int c_x, int c_y)
{
  float c_w = screen_width / grid_width;
  float c_h = screen_height / grid_height;

  float pLeft, cLeft;
  float pRight, cRight;
  float pTop, cTop;
  float pBottom, cBottom;

  pLeft = x;
  pRight = x + 1; //pacman_size/c_w;
  pTop = y;
  pBottom = y + 1; //pacman_size/c_h;

  cLeft = c_x;
  cRight = c_x + 1; // + c_w;
  cTop = c_y;
  cBottom = c_y + 1; //+ c_h;

  if (pBottom < cTop) 
    return false;
  if (pTop > cBottom)
    return false;
  if (pRight < cLeft)
    return false;
  if (pLeft > cRight)
    return false;

  return true; 
}

// Inefficient method to check if cell is occupied by snake.
bool Pacman::PacmanCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }

  /*
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }*/
  return false;
}

void Pacman::RandDirection()
{
  direction = (Pacman::Direction)(rand() % 4);
}