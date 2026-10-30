#include "renderer.h"
#include "mesh.h"
#include "config.h"
#include <math.h>


static const float width = 20.0f;
static const float height = 20.0f;

static const float rightPositionOnWidth = width / 2;
static const float rightPositionOnHeight = height / 2;

static const float pi = 3.14f;

static float dz = 0;
static float angle = 0;


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


Point2D project(Point3D p) {
    return (Point2D){
        .x = p.x / p.z,
        .y = p.y / p.z
    };
}


Point2D screen(Point2D p) {
    Point2D result;

    result.x = (p.x + 1.0f) / 2.0f * SCREEN_WIDTH;
    result.y = (1 - (p.y + 1.0f) / 2.0f) * SCREEN_HEIGHT;

    return result;
}


void translate_z(Point3D pyramid[6], float current_dz) {
    for(int i = 0; i < 8; i++) {
    //  pyramid[i] = Points[i];
        pyramid[i].z += 30.0f;
    }
}


void rotate_xz(Point3D pyramid[6], float current_angle) {
    const float c = cosf(-current_angle);
    const float s = sinf(-current_angle);

    for(int i = 0; i < 8; i++) {
        pyramid[i] = Points[i];
        float original_x = pyramid[i].x;
        float original_y = pyramid[i].y;

        // x-rotation
        pyramid[i].y = original_y * c - pyramid[i].z * s;
        pyramid[i].z = original_y * s + pyramid[i].z * c;

        // y-rotation
        pyramid[i].x = original_x * c - pyramid[i].z * s;
        pyramid[i].z = original_x * s + pyramid[i].z * c;
    }
}


void frame(SDL_Renderer* renderer) {
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
        point_rect(renderer, pixel.x, pixel.y);
    }


    for (int i = 0; i < 8; i++) {
        Point2D p1 = screen(project(pyramid[Faces[i][0]]));
        Point2D p2 = screen(project(pyramid[Faces[i][1]]));
        Point2D p3 = screen(project(pyramid[Faces[i][2]]));


        line(renderer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    }
 
    

    SDL_RenderPresent(renderer);


    
}