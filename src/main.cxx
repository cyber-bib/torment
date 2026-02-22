#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)

#include <cmath>
#include <iostream>
#include "array/dense/core.txx"

#include <SDL2/SDL.h>


template<class T, std::size_t S = 0>
using vec = torment::dense::base<T, 1, std::size_t, torment::dense::urr(S)>;

template<class T, std::size_t R = 0, std::size_t C = 0>
using matrix = torment::dense::base<T, 2, std::size_t, torment::dense::urr(R, C)>;

template<class T, std::size_t R = 0, std::size_t C = 0, std::size_t S = 0>
using tensor3 = torment::dense::base<T, 3, std::size_t, torment::dense::urr(R, C, S)>;

typedef vec<double,3> vec3;


double sigmoid(double t) {
  return 1.0/(1.0 + std::exp(-t));
}
vec3 palette( double t,
              vec3 a,
              vec3 b,
              vec3 c,
              vec3 d  )
{
  vec3 dst;

  for(int i = 0; i < dst.size(); i++)
    dst[i]  = a[i] + b[i]*std::cos(2.0*M_PI*(c[i]*t+d[i]));

  return dst;
}
vec3 rainbow_palette(double t) {
  auto dst = palette( (2*t/3 - 0.25),
                      {0.5, 0.5, 0.5},
                      {0.5, 0.5, 0.5},
                      {1.0, 1.0, 1.0},
                      {0.0, 0.333, 0.667});

  auto s = 6.0, d = 0.75;

  auto Sl = sigmoid(s*t);
  auto Bu = sigmoid(s*(t + d))*sigmoid(-s*(t - d));
  auto Bd = 1.0 - Bu;
  auto b = Sl*Bd;

  for(auto &v : dst) {
    v = (1.0 - 1.0e-10)*(v*Bu + b);
  }

  return dst;
}

void pixel_test_palette (
  unsigned char*pixels,
  std::size_t width,
  std::size_t height
) {
  int pitch = 4*width;

  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      auto color = rainbow_palette((2.0*i)/width -1.0);

      pixels[i*4 + j*pitch + 0] = 256.0f*color[0];
      pixels[i*4 + j*pitch + 1] = 256.0f*color[1];
      pixels[i*4 + j*pitch + 2] = 256.0f*color[2];
      pixels[i*4 + j*pitch + 3] = 255;
    }
  }
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  constexpr std::size_t WIDTH = 16,
                        HEIGHT = 16,
                        CHANNELS = 4;

  int width = WIDTH,
      height = HEIGHT,
      pitch = 4*width;

  bool quit = false;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Texture* texture = nullptr;

  unsigned char *pixels;
  tensor3<short, WIDTH, HEIGHT, CHANNELS> A;

# pragma region SDL2 Start Up [

  if(auto errCode = SDL_Init(SDL_INIT_EVERYTHING); errCode != 0) {
    // std::cerr << "SDL Initialization failed. (Error code: "
    //           << SDL_GetError() << ")\n";
    std::cerr << SDL_GetError() << "\n";
    goto close_program;
  };



  if(!(window = SDL_CreateWindow( "Torment-Or Tor-Mentor",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  width,
                                  height,
                                  SDL_WindowFlags::SDL_WINDOW_SHOWN  )))
  {
    std::cerr << SDL_GetError() << "\n";
    goto close_program;
  }

  if(!(renderer = SDL_CreateRenderer( window,
                                      -1,
                                      SDL_RendererFlags::SDL_RENDERER_ACCELERATED  )))
  {
    std::cerr << SDL_GetError() << "\n";
    goto close_program;
  }

  SDL_GetWindowSize(window, &width, &height);
  if(!(texture = SDL_CreateTexture( renderer,
                                    SDL_PixelFormatEnum::SDL_PIXELFORMAT_ABGR8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    width,
                                    height  )))
  {
    std::cerr << SDL_GetError() << "\n";
    goto close_program;
  }

# pragma endregion ] SDL2 Start Up

  if(auto errCode = SDL_LockTexture(texture,
                                    nullptr,
                                    reinterpret_cast<void**>(&pixels),
                                    &pitch); errCode != 0)
  {
    std::cerr << SDL_GetError() << "\n";
    goto close_program;
  }

  pixel_test_palette(pixels, width, height);

  std::copy_n(pixels, A.size(), A.begin());


  // std::cout << A << "\n";

  SDL_UnlockTexture(texture);

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);

  while(!quit) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_EventType::SDL_QUIT:
          quit = true;
          break;
      }
    };
  }

# pragma region SDL2 Shut Down [
  close_program:

  if(texture) {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
  if(renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }
  if(window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
  SDL_Quit();

# pragma endregion ] SDL2 Shut Down

  /* // some old code, not sure what it does
  auto A = M([&M](std::size_t i, std::size_t j) {
    tormentor::matrix::key_type key;
    key[0] = i < 3 ? i : M.shape()[0];
    key[1] = j < 3 ? j : M.shape()[1];
    return key;
  });

  M.row(1) -= (M(1, 0)/M(0,0))*M.row(0);
  M.row(2) -= (M(2, 0)/M(0,0))*M.row(0);
  M.row(2) -= (M(2, 1)/M(1,1))*M.row(1);

  std::cout << A << "\n\n";
  std::cout << M.row(10) << "\n\n";
  std::cout << "shape(A) = {" << M.rows() << ", " << M.cols() << "}\n";
  std::cout << M.row(0) << "\n\n";
  std::cout << M.col(0) << "\n\n";

  typedef tormentor::vector vec;
  vec v(6, 1);

  for(std::size_t i = 0; i < v.size(); i++) {
    v(i) = i;
  }

  auto u = v([](vec::key_type i) {
    return 2*i;
  });

  std::cout << "u size: " << u.size() << "\n"
            << "u = " << u << "\n"
            << "u*u = " << u*u << "\n";
  std::cout << "End of program (press enter to continue...)\n";
  std::cin.get();
  */

  return 0;
}

