#include "../include/RetroPixelEngine.h"
#include "../include/HelperFunctions.hpp"

/**
 * Sets the Pixel to a color at the position given
 *  Draws pixel for the Main Draw Pixels
 * 
 * @param x - The X-axis position
 * @param y - The Y-axis position
 * @param color - The Color to set the pixel to
 * @param pixels - Pointer to the pixels array
 */
void RetroPixelEngine::drawPixel(int x, int y, u_int32_t color, u_int32_t *pixels) {
    // Map the Pixels
    pixels[x + (y * WIDTH_pixel)] = color;
}

/**
 ***********************************************************
 * Private Static Methods and Callbacks
 *  - Key Presses, Mouse Button, Cursor Movements, and
 *      Mouse Scrolling Callbacks
 ***********************************************************
 */

void RetroPixelEngine::onKey(SDL_KeyboardEvent &k) {
    // Output Key Pressed
    spdlog::info("KEY: Key[{}], ScanCode[{}], State[{}], Mod[{}]", k.keysym.sym, k.keysym.scancode, k.state, k.keysym.mod);
}

void RetroPixelEngine::onMouseClick(SDL_MouseButtonEvent &m) {
    // Output Key Pressed
    spdlog::info("MOUSE: Button[{}], State[{}], Clicks[{}], Coord[{},{}]", m.button, m.state, m.clicks, m.x, m.y);
}

void RetroPixelEngine::onMouse(double xPos, double yPos) {
    // Output Mouse Cursor Position
    spdlog::info("CURSOR: X[{:.2f}], Y[{:.2f}]", xPos, yPos);
}

void RetroPixelEngine::onMouseScroll(double xOffset, double yOffset) {
    // Output Mouse Cursor Position
    spdlog::info("SCROLL: X-off[{:.2f}], Y-off[{:.2f}]", xOffset, yOffset);
}

/**
 ***********************************************************
 * Private Static Methods Backend
 * Initialization of Backend Functionallity
 ***********************************************************
 */

double RetroPixelEngine::getFPS() {
    return FPS;
}


/**
 ***********************************************************
 * Private Helper Functions
 *
 *  - Draw Loop
 *  - Preload Function
 *      - Is Called prior to Draw
 *  - Process Input Function
 *      - Handles User Input from window
 *      - Function is Called Prior to Draw Loop
 *  - Fixed Update
 *      - Physics / Calculation Fixed Step Update
 ***********************************************************
 */

void RetroPixelEngine::Draw() {
    // Update Title: Output FPS to Window Title
    sprintf(titleBuffer, "%s [%.2f FPS]", title, getFPS());
    SDL_SetWindowTitle(window, titleBuffer);


    // Obtain Width and Height
    const int width = this->WIDTH_pixel;
    const int height = this->HEIGHT_pixel;

    // Using the Helper Function for Manipulating Pixels
    //  Drawing a couple Pixels in the middle of the Texture
    manipPixels(texture, [&](uint32_t *pixels) {
        const uint32_t red = 0xFF0000;
        
        // Draw Example
        drawPixel((width/2)     , (height/2)    , red, pixels);
        drawPixel((width/2) - 2 , (height/2) - 3, red, pixels);
        drawPixel((width/2) + 2 , (height/2) - 3, red, pixels);
        drawPixel((width/2) + 2 , (height/2) + 2, red, pixels);
        drawPixel((width/2) + 1 , (height/2) + 2, red, pixels);
        drawPixel((width/2)     , (height/2) + 2, red, pixels);
        drawPixel((width/2) - 1 , (height/2) + 2, red, pixels);
        drawPixel((width/2) - 2 , (height/2) + 2, red, pixels);
    });

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void RetroPixelEngine::Preload() {
    this->InitRender();
}

void RetroPixelEngine::fixedUpdate(const double deltaTime) {}

void RetroPixelEngine::setFixedTimeUpdate(uint64_t deltaTime) {
    if (deltaTime > 0)
        this->fixedTimeUpdate = deltaTime; // Assume user knows what their doing
}

/**
 * Running Fixed Update in a Thread until parent's isLoop is
 *  false
 * 
 * @param deltaTime - Time in Milliseconds to run fixedUpdate
 * @param parent - Pointer to the RetroPixelEngine Parent Object
 */
void RetroPixelEngine::runFixedUpdate(uint64_t deltaTime, RetroPixelEngine *parent) {
    // Keep track of Variables
    double startTime = SDL_GetTicks() / 1000.0f;    // Start Time of Call (Obtain Delta Time)
    double endTime = startTime;                     // End Time of Call
    
    while(parent->isLoop) {
        // Store Current Time
        startTime = SDL_GetTicks() / 1000.0f;   // Store Current Time
        
        // Run Virtual Function
        parent->fixedUpdate(startTime - endTime);   // Evaluate and send Send the deltaTime since last Call

        // Update time
        endTime = startTime;                    // Store the Current Time

        // Run Delay
        std::this_thread::sleep_for(std::chrono::milliseconds(deltaTime));
    }
}

/**
 ***********************************************************
 * Main User-End Methods
 *
 *  - Constructor for Creating the Object :)
 *  - Destructor for Cleaning Up
 *  - Inizializing & Starting the Engine once the User
 *      is ready!
 ***********************************************************
 */

RetroPixelEngine::RetroPixelEngine(int pixelWidth, int pixelHeight, u_int8_t scale, const char *title) : RES_SCALE(scale) {
    WIDTH_pixel = pixelWidth;
    HEIGHT_pixel = pixelHeight;
    this->title = title;
}

RetroPixelEngine::~RetroPixelEngine() {
    printf("\nExiting, cleaning up first...\n");

    /* Destroy Resources */
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/**
 * Initiates Default Rendering Settings
 */
void RetroPixelEngine::InitRender() {
    /* Configure SDL Properties */
    // Initialize Window, Renderer, & Texture
    //  Texture will be used to draw on
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        this->title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH_pixel  * RES_SCALE,
        HEIGHT_pixel * RES_SCALE,
        SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH_pixel,
        HEIGHT_pixel);
}

/**
 * Window Evenet Handler, used to be run in a Thread
 *  Polls every 1ms
 * 
 * @param windowEvent - Pointer to the Window Event from SDL
 * @param parent - Pointer to the Parent Object to access Data
 */
void RetroPixelEngine::handleEventPolling(SDL_Event *windowEvent, RetroPixelEngine *parent) {
    while(true) {
        if (SDL_PollEvent(windowEvent)) {
            // Check if close button was clicked
            if (windowEvent->type == SDL_QUIT) { 
                parent->isLoop = false;
                return;
            }

            switch (windowEvent->type) {
            // Handle Key Presses
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                parent->onKey(windowEvent->key);
                break;

            // Handle Mouse Button
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                parent->onMouseClick(windowEvent->button);
                break;

            case SDL_MOUSEMOTION:
                parent->onMouse(windowEvent->motion.x, windowEvent->motion.y);
                break;

            case SDL_MOUSEWHEEL:
                parent->onMouseScroll(windowEvent->wheel.x, windowEvent->wheel.y);
                break;

            default:
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int RetroPixelEngine::run() {
    /* Keep track of FPS & Fixed Upate */
    u_int32_t lastTime = SDL_GetTicks();
    int frameCount = 0;

    /* Run Pre-Start Function */
    Preload();

    /* Run fixedUpdate in a Thread */
    std::thread fixedUpdateThread(runFixedUpdate, this->fixedTimeUpdate, this);

    /* Keep Window open until Quit - Hanlde Polling in Thread */
    SDL_Event windowEvent;
    std::thread event_thread(handleEventPolling, &windowEvent, this);


    /* Start Looping */
    isLoop = true;
    while (isLoop) {
        // Measure the Speed (FPS)
        u_int32_t currentTime = SDL_GetTicks();
        frameCount++;
        if (currentTime - lastTime >= 1000) {  // 1 Second Elapsed
            FPS = frameCount;
            frameCount = 0;
            lastTime += 1000;
        }

        // Draw Here...
        Draw();

        // Keep Track of Overall FrameCount
        overallFrameCount++;
    }

    // Wait till Threads Quits
    event_thread.join();
    fixedUpdateThread.join();

    // No Issues
    return 0;
}
