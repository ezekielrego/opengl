#include <graphics.h>
#include <conio.h>
#include <cstring>
#include <cmath>
#include <windows.h>

// Separate Part E graphics.h scene.
// Replicates the supplied cartoon city/airplane reference as an animated drawing.

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 650;
const int FRAME_MS = 18;
const int JET_SPEED = 4;
const char* OUTPUT_IMAGE = "part_e_jet_city_animation_output.bmp";

void drawText(int x, int y, const char* text) {
    outtextxy(x, y, const_cast<char*>(text));
}

void fillClosedPolygon(const int points[], int count, int fillColor, int borderColor) {
    int localPoints[80];
    for (int i = 0; i < count * 2; ++i) {
        localPoints[i] = points[i];
    }

    setfillstyle(SOLID_FILL, fillColor);
    fillpoly(count, localPoints);
    setcolor(borderColor);
    for (int i = 0; i < count; ++i) {
        int next = (i + 1) % count;
        line(points[i * 2], points[i * 2 + 1],
             points[next * 2], points[next * 2 + 1]);
    }
}

void drawOutlinedRect(int left, int top, int right, int bottom, int fillColor) {
    setfillstyle(SOLID_FILL, fillColor);
    bar(left, top, right, bottom);
    setcolor(BLACK);
    rectangle(left, top, right, bottom);
}

void drawCloud(int x, int y, int scale) {
    int cloudColor = COLOR(248, 252, 247);
    setfillstyle(SOLID_FILL, cloudColor);
    setcolor(cloudColor);
    fillellipse(x - 42 * scale, y + 8 * scale, 31 * scale, 25 * scale);
    fillellipse(x - 12 * scale, y - 8 * scale, 34 * scale, 30 * scale);
    fillellipse(x + 24 * scale, y - 6 * scale, 37 * scale, 29 * scale);
    fillellipse(x + 58 * scale, y + 10 * scale, 31 * scale, 23 * scale);
    bar(x - 60 * scale, y + 8 * scale, x + 74 * scale, y + 35 * scale);
}

void drawSmallCloud(int x, int y) {
    int cloudColor = COLOR(248, 252, 247);
    setfillstyle(SOLID_FILL, cloudColor);
    setcolor(cloudColor);
    fillellipse(x - 22, y + 5, 18, 14);
    fillellipse(x, y - 6, 20, 17);
    fillellipse(x + 24, y + 3, 22, 14);
    bar(x - 34, y + 5, x + 40, y + 20);
}

void drawFlightDots(int jetX, int frame) {
    setfillstyle(SOLID_FILL, COLOR(245, 250, 242));
    setcolor(COLOR(245, 250, 242));

    for (int i = 0; i < 13; ++i) {
        int x = jetX - 360 + i * 32;
        int y = 190 - i * 8 + ((frame + i) % 2);
        if (x > 6 && x < WINDOW_WIDTH - 6 && y > 50 && y < 260) {
            fillellipse(x, y, 3, 3);
        }
    }
}

void drawWindowsGrid(int left, int top, int cols, int rows, int gapX, int gapY, int color) {
    setfillstyle(SOLID_FILL, color);
    setcolor(BLACK);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = left + col * gapX;
            int y = top + row * gapY;
            bar(x, y, x + 10, y + 22);
            rectangle(x, y, x + 10, y + 22);
        }
    }
}

void drawRoundWindows(int left, int top, int cols, int rows) {
    setfillstyle(SOLID_FILL, COLOR(243, 251, 180));
    setcolor(BLACK);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = left + col * 24;
            int y = top + row * 24;
            fillellipse(x, y, 5, 5);
            circle(x, y, 5);
        }
    }
}

void drawCity() {
    int base = WINDOW_HEIGHT - 1;

    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

    int leftTower[] = {0, 512, 25, 512, 31, 405, 38, 405, 43, 512, 62, 512, 62, base, 0, base};
    fillClosedPolygon(leftTower, 8, COLOR(239, 64, 45), BLACK);
    setfillstyle(SOLID_FILL, COLOR(242, 253, 184));
    bar(21, 548, 33, 631);
    setcolor(BLACK);
    rectangle(21, 548, 33, 631);
    line(32, 405, 36, 405);

    int redRoof[] = {52, base, 52, 442, 84, 442, 117, 520, 117, base};
    fillClosedPolygon(redRoof, 5, COLOR(246, 73, 57), BLACK);
    drawOutlinedRect(60, 455, 78, base, COLOR(122, 211, 203));
    setfillstyle(SOLID_FILL, COLOR(249, 253, 174));
    for (int y = 470; y < 615; y += 24) {
        bar(64, y, 74, y + 13);
        rectangle(64, y, 74, y + 13);
    }

    drawOutlinedRect(80, 520, 139, base, COLOR(245, 84, 61));
    drawWindowsGrid(92, 540, 2, 4, 26, 34, COLOR(251, 252, 170));

    drawOutlinedRect(140, 475, 265, base, COLOR(247, 152, 78));
    drawWindowsGrid(160, 500, 5, 5, 21, 31, COLOR(251, 252, 168));
    setcolor(BLACK);
    line(181, 475, 192, 425);
    line(192, 425, 213, 435);
    line(174, 452, 218, 460);
    line(176, 440, 214, 448);

    drawOutlinedRect(263, 548, 342, base, COLOR(117, 205, 190));
    drawWindowsGrid(279, 568, 2, 3, 33, 34, COLOR(252, 254, 178));

    drawOutlinedRect(342, 476, 446, base, COLOR(246, 77, 69));
    drawWindowsGrid(358, 491, 6, 6, 14, 28, COLOR(252, 253, 173));
    setcolor(BLACK);
    for (int x = 354; x <= 432; x += 18) {
        line(x, 482, x, base);
    }

    int dome[] = {448, 484, 465, 466, 495, 459, 528, 466, 548, 484, 548, base, 448, base};
    fillClosedPolygon(dome, 7, COLOR(84, 190, 179), BLACK);
    setfillstyle(SOLID_FILL, COLOR(247, 151, 79));
    pieslice(498, 485, 0, 180, 52);
    setcolor(BLACK);
    arc(498, 485, 0, 180, 52);
    drawRoundWindows(466, 508, 4, 5);

    drawOutlinedRect(548, 548, 675, base, COLOR(110, 198, 190));
    drawWindowsGrid(568, 565, 4, 3, 26, 32, COLOR(250, 253, 173));
    drawOutlinedRect(585, 528, 643, 548, COLOR(197, 122, 139));

    drawOutlinedRect(676, 430, 762, base, COLOR(247, 68, 55));
    int towerTop[] = {686, 430, 696, 382, 706, 364, 721, 382, 731, 430};
    fillClosedPolygon(towerTop, 5, COLOR(248, 70, 57), BLACK);
    setfillstyle(SOLID_FILL, COLOR(252, 253, 177));
    pieslice(708, 431, 0, 180, 36);
    rectangle(695, 455, 728, 505);
    drawWindowsGrid(691, 535, 3, 3, 18, 30, COLOR(252, 253, 177));
    setcolor(BLACK);
    line(708, 364, 708, 315);
    line(704, 337, 712, 337);

    int orangeBlock[] = {762, base, 762, 565, 833, 513, 833, base};
    fillClosedPolygon(orangeBlock, 4, COLOR(250, 139, 67), BLACK);
    int yellowBlock[] = {833, base, 833, 505, 965, 542, 965, base};
    fillClosedPolygon(yellowBlock, 4, COLOR(253, 244, 127), BLACK);
    setcolor(BLACK);
    for (int y = 545; y < 626; y += 12) {
        line(850, y, 954, y + 8);
    }

    int farRight[] = {965, base, 965, 525, WINDOW_WIDTH, 532, WINDOW_WIDTH, base};
    fillClosedPolygon(farRight, 4, COLOR(111, 200, 190), BLACK);

    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
}

void drawJet(int x, int y, int frame) {
    int black = BLACK;
    int white = COLOR(250, 253, 249);
    int lightGray = COLOR(225, 231, 230);
    int red = COLOR(235, 54, 42);

    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

    int rearWing[] = {x - 132, y - 24, x - 91, y - 31, x - 66, y - 8, x - 121, y - 5};
    fillClosedPolygon(rearWing, 4, white, black);
    int tail[] = {x - 128, y + 2, x - 102, y + 33, x - 70, y + 23, x - 108, y + 3};
    fillClosedPolygon(tail, 4, red, black);

    int body[] = {
        x - 126, y - 18,
        x - 78, y - 26,
        x - 33, y - 12,
        x + 93, y - 25,
        x + 137, y - 5,
        x + 132, y + 14,
        x + 78, y + 34,
        x - 44, y + 26,
        x - 100, y + 12
    };
    fillClosedPolygon(body, 9, white, black);

    int nearWing[] = {x - 35, y + 15, x + 58, y + 10, x + 86, y + 23, x - 14, y + 34};
    fillClosedPolygon(nearWing, 4, lightGray, black);

    setfillstyle(SOLID_FILL, COLOR(198, 229, 232));
    fillellipse(x + 77, y - 20, 20, 8);
    setcolor(black);
    ellipse(x + 77, y - 20, 0, 360, 20, 8);

    setfillstyle(SOLID_FILL, BLACK);
    for (int i = 0; i < 8; ++i) {
        fillellipse(x - 44 + i * 18, y - 8 + (i % 2), 3, 3);
    }

    setcolor(red);
    line(x - 99, y + 4, x - 58, y + 1);
    line(x - 45, y + 15, x + 62, y + 7);
    line(x - 35, y + 21, x + 48, y + 15);

    setfillstyle(SOLID_FILL, lightGray);
    setcolor(black);
    int engine[] = {x + 86, y + 20, x + 127, y + 12, x + 121, y + 22, x + 82, y + 30};
    fillClosedPolygon(engine, 4, lightGray, black);

    int flamePulse = (frame % 8 < 4) ? 10 : 2;
    int flame[] = {x - 131, y + 1, x - 163 - flamePulse, y + 11, x - 129, y + 18};
    fillClosedPolygon(flame, 3, COLOR(248, 109, 57), COLOR(248, 109, 57));
    int innerFlame[] = {x - 137, y + 5, x - 153 - flamePulse, y + 11, x - 136, y + 15};
    fillClosedPolygon(innerFlame, 3, YELLOW, YELLOW);

    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
}

void drawScene(int jetX, int frame, bool paused = false, bool saved = false) {
    setbkcolor(COLOR(140, 211, 221));
    cleardevice();
    setfillstyle(SOLID_FILL, COLOR(140, 211, 221));
    bar(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    drawCloud(150, 78, 1);
    drawCloud(545, 57, 1);
    drawSmallCloud(444, 245);
    drawSmallCloud(647, 291);
    drawSmallCloud(260, 318);
    drawCloud(875, 249, 1);
    drawCloud(218, 300, 1);
    drawCloud(965, 63, 1);

    int jetY = 110 + static_cast<int>(std::sin(frame * 0.075) * 13.0);
    drawFlightDots(jetX, frame);
    drawJet(jetX, jetY, frame);

    drawCity();

    if (paused || saved) {
        setfillstyle(SOLID_FILL, WHITE);
        bar(16, 16, 360, 45);
        setcolor(BLACK);
        rectangle(16, 16, 360, 45);
        if (saved) {
            drawText(28, 25, "Saved output image. Q/Esc closes.");
        } else {
            drawText(28, 25, "Paused. P continues, R restarts, S saves.");
        }
    }
}

void presentFrame(int& drawPage, int jetX, int frame, bool paused, bool saved = false) {
    setactivepage(drawPage);
    drawScene(jetX, frame, paused, saved);
    setvisualpage(drawPage);
    drawPage = 1 - drawPage;
}

void saveOutputImage(int jetX, int frame, bool paused, int& drawPage) {
    setactivepage(drawPage);
    drawScene(jetX, frame, paused, false);
    writeimagefile(const_cast<char*>(OUTPUT_IMAGE), 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
    drawScene(jetX, frame, paused, true);
    setvisualpage(drawPage);
    drawPage = 1 - drawPage;
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
    initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Part E - Animated Jet City Scene");

    int frame = 0;
    int jetX = -180;
    int drawPage = 0;
    bool paused = false;

    if (hasOption(argc, argv, "--save-and-exit")) {
        saveOutputImage(800, 18, false, drawPage);
        delay(250);
        closegraph();
        return 0;
    }

    presentFrame(drawPage, jetX, frame, paused);
    DWORD lastTick = GetTickCount();
    bool needsRedraw = false;
    bool running = true;

    while (running) {
        if (kbhit()) {
            int key = getch();
            if (key == 27 || key == 'q' || key == 'Q') {
                running = false;
            } else if (key == 'p' || key == 'P') {
                paused = !paused;
                needsRedraw = true;
            } else if (key == 'r' || key == 'R') {
                jetX = -180;
                frame = 0;
                paused = false;
                needsRedraw = true;
                lastTick = GetTickCount();
            } else if (key == 's' || key == 'S') {
                saveOutputImage(jetX, frame, paused, drawPage);
                needsRedraw = false;
                lastTick = GetTickCount();
            }
        }

        DWORD now = GetTickCount();
        if (!paused && now - lastTick >= FRAME_MS) {
            jetX += JET_SPEED;
            ++frame;
            if (jetX > WINDOW_WIDTH + 180) {
                jetX = -180;
            }
            lastTick = now;
            needsRedraw = true;
        }

        if (needsRedraw) {
            presentFrame(drawPage, jetX, frame, paused);
            needsRedraw = false;
        }

        delay(1);
    }

    closegraph();
    return 0;
}
