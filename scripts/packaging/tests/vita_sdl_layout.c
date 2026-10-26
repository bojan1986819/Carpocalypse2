/* Compile twice for ARM (SDK short enums and game wide enums), compare data.
 * These are the SDL structs/fields the platform exchanges with SDL, including
 * events and audio specs; the test does not execute target code. */
#include <SDL3/SDL.h>
#include <stddef.h>
#define SIZE(t) sizeof(t)
#define FIELD(t,f) offsetof(t,f)
const unsigned int c2_sdl_layout[] = {
 SIZE(SDL_PathInfo), FIELD(SDL_PathInfo,type), FIELD(SDL_PathInfo,size), FIELD(SDL_PathInfo,create_time), FIELD(SDL_PathInfo,modify_time), FIELD(SDL_PathInfo,access_time),
 SIZE(SDL_AudioSpec), FIELD(SDL_AudioSpec,format), FIELD(SDL_AudioSpec,channels), FIELD(SDL_AudioSpec,freq),
 SIZE(SDL_Surface), FIELD(SDL_Surface,flags), FIELD(SDL_Surface,format), FIELD(SDL_Surface,w), FIELD(SDL_Surface,h), FIELD(SDL_Surface,pitch), FIELD(SDL_Surface,pixels), FIELD(SDL_Surface,refcount),
 SIZE(SDL_DisplayMode), FIELD(SDL_DisplayMode,displayID), FIELD(SDL_DisplayMode,format), FIELD(SDL_DisplayMode,w), FIELD(SDL_DisplayMode,h), FIELD(SDL_DisplayMode,pixel_density), FIELD(SDL_DisplayMode,refresh_rate), FIELD(SDL_DisplayMode,internal),
 SIZE(SDL_Event), FIELD(SDL_Event,type), SIZE(SDL_MouseMotionEvent), FIELD(SDL_MouseMotionEvent,x), FIELD(SDL_MouseMotionEvent,y),
 SIZE(SDL_MouseButtonEvent), FIELD(SDL_MouseButtonEvent,timestamp), FIELD(SDL_MouseButtonEvent,button), FIELD(SDL_MouseButtonEvent,x), FIELD(SDL_MouseButtonEvent,y),
 SIZE(SDL_KeyboardEvent), FIELD(SDL_KeyboardEvent,key), SIZE(SDL_GamepadDeviceEvent), FIELD(SDL_GamepadDeviceEvent,which),
 SIZE(SDL_GamepadButtonEvent), FIELD(SDL_GamepadButtonEvent,which), FIELD(SDL_GamepadButtonEvent,button),
 SIZE(SDL_GamepadAxisEvent), FIELD(SDL_GamepadAxisEvent,which), FIELD(SDL_GamepadAxisEvent,axis), FIELD(SDL_GamepadAxisEvent,value),
 SIZE(SDL_Color), FIELD(SDL_Color,r), FIELD(SDL_Color,g), FIELD(SDL_Color,b), FIELD(SDL_Color,a),
 SIZE(SDL_PixelFormatDetails), FIELD(SDL_PixelFormatDetails,format), FIELD(SDL_PixelFormatDetails,bits_per_pixel), FIELD(SDL_PixelFormatDetails,bytes_per_pixel), FIELD(SDL_PixelFormatDetails,Rmask)
};
