#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL.h>
#include <stdbool.h>

#define WINDOW_WIDTH 640UL
#define WINDOW_HEIGHT 480UL


enum { NS_PER_SECOND = 1000000000, MS_PER_SECOND = 1000, NS_PER_MS = 1000000 };
long get_time_millis() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec * MS_PER_SECOND + t.tv_nsec / NS_PER_MS;
}

char* get_keysym_mod(Uint16 mod) {
   switch(mod) {
       case KMOD_NONE:
           return "none";
       case KMOD_LSHIFT:
           return "left Shift";
       case KMOD_RSHIFT:
            return "right Shift";
       case KMOD_LCTRL:
            return "left Ctrl";
       case KMOD_RCTRL:
            return "right Ctrl";
       case KMOD_LALT:
            return "left Alt";
       case KMOD_RALT:
            return "right Alt";
       case KMOD_LGUI:
            return "LGUI key";
       case KMOD_RGUI:
            return "RGUI key";
       case KMOD_NUM:
            return "Num Lock";
       case KMOD_CAPS:
            return "Caps Lock";
       case KMOD_MODE:
            return "!AltGr"; 
       case KMOD_SCROLL:
            return "Scroll Lock";
       case KMOD_CTRL:
            return "(KMOD_LCTRL|KMOD_RCTRL)";
       case KMOD_SHIFT:
            return "(KMOD_LSHIFT|KMOD_RSHIFT)";
       case KMOD_ALT:
            return "(KMOD_LALT|KMOD_RALT)";
       case KMOD_GUI:
            return "(KMOD_LGUI|KMOD_RGUI)";
       default:
            return "UNknowWn";
   }
}

void print_event_type(SDL_Event* e) {
    Uint32 type = e->type;
    switch(type) {

        case SDL_KEYDOWN: case SDL_KEYUP:
             printf("KEY%s\t\t", type == SDL_KEYDOWN ? "DOWN" : "UP");
             SDL_KeyboardEvent* ke = (SDL_KeyboardEvent*) e;
             printf("[type: %d, timestamp: %d, state: %d, repeat: %d, ",
                    ke->type, ke->timestamp, ke->state, ke->repeat);

             SDL_Keysym ks = ke->keysym;
             printf("keysym  (scancode: %d, sym: %d, mod: %s)]\n",
                     ks.scancode, ks.sym, get_keysym_mod(ks.mod));

             break;
        case SDL_TEXTEDITING:
             printf("TEXTEDITING\t");
             SDL_TextEditingEvent* tee = (SDL_TextEditingEvent*) e;
             printf("[timestamp: %d, windowID: %d, text '%*s', start: %d, length: %d]\n",
                    tee->timestamp,
                    tee->windowID,
                    SDL_TEXTEDITINGEVENT_TEXT_SIZE,
                    tee->text,
                    tee->start,
                    tee->length);
             break;
        case SDL_TEXTINPUT:
            SDL_TextInputEvent* tie = (SDL_TextInputEvent*) e;
            printf("TEXTINPUT\t");
            printf("[winId: %d text; '%*s']\n",
                    tie->windowID, SDL_TEXTINPUTEVENT_TEXT_SIZE, tie->text);
            break;
        case SDL_KEYMAPCHANGED:
            printf("KEYMAPCHANGED\t");
            printf("\n");
            break;
    }
}


int process_input(bool* quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        print_event_type(&e);
        if (e.type == SDL_QUIT) {
            *quit = true;
            return 0;
        }

    }

    return 0;
}



int sdl_init(SDL_Window* window, SDL_Renderer* renderer) {
    
    if (SDL_Init (SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Couldn't initialize SDL\n");
        return -1;
    }

    atexit (SDL_Quit);

    window = SDL_CreateWindow(
        "teclas",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        return -1;
    }
    
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

int main(void)
{
    SDL_Window* w = NULL;
    SDL_Renderer* r = NULL;
    sdl_init(w, r);

    long previous = get_time_millis();
    long lag = 0;
    bool quit = false;

    while (!quit) {
        long current = get_time_millis();
        long elapsed = current - previous;
        previous = current;
        lag += elapsed;

        process_input(&quit);
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w); 
}
