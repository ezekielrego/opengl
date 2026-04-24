# OpenGL Primitives Reference

This repository contains a high quality visual reference of OpenGL primitive types

It is created as part of an assignment to demonstrate understanding of how vertices are grouped to form different primitives in OpenGL

## Included primitives

- GL_POINTS
- GL_LINES
- GL_LINE_STRIP
- GL_LINE_LOOP
- GL_TRIANGLES
- GL_TRIANGLE_STRIP
- GL_TRIANGLE_FAN
- GL_QUADS
- GL_QUAD_STRIP
- GL_POLYGON

## Purpose

The goal of this project is to

- understand how OpenGL constructs shapes from vertices
- compare different primitive types visually
- provide a clear reference for study and revision
- allow extraction of individual shapes from the image

## Explanation of primitives

### GL_POINTS
Each vertex is rendered as a single point

### GL_LINES
Vertices are grouped in pairs and each pair forms a separate line

### GL_LINE_STRIP
Each vertex connects to the previous one forming a continuous line

### GL_LINE_LOOP
Same as line strip but the last vertex connects back to the first

### GL_TRIANGLES
Vertices are grouped in sets of three and each group forms a triangle

### GL_TRIANGLE_STRIP
Each new vertex forms a triangle with the previous two vertices

### GL_TRIANGLE_FAN
All triangles share a common starting vertex creating a fan shape

### GL_QUADS
Vertices are grouped in sets of four and each group forms a quadrilateral

### GL_QUAD_STRIP
Each new pair of vertices forms a connected quad with the previous pair

### GL_POLYGON
All vertices define a single filled polygon

## Usage

This repository can be used for

- learning OpenGL basics
- assignment reference
- visual study material
- presentations

## Program Output

![Program Output](images/live.png)

## Note

Some primitives such as GL_QUADS GL_QUAD_STRIP and GL_POLYGON are deprecated in modern OpenGL but are included here for learning purposes

## Author

Add your name here

## License

Specify a license if required
