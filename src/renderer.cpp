#include "renderer.h"
#include <SDL2/SDL.h>

Renderer::Renderer() : m_pWindow(nullptr), m_pRenderer(nullptr), m_pPatternTexture(nullptr), m_pNametableTexture(nullptr), m_pScreenTexture(nullptr) {}

Renderer::~Renderer() 
{
    if (m_pPatternTexture) SDL_DestroyTexture(m_pPatternTexture);
    if (m_pNametableTexture) SDL_DestroyTexture(m_pNametableTexture);
    if (m_pScreenTexture) SDL_DestroyTexture(m_pScreenTexture);
    if (m_pRenderer) SDL_DestroyRenderer(m_pRenderer);
    if (m_pWindow) SDL_DestroyWindow(m_pWindow);
}

bool Renderer::init() 
{
    SDL_Init(SDL_INIT_VIDEO);

    m_pWindow = SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 480, 0);
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

    m_pPatternTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 128, 128);
    m_pNametableTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);
    m_pScreenTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);

    return m_pWindow && m_pRenderer && m_pPatternTexture && m_pNametableTexture && m_pScreenTexture;
}

void Renderer::renderPatternTable(const uint32_t* pixelData)
{
    SDL_UpdateTexture(m_pPatternTexture, nullptr, pixelData, 128 * sizeof(uint32_t));

    SDL_RenderClear(m_pRenderer);
    SDL_RenderCopy(m_pRenderer, m_pPatternTexture, nullptr, nullptr);
    SDL_RenderPresent(m_pRenderer);
}

void Renderer::renderNametable(const uint32_t* /*pixelData*/)
{
    // TODO
}

void Renderer::renderScreen(const uint32_t* /*pixelData*/)
{
    // TODO
}