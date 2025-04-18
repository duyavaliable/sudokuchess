#include "interface.h"
#include <SDL_image.h>
#include <iostream>

Image::Image(const std::string& path, int x, int y, int w, int h, SDL_Renderer* renderer) {
    // Tải ảnh vào texture
    texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        std::cerr << "Unable to load texture from " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
    }
    rect = { x, y, w, h };
}

Image::~Image() {
    // Hủy texture nếu nó được tạo
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Image::draw(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    } else {
        std::cerr << "Error: Texture not loaded." << std::endl;
    }
}

bool Image::isClicked(int mouseX, int mouseY) {
    return mouseX >= rect.x && mouseX <= rect.x + rect.w &&
           mouseY >= rect.y && mouseY <= rect.y + rect.h;
}

void renderInterface(SDL_Renderer* renderer, bool showIcons) {
    if (showIcons) {
        // Vẽ các icon sau khi game bắt đầu
      /*  Image downHeadIcon("images/down_head_icon1.png", 60, 60, 50, 50, renderer);
        downHeadIcon.draw(renderer);*/

        Image returnIcon("images/return_icon1.png", 60, SCREEN_HEIGHT - 100, 40, 40, renderer);
        returnIcon.draw(renderer);

        Image xeIcon("images/white_rock.png", 60, 100, 50, 50, renderer);
        xeIcon.draw(renderer);

        Image tuongIcon("images/white_bishop.png", 60, 200, 50, 50, renderer);
        tuongIcon.draw(renderer);

        Image totIcon("images/white_pawn.png", 60, 300, 50, 50, renderer);
        totIcon.draw(renderer);
    } else {
        // Vẽ giao diện chính (Start, Quit, Game)
        Image gameButton("images/Game.png", SCREEN_WIDTH / 2 - 100, 100, 200, 60, renderer);
        gameButton.draw(renderer);

        Image startButton("images/Start.png", SCREEN_WIDTH / 2 - 100, 300, 200, 60, renderer);
        startButton.draw(renderer);

        Image quitButton("images/Quit.png", SCREEN_WIDTH / 2 - 100, 400, 200, 60, renderer);
        quitButton.draw(renderer);
    }
}
