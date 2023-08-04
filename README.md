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

![Perf](https://github.com/TheWrecker/GameDev/assets/14818908/08fb3c0a-f277-49bc-9a2e-44c5695ae821)

 
## Gameplay
### Infinite World
World is generated around the player as they move, making the world horizontally infinite.
- Can be switched off.

![2023-07-20 20-46-52(2)](https://github.com/TheWrecker/GameDev/assets/14818908/cc0e194c-d25f-4152-8234-2e5f9ece26cb)

 
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
  
![2023-08-04 17-16-20](https://github.com/TheWrecker/GameDev/assets/14818908/6c1a0f95-f7dc-4600-9126-a6ba553d5701)


