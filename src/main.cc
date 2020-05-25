#include "../include/RetroPixelEngine.h"


class Game: public RetroPixelEngine {
    public:
        Game(int pixelWidth, int pixelHeight, u_int8_t scale, const char *title): 
            RetroPixelEngine(pixelWidth, pixelHeight, scale, title) {}
    
    private:
        // Override the fixedUpate method to display it's deltaTime capabilities
        void fixedUpdate(const double deltaTime) {
            spdlog::info("fixedUpdate was called: {:.2f}ms ago", deltaTime);
        }
};


int main(int argc, char **argv) {
    // Window Size of 512x256
    Game rpe(64, 32, 8, "RetroPixelEngine: Simple Example");

    // Run Engine Until Close
    int status = rpe.run();

    // Ouptut Status of Engine
    spdlog::warn("RetroPixelEngine Exited with: {}", status);
    return status;    
}