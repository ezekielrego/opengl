#include <graphics.h>
#include <conio.h>
#include <cstring>

// Great Zimbabwe University
// Computer Graphics Practical Assignment - Part E
// Image creation using the graphics.h library in C/C++.

const int WINDOW_WIDTH = 1180;
const int WINDOW_HEIGHT = 760;
const char* OVERVIEW_OUTPUT_IMAGE = "part_e_graphics_h_output.bmp";
const char* REFERENCE_OUTPUT_IMAGE = "part_e_all_primitives.bmp";
const int FOOTER_TOP = WINDOW_HEIGHT - 38;
const int NAV_BUTTON_WIDTH = 104;
const int NAV_BUTTON_HEIGHT = 32;
const int NAV_BUTTON_LEFT = WINDOW_WIDTH - NAV_BUTTON_WIDTH - 14;
const int NAV_BUTTON_TOP = 12;
const int NAV_BUTTON_RIGHT = NAV_BUTTON_LEFT + NAV_BUTTON_WIDTH;
const int NAV_BUTTON_BOTTOM = NAV_BUTTON_TOP + NAV_BUTTON_HEIGHT;

void drawText(int x, int y, const char* text) {
    outtextxy(x, y, const_cast<char*>(text));
}

void drawCenteredText(int left, int right, int y, const char* text) {
    int width = textwidth(const_cast<char*>(text));
    drawText(left + (right - left - width) / 2, y, text);
}

void drawClosedPolygon(const int points[], int count) {
    for (int i = 0; i < count; ++i) {
        int next = (i + 1) % count;
        line(points[i * 2], points[i * 2 + 1],
             points[next * 2], points[next * 2 + 1]);
    }
}

void fillClosedPolygon(const int points[], int count, int fillColor, int borderColor) {
    int localPoints[64];
    for (int i = 0; i < count * 2; ++i) {
        localPoints[i] = points[i];
    }

    setfillstyle(SOLID_FILL, fillColor);
    fillpoly(count, localPoints);
    setcolor(borderColor);
    drawClosedPolygon(points, count);
}

void drawReferenceLabel(int x, int y, const char* text) {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    drawText(x, y, text);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

void drawReferenceTitle(int left, int right, int y, const char* text) {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    drawCenteredText(left, right, y, text);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

void drawReferencePoint(int x, int y, const char* label, int labelX, int labelY) {
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x, y, 5, 5);
    drawReferenceLabel(labelX, labelY, label);
}

void drawNavButton(const char* label, int textBackgroundColor) {
    setcolor(BLACK);
    rectangle(NAV_BUTTON_LEFT, NAV_BUTTON_TOP, NAV_BUTTON_RIGHT, NAV_BUTTON_BOTTOM);
    line(NAV_BUTTON_LEFT + 2, NAV_BUTTON_BOTTOM + 1, NAV_BUTTON_RIGHT + 1, NAV_BUTTON_BOTTOM + 1);
    line(NAV_BUTTON_RIGHT + 1, NAV_BUTTON_TOP + 2, NAV_BUTTON_RIGHT + 1, NAV_BUTTON_BOTTOM + 1);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setbkcolor(textBackgroundColor);
    int labelWidth = textwidth(const_cast<char*>(label));
    int labelHeight = textheight(const_cast<char*>(label));
    drawText(NAV_BUTTON_LEFT + (NAV_BUTTON_WIDTH - labelWidth) / 2,
             NAV_BUTTON_TOP + (NAV_BUTTON_HEIGHT - labelHeight) / 2,
             label);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

bool isInsideNavButton(int x, int y) {
    return x >= NAV_BUTTON_LEFT && x <= NAV_BUTTON_RIGHT &&
           y >= NAV_BUTTON_TOP && y <= NAV_BUTTON_BOTTOM;
}

void drawCloud(int x, int y, int scale) {
    setfillstyle(SOLID_FILL, WHITE);
    setcolor(LIGHTGRAY);
    fillellipse(x - 22 * scale, y + 7 * scale, 24 * scale, 13 * scale);
    fillellipse(x, y, 30 * scale, 17 * scale);
    fillellipse(x + 28 * scale, y + 8 * scale, 25 * scale, 13 * scale);
}

void drawTree(int x, int y, int scale) {
    setfillstyle(SOLID_FILL, BROWN);
    bar(x - 4 * scale, y - 25 * scale, x + 4 * scale, y);

    setfillstyle(SOLID_FILL, GREEN);
    setcolor(GREEN);
    fillellipse(x, y - 38 * scale, 18 * scale, 22 * scale);
    fillellipse(x - 14 * scale, y - 25 * scale, 15 * scale, 17 * scale);
    fillellipse(x + 14 * scale, y - 25 * scale, 15 * scale, 17 * scale);
}

void drawPanelFrame(int x, int y, int width, int height, const char* title) {
    setfillstyle(SOLID_FILL, WHITE);
    bar(x, y, x + width, y + height);

    setcolor(BLACK);
    rectangle(x, y, x + width, y + height);

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x + 1, y + 1, x + width - 1, y + 24);
    setcolor(BLACK);
    rectangle(x + 1, y + 1, x + width - 1, y + 24);
    drawCenteredText(x, x + width, y + 8, title);
}

void drawLandscapeImage(int x, int y, int width, int height) {
    drawPanelFrame(x, y, width, height, "Image 1: Landscape");

    int top = y + 25;
    int bottom = y + height - 1;
    int ground = y + height - 86;

    // Sky with gradient effect using lines
    for (int yy = top; yy < ground; ++yy) {
        int blue = 206 + (yy - top) * 49 / (ground - top);
        setcolor(COLOR(135, 206, blue));
        line(x + 1, yy, x + width - 1, yy);
    }
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(x + 1, ground, x + width - 1, bottom);

    // Sun with rays
    setfillstyle(SOLID_FILL, YELLOW);
    setcolor(YELLOW);
    fillellipse(x + width - 85, top + 44, 35, 35);
    setcolor(YELLOW);
    for (int i = 0; i < 16; ++i) {
        int dx = (i % 4 - 1) * 25;
        int dy = (i / 4 - 1) * 22;
        line(x + width - 85, top + 44, x + width - 85 + dx, top + 44 + dy);
    }

    // More clouds
    drawCloud(x + 112, top + 56, 1);
    drawCloud(x + width - 205, top + 78, 1);
    drawCloud(x + 50, top + 30, 0.8);
    drawCloud(x + width - 100, top + 20, 1.2);

    // Mountains with more peaks
    int mountain1[] = {
        x + 40, ground,
        x + 120, top + 70,
        x + 200, top + 50,
        x + 280, top + 80,
        x + 292, ground
    };
    int mountain2[] = {
        x + 225, ground,
        x + 300, top + 60,
        x + 380, top + 40,
        x + 450, top + 70,
        x + width - 40, ground
    };
    fillClosedPolygon(mountain1, 5, DARKGRAY, BLACK);
    fillClosedPolygon(mountain2, 5, LIGHTGRAY, BLACK);

    // Snow caps
    int snow1[] = {
        x + 110, top + 75,
        x + 120, top + 70,
        x + 130, top + 78
    };
    int snow2[] = {
        x + 290, top + 85,
        x + 300, top + 60,
        x + 310, top + 88
    };
    int snow3[] = {
        x + 370, top + 45,
        x + 380, top + 40,
        x + 390, top + 50
    };
    fillClosedPolygon(snow1, 3, WHITE, WHITE);
    fillClosedPolygon(snow2, 3, WHITE, WHITE);
    fillClosedPolygon(snow3, 3, WHITE, WHITE);

    // River with waves
    int river[] = {
        x + 240, ground,
        x + 312, ground,
        x + 365, bottom,
        x + 159, bottom
    };
    fillClosedPolygon(river, 4, CYAN, BLUE);
    setcolor(BLUE);
    for (int rx = x + 180; rx < x + 350; rx += 20) {
        arc(rx, ground + 10, 0, 180, 10);
    }

    // More trees
    drawTree(x + 70, bottom - 14, 2);
    drawTree(x + width - 86, bottom - 10, 2);
    drawTree(x + width - 150, bottom - 20, 1);
    drawTree(x + 150, bottom - 18, 1.5);
    drawTree(x + width - 200, bottom - 12, 1);

    // Add birds
    setcolor(BLACK);
    for (int bx = x + 50; bx < x + width - 50; bx += 100) {
        int by = top + 20 + (bx % 40);
        line(bx - 5, by, bx + 5, by);
        line(bx - 3, by - 3, bx + 3, by + 3);
        line(bx + 3, by - 3, bx - 3, by + 3);
    }

    setcolor(BLACK);
}

void drawCityImage(int x, int y, int width, int height) {
    drawPanelFrame(x, y, width, height, "Image 2: City Road");

    int top = y + 25;
    int bottom = y + height - 1;
    int horizon = y + height - 92;

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(x + 1, top, x + width - 1, horizon);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(x + 1, horizon, x + width - 1, horizon + 26);

    // More buildings with varying heights
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x + 48, top + 72, x + 124, horizon);
    bar(x + 140, top + 42, x + 224, horizon);
    bar(x + width - 214, top + 55, x + width - 130, horizon);
    bar(x + width - 112, top + 82, x + width - 42, horizon);
    bar(x + 20, top + 90, x + 40, horizon); // Small building
    bar(x + width - 30, top + 70, x + width - 10, horizon); // Another

    // Windows with some lit
    setfillstyle(SOLID_FILL, YELLOW);
    for (int bx = x + 60; bx < x + 220; bx += 32) {
        for (int by = top + 84; by < horizon - 12; by += 28) {
            if ((bx + by) % 2 == 0) bar(bx, by, bx + 12, by + 13);
        }
    }
    for (int bx = x + width - 196; bx < x + width - 55; bx += 32) {
        for (int by = top + 86; by < horizon - 12; by += 28) {
            if ((bx + by) % 3 == 0) bar(bx, by, bx + 12, by + 13);
        }
    }

    // Road with lane markings
    int road[] = {
        x + 180, horizon,
        x + width - 180, horizon,
        x + width - 42, bottom,
        x + 42, bottom
    };
    fillClosedPolygon(road, 4, DARKGRAY, BLACK);

    setcolor(WHITE);
    for (int yy = horizon + 12; yy < bottom - 20; yy += 36) {
        line(x + width / 2, yy, x + width / 2, yy + 20);
        // Add side lines
        line(x + 60, yy, x + 60, yy + 20);
        line(x + width - 60, yy, x + width - 60, yy + 20);
    }

    // Enhanced car
    setfillstyle(SOLID_FILL, RED);
    bar(x + 170, bottom - 74, x + 304, bottom - 38);
    fillellipse(x + 235, bottom - 76, 45, 22);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(x + 212, bottom - 94, x + 266, bottom - 72);
    // Windshield
    setfillstyle(SOLID_FILL, CYAN);
    bar(x + 220, bottom - 90, x + 250, bottom - 78);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x + 195, bottom - 35, 15, 15);
    fillellipse(x + 280, bottom - 35, 15, 15);
    // Headlights
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(x + 170, bottom - 50, 5, 5);
    fillellipse(x + 304, bottom - 50, 5, 5);

    // Traffic light with signals
    setfillstyle(SOLID_FILL, BLACK);
    bar(x + width - 92, horizon - 82, x + width - 72, horizon - 20);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x + width - 82, horizon - 72, 6, 6);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(x + width - 82, horizon - 52, 6, 6);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(x + width - 82, horizon - 32, 6, 6);
    setcolor(BLACK);
    line(x + width - 82, horizon - 20, x + width - 82, horizon + 30);
}

void drawStudentImage(int x, int y, int width, int height) {
    drawPanelFrame(x, y, width, height, "Image 3: Cartoon Student");

    int top = y + 25;
    int bottom = y + height - 1;
    int ground = y + height - 68;
    int cx = x + width / 2;

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(x + 1, top, x + width - 1, ground);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(x + 1, ground, x + width - 1, bottom);

    // Building with door
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x + 48, top + 102, x + 178, ground);
    setcolor(BLACK);
    rectangle(x + 48, top + 102, x + 178, ground);
    drawText(x + 70, top + 118, "GZU LAB");
    rectangle(x + 100, ground - 20, x + 120, ground); // Door

    // Student body
    setfillstyle(SOLID_FILL, BROWN);
    bar(cx - 58, ground - 96, cx + 58, ground - 16);
    // Shirt
    setfillstyle(SOLID_FILL, BLUE);
    bar(cx - 70, ground - 118, cx + 70, ground - 64);
    setcolor(BLACK);
    rectangle(cx - 70, ground - 118, cx + 70, ground - 64);

    // Head
    setfillstyle(SOLID_FILL, BROWN);
    fillellipse(cx, ground - 165, 46, 55);
    // Hair
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(cx, ground - 205, 48, 20);
    // Eyes
    fillellipse(cx - 38, ground - 177, 10, 23);
    fillellipse(cx + 38, ground - 177, 10, 23);

    // Eye whites and pupils
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(cx - 18, ground - 168, 10, 7);
    fillellipse(cx + 18, ground - 168, 10, 7);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(cx - 18, ground - 168, 3, 3);
    fillellipse(cx + 18, ground - 168, 3, 3);
    // Smile
    setcolor(BLACK);
    arc(cx, ground - 151, 200, 340, 18);

    // Arms
    line(cx - 72, ground - 105, cx - 116, ground - 56);
    line(cx + 72, ground - 105, cx + 116, ground - 56);
    // Hands
    setfillstyle(SOLID_FILL, BROWN);
    fillellipse(cx - 118, ground - 54, 9, 9);
    fillellipse(cx + 118, ground - 54, 9, 9);

    // Legs
    setfillstyle(SOLID_FILL, BLUE);
    bar(cx - 52, ground - 48, cx + 52, ground - 8);
    setcolor(BLACK);
    rectangle(cx - 52, ground - 48, cx + 52, ground - 8);
    line(cx, ground - 48, cx, ground - 8);
    // Shoes
    setfillstyle(SOLID_FILL, BLACK);
    bar(cx - 60, ground - 8, cx - 44, bottom - 8);
    bar(cx + 44, ground - 8, cx + 60, bottom - 8);

    // Backpack
    setfillstyle(SOLID_FILL, RED);
    bar(cx + 58, ground - 118, cx + 78, ground - 64);
    setcolor(BLACK);
    rectangle(cx + 58, ground - 118, cx + 78, ground - 64);
    // Strap
    line(cx + 58, ground - 105, cx + 70, ground - 105);

    // Book in hand
    setfillstyle(SOLID_FILL, GREEN);
    bar(cx - 130, ground - 70, cx - 110, ground - 50);
    setcolor(BLACK);
    rectangle(cx - 130, ground - 70, cx - 110, ground - 50);

    // ID badge
    setfillstyle(SOLID_FILL, WHITE);
    bar(cx - 52, ground - 48, cx + 52, ground - 28);
    setcolor(BLACK);
    rectangle(cx - 52, ground - 48, cx + 52, ground - 28);
    drawText(cx - 38, ground - 34, "M230913");
}

void drawPrimitiveImage(int x, int y, int width, int height) {
    drawPanelFrame(x, y, width, height, "Image 4: graphics.h Primitives");

    int top = y + 25;
    int left = x + 1;
    int right = x + width - 1;
    int bottom = y + height - 1;

    setfillstyle(SOLID_FILL, WHITE);
    bar(left, top, right, bottom);

    setcolor(BLACK);
    line(x + 42, top + 48, x + 180, top + 86);
    drawText(x + 62, top + 94, "line()");

    rectangle(x + 230, top + 35, x + 355, top + 112);
    drawText(x + 246, top + 123, "rectangle()");

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(x + 405, top + 42, x + 522, top + 108);
    setcolor(BLACK);
    rectangle(x + 405, top + 42, x + 522, top + 108);
    drawText(x + 438, top + 123, "bar()");

    setfillstyle(SOLID_FILL, YELLOW);
    setcolor(BLACK);
    fillellipse(x + 92, top + 196, 44, 44);
    circle(x + 92, top + 196, 44);
    drawText(x + 65, top + 246, "circle()");

    setfillstyle(SOLID_FILL, LIGHTMAGENTA);
    fillellipse(x + 272, top + 194, 70, 36);
    ellipse(x + 272, top + 194, 0, 360, 70, 36);
    drawText(x + 242, top + 246, "ellipse()");

    int poly[] = {
        x + 425, top + 170,
        x + 516, top + 170,
        x + 538, top + 226,
        x + 472, top + 260,
        x + 404, top + 224
    };
    fillClosedPolygon(poly, 5, LIGHTGREEN, BLACK);
    drawText(x + 438, top + 246, "fillpoly()");

    setfillstyle(SOLID_FILL, LIGHTRED);
    pieslice(x + 180, top + 192, 30, 310, 36);
    drawText(x + 157, top + 246, "pieslice()");

    setcolor(BLUE);
    arc(x + 360, top + 203, 20, 170, 42);
    setcolor(BLACK);
    drawText(x + 350, top + 246, "arc()");
}

void drawGLPointsCell(int x, int y, int width) {
    drawReferencePoint(x + 135, y + 22, "v0", x + 95, y + 14);
    drawReferencePoint(x + 160, y + 58, "v1", x + 125, y + 50);
    drawReferencePoint(x + 220, y + 58, "v2", x + 232, y + 50);
    drawReferencePoint(x + 245, y + 22, "v3", x + 258, y + 14);
    drawReferencePoint(x + 270, y + 3, "v4", x + 283, y - 5);
    drawReferenceTitle(x, x + width, y + 102, "GL_POINTS");
}

void drawGLLinesCell(int x, int y, int width) {
    setcolor(BLACK);
    line(x + 55, y + 72, x + 185, y + 14);
    line(x + 190, y + 58, x + 340, y + 84);
    line(x + 112, y + 92, x + 280, y + 124);
    line(x + 86, y + 126, x + 282, y + 104);

    drawReferenceLabel(x + 18, y + 66, "v0");
    drawReferenceLabel(x + 190, y + 8, "v1");
    drawReferenceLabel(x + 158, y + 50, "v2");
    drawReferenceLabel(x + 345, y + 78, "v3");
    drawReferenceLabel(x + 88, y + 84, "v4");
    drawReferenceLabel(x + 285, y + 116, "v5");
    drawReferenceLabel(x + 58, y + 118, "v6");
    drawReferenceLabel(x + 288, y + 96, "v7");
    drawReferenceTitle(x, x + width, y + 138, "GL_LINES");
}

void drawGLLineStripCell(int x, int y, int width) {
    int vx[] = {x + 78, x + 245, x + 214, x + 110, x + 96};
    int vy[] = {y + 62, y + 112, y + 66, y + 84, y + 112};

    setcolor(BLACK);
    for (int i = 0; i < 4; ++i) {
        line(vx[i], vy[i], vx[i + 1], vy[i + 1]);
    }

    drawReferenceLabel(x + 50, y + 55, "v0");
    drawReferenceLabel(x + 250, y + 104, "v1");
    drawReferenceLabel(x + 220, y + 58, "v2");
    drawReferenceLabel(x + 82, y + 78, "v3");
    drawReferenceLabel(x + 70, y + 102, "v4");
    drawReferenceTitle(x, x + width, y + 138, "GL_LINE_STRIP");
}

void drawGLLineLoopCell(int x, int y, int width) {
    int pts[] = {
        x + 88, y + 50,
        x + 135, y + 124,
        x + 286, y + 124,
        x + 246, y + 50,
        x + 204, y + 101,
        x + 166, y + 18
    };

    setcolor(BLACK);
    drawClosedPolygon(pts, 6);

    drawReferenceLabel(x + 58, y + 44, "v0");
    drawReferenceLabel(x + 104, y + 128, "v1");
    drawReferenceLabel(x + 294, y + 120, "v2");
    drawReferenceLabel(x + 250, y + 38, "v3");
    drawReferenceLabel(x + 190, y + 86, "v4");
    drawReferenceLabel(x + 170, y + 8, "v5");
    drawReferenceTitle(x, x + width, y + 138, "GL_LINE_LOOP");
}

void drawGLTrianglesCell(int x, int y, int width) {
    int triangle1[] = {
        x + 72, y + 107,
        x + 72, y + 42,
        x + 144, y + 107
    };
    int triangle2[] = {
        x + 184, y + 32,
        x + 332, y + 32,
        x + 238, y + 120
    };

    fillClosedPolygon(triangle1, 3, LIGHTGRAY, BLACK);
    fillClosedPolygon(triangle2, 3, LIGHTGRAY, BLACK);

    drawReferenceLabel(x + 32, y + 100, "v0");
    drawReferenceLabel(x + 40, y + 34, "v1");
    drawReferenceLabel(x + 150, y + 102, "v2");
    drawReferenceLabel(x + 224, y + 122, "v3");
    drawReferenceLabel(x + 148, y + 24, "v4");
    drawReferenceLabel(x + 338, y + 24, "v5");
    drawReferenceTitle(x, x + width, y + 138, "GL_TRIANGLES");
}

void drawGLTriangleStripCell(int x, int y, int width) {
    int strip[] = {
        x + 92, y + 38,
        x + 212, y + 38,
        x + 318, y + 38,
        x + 338, y + 125,
        x + 260, y + 118,
        x + 92, y + 118
    };

    fillClosedPolygon(strip, 6, LIGHTGRAY, BLACK);

    setcolor(BLACK);
    line(x + 92, y + 118, x + 212, y + 38);
    line(x + 212, y + 38, x + 260, y + 118);
    line(x + 260, y + 118, x + 318, y + 38);

    drawReferenceLabel(x + 60, y + 30, "v0");
    drawReferenceLabel(x + 60, y + 112, "v1");
    drawReferenceLabel(x + 198, y + 24, "v2");
    drawReferenceLabel(x + 244, y + 122, "v3");
    drawReferenceLabel(x + 322, y + 28, "v4");
    drawReferenceLabel(x + 344, y + 118, "v5");
    drawReferenceTitle(x, x + width, y + 138, "GL_TRIANGLE_STRIP");
}

void drawGLTriangleFanCell(int x, int y, int width) {
    int fan[] = {
        x + 104, y + 100,
        x + 124, y + 30,
        x + 224, y + 58,
        x + 252, y + 104,
        x + 246, y + 132
    };

    fillClosedPolygon(fan, 5, LIGHTGRAY, BLACK);

    setcolor(BLACK);
    line(x + 104, y + 100, x + 224, y + 58);
    line(x + 104, y + 100, x + 252, y + 104);

    drawReferenceLabel(x + 72, y + 94, "v0");
    drawReferenceLabel(x + 92, y + 18, "v1");
    drawReferenceLabel(x + 228, y + 48, "v2");
    drawReferenceLabel(x + 258, y + 96, "v3");
    drawReferenceLabel(x + 252, y + 124, "v4");
    drawReferenceTitle(x, x + width, y + 138, "GL_TRIANGLE_FAN");
}

void drawGLQuadsCell(int x, int y, int width) {
    int quad1[] = {
        x + 52, y + 96,
        x + 88, y + 28,
        x + 184, y + 16,
        x + 184, y + 96
    };
    int quad2[] = {
        x + 236, y + 112,
        x + 250, y + 44,
        x + 342, y + 8,
        x + 316, y + 104
    };

    fillClosedPolygon(quad1, 4, LIGHTGRAY, BLACK);
    fillClosedPolygon(quad2, 4, LIGHTGRAY, BLACK);

    drawReferenceLabel(x + 18, y + 88, "v0");
    drawReferenceLabel(x + 52, y + 18, "v1");
    drawReferenceLabel(x + 190, y + 8, "v2");
    drawReferenceLabel(x + 188, y + 94, "v3");
    drawReferenceLabel(x + 202, y + 106, "v4");
    drawReferenceLabel(x + 210, y + 34, "v5");
    drawReferenceLabel(x + 348, y + 0, "v6");
    drawReferenceLabel(x + 320, y + 102, "v7");
    drawReferenceTitle(x, x + width, y + 138, "GL_QUADS");
}

void drawGLQuadStripCell(int x, int y, int width) {
    int strip[] = {
        x + 70, y + 104,
        x + 64, y + 36,
        x + 144, y + 30,
        x + 206, y + 20,
        x + 318, y + 8,
        x + 326, y + 82,
        x + 236, y + 92,
        x + 146, y + 104
    };

    fillClosedPolygon(strip, 8, LIGHTGRAY, BLACK);

    setcolor(BLACK);
    line(x + 146, y + 104, x + 144, y + 30);
    line(x + 236, y + 92, x + 206, y + 20);

    drawReferenceLabel(x + 42, y + 96, "v0");
    drawReferenceLabel(x + 34, y + 28, "v1");
    drawReferenceLabel(x + 142, y + 100, "v2");
    drawReferenceLabel(x + 134, y + 22, "v3");
    drawReferenceLabel(x + 230, y + 92, "v4");
    drawReferenceLabel(x + 196, y + 12, "v5");
    drawReferenceLabel(x + 330, y + 78, "v6");
    drawReferenceLabel(x + 320, y + 0, "v7");
    drawReferenceTitle(x, x + width, y + 138, "GL_QUAD_STRIP");
}

void drawGLPolygonCell(int x, int y, int width) {
    int polygon[] = {
        x + 114, y + 42,
        x + 250, y + 22,
        x + 220, y + 88,
        x + 96, y + 124,
        x + 66, y + 74
    };

    fillClosedPolygon(polygon, 5, LIGHTGRAY, BLACK);

    drawReferenceLabel(x + 84, y + 34, "v0");
    drawReferenceLabel(x + 254, y + 16, "v1");
    drawReferenceLabel(x + 224, y + 82, "v2");
    drawReferenceLabel(x + 88, y + 124, "v3");
    drawReferenceLabel(x + 38, y + 68, "v4");
    drawReferenceTitle(x, x + width, y + 138, "GL_POLYGON");
}

void drawReferenceHeader() {
    setfillstyle(SOLID_FILL, WHITE);
    bar(0, 0, WINDOW_WIDTH, 56);
    setcolor(BLACK);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    drawCenteredText(0, WINDOW_WIDTH, 10, "PART E - graphics.h RECREATION OF images/all.png");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    drawCenteredText(0, WINDOW_WIDTH, 38, "OpenGL primitive reference recreated with BGI line(), circle(), fillpoly(), rectangle(), and text calls");
}

void drawReferenceFooter(bool saved) {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, FOOTER_TOP, WINDOW_WIDTH, WINDOW_HEIGHT);
    setcolor(BLACK);
    rectangle(0, FOOTER_TOP, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);

    if (saved) {
        drawCenteredText(0, WINDOW_WIDTH, WINDOW_HEIGHT - 25, "Saved part_e_all_primitives.bmp. Click More for the other drawings, S saves again, Q/Esc closes.");
    } else {
        drawCenteredText(0, WINDOW_WIDTH, WINDOW_HEIGHT - 25, "Recreated from images/all.png. Click More for the other drawings, S saves, Q/Esc closes.");
    }
    drawNavButton("More", WHITE);
}

void drawAllPrimitiveReference(bool saved = false) {
    setbkcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    cleardevice();
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

    drawReferenceHeader();

    const int colW = WINDOW_WIDTH / 3;
    const int rows[] = {70, 220, 380, 545};

    drawGLPointsCell(0, rows[0], colW);

    drawGLLinesCell(0, rows[1], colW);
    drawGLLineStripCell(colW, rows[1], colW);
    drawGLLineLoopCell(colW * 2, rows[1], colW);

    drawGLTrianglesCell(0, rows[2], colW);
    drawGLTriangleStripCell(colW, rows[2], colW);
    drawGLTriangleFanCell(colW * 2, rows[2], colW);

    drawGLQuadsCell(0, rows[3], colW);
    drawGLQuadStripCell(colW, rows[3], colW);
    drawGLPolygonCell(colW * 2, rows[3], colW);

    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    drawReferenceFooter(saved);
}

void drawHeader() {
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(0, 0, WINDOW_WIDTH, 56);
    setcolor(BLACK);
    rectangle(0, 0, WINDOW_WIDTH - 1, 56);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    drawCenteredText(0, WINDOW_WIDTH, 10, "PART E - IMAGE CREATION USING graphics.h");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    drawCenteredText(0, WINDOW_WIDTH, 38, "C/C++ BGI graphics primitives: line, rectangle, circle, ellipse, arc, pieslice, bar, fillpoly and text");
}

void drawFooter(bool saved) {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, FOOTER_TOP, WINDOW_WIDTH, WINDOW_HEIGHT);
    setcolor(BLACK);
    rectangle(0, FOOTER_TOP, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);

    if (saved) {
        drawCenteredText(0, WINDOW_WIDTH, WINDOW_HEIGHT - 25, "Saved part_e_graphics_h_output.bmp. Click All to return, S saves again, Q/Esc closes.");
    } else {
        drawCenteredText(0, WINDOW_WIDTH, WINDOW_HEIGHT - 25, "Other Part E drawings. Click All to return, S saves this page, Q/Esc closes. Student ID: M230913");
    }
    drawNavButton("All", LIGHTBLUE);
}

void drawOverview(bool saved = false) {
    setbkcolor(WHITE);
    cleardevice();
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    drawHeader();

    const int margin = 22;
    const int gap = 18;
    const int panelW = (WINDOW_WIDTH - margin * 2 - gap) / 2;
    const int panelH = 300;
    const int row1 = 72;
    const int row2 = row1 + panelH + gap;

    drawLandscapeImage(margin, row1, panelW, panelH);
    drawCityImage(margin + panelW + gap, row1, panelW, panelH);
    drawStudentImage(margin, row2, panelW, panelH);
    drawPrimitiveImage(margin + panelW + gap, row2, panelW, panelH);

    drawFooter(saved);
}

void saveOverviewImage() {
    drawOverview(false);
    writeimagefile(const_cast<char*>(OVERVIEW_OUTPUT_IMAGE), 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
    drawOverview(true);
}

void saveReferenceImage() {
    drawAllPrimitiveReference(false);
    writeimagefile(const_cast<char*>(REFERENCE_OUTPUT_IMAGE), 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
    drawAllPrimitiveReference(true);
}

void drawCurrentPage(bool referencePage, bool saved = false) {
    if (referencePage) {
        drawAllPrimitiveReference(saved);
    } else {
        drawOverview(saved);
    }
}

void saveCurrentPage(bool referencePage) {
    if (referencePage) {
        saveReferenceImage();
    } else {
        saveOverviewImage();
    }
}

bool hasOption(int argc, char* argv[], const char* option) {
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], option) == 0) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Part E - graphics.h Image Creation");

    if (hasOption(argc, argv, "--save-and-exit")) {
        saveOverviewImage();
        saveReferenceImage();
        delay(250);
        closegraph();
        return 0;
    }

    if (hasOption(argc, argv, "--save-reference-and-exit")) {
        saveReferenceImage();
        delay(250);
        closegraph();
        return 0;
    }

    bool referencePage = !hasOption(argc, argv, "--show-overview");
    drawCurrentPage(referencePage, false);

    bool running = true;
    while (running) {
        if (kbhit()) {
            int key = getch();
            if (key == 27 || key == 'q' || key == 'Q') {
                running = false;
            } else if (key == 's' || key == 'S') {
                saveCurrentPage(referencePage);
            } else if (key == 'a' || key == 'A') {
                referencePage = true;
                drawCurrentPage(referencePage, false);
            } else if (key == 'm' || key == 'M' || key == 'o' || key == 'O') {
                referencePage = false;
                drawCurrentPage(referencePage, false);
            }
        }

        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mouseX = 0;
            int mouseY = 0;
            getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);
            if (isInsideNavButton(mouseX, mouseY)) {
                referencePage = !referencePage;
                drawCurrentPage(referencePage, false);
            }
        }
        delay(20);
    }

    closegraph();
    return 0;
}
