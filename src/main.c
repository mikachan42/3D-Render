#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

const float screenWidth = 800.0f;
const float screenHeight = 800.0f;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const float width = 20.0f;
const float height = 20.0f;
const float rightPositionOnWidth = width / 2;
const float rightPositionOnHeight = height / 2;

const float FPS = 60;
float dz = 0;
float angle = 0;
const float pi = 3.14f;


typedef struct {
    float x;
    float y;
} Point2D;

typedef struct {
    float x;
    float y;
    float z;
} Point3D;


void init() {
     if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Fehler: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_CreateWindowAndRenderer(screenWidth, screenHeight, SDL_WINDOW_BORDERLESS, &window, &renderer);
}


void clear_screen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}


void point_rect(SDL_Renderer* renderer, float x, float y) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect rect1 = {x - rightPositionOnWidth, y - rightPositionOnHeight, width, height};
    SDL_RenderFillRect(renderer, &rect1);
    
}

void line(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, float x3, float y3) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
}

void terminate() {
    printf("debugggggging");
    // Wait for windows to close, without blocking Wayland
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_KEYDOWN) running = 0; // exit button
        }
        SDL_Delay(16); // ~60fps, CPU Conservation
    }

    SDL_DestroyRenderer(renderer); // frees up renderer and its memory
    SDL_DestroyWindow(window); // closes windows and releases it
    SDL_Quit(); // Shuts down SDL, frees up all internal resources
}






Point2D project(Point3D p) {
    return (Point2D){
        .x = p.x / p.z,
        .y = p.y / p.z
    };
}

Point2D screen(Point2D p) {
    Point2D result;

    result.x = (p.x + 1.0f) / 2.0f * screenWidth;
    result.y = (1 - (p.y + 1.0f) / 2.0f) * screenHeight;

    return result;
}

Point3D Points[6] = {
        {10.0f, -11.0f, -10.0f},
        {10.0f, 9.0f, -10.0f},
        {0.0f, -1.0f, 10.0f},
        {-10.0f, 9.0f, -10.0f},
        {-10.0f, -11.0f, -10.0f},

        {0.0f, -1.0f, -10.0f},
    };



int Faces[8][3] = {
    {0, 1, 2},
    {1, 3, 2},
    {3, 4, 2},
    {4, 0, 2},
    {1, 0, 5},
    {3, 1, 5},
    {4, 3, 5},
    {0, 4, 5}
};
 


void translate_z(Point3D pyramid[8], float current_dz) {
    for(int i = 0; i < 8; i++) {
      //  pyramid[i] = Points[i];
        pyramid[i].z += 30.0f;
    }
}


void rotate_xz(Point3D pyramid[8], float current_angle) {
    const float c = cosf(-current_angle);
    const float s = sinf(-current_angle);

    for(int i = 0; i < 8; i++) {
        pyramid[i] = Points[i];
        float original_x = pyramid[i].x;
        float original_y = pyramid[i].y;
        float original_z = pyramid[i].z;

        // x-rotation
        pyramid[i].y = original_y * c - pyramid[i].z * s;
        pyramid[i].z = original_y * s + pyramid[i].z * c;

        // y-rotation
        pyramid[i].x = original_x * c - pyramid[i].z * s;
        pyramid[i].z = original_x * s + pyramid[i].z * c;
    }
}


void frame() {
    const float dt = 0.5/FPS;
    angle += pi * dt;
    dz += 1*dt;
    clear_screen(renderer);


    Point3D pyramid[8];


    rotate_xz(pyramid, angle);

    translate_z(pyramid, dz);


    for (int i = 0; i < 6; i++) {
        Point2D projected = project(pyramid[i]);
        Point2D pixel = screen(projected);
    //  point_rect(renderer, pixel.x, pixel.y);
    }


    for (int i = 0; i < 8; i++) {
        Point2D p1 = screen(project(pyramid[Faces[i][0]]));
        Point2D p2 = screen(project(pyramid[Faces[i][1]]));
        Point2D p3 = screen(project(pyramid[Faces[i][2]]));


        line(renderer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    }
 
    

    SDL_RenderPresent(renderer);


    
}

void video () {
    init();
    int timer = 600;
    int running = 1;

    while (running) {

         SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }


        if (timer >= 0) {
            frame();
            timer--;
        } else {
            running = 0;
        }

        SDL_Delay(1000 / FPS);
    }

    terminate();


}





int main(void) {


    video();

    
    return 0;   
}