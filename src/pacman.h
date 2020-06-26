#ifndef PACMAN_H
#define PACMAN_H

#include <vector>
#include "SDL.h"

class Pacman {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Pacman(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update();

  //void GrowBody();
  bool PacmanCell(int x, int y);
  bool TouchingBorder(Direction dir);

  Direction direction = Direction::kRight;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

  void SetBorderSize(int i);
  void SetScreenWidth(int i);
  void SetScreenHeight(int i);

 private:
  void UpdateHead();
  //void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;

  int border_size;
  int screen_width;
  int screen_height;
};

#endif