#pragma once
#include <cstdint>

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();
    void renderPatternTable(const uint32_t* pixelData);
    void renderNametable(const uint32_t* pixelData);
    void renderScreen(const uint32_t* pixelData);

private:
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    SDL_Texture* m_pPatternTexture;
    SDL_Texture* m_pNametableTexture;
    SDL_Texture* m_pScreenTexture;
};
