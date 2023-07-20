# GameDev
A voxel-based hobby project which serves as a testing-and-learning-ground for me to learn software design, game developement and graphics programming.
 
 
# Current Features:

## Software Design




### Very Basic Multithreading
World and mesh generation are done on a separate thread, so game logic and rendering can be done without stutters.
 
 
### Basic Performance Info
Currently displays:
- Frames per Second
- Average Frame Time
- Last Frame Time

![Performance Profiler](https://github.com/TheWrecker/GameDev/assets/14818908/534e7e4e-9b8a-4438-9dcf-00019555fe17)

 
## Gameplay
### Infinite World
World is generated around the player as they move, making the world horizontally infinite.
- Can be switched off.

 ![2023-07-20 20-46-52](https://github.com/TheWrecker/GameDev/assets/14818908/e7417d22-86ec-4544-86e3-92f81a01ccab)

 
### Block Placement and Removal
Basic placement and removal of blocks are currently supported.
 
 
### Basic Movement System
- While physics is enabled:
  - Basic WASD Movement
  - Basic Jumping
- While physics is disabled:
  - Free-flying FPS Movement
 
 
### Basic Physics System
- Currently supports these items for the player:
  - Basic Friction
  - Basic Gravity
  - Basic Collision
- Can be switched off.
 
 
### Variable World Height
Minimal-level height variation based on noise-map.
 
 
### Basic Inventory
Inventory slots which add and remove blocks automatically upon block placement/removal.
 
 
## Graphics and Rendering
### Basic Sky
Sky rendered utilizing a texture cubemap.
 
 
### Texture Array for Blocks
All block textures are chained together, which allows for better performance and allows texture changes on the fly without any state changes.

 
### Different Textures for Different Block Faces
A different texture can be set for each of these groups:
- Top Face
- Side Faces
- Bottom Face
 
 
### Mesh Optimization
Voxel meshes are constantly merged to create a solid exterior, optimized for reducing rendering load.
- Can be switched off.
 
 
### Frustrum Culling
Segments outside the current view frustrum are not rendered to increase the performance.
 
 
### Global Directional Lighting
Voxel faces are dynamically illuminated based on the global light direction. Currently supports:
- Adjustable Ambient Color
- Adjustable Light Color
- Adjustable Light Direction
 
 
### Shadow Map (**Work in Progress**)
Basic shadowmapping.
- Adjustable light frustrum
- Able to use either a render-target or a depth-map.
