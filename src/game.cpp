#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t border_width, std::size_t border_height, std::size_t pacman_size)
    : pacman(grid_width, grid_height, (int)pacman_size),
      ghost(grid_width, grid_height, (int)pacman_size),
      engine(dev()),
      random_w(border_width, static_cast<int>(grid_width - (border_width * 2) )),
      random_h(border_width, static_cast<int>(grid_height - (border_height * 2))),
      border_width((double)border_width),
      border_height((double)border_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  PlaceCoin();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  renderer.start();

  pacman.SetBorderSize(renderer.GetBorderWidth());
  pacman.SetScreenHeight(renderer.GetScreenHeight());
  pacman.SetScreenWidth(renderer.GetScreenWidth());
  ghost.SetBorderSize(renderer.GetBorderWidth());
  ghost.SetScreenHeight(renderer.GetScreenHeight());
  ghost.SetScreenWidth(renderer.GetScreenWidth());

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, pacman);
    Update();
    renderer.Render(pacman, coin, ghost);

    if (!pacman.alive) 
    {
      GameOver(renderer);
      running = false;
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count, level);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  //GameOver(renderer);
}

void Game::GameOver(Renderer &renderer) {
  SDL_Event e;
  bool pressed = false;

  Update();
  renderer.Render(pacman, coin, ghost);

  while (!pressed) 
  {
    while ( SDL_PollEvent(&e) ) 
    {
      if (e.type == SDL_KEYDOWN) 
      {
        if (e.key.keysym.sym == SDLK_q)
          pressed = true; 
      }
    }
  }
}


/*
  C-2: The accepts input from a user as part of the necessary operation of the program.
*/
void Game::Wait(Renderer &renderer) {
  SDL_Event e;
  bool pressed = false;

  Update();
  renderer.Render(pacman, coin, ghost);

  while (!pressed) 
  {
    while ( SDL_PollEvent(&e) ) 
    {
      if (e.type == SDL_KEYDOWN) 
      {
        pressed = true; 
        _started = true; 
      }
    }
  }
}

void Game::PlaceCoin() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // coin.
    if (!pacman.PacmanCell(x, y)) {
      coin.x = x;
      coin.y = y;
      return;
    }
  }
}

void Game::PlaceGhost() 
{
  int x, y;
  while (true) 
  {
    x = random_w(engine);
    y = random_h(engine);

//    if (!pacman.PacmanCell(x, y)) {
    if (!pacman.TouchPacman(pacman.head_x, pacman.head_y, x, y)) {
      ghost.head_x = x;
      ghost.head_y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!pacman.alive) return;

  pacman.Update();

  int new_x = static_cast<int>(pacman.head_x) + border_width;
  int new_y = static_cast<int>(pacman.head_y) + border_height;

  // Check if there's coin over here
  if (pacman.TouchPacman(new_x, new_y, coin.x, coin.y)) {
    score += (level + 1) * 10;
    level++; 
    PlaceCoin();

    if (level == 1) {
      ghost.show = true; 
    }
    PlaceGhost();
    // Grow snake and increase speed.
    pacman.speed += 0.02;
    ghost.speed += 0.02;
  }
  
  if (level >= 1) {
    if (pacman.TouchPacman(new_x, new_y, ghost.head_x, ghost.head_y)) {
      pacman.alive = false;
    }
  }

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return pacman.size; }