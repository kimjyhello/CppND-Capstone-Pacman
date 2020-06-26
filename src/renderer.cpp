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
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      border_width(32) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = std::unique_ptr<SDL_Window, delete_sdl>(SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN));
  //sdl_window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>(SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED,
  //                              SDL_WINDOWPOS_CENTERED, screen_width,
  //                              screen_height, SDL_WINDOW_SHOWN), SDL_DESTROY_WINDOW);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  //sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  //SDL_Renderer *renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  //sdl_renderer = std::make_shared<SDL_Renderer>(SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED), delete_sdl);
  sdl_renderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(sdl_window.get(), -1, SDL_RENDERER_ACCELERATED), 
    [](SDL_Renderer* r) { SDL_DestroyRenderer(r); } );
  //sdl_renderer.reset(SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED));
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
}


Renderer::~Renderer() {
  //SDL_DestroyWindow(sdl_window);
  IMG_Quit();
  SDL_Quit();
}

void Renderer::Render(Pacman const pacman, SDL_Point const &food) {

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

  //SDL_RenderPresent(sdl_renderer);
  if (!_started) 
  {
    _message_texture = new WTexture(sdl_renderer.get());
    //std::unique_ptr<SDL_Texture, delete_sdl>(SDL_CreateTexture(sdl_renderer.get()));
    if (!_message_texture->loadFromFile( "../images/start.png" ))
    {
      std::cerr << "Failed to load start message texture! \n";
    }
    else 
    {
      SDL_Rect sq = {0, (int)screen_height - (int)border_width, (int)screen_width, (int)border_width};
      sq = {0, 0, (int)screen_width, (int)border_width};

      _message_texture->render(0, (int)screen_height - (int)border_width, &sq);
      //SDL_RenderCopy( sdl_renderer.get(), _message_texture, NULL, &renderQuad );
    }
  }

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer.get(), &block);

  // Render snake's body
  /*SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : pacman.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }*/

  // Render snake's head
  block.x = static_cast<int>(pacman.head_x) * block.w;
  block.y = static_cast<int>(pacman.head_y) * block.h;
  /*if (pacman.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);*/

  SDL_Rect * curr = &pacmanMoves[ (int)pacman.direction * 2 + frame / 8 ];
  sdl_texture->render( block.x, block.y, curr );
  ++frame;
  if (frame / 8 >= PACMAN_ANIMATION_FRAMES)
  {
    frame = 0;
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer.get());
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Pacman Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window.get(), title.c_str());
}

void Renderer::start() 
{
  _started = true; 
}

void Renderer::loadMedia()
{
  /*
    C-1: The project reads data from an external file
  */
  if (!sdl_texture->loadFromFile( "../images/pacman.png" ))
  {
    std::cerr << "Failed to load pacman texture! \n";
  }
  else 
  {
    pacmanMoves[6].x = 0;
    pacmanMoves[6].y = 0;
    pacmanMoves[6].w = 64;
    pacmanMoves[6].h = 64;

    pacmanMoves[7].x = 64;
    pacmanMoves[7].y = 0;
    pacmanMoves[7].w = 64;
    pacmanMoves[7].h = 64;

    pacmanMoves[2].x = 0;
    pacmanMoves[2].y = 64;
    pacmanMoves[2].w = 64;
    pacmanMoves[2].h = 64;

    pacmanMoves[3].x = 64;
    pacmanMoves[3].y = 64;
    pacmanMoves[3].w = 64;
    pacmanMoves[3].h = 64;

    pacmanMoves[4].x = 0;
    pacmanMoves[4].y = 128;
    pacmanMoves[4].w = 64;
    pacmanMoves[4].h = 64;

    pacmanMoves[5].x = 64;
    pacmanMoves[5].y = 128;
    pacmanMoves[5].w = 64;
    pacmanMoves[5].h = 64;

    pacmanMoves[0].x = 0;
    pacmanMoves[0].y = 192;
    pacmanMoves[0].w = 64;
    pacmanMoves[0].h = 64;

    pacmanMoves[1].x = 64;
    pacmanMoves[1].y = 192;
    pacmanMoves[1].w = 64;
    pacmanMoves[1].h = 64;
  }

}

WTexture::WTexture(SDL_Renderer *renderer)
{
  _renderer = renderer;
  //_texture = NULL;
  _width = 0;
  _height = 0;
}

WTexture::~WTexture()
{
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
    //return false; 
  }
  else
  {
    SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0, 0xFF, 0xFF ));

    texture = std::unique_ptr<SDL_Texture, delete_sdl>(SDL_CreateTextureFromSurface( _renderer, surface ));
    if (texture == NULL)
    {
      std::cerr << "Cannot create texture from " << path.c_str() << "! : " << SDL_GetError() << "\n";
      //return false;
    }
    else 
    {
      SDL_FreeSurface( surface );
    }
    //_texture = texture;
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