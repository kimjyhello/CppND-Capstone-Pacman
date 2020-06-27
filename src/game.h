#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "pacman.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, 
        std::size_t border_width, std::size_t border_height,
        std::size_t pacman_size);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  /*
    C8: At least two functions use pass-by-reference in the project code
  */
  void Wait(Renderer &renderer);
  void GameOver(Renderer &renderer);

 private:
  Pacman pacman;
  Pacman ghost; 
  SDL_Point coin;
  bool _started {false};
  double border_width;
  double border_height;

  std::size_t grid_width;
  std::size_t grid_height;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int level{0};

  void PlaceCoin();
  void PlaceGhost();
  void Update();
};

#endif