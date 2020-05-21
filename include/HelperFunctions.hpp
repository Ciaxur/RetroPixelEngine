#pragma once
#include "RetroPixelEngine.h"

/* Helper Functions */

/**
 * Map value x that is between a-b to c-d
 * 
 * @param x - Value to be Mapped
 * @param a - Minimum Value of x
 * @param b - Maximum Value of x
 * @param c - Minimum Mapped Value
 * @param d - Maximum Mapped Value
 */
float map(float x, float a, float b, float c, float d) {
    return (x - a) / (b - a) * (d - c) + c;
}

/**
 * Simple Helper Function to manipulate a Texture's
 *  pixels using a lambda function
 * 
 * @param texture - Pointer to the SDL_Texture to extract it's pixels
 * @param fn - Lambda Function that returns void and takes in uint32_t
 *  which is a pointer to the pixels of the Texture
 */
void manipPixels(SDL_Texture *texture, std::function<void(uint32_t *pixels)> fn) {
    // Get Texture Pixels
    void *pixels_ptr;
    int pitch;

    SDL_LockTexture(texture, nullptr, &pixels_ptr, &pitch);
    uint32_t *pixels = static_cast<uint32_t *>(pixels_ptr);

    // Handle Pixles
    fn(pixels);

    // Apply Updated Pixels & Refresh Renderer
    SDL_UnlockTexture(texture);
}