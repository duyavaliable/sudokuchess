// image.cpp
#include "interface.h"
#include <SDL_image.h>
#include <iostream>

// Constructor
Image::Image(const std::string &path, int x, int y, int w, int h, SDL_Renderer *renderer)
{
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface)
    {
        std::cerr << "Failed to load image: " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
    {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return;
    }
    rect = {x, y, w, h};
}

// Destructor
Image::~Image()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

// Draw the image
void Image::draw(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

// Check if the image is clicked
bool Image::isClicked(int mouseX, int mouseY)
{
    return mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h;
}