# OpenGL Primitive Types Assignment Submission

## Student Details

- Name: `EZEKIEL REGO`
- Module Code: `ISH 407 / HCS411`
- Institution: `Great Zimbabwe University`

## Submission Overview

This repository contains my OpenGL practical assignment submission for the Computer Graphics module.  
The work demonstrates how different OpenGL primitive types are formed from vertex coordinates and how those primitives appear when rendered on a single screen.

The main goal of this program is to reproduce the primitive reference diagrams provided in the assignment brief as closely as possible.  
To achieve that, the program places each primitive on one page, labels each vertex, and adjusts the layout to visually match the supplied reference images.

## Implemented Primitive Types

The program includes the following OpenGL primitives:

- `GL_POINTS`
- `GL_LINES`
- `GL_LINE_STRIP`
- `GL_LINE_LOOP`
- `GL_TRIANGLES`
- `GL_TRIANGLE_STRIP`
- `GL_TRIANGLE_FAN`
- `GL_QUADS`
- `GL_QUAD_STRIP`
- `GL_POLYGON`

## Files Included

- [main.cpp](main.cpp): main OpenGL source file used to render the primitive chart
- [main copy.cpp](main%20copy.cpp): backup working copy of the same program
- [images](images): reference images used to compare and refine the primitive shapes

## How The Program Works

The program uses the GLUT library with a 2D orthographic projection.  
Each primitive is defined using explicit vertex coordinates.  
Small helper functions are used to:

- draw primitive names
- draw vertex markers and labels such as `v0`, `v1`, `v2`
- render filled and outlined primitives
- support scrolling so the entire page can be viewed even when the bottom section extends beyond the visible window

Some primitives such as `GL_TRIANGLES`, `GL_QUADS`, `GL_QUAD_STRIP`, and `GL_TRIANGLE_FAN` were refined with custom outline logic so that their visual appearance matches the reference images more closely.

## Program Output

The screenshot below shows the current program output:

![Program Output](images/live.png)

## Build And Run

Example compile command used in this project:

```powershell
C:\msys64\mingw64\bin\g++.exe main.cpp -o app.exe -IC:\msys64\mingw64\include -LC:\msys64\mingw64\lib -lfreeglut -lopengl32 -lglu32
```

Run the program with:

```powershell
.\app.exe
```

## Controls

- Mouse wheel: scroll up or down
- `Up` / `Page Up`: scroll upward
- `Down` / `Page Down`: scroll downward

## Academic Note

This repository is prepared as an academic submission.  
The code and images are organised to make marking easier by showing:

- the final rendered output
- the source code used to generate it
- the reference images used during development

## Notes For Lecturer

- The implementation focuses on visual similarity to the reference sheet provided in the assignment.
- Deprecated legacy primitives such as `GL_QUADS`, `GL_QUAD_STRIP`, and `GL_POLYGON` are included intentionally because they are part of the assignment requirements.
