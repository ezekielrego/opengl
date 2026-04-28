# Part B - 3D Cartoon Character Near/Far Plane

This is a separate OpenGL/freeGLUT C++ project for Part B of the Computer Graphics assignment.

## What It Shows

- A large smooth 3D-style cartoon student character in the foreground to represent the near plane.
- Smaller mountains, trees, clouds, buildings, sun, ground, and path behind the character to represent the far plane.
- Rounded connected body parts using OpenGL/GLUT 3D primitives: `glutSolidSphere`, `gluCylinder`, `gluCylinder` cones, `glutSolidCube`, and `glutSolidTorus`.
- Perspective depth using `gluPerspective()` and `gluLookAt()`.
- 3D rendering settings: `GL_DEPTH_TEST`, `GL_LIGHTING`, `GL_LIGHT0`, `GL_SMOOTH`, materials, and normalized scaled shapes.
- Character details: glasses, headphones, blue jacket, hoodie collar, book, rounded arms/hands/legs/shoes, hair, facial expression, hoverboard, glow, and shadow.
- Game-style keyboard control: pressing movement keys makes the character step onto a hoverboard, then glide smoothly.
- The walking motion is only used briefly during the hoverboard mounting animation, so the main movement looks cleaner.
- Looping environment motion: path stones, trees, buildings, mountains, and clouds move at different speeds to make the scene feel like travelling.
- Houses and trees grow to a reasonable size as they approach, move toward the side edges, and recycle only after passing near the camera.
- Facial animation: blinking eyes and a subtle animated smile.

## Files

- `part_b_cartoon_character.cpp`: main C++ OpenGL source file.
- `part_b_cartoon_character_travel.exe`: compiled verified interactive 3D program.

## Build

From this folder, compile with MinGW/freeGLUT:

```powershell
C:\msys64\mingw64\bin\g++.exe part_b_cartoon_character.cpp -o part_b_cartoon_character_travel.exe -IC:\msys64\mingw64\include -LC:\msys64\mingw64\lib -lfreeglut -lopengl32 -lglu32
```

## Run

```powershell
.\part_b_cartoon_character_travel.exe
```

## Controls

- `W` / `Up Arrow`: mount hoverboard and glide forward
- `S` / `Down Arrow`: mount hoverboard and glide backward
- `A` / `Left Arrow`: turn left
- `D` / `Right Arrow`: turn right
- `R`: reset the character position and dismount the hoverboard
- `P`: save a screenshot as `part_b_3d_cartoon_scene.ppm`
- `Q` or `Esc`: close the program
