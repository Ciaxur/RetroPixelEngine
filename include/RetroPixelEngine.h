#pragma once

// Core Libraries
#include <thread>

// SDL Libraries
#include <SDL2/SDL.h>

// Logging Libraries
#include <spdlog/spdlog.h>



class RetroPixelEngine {
  private:      // Private Untouchable Variables
    bool    isLoop;                     // Loop State of Engine (Shuts off if False)
    int     WIDTH_pixel;                // Pixel Width of Texture (Will be Stretched to fit Window Width)
    int     HEIGHT_pixel;               // Pixel Height of Texture (Will be Stretched to fit Window Height)
    uint64_t  fixedTimeUpdate = 200;    // Milliseconds that the fixedUpdate method will be called (Default 200ms)

  protected:    // Protected Variables | GL Window Data
    const u_int8_t  RES_SCALE;              // Scaled Multiplier of Pixel Dimensions
    double          FPS;                    // Current Calculated FPS Value
    uint32_t        overallFrameCount = 0;  // Overall Frame Counter

  protected:  // Shared Window Data
    SDL_Window    *window;                        // SDL Window
    SDL_Renderer  *renderer;                      // Default SDL Renderer Used
    SDL_Texture   *texture;                       // Default SDL Texture that is Stretch onto entire Window (Renderer)
    const char    *title = "RetroPixelEngine";    // Default Window Title
    char           titleBuffer[256];              // Used for Temporary Character Storage (Window Title)


  protected:  // Shared Overrideable Callbacks (Handled in Seperate Thread)
    virtual void onKey(SDL_KeyboardEvent &k);                       // Virtual Method: Keyboard Events
    virtual void onMouseClick(SDL_MouseButtonEvent &m);             // Virtual Method: Mouse Click Events
    virtual void onMouse(double xPos, double yPos);                 // Virtual Method: Mouse Movement Events
    virtual void onMouseScroll(double xOffset, double yOffset);     // Virtual Method: Mouse Scroll Events

  private:    // Private Static Methods (Threads)
    static void handleEventPolling(SDL_Event *windowEvent, RetroPixelEngine *parent);
    static void runFixedUpdate(uint64_t deltaTime, RetroPixelEngine *parent);

  protected:  // Shared Methods
    /**
     * Returns the Calculated Frames Per Second from Draw Loop
     *	@returns FPS Value
     */
    double getFPS();



  protected:  // Helper Functions
    /**
     * Draw loop
     */
    virtual void Draw();

    /**
     * Data/Properties to configure prior to Drawing
     */
    virtual void Preload();

    /* 
     * Fixed Interval Update
     *  Update is called in a Thread at a given Fixed Time
     *   defined by fixedTimeUpdate. The thread calls this function
     *   every "fixedTimeUpdate" milliseconds
     * 
     * @param deltaTime - The change in Time it took from last 
     *  time fixedUpate was called
     */
    virtual void fixedUpdate(const double deltaTime);

    /**
     * Sets the time in milliseconds to run the "fixedUpdate" method
     *  changing from it's default value of 200ms
     * Running this method takes no effect unless it was run prior
     *  to the "run" method
     * 
     * @param deltaTime - Time in Milliseconds that the fixedUpdate method will
     *  be ran
     */
    void setFixedTimeUpdate(uint64_t deltaTime);



  public:  // Main User-End Operation Functions
    /**
     * Constuctor that initalizes SDL
     * @param pixelWidth - Width of Texture that will be Upscaled
     * @param pixelHeight - Height of Texture that will be Upscaled
     * @param scale - Upscale Resolution
     * @param title - Window Title
     */
    RetroPixelEngine(int pixelWidth, int pixelHeight, u_int8_t scale, const char *title);

    /**
     * Destructor to clean up Resources used by SDL
     */
    ~RetroPixelEngine();

    /**
     * Constuctor that initalizes GLFW with default Width and Height
     */
    void InitRender();

    /**
     * Starts running OpenGL window
     * @returns - Value '-1' or '0' to determine the exit State
     */
    int run();

    /**
     * Sets the Pixel to a color at the position given
     * 
     * @param x - The X-axis position
     * @param y - The Y-axis position
     * @param color - The Color to set the pixel to
     * @param pixels - Pointer to the pixels array
     */
    void drawPixel(int x, int y, u_int32_t color, u_int32_t *pixels);
};
