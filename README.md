# RetroPixelEngine 👾
A basic Game Engine, or Visual Engine, that's main purpose is to ease development on Retro-Type Pixle Manipulation with a small resolution. The Engine has multiple Virtual Methods to help obstract the underlying Engine such as Running, FPS, Initialization, and Clean up. The Engine is meant to be Extended through Inheritance so be creative and have fun! 😊

## Build & Run 🔨
CMake takes care of the heavy Building.

```bash
git submodule update --init --recursive     # Update all Submodules
mkdir build && cd build                     # Create Build Directory
cmake ..                                    # Generate Build
make                                        # Build the Project
./retroApp                                  # Runs the App under the build directory
```

## Contributing: Formatting 📝
The Project uses `clang` for it's Style Fomatting Rules, but all that is asked is to make it nice and no need to format the ENTIRE document. Just a selection of it that doesn't meet Comments.

Make it nice and readable 📖

## License
Licensed under [GNU GPLv3](LICENSE).