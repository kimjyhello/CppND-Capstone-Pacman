#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "pacman.h"
#include "SDL_image.h"

class WTexture;
const int PACMAN_ANIMATION_FRAMES = 2;
const int PACMAN_DIRECTIONS = 4;

/*
  C6: One function is overloaded with different signatures for the same function name.
*/
struct delete_sdl
{
  void operator() (SDL_Renderer *r) const {
    SDL_DestroyRenderer(r);
  }
  void operator() (SDL_Texture *t) const {
    SDL_DestroyTexture(t);
  }
  void operator() (SDL_Window *w) const {
    SDL_DestroyWindow(w);
  }
};

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Pacman const pacman, SDL_Point const &food);
  void UpdateWindowTitle(int score, int fps);
  void loadMedia();
  void start();

  const std::size_t GetScreenWidth();
  const std::size_t GetScreenHeight();
  const std::size_t GetBorderWidth();

 private:
 /* 
   The project uses at least one smart pointer. 
 */
  std::unique_ptr<SDL_Window, delete_sdl> sdl_window;
  std::shared_ptr<SDL_Renderer> sdl_renderer;
  WTexture *sdl_texture;
  SDL_Rect pacmanMoves[PACMAN_ANIMATION_FRAMES * PACMAN_DIRECTIONS];

  WTexture *_message_texture;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  const std::size_t border_width;

  int frame{0};

  bool _started {false};
};

class WTexture
{
 public:
  WTexture(SDL_Renderer *renderer);
  ~WTexture();

  bool loadFromFile( std::string path );
  void free();
  void render( int x, int y, SDL_Rect* clip = NULL );

  int getWidth();
  int getHeight();


 private:
  std::unique_ptr<SDL_Texture, delete_sdl> _texture;
  SDL_Renderer *_renderer; 

  int _width;
  int _height;
};

#endif