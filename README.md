**HUMANGL - 3D Model Renderer with Animation**

HUMANGL is a 3D model rendering application developed at 42 Ecole, designed to load .dae files with animations and render them using OpenGL. The application allows users to define various animations, like idle, walk, jump, and kick, through a configuration file. Models are parsed with textures to provide a more realistic visual experience.

**Features:**

Animation Configuration: Configure animations for idle, walk, jump, and kick states using a simple config file.

Model Parsing with Assimp: Supports COLLADA (.dae) files for both models and animations.

Texture Parsing with STB Image: Loads textures associated with models, using the stb_image library for image file parsing.

Custom Math Library: A GLM-like library handles matrix operations to meet project constraints that disallow external math libraries.

**Usage:**

To render a model with animations, create a configuration file (see example below) and run the application with the path to your config file:

./humangl pathToConfigFile

**Controls:**

**W, A, S, D** - Move object along the XY axis

**Q, E** - Move object along the Z axis

**Arrow keys** - Rotate object

**F** - Wireframe mode

**U** - Fill mode

**-/+** - Scale object (hold Left Shift with + for finer scaling)

**Animations:**

**Idle** Plays automatically

**G** - Walk animation

**SPACE** - Jump animation

**ENTER** - Kick animation

**Config File Example:**

_model = res/model/model.dae_

_idle = res/model/idle.dae_

_jump = res/model/jump.dae_

_walk = res/model/walk.dae_

_kick = res/model/kick.dae_

https://github.com/user-attachments/assets/9362931f-c0a0-4d92-9d91-e142bc4119f3

https://github.com/user-attachments/assets/79219348-aa7c-4e19-b595-fcb216ff7dfe

https://github.com/user-attachments/assets/1fbd60b4-a0f7-4fc9-a75f-26f5928d7e9b





