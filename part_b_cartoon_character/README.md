# Part B - 3D Cartoon Character Near/Far Plane

This is a separate OpenGL/freeGLUT C++ project for Part B of the Computer Graphics assignment.

## Hosted Output Page

[https://gl.applications.co.zw/demos/near-far.html](https://gl.applications.co.zw/demos/near-far.html)

## Screenshot

![Part B output](part_b_3d_cartoon_scene.png)

## What It Shows

- A large smooth 3D-style cartoon student character in the foreground to represent the near plane.
- Smaller mountains, trees, clouds, buildings, sun, ground, and path behind the character to represent the far plane.
- Rounded connected body parts using OpenGL/GLUT 3D primitives: `glutSolidSphere`, `gluCylinder`, `gluCylinder` cones, `glutSolidCube`, and `glutSolidTorus`.
- Perspective depth using `gluPerspective()` and `gluLookAt()`.
- 3D rendering settings: `GL_DEPTH_TEST`, `GL_LIGHTING`, `GL_LIGHT0`, `GL_SMOOTH`, materials, and normalized scaled shapes.
- Character details: glasses, headphones, blue jacket, hoodie collar, book, rounded arms/hands/legs/shoes, hair, facial expression, hoverboard, glow, and shadow.
- Keyboard control: pressing movement keys makes the character step onto a hoverboard, then move only a short distance left, right, forward, or backward.
- The walking motion is only used briefly during the hoverboard mounting animation, so the main movement looks cleaner.
- Bounded movement keeps the character close to the center instead of travelling continuously through the scene.
- Far-plane houses, trees, mountains, clouds, and sun stay behind the character to support the near/far depth view.
- Facial animation: blinking eyes and a subtle animated smile.
- Cloud watermark text: `M230913`.

## Files

- `part_b_cartoon_character.cpp`: main C++ OpenGL source file.
- `part_b_3d_cartoon_scene.png`: captured screenshot of the final rendered scene.
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

- `W` / `Up Arrow`: small step forward
- `S` / `Down Arrow`: small step backward
- `A` / `Left Arrow`: rotate left with a small side step
- `D` / `Right Arrow`: rotate right with a small side step
- `R`: reset the character position and dismount the hoverboard
- `P`: save a screenshot as `part_b_3d_cartoon_scene.ppm`
- `Q` or `Esc`: close the program
