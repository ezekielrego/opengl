#include <GL/glut.h>

struct Point2D {
    float x;
    float y;
};

struct LabelOffset {
    float dx;
    float dy;
};

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text);
        ++text;
    }
}

void drawVertexMarkers(const Point2D points[], const LabelOffset offsets[], int count) {
    glPointSize(5.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < count; ++i) {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();

    for (int i = 0; i < count; ++i) {
        char label[8];
        label[0] = 'v';
        label[1] = static_cast<char>('0' + i);
        label[2] = '\0';
        float dx = offsets ? offsets[i].dx : -3.0f;
        float dy = offsets ? offsets[i].dy : -4.0f;
        drawText(points[i].x + dx, points[i].y + dy, label);
    }
}

void drawPrimitive(const Point2D points[], int count, GLenum mode) {
    glBegin(mode);
    for (int i = 0; i < count; ++i) {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();
}

void drawFilledPrimitive(const Point2D points[], int count, GLenum mode) {
    glColor3f(0.85f, 0.85f, 0.85f);
    drawPrimitive(points, count, mode);

    glColor3f(0.0f, 0.0f, 0.0f);
    if (mode == GL_TRIANGLES || mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN) {
        drawPrimitive(points, count, GL_LINE_STRIP);
    } else {
        drawPrimitive(points, count, GL_LINE_LOOP);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);

    const Point2D pointsExample[] = {
        {12.0f, 92.0f}, {12.0f, 87.0f}, {18.0f, 88.0f}, {18.0f, 91.0f}, {24.0f, 95.0f}
    };
    const LabelOffset pointsLabels[] = {
        {-5.0f, 0.5f},
        {-5.0f, -1.0f},
        {2.0f, -1.5f},
        {2.0f, 0.0f},
        {2.0f, 0.5f}
    };
    drawVertexMarkers(pointsExample, pointsLabels, 5);
    drawText(9.0f, 77.0f, "GL_POINTS");

    const Point2D linesExample[] = {
        {6.0f, 63.0f}, {12.0f, 68.0f},
        {13.8f, 61.0f}, {26.8f, 58.0f},
        {8.0f, 60.0f}, {20.0f, 56.0f},
        {7.0f, 57.0f}, {17.0f, 61.0f}
    };
    const LabelOffset linesLabels[] = {
        {-4.5f, -2.5f},
        {1.5f, -0.5f},
        {-2.6f, 1.0f},
        {1.2f, -1.0f},
        {-2.0f, 0.6f},
        {1.0f, -2.2f},
        {-2.2f, -0.8f},
        {1.3f, 0.3f}
    };
    drawPrimitive(linesExample, 8, GL_LINES);
    drawVertexMarkers(linesExample, linesLabels, 8);
    drawText(10.0f, 49.0f, "GL_LINES");

    const Point2D lineStripExample[] = {
        {42.0f, 68.0f}, {48.0f, 63.0f}, {56.0f, 67.0f}, {62.0f, 61.0f}, {66.0f, 69.0f}
    };
    drawPrimitive(lineStripExample, 5, GL_LINE_STRIP);
    drawVertexMarkers(lineStripExample, nullptr, 5);
    drawText(45.0f, 57.0f, "GL_LINE_STRIP");

    const Point2D lineLoopExample[] = {
        {79.0f, 68.0f}, {83.0f, 75.0f}, {91.0f, 63.0f}, {86.0f, 70.0f}, {81.0f, 62.0f}
    };
    drawPrimitive(lineLoopExample, 5, GL_LINE_LOOP);
    drawVertexMarkers(lineLoopExample, nullptr, 5);
    drawText(78.0f, 57.0f, "GL_LINE_LOOP");

    const Point2D trianglesExample[] = {
        {10.0f, 34.0f}, {16.0f, 44.0f}, {22.0f, 34.0f},
        {20.0f, 30.0f}, {28.0f, 44.0f}, {34.0f, 44.0f}
    };
    drawFilledPrimitive(trianglesExample, 6, GL_TRIANGLES);
    drawVertexMarkers(trianglesExample, nullptr, 6);
    drawText(10.0f, 24.0f, "GL_TRIANGLES");

    const Point2D triangleStripExample[] = {
        {44.0f, 44.0f}, {42.0f, 32.0f}, {50.0f, 46.0f},
        {58.0f, 32.0f}, {62.0f, 45.0f}, {66.0f, 31.0f}
    };
    drawFilledPrimitive(triangleStripExample, 6, GL_TRIANGLE_STRIP);
    drawVertexMarkers(triangleStripExample, nullptr, 6);
    drawText(40.0f, 24.0f, "GL_TRIANGLE_STRIP");

    const Point2D triangleFanExample[] = {
        {82.0f, 38.0f}, {78.0f, 46.0f}, {86.0f, 46.0f},
        {90.0f, 38.0f}, {87.0f, 30.0f}, {79.0f, 30.0f}
    };
    drawFilledPrimitive(triangleFanExample, 6, GL_TRIANGLE_FAN);
    drawVertexMarkers(triangleFanExample, nullptr, 6);
    drawText(77.0f, 24.0f, "GL_TRIANGLE_FAN");

    const Point2D quadsExample[] = {
        {10.0f, 12.0f}, {18.0f, 15.0f}, {17.0f, 24.0f}, {8.0f, 22.0f},
        {24.0f, 13.0f}, {30.0f, 15.0f}, {31.0f, 26.0f}, {25.0f, 24.0f}
    };
    drawFilledPrimitive(quadsExample, 8, GL_QUADS);
    drawVertexMarkers(quadsExample, nullptr, 8);
    drawText(12.0f, 6.0f, "GL_QUADS");

    const Point2D quadStripExample[] = {
        {42.0f, 13.0f}, {44.0f, 24.0f}, {50.0f, 12.0f}, {52.0f, 25.0f},
        {58.0f, 12.0f}, {60.0f, 24.0f}, {66.0f, 13.0f}, {68.0f, 25.0f}
    };
    drawFilledPrimitive(quadStripExample, 8, GL_QUAD_STRIP);
    drawVertexMarkers(quadStripExample, nullptr, 8);
    drawText(44.0f, 6.0f, "GL_QUAD_STRIP");

    const Point2D polygonExample[] = {
        {79.0f, 17.0f}, {86.0f, 20.0f}, {90.0f, 15.0f}, {88.0f, 9.0f}, {80.0f, 10.0f}
    };
    drawFilledPrimitive(polygonExample, 5, GL_POLYGON);
    drawVertexMarkers(polygonExample, nullptr, 5);
    drawText(79.0f, 6.0f, "GL_POLYGON");

    glFlush();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 750);
    glutCreateWindow("OpenGL Primitive Examples");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
