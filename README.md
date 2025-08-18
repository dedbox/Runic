# Runic Engine

My personal sandbox for learning how to build a modern game/graphics engine.

Second try: This time, I'm going to try some actual planning and project management.

## Development Priorities

The first try was a half hearted attempt to follow @TheCherno 's youtube series
on graphics engine development, which spends a lot of time on features I'm
already familiar with or not very interested in, like logging, input internals,
event management, and "future proofing."

This time, I'm just going to start with SDL3 ad OpenGL 4.6 on Linux, and
(mostly) just build on what they provide. Later, I will add a Vulkan renderer.
While I would like the architecture to be flexible enough to support multiple
host platforms and graphics APIs, I think it will be easier and more informative
to get everything working in one familiar setting first.

I would, however, like the engine to provide a single consistent API, which most
probably will require abstractions around these underlying support
mechanisms. Hopefully, they will serve as a useful starting point for eventual
platform-agnostification efforts.

Similarly, the initial focus will be on 3D, with support for 2D possibly coming
later.
