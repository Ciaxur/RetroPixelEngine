#include "../include/RetroPixelEngine.h"

int main(int argc, char **argv) {
    // Window Size of 512x256
    RetroPixelEngine rpe(64, 32, 8, "RetroPixelEngine: Simple Example");

    // Run Engine Until Close
    int status = rpe.run();

    // Ouptut Status of Engine
    spdlog::warn("RetroPixelEngine Exited with: {}", status);
    return status;    
}