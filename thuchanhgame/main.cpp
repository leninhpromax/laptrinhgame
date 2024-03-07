#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 1600;
const int SCREEN_HIGHT = 800;
const int SCREEN_BPP = 32;

SDL_Surface *g_screen = NULL;
SDL_Surface *g_bkground = NULL;
SDL_Event g_even;

bool Init()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    return false;
  }
  g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
  if (g_screen == NULL)
  {
    return false;
  }
  return true;
}

SDL_Surface* LoadImage(std::string file_path)
{
  SDL_Surface * load_image = NULL;
  SDL_Surface* optimize_image = NULL;
  load_image = IMG_Load(file_path.c_str());
  if (load_image != NULL)
  {
    optimize_image= SDL_DisplayFormat(load_image);
    SDL_FreeSurface(load_image);
  }
  return optimize_image;
}

void ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y)
{
  SDL_Rect offset;
  offset.x = x;
  offset.y = y;
  SDL_BlitSurface(src, NULL, des, &offset);
}

void CleanUp()
{
  SDL_FreeSurface(g_screen);
  SDL_FreeSurface(g_bkground);
}

int main(int arc, char* argv[])
{
  bool is_quit = false;
  if (Init() == false)
   return 0;

  g_bkground = LoadImage("bkground.png");
  if (g_bkground == NULL)
  {
    return 0;
  }

  ApplySurface(g_bkground, g_screen, 0, 0);

  while (!is_quit)
  {
    while (SDL_PollEvent(&g_even))
    {
      if (g_even.type == SDL_QUIT)
      {
        is_quit = true;
        break;
      }
    }
    if ( SDL_Flip(g_screen) == -1)
    return 0;
  }
  CleanUp();
  SDL_Quit();
  return 1;
}
