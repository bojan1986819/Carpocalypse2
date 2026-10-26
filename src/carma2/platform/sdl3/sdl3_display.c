/* SDL-only display settings; retail screen/data layouts stay unchanged. */
#include "sdl3_display.h"
#include "sdl3.h"
#include "interface.h"
#include "input.h"
#include <brgl_display.h>

extern int carpocalypse2_UseGL(void);
extern int C2_HOOK_FASTCALL Generic_MenuHandler(tFrontend_spec*);
extern int gFrontend_scrollbars_updated;
extern tFrontend_slider* gCurrent_frontend_scrollbars;
extern tConnected_items* gConnected_items;
extern tStruct_00686508* gPTR_00686508;

#define DISPLAY_CONFIG "C2DISPLAY.CFG"
static int saved_width = 640, saved_height = 480, saved_fullscreen;
static int pending_width, pending_height, pending_fullscreen;
static int mode_width[24], mode_height[24], mode_count;
static tFrontend_spec display_menu;
static int initialized;

static const SDL_DisplayMode* desktop_mode(void) {
    SDL_DisplayID id = g_SDL_Window != NULL ? SDL_GetDisplayForWindow(g_SDL_Window) : SDL_GetPrimaryDisplay();
    return SDL_GetDesktopDisplayMode(id);
}
void carpocalypse2_DisplayRefresh(void) {
    int width=640,height=480,x,y,w,h;
    if (g_SDL_Window == NULL) return;
    SDL_GetWindowSizeInPixels(g_SDL_Window,&width,&height);
    C2GLFitOutput(width,height,&x,&y,&w,&h);
    gGLPresentationAspect = (float)w / h;
}
void carpocalypse2_DisplayMouse(float x,float y,float* gx,float* gy) {
    int width=640,height=480;
    if (g_SDL_Window != NULL) SDL_GetWindowSize(g_SDL_Window,&width,&height);
    C2GLMapMouse(width,height,x,y,gx,gy);
}
float PDWidescreenFactor(void) {
    carpocalypse2_DisplayRefresh();
    return carpocalypse2_UseGL() ? gGLPresentationAspect / (4.f/3.f) : 1.f;
}
int PDSetWideBackdrop(int active) {
    int previous = gGLWideBackdrop;
    gGLWideBackdrop = active;
    return previous;
}
int PDSetWorldRendering(int active) {
    int previous = gGLWorldRendering;
    gGLWorldRendering = active;
    if (active) gGLFrameHasWorld = 1;
    return previous;
}
static int valid_size(int w,int h) {
    const SDL_DisplayMode* dm=desktop_mode();
    return w>=640 && h>=480 && w<=7680 && h<=4320
        && (dm==NULL || (w<=dm->w && h<=dm->h));
}
static int set_window(int w,int h,int fullscreen) {
    if (!SDL_SetWindowFullscreen(g_SDL_Window,false)) return 0;
    if (fullscreen) {
        if (!SDL_SetWindowFullscreenMode(g_SDL_Window,NULL) || !SDL_SetWindowFullscreen(g_SDL_Window,true)) return 0;
    } else {
        if (!SDL_SetWindowSize(g_SDL_Window,w,h)) return 0;
        SDL_SetWindowPosition(g_SDL_Window,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
    }
    SDL_SyncWindow(g_SDL_Window);
    carpocalypse2_DisplayRefresh();
    return 1;
}
void carpocalypse2_DisplayInit(void) {
#ifdef __vita__
    carpocalypse2_DisplayRefresh();
    return;
#endif
    size_t size=0;
    char* text;
    int version,w,h,fullscreen;
    SDL_SetWindowMinimumSize(g_SDL_Window,640,480);
    text=SDL_LoadFile(DISPLAY_CONFIG,&size);
    if (text!=NULL) {
        if (SDL_sscanf(text,"C2DISPLAY %d %d %d %d",&version,&w,&h,&fullscreen)==4
                && version==1 && valid_size(w,h) && (fullscreen==0 || fullscreen==1)) {
            if (set_window(w,h,fullscreen)) { saved_width=w; saved_height=h; saved_fullscreen=fullscreen; }
        }
        SDL_free(text);
    }
    carpocalypse2_DisplayRefresh();
}
static void add_mode(int w,int h) {
    int i;
    if (!valid_size(w,h) || mode_count>=24) return;
    for (i=0;i<mode_count;i++) if (mode_width[i]==w && mode_height[i]==h) return;
    mode_width[mode_count]=w; mode_height[mode_count++]=h;
}
static void enumerate_modes(void) {
    static const int modes[][2]={{640,480},{800,600},{1024,768},{1280,720},{1280,800},
        {1366,768},{1600,900},{1920,1080},{1920,1200},{2560,1080},{2560,1440},{3440,1440},{3840,2160}};
    const SDL_DisplayMode* dm=desktop_mode();
    int i;
    mode_count=0;
    for (i=0;i<(int)SDL_arraysize(modes);i++) add_mode(modes[i][0],modes[i][1]);
    if (dm!=NULL) add_mode(dm->w,dm->h);
    add_mode(pending_width,pending_height);
}
static void update_labels(void) {
    const SDL_DisplayMode* dm=desktop_mode();
    int w=pending_fullscreen && dm!=NULL ? dm->w : pending_width;
    int h=pending_fullscreen && dm!=NULL ? dm->h : pending_height;
    SDL_snprintf(display_menu.items[1].text,255,"RESOLUTION: %d X %d",w,h);
    display_menu.items[1].enabled=pending_fullscreen ? 0 : 1;
    SDL_snprintf(display_menu.items[2].text,255,"WINDOW: %s",pending_fullscreen ? "FULLSCREEN" : "WINDOWED");
    SDL_strlcpy(display_menu.items[5].text,pending_fullscreen
        ? "DESKTOP RESOLUTION. APPLY TO SAVE."
        : "CHOOSE A SIZE. APPLY TO SAVE.",255);
}
static int C2_HOOK_FASTCALL cycle_resolution(tFrontend_spec* menu) {
    int i;
    (void)menu;
    for (i=0;i<mode_count;i++) if (mode_width[i]==pending_width && mode_height[i]==pending_height) break;
    i=(i+1)%mode_count;
    pending_width=mode_width[i]; pending_height=mode_height[i];
    update_labels();
    return 4;
}
static int C2_HOOK_FASTCALL cycle_window(tFrontend_spec* menu) {
    (void)menu; pending_fullscreen=!pending_fullscreen; update_labels(); return 4;
}
static int C2_HOOK_FASTCALL apply_display(tFrontend_spec* menu) {
    char text[100];
    int length;
    (void)menu;
    if (!set_window(pending_width,pending_height,pending_fullscreen)) {
        set_window(saved_width,saved_height,saved_fullscreen);
        SDL_strlcpy(display_menu.items[5].text,"CHANGE FAILED. SETTINGS RESTORED.",255);
        return 4;
    }
    saved_width=pending_width; saved_height=pending_height; saved_fullscreen=pending_fullscreen;
    length=SDL_snprintf(text,sizeof(text),"C2DISPLAY 1 %d %d %d\n",saved_width,saved_height,saved_fullscreen);
    SDL_strlcpy(display_menu.items[5].text,SDL_SaveFile(DISPLAY_CONFIG,text,length)
        ? "DISPLAY SETTINGS SAVED." : "DISPLAY APPLIED. SAVE FAILED.",255);
    return 4;
}
static void make_item(int index,const char* text,int x,int y,int width,int height,int font,int highlight,int enabled) {
    tFrontend_item_spec* item=&display_menu.items[index];
    SDL_memset(item,0,sizeof(*item));
    item->stringId=0x402; item->action=temp;
    item->x=x; item->y=y; item->width=width; item->height=height;
    item->unlitFont=font; item->highFont=highlight;
    item->enabled=enabled; item->visible=1;
    SDL_strlcpy(item->text,text,sizeof(item->text));
}
static int C2_HOOK_FASTCALL display_in(tFrontend_spec* menu) {
    (void)menu;
    pending_width=saved_width; pending_height=saved_height; pending_fullscreen=saved_fullscreen;
    enumerate_modes(); update_labels();
    display_menu.default_item = pending_fullscreen ? 2 : 1;
    gFrontend_scrollbars_updated=0; gCurrent_frontend_scrollbars=NULL; gConnected_items=NULL; gPTR_00686508=NULL;
    EdgeTriggerModeOff(); WaitForNoKeys(); EdgeTriggerModeOn();
    return 1;
}
void PDAddDisplayOptions(tFrontend_spec* options) {
#ifdef __vita__
    /* Vita has a fixed panel; the desktop window-size submenu does not apply. */
    return;
#endif

    tFrontend_item_spec* button=&options->items[18];
    if (!initialized) {
        initialized=1;
        SDL_strlcpy(display_menu.name,"Display",sizeof(display_menu.name));
        display_menu.count_items=7; display_menu.create=display_in; display_menu.tick=Generic_MenuHandler;
        display_menu.previous=options; display_menu.default_item=1; display_menu.loaded=1;
        display_menu.model_A_index=options->model_A_index;
        SDL_strlcpy(display_menu.backdrop_name,options->backdrop_name,sizeof(display_menu.backdrop_name));
        /* opt.tif has a narrow header, then a wider dark body below y=146. */
        make_item(0,"DISPLAY",100,66,230,40,5,5,0);
        make_item(1,"",145,164,400,28,kPolyfont_hand_green_15pt_unlit,kPolyfont_hand_green_15pt_lit,1); display_menu.items[1].action=cycle_resolution;
        make_item(2,"",145,204,400,28,kPolyfont_hand_green_15pt_unlit,kPolyfont_hand_green_15pt_lit,1); display_menu.items[2].action=cycle_window;
        make_item(3,"APPLY",145,248,180,32,6,7,1); display_menu.items[3].action=apply_display;
        make_item(4,"BACK",365,248,180,32,6,7,1); display_menu.items[4].menuInfo=(tFrontend_spec*)1;
        /* Keep both help rows to one line within the panel bottom at y=350. */
        make_item(5,"",145,290,400,16,0,0,0); display_menu.items[5].wrapText=1;
        make_item(6,carpocalypse2_UseGL() ? "WIDESCREEN VIEW - 4:3 MENUS AND HUD" : "4:3 VIEW - OPENGL FOR WIDESCREEN",145,316,400,16,0,0,0);
    }
    SDL_memset(button,0,sizeof(*button));
    button->stringId=0x402; button->action=temp; button->menuInfo=&display_menu;
    /* Share the SETTINGS row within the wide body of the existing panel. */
    button->x=360; button->y=199; button->width=180; button->height=32;
    button->unlitFont=6; button->highFont=7; button->enabled=1; button->visible=1;
    SDL_strlcpy(button->text,"DISPLAY",sizeof(button->text));
    options->count_items=19;
}
