#include <GL/glut.h>

struct Point2D {
    float x;
    float y;
};

struct LabelOffset {
    float dx;
    float dy;
};

float gScrollOffset = 0.0f;
const float kScrollStep = 4.0f;
const float kViewBottom = -12.0f;
const float kViewTop = 100.0f;
const float kMaxScrollOffset = 12.0f;

void updateProjection(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, kViewBottom - gScrollOffset, kViewTop - gScrollOffset);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void scrollView(float delta) {
    gScrollOffset += delta;

    if (gScrollOffset < 0.0f) {
        gScrollOffset = 0.0f;
    }
    if (gScrollOffset > kMaxScrollOffset) {
        gScrollOffset = kMaxScrollOffset;
    }

    updateProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    glutPostRedisplay();
}

void handleSpecialKeys(int key, int, int) {
    if (key == GLUT_KEY_DOWN || key == GLUT_KEY_PAGE_DOWN) {
        scrollView(kScrollStep);
    } else if (key == GLUT_KEY_UP || key == GLUT_KEY_PAGE_UP) {
        scrollView(-kScrollStep);
    }
}

void handleMouse(int button, int state, int, int) {
    if (state != GLUT_DOWN) {
        return;
    }

    if (button == 4) {
        scrollView(kScrollStep);
    } else if (button == 3) {
        scrollView(-kScrollStep);
    }
}

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
        float dx = offsets ? offsets[i].dx : 0.5f;
        float dy = offsets ? offsets[i].dy : 0.5f;
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
        {-1.8f, 0.3f},
        {-1.8f, -0.3f},
        {0.7f, -0.5f},
        {0.7f, 0.1f},
        {0.7f, 0.4f}
    };
    drawVertexMarkers(pointsExample, pointsLabels, 5);
    drawText(9.0f, 77.0f, "GL_POINTS");

    const Point2D linesExample[] = {
        {6.0f, 63.0f}, {12.0f, 68.0f},
        {12.5f, 65.0f}, {25.5f, 62.0f},
        {8.0f, 60.0f}, {20.0f, 56.0f},
        {7.0f, 57.0f}, {17.0f, 61.0f}
    };
    const LabelOffset linesLabels[] = {
        {-1.7f, -0.7f},
        {0.7f, -0.2f},
        {-1.4f, 0.3f},
        {0.7f, -0.5f},
        {-1.2f, 0.3f},
        {0.6f, -0.9f},
        {-1.3f, -0.4f},
        {0.7f, 0.2f}
    };
    drawPrimitive(linesExample, 8, GL_LINES);
    drawVertexMarkers(linesExample, linesLabels, 8);
    drawText(10.0f, 49.0f, "GL_LINES");

    const Point2D lineStripExample[] = {
        {43.0f, 68.0f}, {62.0f, 56.0f}, {56.5f, 62.0f}, {47.0f, 59.0f}, {44.5f, 54.0f}
    };
    const LabelOffset lineStripLabels[] = {
        {-1.6f, 0.4f},
        {0.8f, -0.8f},
        {0.8f, 0.3f},
        {-1.5f, 0.4f},
        {-1.5f, -0.7f}
    };
    drawPrimitive(lineStripExample, 5, GL_LINE_STRIP);
    drawVertexMarkers(lineStripExample, lineStripLabels, 5);
    drawText(44.0f, 49.0f, "GL_LINE_STRIP");

    const Point2D lineLoopExample[] = {
        {77.0f, 60.0f}, {81.0f, 48.0f}, {95.0f, 48.0f}, {90.0f, 60.0f}, {86.0f, 52.0f}, {82.0f, 63.5f}
    };
    const LabelOffset lineLoopLabels[] = {
        {-1.8f, 0.2f},
        {-1.3f, -0.6f},
        {0.7f, -0.6f},
        {0.8f, 0.2f},
        {-0.5f, 0.3f},
        {0.8f, 0.3f}
    };
    drawPrimitive(lineLoopExample, 6, GL_LINE_LOOP);
    drawVertexMarkers(lineLoopExample, lineLoopLabels, 6);
    drawText(79.0f, 39.0f, "GL_LINE_LOOP");

    const Point2D trianglesExample[] = {
        {7.0f, 25.5f}, {7.0f, 36.0f}, {16.0f, 25.5f},
        {26.5f, 30.5f}, {18.0f, 43.5f}, {35.5f, 43.5f}
    };
    const LabelOffset trianglesLabels[] = {
        {-1.5f, -0.5f},
        {-1.6f, 0.3f},
        {0.7f, -0.5f},
        {-0.2f, -1.1f},
        {-1.7f, 0.3f},
        {0.7f, 0.3f}
    };
    glColor3f(0.85f, 0.85f, 0.85f);
    drawPrimitive(trianglesExample, 6, GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawPrimitive(trianglesExample, 3, GL_LINE_LOOP);
    drawPrimitive(trianglesExample + 3, 3, GL_LINE_LOOP);
    drawVertexMarkers(trianglesExample, trianglesLabels, 6);
    drawText(9.0f, 15.5f, "GL_TRIANGLES");

    const Point2D triangleStripExample[] = {
        {42.0f, 43.0f}, {42.0f, 28.0f}, {52.0f, 43.0f},
        {60.0f, 28.0f}, {68.0f, 43.0f}, {70.0f, 21.0f}
    };
    const LabelOffset triangleStripLabels[] = {
        {-1.7f, 0.4f},
        {-1.4f, -0.8f},
        {-0.3f, 0.7f},
        {-0.3f, -1.0f},
        {0.7f, 0.4f},
        {0.8f, -0.8f}
    };
    drawFilledPrimitive(triangleStripExample, 6, GL_TRIANGLE_STRIP);
    drawVertexMarkers(triangleStripExample, triangleStripLabels, 6);
    drawText(39.0f, 15.5f, "GL_TRIANGLE_STRIP");

    const Point2D triangleFanExample[] = {
        {78.0f, 29.0f}, {78.0f, 43.0f}, {87.0f, 39.0f}, {88.5f, 31.5f}, {87.5f, 21.5f}
    };
    const LabelOffset triangleFanLabels[] = {
        {-1.3f, -0.5f},
        {-1.3f, 0.4f},
        {0.7f, 0.4f},
        {0.8f, -0.1f},
        {0.7f, -0.8f}
    };
    glColor3f(0.85f, 0.85f, 0.85f);
    drawPrimitive(triangleFanExample, 5, GL_TRIANGLE_FAN);
    glColor3f(0.0f, 0.0f, 0.0f);
    const Point2D triangleFanOuter[] = {
        triangleFanExample[1], triangleFanExample[2], triangleFanExample[3], triangleFanExample[4], triangleFanExample[0]
    };
    drawPrimitive(triangleFanOuter, 5, GL_LINE_LOOP);
    const Point2D triangleFanSpokes[] = {
        triangleFanExample[0], triangleFanExample[2],
        triangleFanExample[0], triangleFanExample[3]
    };
    drawPrimitive(triangleFanSpokes, 4, GL_LINES);
    drawVertexMarkers(triangleFanExample, triangleFanLabels, 5);
    drawText(75.0f, 15.5f, "GL_TRIANGLE_FAN");

    const Point2D quadsExample[] = {
        {5.5f, 0.5f}, {8.8f, 7.5f}, {18.4f, 8.5f}, {18.4f, 1.5f},
        {27.0f, -0.5f}, {28.0f, 7.5f}, {39.0f, 13.0f}, {34.5f, -0.5f}
    };
    const LabelOffset quadsLabels[] = {
        {-1.6f, -0.2f},
        {-1.6f, 0.2f},
        {0.7f, 0.2f},
        {0.7f, -0.4f},
        {-1.5f, -0.6f},
        {-1.6f, 0.2f},
        {0.7f, 0.2f},
        {0.7f, -0.4f}
    };
    glColor3f(0.85f, 0.85f, 0.85f);
    drawPrimitive(quadsExample, 8, GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawPrimitive(quadsExample, 4, GL_LINE_LOOP);
    drawPrimitive(quadsExample + 4, 4, GL_LINE_LOOP);
    drawVertexMarkers(quadsExample, quadsLabels, 8);
    drawText(12.0f, -3.8f, "GL_QUADS");

    const Point2D quadStripExample[] = {
        {42.5f, 1.5f}, {41.5f, 12.0f}, {51.5f, 1.5f}, {51.5f, 13.0f},
        {60.0f, 3.2f}, {56.8f, 14.5f}, {69.2f, 4.5f}, {67.8f, 16.0f}
    };
    const LabelOffset quadStripLabels[] = {
        {-1.3f, -0.5f},
        {-1.3f, 0.2f},
        {-0.2f, -0.8f},
        {-0.2f, 0.3f},
        {-0.3f, -0.9f},
        {-0.2f, 0.4f},
        {0.8f, -0.5f},
        {0.8f, 0.3f}
    };
    glColor3f(0.85f, 0.85f, 0.85f);
    drawPrimitive(quadStripExample, 8, GL_QUAD_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    const Point2D quadStripOuter[] = {
        quadStripExample[1], quadStripExample[0], quadStripExample[2], quadStripExample[4],
        quadStripExample[6], quadStripExample[7], quadStripExample[5], quadStripExample[3]
    };
    drawPrimitive(quadStripOuter, 8, GL_LINE_LOOP);
    const Point2D quadStripInnerLeft[] = {quadStripExample[3], quadStripExample[2]};
    const Point2D quadStripInnerRight[] = {quadStripExample[5], quadStripExample[4]};
    drawPrimitive(quadStripInnerLeft, 2, GL_LINES);
    drawPrimitive(quadStripInnerRight, 2, GL_LINES);
    drawVertexMarkers(quadStripExample, quadStripLabels, 8);
    drawText(43.5f, -3.8f, "GL_QUAD_STRIP");

    const Point2D polygonExample[] = {
        {80.5f, 7.0f}, {90.0f, 9.3f}, {86.8f, 2.3f}, {78.5f, -1.5f}, {76.3f, 3.9f}
    };
    const LabelOffset polygonLabels[] = {
        {-1.5f, 0.3f},
        {0.7f, 0.3f},
        {0.7f, -0.2f},
        {-0.2f, -0.8f},
        {-1.6f, -0.1f}
    };
    drawFilledPrimitive(polygonExample, 5, GL_POLYGON);
    drawVertexMarkers(polygonExample, polygonLabels, 5);
    drawText(78.5f, -3.8f, "GL_POLYGON");

    glFlush();
}

void reshape(int width, int height) {
    updateProjection(width, height);
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
    glutSpecialFunc(handleSpecialKeys);
    glutMouseFunc(handleMouse);
    glutMainLoop();
    return 0;
}
