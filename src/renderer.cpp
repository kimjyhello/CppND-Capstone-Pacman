#include "renderer.h"
#include <iostream>
#include <string>


const std::size_t Renderer::GetScreenWidth() 
{
  return screen_width;
}

const std::size_t Renderer::GetScreenHeight()
{
  return screen_height;
}

const std::size_t Renderer::GetBorderWidth()
{
  return border_width;
}

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, 
                   const std::size_t grid_height,
                   const std::size_t border_size)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      border_width(border_size) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = std::unique_ptr<SDL_Window, delete_sdl>(SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN));

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(sdl_window.get(), -1, SDL_RENDERER_ACCELERATED), 
    [](SDL_Renderer* r) { SDL_DestroyRenderer(r); } );
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  else
  {
    int imgFlags = IMG_INIT_PNG;
    if ( !( IMG_Init( imgFlags ) & imgFlags) ) {
      std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
      
    }
  }

  sdl_texture = new WTexture(sdl_renderer.get());
  ghost_texture = new WTexture(sdl_renderer.get());
}


Renderer::~Renderer() {
  //SDL_DestroyWindow(sdl_window);
  std::cout << "DELETING Renderer\n";
  IMG_Quit();
  SDL_Quit();
}


void Renderer::Render(Pacman const pacman, SDL_Point const &coin, Pacman& ghost) {

  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer.get(), 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer.get());

  SDL_Rect borderLeft = {0, 0, (int)border_width, (int)screen_height}; 
  SDL_SetRenderDrawColor( sdl_renderer.get(), 0xB4, 0xED, 0xE0, 0xFF);
  SDL_RenderFillRect( sdl_renderer.get(), &borderLeft);

  SDL_Rect borderTop = {0, 0, (int)screen_width, (int)border_width}; 
  SDL_SetRenderDrawColor( sdl_renderer.get(), 0xB4, 0xED, 0xE0, 0xFF);
  SDL_RenderFillRect( sdl_renderer.get(), &borderTop);

  SDL_Rect borderRight = {(int)screen_width - (int)border_width, 0, (int)border_width, (int)screen_height}; 
  SDL_SetRenderDrawColor( sdl_renderer.get(), 0xB4, 0xED, 0xE0, 0xFF);
  SDL_RenderFillRect( sdl_renderer.get(), &borderRight);

  SDL_Rect borderBottom = {0, (int)screen_height - (int)border_width, (int)screen_width, (int)border_width}; 
  SDL_SetRenderDrawColor( sdl_renderer.get(), 0xB4, 0xED, 0xE0, 0xFF);
  SDL_RenderFillRect( sdl_renderer.get(), &borderBottom);

  if (!_started) 
  {
    _message_texture = new WTexture(sdl_renderer.get());
     if (!_message_texture->loadFromFile( "../images/start.png" ))
    {
      std::cerr << "Failed to load start message texture! \n";
    }
    else 
    {
      SDL_Rect sq = {0, (int)screen_height - (int)border_width, (int)screen_width, (int)border_width};
      sq = {0, 0, (int)screen_width, (int)border_width};

      _message_texture->render(0, (int)screen_height - (int)border_width, &sq);
    }
  }

  // Render coin
  SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xCC, 0x00, 0xFF);
  block.x = coin.x * block.w;
  block.y = coin.y * block.h;
  SDL_RenderFillRect(sdl_renderer.get(), &block);

  // Render ghost if level >= 1
  if (ghost.show) 
  {
    block.x = static_cast<int>(ghost.head_x) * block.w;
    block.y = static_cast<int>(ghost.head_y) * block.h;

    SDL_Rect * ghostRect = &ghostMoves[ (int)ghost.direction ];
    ghost_texture->render( block.x, block.y, ghostRect );
    ++ghost_frame;
    if (ghost_frame % 4 == 0) 
    {
      ghost.Update();
    }
    if (ghost_frame >= 48)
    {
      ghost_frame = 0;
      ghost.RandDirection();
    } 
  }

  // Render pacman
  block.x = static_cast<int>(pacman.head_x) * block.w;
  block.y = static_cast<int>(pacman.head_y) * block.h;

  SDL_Rect * curr = &pacmanMoves[ (int)pacman.direction * 2 + frame / 8 ];
  sdl_texture->render( block.x, block.y, curr );
  ++frame;
  if (frame / 8 >= PACMAN_ANIMATION_FRAMES)
  {
    frame = 0;
  }

  if (!pacman.alive)
  {
    _message_texture = new WTexture(sdl_renderer.get());
     if (!_message_texture->loadFromFile( "../images/gameover.png" ))
    {
      std::cerr << "Failed to load start message texture! \n";
    }
    else 
    {
      SDL_Rect sq = {0, (int)screen_height - (int)border_width, (int)screen_width, (int)border_width};
      sq = {0, 0, (int)screen_width, (int)screen_height};

      _message_texture->render(0, 0, &sq);
    }
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer.get());
}

void Renderer::UpdateWindowTitle(int score, int fps, int level) {
  std::string title{"Pacman Level: " + std::to_string(level) + "; Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window.get(), title.c_str());
}

void Renderer::start() 
{
  _started = true; 
}

void Renderer::loadMedia(const std::size_t size)
{
  /*
    C1: The project reads data from an external file
  */
  int pSize = (int)size;
  if (!sdl_texture->loadFromFile( "../images/pacman.png" ))
  {
    std::cerr << "Failed to load pacman texture! \n";
  }
  else 
  {
    pacmanMoves[6].x = 0;
    pacmanMoves[6].y = 0;
    pacmanMoves[6].w = pSize;
    pacmanMoves[6].h = pSize;

    pacmanMoves[7].x = pSize;
    pacmanMoves[7].y = 0;
    pacmanMoves[7].w = pSize;
    pacmanMoves[7].h = pSize;

    pacmanMoves[2].x = 0;
    pacmanMoves[2].y = pSize;
    pacmanMoves[2].w = pSize;
    pacmanMoves[2].h = pSize;

    pacmanMoves[3].x = pSize;
    pacmanMoves[3].y = pSize;
    pacmanMoves[3].w = pSize;
    pacmanMoves[3].h = pSize;

    pacmanMoves[4].x = 0;
    pacmanMoves[4].y = pSize * 2;
    pacmanMoves[4].w = pSize;
    pacmanMoves[4].h = pSize;

    pacmanMoves[5].x = pSize;
    pacmanMoves[5].y = pSize * 2;
    pacmanMoves[5].w = pSize;
    pacmanMoves[5].h = pSize;

    pacmanMoves[0].x = 0;
    pacmanMoves[0].y = pSize * 3;
    pacmanMoves[0].w = pSize;
    pacmanMoves[0].h = pSize;

    pacmanMoves[1].x = pSize;
    pacmanMoves[1].y = pSize * 3;
    pacmanMoves[1].w = pSize;
    pacmanMoves[1].h = pSize;
  }

  if (!ghost_texture->loadFromFile( "../images/ghost.png" ))
  {
    std::cerr << "Failed to load ghost texture! \n";
  }
  else 
  {
    ghostMoves[2].x = 0;
    ghostMoves[2].y = 0;
    ghostMoves[2].w = pSize;
    ghostMoves[2].h = pSize;

    ghostMoves[0].y = pSize;
    ghostMoves[0].x = 0;
    ghostMoves[0].w = pSize;
    ghostMoves[0].h = pSize;

    ghostMoves[1].y = pSize * 2;
    ghostMoves[1].x = 0;
    ghostMoves[1].w = pSize;
    ghostMoves[1].h = pSize;

    ghostMoves[3].y = pSize * 3;
    ghostMoves[3].x = 0;
    ghostMoves[3].w = pSize;
    ghostMoves[3].h = pSize;
  }

}

WTexture::WTexture(SDL_Renderer *renderer)
{
  _renderer = renderer;
  _width = 0;
  _height = 0;
}

WTexture::~WTexture()
{
  std::cout << "DELETING WTexture \n";
  free();
}

bool WTexture::loadFromFile(std::string path)
{
  free();

  std::unique_ptr<SDL_Texture, delete_sdl> texture; // = NULL;

  SDL_Surface* surface = IMG_Load( path.c_str() );

  if (surface == NULL) 
  {
    std::cerr << "Could not load image " << path.c_str() << "! : " << IMG_GetError() << "\n";
  }
  else
  {
    SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0, 0xFF, 0xFF ));

    texture = std::unique_ptr<SDL_Texture, delete_sdl>(SDL_CreateTextureFromSurface( _renderer, surface ));
    if (texture == NULL)
    {
      std::cerr << "Cannot create texture from " << path.c_str() << "! : " << SDL_GetError() << "\n";
    }
    else 
    {
      SDL_FreeSurface( surface );
    }
  }
  _texture = std::move(texture);
  return (_texture != NULL); 
}

void WTexture::free()
{
  if (_texture != NULL)
  {
    //SDL_DestroyTexture( _texture.get() );
    //_texture = NULL;
    _width = 0;
    _height = 0;
  }
}

void WTexture::render( int x, int y, SDL_Rect* clip )
{
  SDL_Rect renderQuad = {x, y, _width, _height};

  if (clip != NULL)
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  SDL_RenderCopy( _renderer, _texture.get(), clip, &renderQuad );
}

int WTexture::getWidth()
{
  return _width;
}

int WTexture::getHeight()
{
  return _height;
}