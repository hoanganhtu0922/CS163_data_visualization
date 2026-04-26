#include "raylib.h"
#include <map>
#include <string>
#pragma once 
class assets {
public:
    // Hàm để lấy instance duy nhất (Singleton)
    static assets& Instance() {
        static assets instance;
        return instance;
    }

    // Load tất cả tài nguyên cần thiết ở đầu chương trình
    void MakeColorTransparent(Image *img, Color targetColor) {
        // Lấy mảng các màu từ Image (đây là mảng 1 chiều)
        Color *pixels = LoadImageColors(*img);

        for (int i = 0; i < img->width * img->height; i++) {
            // Kiểm tra xem pixel hiện tại có trùng với màu mục tiêu không
            if (pixels[i].r == targetColor.r && 
                pixels[i].g == targetColor.g && 
                pixels[i].b == targetColor.b) 
            {
                // Biến nó thành trong suốt hoàn toàn
                pixels[i].a = 0;
            }
        }

        for (int i = 0; i < img->width * img->height; i++) {
            img[i].data = &pixels[i]; // Cập nhật lại data của Image để trỏ đến mảng màu đã chỉnh sửa     
        }

        // Quan trọng: Giải phóng mảng màu đã load vào RAM
        UnloadImageColors(pixels);
    }

    bool is_hover(Vector2 texPos, std::string key, float scale = 1.0f) {
        if (textures.find(key) == textures.end()) return false;
        Texture2D tex = textures[key];
        Vector2 mousePos = GetMousePosition();
        float width = tex.width * scale;
        float height = tex.height * scale;
        return (mousePos.x >= texPos.x && mousePos.x <= texPos.x + width &&
                mousePos.y >= texPos.y && mousePos.y <= texPos.y + height);
    }

    bool is_clicked(Vector2 texPos, std::string key, float scale = 1.0f) {
        if (is_hover(texPos, key, scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
        return false;
    }

    void LoadResources() {
        Image myImage = LoadImage("../../assets/image/home_icon.png");
        Color blueBG = { 54, 113, 176, 255 };
        //MakeColorTransparent(&myImage, blueBG);
        textures["home_icon"] = LoadTextureFromImage(myImage);

        myImage = LoadImage("../../assets/image/random_icon.png");
        //MakeColorTransparent(&myImage, blueBG);
        textures["random_icon"] = LoadTextureFromImage(myImage);

        myImage = LoadImage("../../assets/image/go_icon.png");
        //MakeColorTransparent(&myImage, blueBG);
        textures["go_icon"] = LoadTextureFromImage(myImage);

        myImage = LoadImage("../../assets/image/play_icon.png");
        //MakeColorTransparent(&myImage, blueBG);
        textures["play_icon"] = LoadTextureFromImage(myImage);

        myImage = LoadImage("../../assets/image/pause_icon.png");
        //MakeColorTransparent(&myImage, blueBG);
        textures["pause_icon"] = LoadTextureFromImage(myImage);
    }

    // Lấy texture theo key mà không cần load lại
    Texture2D Get(std::string key) {
        return textures[key];
    }

    void draw_texture(std::string key, Vector2 position, float scale = 1.0f) {
        if (textures.find(key) != textures.end()) {
            Texture2D tex = textures[key];
            DrawTextureEx(tex, position, 0.0f, scale, WHITE);
        }
    }

    // Giải phóng bộ nhớ khi tắt app
    void UnloadResources() {
        for (auto& pair : textures) {
            UnloadTexture(pair.second);
        }
    }

private:
    assets() {} // Constructor private để chặn tạo mới
    std::map<std::string, Texture2D> textures;
};