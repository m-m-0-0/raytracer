//
// Created by molar on 31/08/2023.
//

#ifndef RAYTRACER_WINDOW_H
#define RAYTRACER_WINDOW_H

#include <SDL2/SDL.h>
#include "Image.h"

class Window {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

public:
    int width;
    int height;

    Window(int width, int height){
        this->width = width;
        this->height = height;

        SDL_Init(SDL_INIT_VIDEO);

        window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    }

    //dtor
    ~Window(){
        close();
    }

    void update(Image* image){
        //convert image to
        SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
        for(int i = 0; i < width * height; i++){
            //getpixel(x, y)
            //to_sdl_color(SDL_PixelFormat*)
            int x = i % width;
            int y = i / width;
            ((Uint32*)surface->pixels)[i] = image->get_pixel(x, y).to_sdl_color(surface->format);
        }

        SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    void handle_events(){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                close();
                exit(0);
            }
        }
    }

    void close(){
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

};


#endif //RAYTRACER_WINDOW_H
