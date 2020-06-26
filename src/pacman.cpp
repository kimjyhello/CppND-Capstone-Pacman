#include "pacman.h"
#include <cmath>
#include <iostream>

void Pacman::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    //UpdateBody(current_cell, prev_cell);
  }
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
      if ((head_y + speed) * height > (screen_height - border_size - 64))
      {
        return true;
      }
      break;
    case Direction::kRight:
      if ((head_x + speed) * width > (screen_width - border_size - 64)) 
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
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
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

// Inefficient method to check if cell is occupied by snake.
bool Pacman::PacmanCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}