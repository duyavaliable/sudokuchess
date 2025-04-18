#ifndef INTERFACE_MAP_H
#define INTERFACE_MAP_H

// Khai báo hằng số SCREEN_WIDTH, SCREEN_HEIGHT, và NOISE_DENSITY
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int NOISE_DENSITY = 500; // Số lượng noise

#include <SDL.h>
#include <string>


class Image {
public:
    // Constructor: Tạo đối tượng Image từ đường dẫn và kích thước
    Image(const std::string& path, int x, int y, int w, int h, SDL_Renderer* renderer);
    ~Image();

    // Vẽ ảnh ra màn hình
    void draw(SDL_Renderer* renderer);

    // Kiểm tra xem chuột có click vào vùng của ảnh không
    bool isClicked(int mouseX, int mouseY);

private:
    SDL_Texture* texture = nullptr;
    SDL_Rect rect;
};

void renderInterface(SDL_Renderer* renderer, bool showIcons);

#endif
