#include <GL/glut.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

// Great Zimbabwe University
// Computer Graphics Practical Assignment - Part B
// 3D-style cartoon character scene demonstrating near and far plane depth.

const float PI = 3.1415926535f;

struct Color {
    float r;
    float g;
    float b;
};

struct Vec3 {
    float x;
    float y;
    float z;
};

GLUquadric* gQuadric = NULL;
int gWindowWidth = 1000;
int gWindowHeight = 820;
float gCharacterX = 0.0f;
float gCharacterZ = 1.20f;
float gCharacterYaw = 0.0f;
float gRunPhase = 0.0f;
bool gIsRunning = false;
bool gHoverboardActive = false;
float gMountProgress = 0.0f;
float gHoverPhase = 0.0f;
float gWorldTravel = 0.0f;
float gFaceTime = 0.0f;
float gBlinkAmount = 0.0f;
bool gKeyDown[256] = {};
bool gSpecialKeyDown[512] = {};

const Color SKIN = {0.89f, 0.61f, 0.38f};
const Color SKIN_LIGHT = {0.98f, 0.72f, 0.48f};
const Color CHEEK = {0.93f, 0.34f, 0.28f};
const Color HAIR = {0.06f, 0.04f, 0.03f};
const Color HAIR_HIGHLIGHT = {0.14f, 0.08f, 0.04f};
const Color JACKET = {0.03f, 0.22f, 0.48f};
const Color JACKET_LIGHT = {0.05f, 0.36f, 0.76f};
const Color HOODIE = {0.92f, 0.94f, 0.92f};
const Color TROUSERS = {0.61f, 0.50f, 0.35f};
const Color SHOES = {0.03f, 0.03f, 0.035f};
const Color BOOK_COVER = {0.02f, 0.08f, 0.15f};
const Color BOOK_PAGE = {0.96f, 0.91f, 0.78f};
const Color BLACK = {0.015f, 0.015f, 0.018f};

Vec3 makeVec(float x, float y, float z) {
    Vec3 v = {x, y, z};
    return v;
}

Vec3 subtract(Vec3 a, Vec3 b) {
    return makeVec(a.x - b.x, a.y - b.y, a.z - b.z);
}

float length(Vec3 v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float clampValue(float value, float minimum, float maximum) {
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

float loopDepth(float baseZ, float travel, float period, float frontZ) {
    float z = baseZ + std::fmod(travel, period);
    while (z > frontZ) {
        z -= period;
    }
    while (z < frontZ - period) {
        z += period;
    }
    return z;
}

float approach01(float z, float farZ, float frontZ) {
    return clampValue((z - farZ) / (frontZ - farZ), 0.0f, 1.0f);
}

float sideLaneX(float baseX, float z, float farZ, float frontZ, float extraSpread) {
    float side = baseX < 0.0f ? -1.0f : 1.0f;
    float t = approach01(z, farZ, frontZ);
    return baseX + side * extraSpread * t * t;
}

float approachScale(float baseScale, float z, float farZ, float frontZ, float extraScale) {
    float t = approach01(z, farZ, frontZ);
    return baseScale * (1.0f + extraScale * t);
}

void setMaterial(const Color& color, float shininess = 28.0f, float specularStrength = 0.25f) {
    GLfloat ambient[] = {
        color.r * 0.28f,
        color.g * 0.28f,
        color.b * 0.28f,
        1.0f
    };
    GLfloat diffuse[] = {color.r, color.g, color.b, 1.0f};
    GLfloat specular[] = {
        specularStrength,
        specularStrength,
        specularStrength,
        1.0f
    };
    GLfloat shine[] = {shininess};
    GLfloat noEmission[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    glColor3f(color.r, color.g, color.b);
}

void setEmissionMaterial(const Color& color) {
    GLfloat ambient[] = {color.r * 0.35f, color.g * 0.35f, color.b * 0.35f, 1.0f};
    GLfloat diffuse[] = {color.r, color.g, color.b, 1.0f};
    GLfloat specular[] = {0.25f, 0.22f, 0.08f, 1.0f};
    GLfloat shine[] = {18.0f};
    GLfloat emission[] = {color.r * 0.65f, color.g * 0.55f, color.b * 0.12f, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glColor3f(color.r, color.g, color.b);
}

void drawRawSphere(float x, float y, float z, float sx, float sy, float sz, int slices = 48, int stacks = 32) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glutSolidSphere(1.0, slices, stacks);
    glPopMatrix();
}

void drawSphere(float x, float y, float z, float sx, float sy, float sz,
                const Color& color, float shininess = 28.0f, float specularStrength = 0.25f) {
    setMaterial(color, shininess, specularStrength);
    drawRawSphere(x, y, z, sx, sy, sz);
}

void drawCube(float x, float y, float z, float sx, float sy, float sz,
              const Color& color, float shininess = 18.0f, float specularStrength = 0.16f) {
    setMaterial(color, shininess, specularStrength);
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
    glPopMatrix();
}

void orientLocalZToVector(Vec3 direction) {
    float len = length(direction);
    if (len <= 0.0001f) {
        return;
    }

    float cosAngle = direction.z / len;
    if (cosAngle > 1.0f) {
        cosAngle = 1.0f;
    }
    if (cosAngle < -1.0f) {
        cosAngle = -1.0f;
    }

    float angle = std::acos(cosAngle) * 180.0f / PI;
    float axisX = -direction.y;
    float axisY = direction.x;
    float axisLength = std::sqrt(axisX * axisX + axisY * axisY);

    if (axisLength < 0.0001f) {
        if (direction.z < 0.0f) {
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        }
        return;
    }

    glRotatef(angle, axisX, axisY, 0.0f);
}

void drawCylinderBetween(Vec3 start, Vec3 end, float radius, const Color& color,
                         float shininess = 24.0f, float specularStrength = 0.20f, bool capEnds = true) {
    Vec3 direction = subtract(end, start);
    float len = length(direction);
    if (len <= 0.0001f) {
        return;
    }

    setMaterial(color, shininess, specularStrength);
    glPushMatrix();
    glTranslatef(start.x, start.y, start.z);
    orientLocalZToVector(direction);

    if (capEnds) {
        gluDisk(gQuadric, 0.0, radius, 36, 1);
    }
    gluCylinder(gQuadric, radius, radius, len, 36, 10);
    if (capEnds) {
        glTranslatef(0.0f, 0.0f, len);
        gluDisk(gQuadric, 0.0, radius, 36, 1);
    }

    glPopMatrix();
}

void drawConeBetween(Vec3 base, Vec3 tip, float radius, const Color& color,
                     float shininess = 20.0f, float specularStrength = 0.16f) {
    Vec3 direction = subtract(tip, base);
    float len = length(direction);
    if (len <= 0.0001f) {
        return;
    }

    setMaterial(color, shininess, specularStrength);
    glPushMatrix();
    glTranslatef(base.x, base.y, base.z);
    orientLocalZToVector(direction);
    gluDisk(gQuadric, 0.0, radius, 36, 1);
    gluCylinder(gQuadric, radius, 0.02f, len, 36, 10);
    glPopMatrix();
}

void drawTorus(float x, float y, float z, float tubeRadius, float ringRadius,
               float sx, float sy, float sz, const Color& color) {
    setMaterial(color, 32.0f, 0.22f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glutSolidTorus(tubeRadius, ringRadius, 18, 48);
    glPopMatrix();
}

void drawLine3D(Vec3 a, Vec3 b, const Color& color, float width) {
    glDisable(GL_LIGHTING);
    glColor3f(color.r, color.g, color.b);
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawSky() {
    // The sky is the farthest visual layer, so it is drawn first behind every 3D object.
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);

    glBegin(GL_QUADS);
    glColor3f(0.20f, 0.45f, 0.78f);
    glVertex3f(-65.0f, 32.0f, -55.0f);
    glVertex3f(65.0f, 32.0f, -55.0f);
    glColor3f(0.72f, 0.88f, 0.98f);
    glVertex3f(65.0f, -5.0f, -55.0f);
    glVertex3f(-65.0f, -5.0f, -55.0f);
    glEnd();

    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
}

void drawGround() {
    setMaterial({0.34f, 0.60f, 0.26f}, 12.0f, 0.05f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-20.0f, 0.0f, 7.0f);
    glVertex3f(20.0f, 0.0f, 7.0f);
    glVertex3f(32.0f, 0.0f, -46.0f);
    glVertex3f(-32.0f, 0.0f, -46.0f);
    glEnd();

    setMaterial({0.42f, 0.70f, 0.32f}, 12.0f, 0.04f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-32.0f, 0.03f, -15.0f);
    glVertex3f(32.0f, 0.03f, -15.0f);
    glVertex3f(38.0f, 0.03f, -46.0f);
    glVertex3f(-38.0f, 0.03f, -46.0f);
    glEnd();
}

void drawPath() {
    // The path narrows as it moves into the far plane, making depth easier to read.
    setMaterial({0.72f, 0.58f, 0.39f}, 10.0f, 0.06f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-2.7f, 0.055f, 7.0f);
    glVertex3f(2.7f, 0.055f, 7.0f);
    glVertex3f(0.55f, 0.055f, -35.0f);
    glVertex3f(-0.55f, 0.055f, -35.0f);
    glEnd();

    drawSphere(-1.7f, 0.12f, -2.0f, 0.13f, 0.04f, 0.10f, {0.62f, 0.55f, 0.44f}, 8.0f, 0.04f);
    drawSphere(1.4f, 0.12f, -6.0f, 0.12f, 0.04f, 0.09f, {0.64f, 0.57f, 0.47f}, 8.0f, 0.04f);
    drawSphere(-0.5f, 0.11f, -14.0f, 0.09f, 0.03f, 0.07f, {0.67f, 0.60f, 0.49f}, 8.0f, 0.04f);

    // Looping stones slide along the path to show that the hoverboard is travelling.
    const float markerSpacing = 4.8f;
    for (int i = 0; i < 10; ++i) {
        float z = loopDepth(-2.0f - markerSpacing * static_cast<float>(i), gWorldTravel, markerSpacing * 10.0f, 7.2f);
        float depthT = clampValue((7.2f - z) / 44.0f, 0.0f, 1.0f);
        float xOffset = (i % 2 == 0 ? -0.62f : 0.62f) * (1.0f - depthT * 0.55f);
        float scale = 0.18f - depthT * 0.07f;
        drawSphere(xOffset, 0.13f, z, scale * 1.4f, scale * 0.38f, scale * 0.78f,
                   {0.62f, 0.56f, 0.46f}, 8.0f, 0.04f);
    }
}

void drawSun() {
    // The sun is small and far behind the character, so it belongs to the far plane.
    setEmissionMaterial({1.0f, 0.82f, 0.14f});
    drawRawSphere(8.2f, 8.8f, -28.0f, 1.0f, 1.0f, 1.0f, 48, 28);
}

void drawCloud(float x, float y, float z, float scale) {
    // Clouds are small grouped spheres placed high and behind the character.
    Color cloud = {0.91f, 0.95f, 0.98f};
    drawSphere(x - 0.55f * scale, y, z, 0.75f * scale, 0.38f * scale, 0.32f * scale, cloud, 10.0f, 0.08f);
    drawSphere(x, y + 0.24f * scale, z, 0.85f * scale, 0.48f * scale, 0.36f * scale, cloud, 10.0f, 0.08f);
    drawSphere(x + 0.65f * scale, y, z, 0.70f * scale, 0.35f * scale, 0.30f * scale, cloud, 10.0f, 0.08f);
}

void drawClouds() {
    float drift = std::sin(gFaceTime * 0.20f) * 0.32f;
    float z1 = loopDepth(-16.5f, gWorldTravel * 0.22f, 32.0f, 5.8f);
    float z2 = loopDepth(-22.0f, gWorldTravel * 0.22f, 32.0f, 5.8f);
    float z3 = loopDepth(-31.0f, gWorldTravel * 0.22f, 32.0f, 5.8f);
    drawCloud(sideLaneX(-4.8f, z1, -26.0f, 5.8f, 3.2f) + drift, 8.0f, z1,
              approachScale(0.72f, z1, -26.0f, 5.8f, 0.72f));
    drawCloud(sideLaneX(4.5f, z2, -26.0f, 5.8f, 3.0f) - drift * 0.7f, 7.0f, z2,
              approachScale(0.50f, z2, -26.0f, 5.8f, 0.68f));
    drawCloud(sideLaneX(9.2f, z3, -31.0f, 5.8f, 2.5f) + drift * 0.5f, 8.6f, z3,
              approachScale(0.46f, z3, -31.0f, 5.8f, 0.60f));
}

void drawMountain(float x, float z, float radius, float height, const Color& color) {
    // Mountains are far objects: smaller, lighter, and farther back than the main character.
    drawConeBetween(makeVec(x, 0.0f, z), makeVec(x, height, z), radius, color, 12.0f, 0.10f);
    drawConeBetween(makeVec(x, height * 0.66f, z), makeVec(x, height + 0.02f, z),
                    radius * 0.34f, {0.94f, 0.95f, 0.95f}, 16.0f, 0.08f);
}

void drawMountains() {
    const float frontZ = 4.0f;
    const float farZ = -38.0f;
    float z1 = loopDepth(-24.0f, gWorldTravel * 0.15f, 44.0f, frontZ);
    float z2 = loopDepth(-33.0f, gWorldTravel * 0.15f, 44.0f, frontZ);
    float z3 = loopDepth(-29.0f, gWorldTravel * 0.15f, 44.0f, frontZ);
    float s1 = approachScale(1.0f, z1, farZ, frontZ, 0.58f);
    float s2 = approachScale(1.0f, z2, farZ, frontZ, 0.48f);
    float s3 = approachScale(1.0f, z3, farZ, frontZ, 0.52f);
    drawMountain(sideLaneX(-8.2f, z1, farZ, frontZ, 7.2f), z1, 4.1f * s1, 7.7f * s1, {0.48f, 0.54f, 0.62f});
    drawMountain(sideLaneX(-2.8f, z2, farZ, frontZ, 6.0f), z2, 3.4f * s2, 6.3f * s2, {0.54f, 0.60f, 0.68f});
    drawMountain(sideLaneX(7.3f, z3, farZ, frontZ, 7.0f), z3, 3.8f * s3, 6.9f * s3, {0.50f, 0.58f, 0.66f});
}

void drawTree(float x, float z, float scale, bool roundTop) {
    // Trees in the background are drawn with small cylinders and cones to show the far plane.
    float trunkHeight = 1.05f * scale;
    drawCylinderBetween(makeVec(x, 0.0f, z), makeVec(x, trunkHeight, z), 0.13f * scale,
                        {0.37f, 0.22f, 0.12f}, 12.0f, 0.08f);

    if (roundTop) {
        drawSphere(x, trunkHeight + 0.55f * scale, z, 0.55f * scale, 0.65f * scale, 0.55f * scale,
                   {0.25f, 0.55f, 0.26f}, 12.0f, 0.06f);
        drawSphere(x, trunkHeight + 1.05f * scale, z, 0.45f * scale, 0.52f * scale, 0.45f * scale,
                   {0.30f, 0.62f, 0.30f}, 12.0f, 0.06f);
    } else {
        drawConeBetween(makeVec(x, trunkHeight, z), makeVec(x, trunkHeight + 1.0f * scale, z),
                        0.65f * scale, {0.24f, 0.54f, 0.25f}, 12.0f, 0.06f);
        drawConeBetween(makeVec(x, trunkHeight + 0.55f * scale, z), makeVec(x, trunkHeight + 1.5f * scale, z),
                        0.52f * scale, {0.31f, 0.63f, 0.31f}, 12.0f, 0.06f);
    }
}

void drawFarTrees() {
    const float frontZ = 6.6f;
    const float farZ = -24.0f;
    float z1 = loopDepth(-12.5f, gWorldTravel * 0.72f, 32.0f, frontZ);
    float z2 = loopDepth(-17.0f, gWorldTravel * 0.72f, 32.0f, frontZ);
    float z3 = loopDepth(-14.0f, gWorldTravel * 0.72f, 32.0f, frontZ);
    float z4 = loopDepth(-20.0f, gWorldTravel * 0.72f, 32.0f, frontZ);
    float z5 = loopDepth(-10.8f, gWorldTravel * 0.72f, 32.0f, frontZ);
    drawTree(sideLaneX(-8.7f, z1, farZ, frontZ, 4.6f), z1, approachScale(0.72f, z1, farZ, frontZ, 0.95f), false);
    drawTree(sideLaneX(-5.7f, z2, farZ, frontZ, 4.0f), z2, approachScale(0.48f, z2, farZ, frontZ, 1.05f), false);
    drawTree(sideLaneX(5.9f, z3, farZ, frontZ, 4.0f), z3, approachScale(0.58f, z3, farZ, frontZ, 1.00f), false);
    drawTree(sideLaneX(8.1f, z4, farZ, frontZ, 4.6f), z4, approachScale(0.43f, z4, farZ, frontZ, 1.10f), true);
    drawTree(sideLaneX(10.8f, z5, farZ, frontZ, 5.0f), z5, approachScale(0.88f, z5, farZ, frontZ, 0.85f), true);
}

void drawBuilding(float x, float z, float scale) {
    drawCube(x, 0.62f * scale, z, 1.18f * scale, 1.24f * scale, 0.90f * scale,
             {0.74f, 0.70f, 0.62f}, 10.0f, 0.06f);
    drawConeBetween(makeVec(x, 1.25f * scale, z), makeVec(x, 1.88f * scale, z),
                    0.88f * scale, {0.38f, 0.30f, 0.28f}, 10.0f, 0.06f);

    drawCube(x - 0.26f * scale, 0.72f * scale, z + 0.47f * scale, 0.20f * scale, 0.24f * scale, 0.035f * scale,
             {0.23f, 0.42f, 0.62f}, 8.0f, 0.04f);
    drawCube(x + 0.26f * scale, 0.72f * scale, z + 0.47f * scale, 0.20f * scale, 0.24f * scale, 0.035f * scale,
             {0.23f, 0.42f, 0.62f}, 8.0f, 0.04f);
    drawCube(x, 0.23f * scale, z + 0.48f * scale, 0.28f * scale, 0.46f * scale, 0.04f * scale,
             {0.32f, 0.21f, 0.14f}, 8.0f, 0.04f);
    drawCylinderBetween(makeVec(x + 0.42f * scale, 1.34f * scale, z - 0.10f * scale),
                        makeVec(x + 0.42f * scale, 1.83f * scale, z - 0.10f * scale),
                        0.08f * scale, {0.28f, 0.19f, 0.16f}, 8.0f, 0.04f);
}

void drawFarBuildings() {
    const float frontZ = 6.4f;
    const float farZ = -27.0f;
    float z1 = loopDepth(-18.5f, gWorldTravel * 0.58f, 36.0f, frontZ);
    float z2 = loopDepth(-24.0f, gWorldTravel * 0.58f, 36.0f, frontZ);
    float z3 = loopDepth(-30.0f, gWorldTravel * 0.58f, 36.0f, frontZ);
    drawBuilding(sideLaneX(-5.8f, z1, farZ, frontZ, 5.2f), z1,
                 approachScale(0.78f, z1, farZ, frontZ, 1.35f));
    drawBuilding(sideLaneX(5.2f, z2, farZ, frontZ, 5.0f), z2,
                 approachScale(0.70f, z2, farZ, frontZ, 1.42f));
    drawBuilding(sideLaneX(9.0f, z3, farZ, frontZ, 5.3f), z3,
                 approachScale(0.62f, z3, farZ, frontZ, 1.50f));
}

void drawCharacterShadow() {
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    float hoverScale = 1.0f + gMountProgress * 0.20f;
    glColor4f(0.03f, 0.04f, 0.03f, 0.24f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(gCharacterX, 0.07f, gCharacterZ);
    for (int i = 0; i <= 80; ++i) {
        float angle = 2.0f * PI * static_cast<float>(i) / 80.0f;
        glVertex3f(gCharacterX + std::cos(angle) * 1.35f * hoverScale,
                   0.07f,
                   gCharacterZ + std::sin(angle) * 0.58f * hoverScale);
    }
    glEnd();
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
}

void drawHoverGlow(float x, float y, float z, float radius, float alpha) {
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glColor4f(0.18f, 0.72f, 1.0f, alpha);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y, z);
    for (int i = 0; i <= 48; ++i) {
        float angle = 2.0f * PI * static_cast<float>(i) / 48.0f;
        glVertex3f(x + std::cos(angle) * radius, y, z + std::sin(angle) * radius * 0.42f);
    }
    glEnd();
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
}

void drawHoverboard() {
    // The hoverboard hides the awkward walking cycle after a short mount animation.
    // It is made from scaled spheres, cylinders, cubes, and glowing far-future lights.
    if (gMountProgress <= 0.01f) {
        return;
    }

    float hoverBob = std::sin(gHoverPhase) * 0.025f * gMountProgress;
    glPushMatrix();
    glTranslatef(0.0f, 0.02f + hoverBob, 0.25f);
    glScalef(gMountProgress, gMountProgress, gMountProgress);

    drawHoverGlow(0.0f, -0.045f, 0.0f, 1.02f, 0.22f);
    drawHoverGlow(-0.48f, -0.035f, 0.0f, 0.30f, 0.38f);
    drawHoverGlow(0.48f, -0.035f, 0.0f, 0.30f, 0.38f);

    drawSphere(0.0f, 0.08f, 0.0f, 0.96f, 0.09f, 0.34f, {0.025f, 0.030f, 0.038f}, 38.0f, 0.28f);
    drawSphere(0.0f, 0.12f, 0.02f, 0.82f, 0.045f, 0.24f, {0.06f, 0.10f, 0.16f}, 34.0f, 0.20f);
    drawCylinderBetween(makeVec(-0.72f, 0.11f, -0.02f), makeVec(0.72f, 0.11f, -0.02f),
                        0.035f, {0.42f, 0.50f, 0.58f}, 22.0f, 0.18f);
    drawSphere(-0.60f, 0.08f, 0.25f, 0.11f, 0.035f, 0.055f, {0.10f, 0.78f, 1.0f}, 28.0f, 0.16f);
    drawSphere(0.60f, 0.08f, 0.25f, 0.11f, 0.035f, 0.055f, {0.10f, 0.78f, 1.0f}, 28.0f, 0.16f);

    setEmissionMaterial({0.10f, 0.78f, 1.0f});
    drawRawSphere(-0.46f, 0.025f, -0.18f, 0.11f, 0.03f, 0.07f, 24, 12);
    drawRawSphere(0.46f, 0.025f, -0.18f, 0.11f, 0.03f, 0.07f, 24, 12);

    glPopMatrix();
}

void drawBookPanel(float x, float y, float z, float angleY, const Color& color,
                   float width, float height, float thickness) {
    setMaterial(color, 18.0f, 0.14f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glScalef(width, height, thickness);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawBook() {
    // The book is a small 3D object held in front of the body.
    drawBookPanel(-0.26f, 1.82f, 0.90f, -12.0f, BOOK_COVER, 0.52f, 0.52f, 0.055f);
    drawBookPanel(0.26f, 1.82f, 0.90f, 12.0f, BOOK_COVER, 0.52f, 0.52f, 0.055f);
    drawBookPanel(-0.25f, 1.87f, 0.94f, -12.0f, BOOK_PAGE, 0.46f, 0.42f, 0.025f);
    drawBookPanel(0.25f, 1.87f, 0.94f, 12.0f, BOOK_PAGE, 0.46f, 0.42f, 0.025f);

    drawCylinderBetween(makeVec(0.0f, 1.56f, 0.93f), makeVec(0.0f, 2.08f, 0.93f),
                        0.025f, {0.015f, 0.018f, 0.025f}, 12.0f, 0.08f);

    glDisable(GL_LIGHTING);
    glColor3f(0.38f, 0.34f, 0.27f);
    glLineWidth(1.4f);
    glBegin(GL_LINES);
    glVertex3f(-0.41f, 1.94f, 0.975f);
    glVertex3f(-0.11f, 1.99f, 1.035f);
    glVertex3f(-0.41f, 1.83f, 0.975f);
    glVertex3f(-0.11f, 1.88f, 1.035f);
    glVertex3f(0.11f, 1.99f, 1.035f);
    glVertex3f(0.41f, 1.94f, 0.975f);
    glVertex3f(0.11f, 1.88f, 1.035f);
    glVertex3f(0.41f, 1.83f, 0.975f);
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawSmile() {
    float smilePulse = 0.5f + 0.5f * std::sin(gFaceTime * 2.0f);
    float smileRy = 0.105f + smilePulse * 0.025f;
    float smileY = 3.42f + smilePulse * 0.015f;

    glDisable(GL_LIGHTING);
    glColor3f(0.13f, 0.06f, 0.04f);
    glLineWidth(2.2f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 28; ++i) {
        float t = static_cast<float>(i) / 28.0f;
        float degrees = 205.0f + (335.0f - 205.0f) * t;
        float angle = degrees * PI / 180.0f;
        glVertex3f(std::cos(angle) * 0.18f, smileY + std::sin(angle) * smileRy, 0.64f);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawFaceDetails() {
    float eyeOpen = 1.0f - gBlinkAmount * 0.92f;
    float eyeRy = 0.018f + 0.057f * eyeOpen;
    float pupilRy = 0.010f + 0.032f * eyeOpen;

    drawSphere(-0.22f, 3.66f, 0.56f, 0.095f, eyeRy, 0.025f, {1.0f, 0.98f, 0.93f}, 20.0f, 0.10f);
    drawSphere(0.22f, 3.66f, 0.56f, 0.095f, eyeRy, 0.025f, {1.0f, 0.98f, 0.93f}, 20.0f, 0.10f);
    drawSphere(-0.20f, 3.65f, 0.60f, 0.037f, pupilRy, 0.018f, BLACK, 28.0f, 0.12f);
    drawSphere(0.20f, 3.65f, 0.60f, 0.037f, pupilRy, 0.018f, BLACK, 28.0f, 0.12f);

    if (gBlinkAmount > 0.65f) {
        drawCylinderBetween(makeVec(-0.30f, 3.66f, 0.63f), makeVec(-0.13f, 3.66f, 0.63f), 0.010f, BLACK, 16.0f, 0.06f);
        drawCylinderBetween(makeVec(0.13f, 3.66f, 0.63f), makeVec(0.30f, 3.66f, 0.63f), 0.010f, BLACK, 16.0f, 0.06f);
    }

    drawTorus(-0.22f, 3.66f, 0.625f, 0.010f, 0.125f, 1.12f, 0.88f, 0.10f, BLACK);
    drawTorus(0.22f, 3.66f, 0.625f, 0.010f, 0.125f, 1.12f, 0.88f, 0.10f, BLACK);
    drawCylinderBetween(makeVec(-0.09f, 3.66f, 0.625f), makeVec(0.09f, 3.66f, 0.625f), 0.010f, BLACK, 20.0f, 0.08f);

    drawCylinderBetween(makeVec(-0.34f, 3.83f, 0.59f), makeVec(-0.10f, 3.88f, 0.59f), 0.015f, HAIR, 16.0f, 0.08f);
    drawCylinderBetween(makeVec(0.10f, 3.88f, 0.59f), makeVec(0.34f, 3.83f, 0.59f), 0.015f, HAIR, 16.0f, 0.08f);

    drawSphere(0.0f, 3.52f, 0.64f, 0.085f, 0.075f, 0.075f, SKIN_LIGHT, 22.0f, 0.10f);
    drawSphere(-0.38f, 3.45f, 0.58f, 0.090f, 0.040f, 0.020f, CHEEK, 12.0f, 0.04f);
    drawSphere(0.38f, 3.45f, 0.58f, 0.090f, 0.040f, 0.020f, CHEEK, 12.0f, 0.04f);
    drawSmile();
}

void drawHair() {
    // Hair uses small dark spheres and cones instead of flat 2D polygons.
    drawSphere(0.0f, 4.06f, -0.03f, 0.58f, 0.28f, 0.56f, HAIR, 26.0f, 0.12f);
    drawSphere(-0.26f, 4.11f, 0.18f, 0.32f, 0.18f, 0.30f, HAIR_HIGHLIGHT, 24.0f, 0.10f);
    drawSphere(0.28f, 4.11f, 0.14f, 0.34f, 0.18f, 0.30f, HAIR, 24.0f, 0.10f);
    drawConeBetween(makeVec(-0.30f, 4.05f, 0.28f), makeVec(-0.48f, 3.88f, 0.68f), 0.14f, HAIR, 20.0f, 0.08f);
    drawConeBetween(makeVec(-0.04f, 4.14f, 0.34f), makeVec(-0.12f, 3.88f, 0.72f), 0.16f, HAIR_HIGHLIGHT, 20.0f, 0.08f);
    drawConeBetween(makeVec(0.25f, 4.08f, 0.30f), makeVec(0.46f, 3.92f, 0.66f), 0.14f, HAIR, 20.0f, 0.08f);
}

void drawHeadphones() {
    // Rounded headphones keep the student character recognizable in 3D.
    drawSphere(-0.68f, 3.58f, 0.03f, 0.16f, 0.34f, 0.25f, {0.06f, 0.075f, 0.08f}, 24.0f, 0.16f);
    drawSphere(0.68f, 3.58f, 0.03f, 0.16f, 0.34f, 0.25f, {0.06f, 0.075f, 0.08f}, 24.0f, 0.16f);
    drawSphere(-0.70f, 3.58f, 0.08f, 0.07f, 0.24f, 0.16f, {0.13f, 0.15f, 0.16f}, 20.0f, 0.12f);
    drawSphere(0.70f, 3.58f, 0.08f, 0.07f, 0.24f, 0.16f, {0.13f, 0.15f, 0.16f}, 20.0f, 0.12f);

    Vec3 previous = makeVec(-0.67f, 3.72f, -0.02f);
    for (int i = 1; i <= 18; ++i) {
        float t = static_cast<float>(i) / 18.0f;
        float theta = PI - PI * t;
        Vec3 current = makeVec(0.67f * std::cos(theta), 3.72f + 0.62f * std::sin(theta), -0.02f);
        drawCylinderBetween(previous, current, 0.035f, BLACK, 20.0f, 0.10f);
        previous = current;
    }
}

void drawTorso() {
    // The torso is rounded and connected using a cylinder plus scaled spheres.
    drawCylinderBetween(makeVec(0.0f, 1.42f, 0.0f), makeVec(0.0f, 2.62f, 0.0f), 0.55f, JACKET, 28.0f, 0.16f);
    drawSphere(0.0f, 2.62f, 0.0f, 0.59f, 0.20f, 0.47f, JACKET_LIGHT, 26.0f, 0.16f);
    drawSphere(0.0f, 1.42f, 0.0f, 0.57f, 0.20f, 0.43f, JACKET, 22.0f, 0.12f);

    drawCube(0.0f, 2.04f, 0.56f, 0.035f, 0.88f, 0.035f, {0.76f, 0.78f, 0.78f}, 14.0f, 0.08f);
    drawSphere(0.0f, 1.61f, 0.59f, 0.045f, 0.045f, 0.028f, {0.80f, 0.82f, 0.82f}, 18.0f, 0.08f);

    drawCylinderBetween(makeVec(-0.38f, 2.64f, 0.43f), makeVec(-0.10f, 2.34f, 0.58f), 0.065f, HOODIE, 16.0f, 0.06f);
    drawCylinderBetween(makeVec(0.38f, 2.64f, 0.43f), makeVec(0.10f, 2.34f, 0.58f), 0.065f, HOODIE, 16.0f, 0.06f);
}

void drawArmsAndHands() {
    // Arms start from the shoulders and hands connect to the arm cylinders.
    float mountMotion = gHoverboardActive ? (1.0f - gMountProgress) : 0.0f;
    float swing = std::sin(gRunPhase) * mountMotion;
    float lift = std::fabs(std::sin(gRunPhase)) * mountMotion;
    float glideLean = gIsRunning ? gMountProgress * 0.035f : 0.0f;
    float leftHandZ = 0.91f - swing * 0.08f + glideLean;
    float rightHandZ = 0.91f + swing * 0.08f + glideLean;
    float leftHandY = 1.78f + lift * 0.020f;
    float rightHandY = 1.78f + (1.0f - lift) * 0.014f * mountMotion;

    drawSphere(-0.58f, 2.45f, 0.04f, 0.18f, 0.18f, 0.18f, JACKET_LIGHT, 24.0f, 0.12f);
    drawSphere(0.58f, 2.45f, 0.04f, 0.18f, 0.18f, 0.18f, JACKET_LIGHT, 24.0f, 0.12f);

    drawCylinderBetween(makeVec(-0.58f, 2.42f, 0.05f), makeVec(-0.88f, 2.08f, 0.24f), 0.125f, JACKET, 26.0f, 0.14f);
    drawCylinderBetween(makeVec(-0.88f, 2.08f, 0.24f), makeVec(-0.47f, leftHandY, leftHandZ), 0.115f, JACKET, 26.0f, 0.14f);

    drawCylinderBetween(makeVec(0.58f, 2.42f, 0.05f), makeVec(0.88f, 2.08f, 0.24f), 0.125f, JACKET, 26.0f, 0.14f);
    drawCylinderBetween(makeVec(0.88f, 2.08f, 0.24f), makeVec(0.47f, rightHandY, rightHandZ), 0.115f, JACKET, 26.0f, 0.14f);

    drawSphere(-0.88f, 2.08f, 0.24f, 0.13f, 0.13f, 0.13f, JACKET_LIGHT, 24.0f, 0.12f);
    drawSphere(0.88f, 2.08f, 0.24f, 0.13f, 0.13f, 0.13f, JACKET_LIGHT, 24.0f, 0.12f);
    drawSphere(-0.47f, leftHandY, leftHandZ + 0.05f, 0.145f, 0.120f, 0.120f, SKIN, 24.0f, 0.10f);
    drawSphere(0.47f, rightHandY, rightHandZ + 0.05f, 0.145f, 0.120f, 0.120f, SKIN, 24.0f, 0.10f);
}

void drawLegsAndShoes() {
    // Legs connect under the body and shoes sit on the ground.
    float mountMotion = gHoverboardActive ? (1.0f - gMountProgress) : 0.0f;
    float swing = std::sin(gRunPhase) * mountMotion;
    float oppositeSwing = std::sin(gRunPhase + PI) * mountMotion;
    float leftLift = std::max(0.0f, swing) * 0.16f;
    float rightLift = std::max(0.0f, oppositeSwing) * 0.16f;
    float leftFootZ = 0.25f + swing * 0.22f;
    float rightFootZ = 0.25f + oppositeSwing * 0.22f;
    float boardStance = gMountProgress * 0.08f;
    Vec3 leftAnkle = makeVec(-0.32f - boardStance, 0.30f + leftLift, leftFootZ - 0.04f);
    Vec3 rightAnkle = makeVec(0.32f + boardStance, 0.30f + rightLift, rightFootZ - 0.04f);

    drawCylinderBetween(leftAnkle,
                        makeVec(-0.30f - boardStance, 1.43f, 0.0f), 0.155f, TROUSERS, 18.0f, 0.10f);
    drawCylinderBetween(rightAnkle,
                        makeVec(0.30f + boardStance, 1.43f, 0.0f), 0.155f, TROUSERS, 18.0f, 0.10f);
    drawSphere(-0.30f - boardStance, 1.42f, 0.0f, 0.22f, 0.18f, 0.22f, TROUSERS, 18.0f, 0.08f);
    drawSphere(0.30f + boardStance, 1.42f, 0.0f, 0.22f, 0.18f, 0.22f, TROUSERS, 18.0f, 0.08f);
    drawSphere(leftAnkle.x, leftAnkle.y, leftAnkle.z, 0.17f, 0.12f, 0.15f, TROUSERS, 18.0f, 0.08f);
    drawSphere(rightAnkle.x, rightAnkle.y, rightAnkle.z, 0.17f, 0.12f, 0.15f, TROUSERS, 18.0f, 0.08f);

    drawSphere(-0.32f - boardStance, 0.18f + leftLift, leftFootZ, 0.31f, 0.13f, 0.47f, SHOES, 24.0f, 0.18f);
    drawSphere(0.32f + boardStance, 0.18f + rightLift, rightFootZ, 0.31f, 0.13f, 0.47f, SHOES, 24.0f, 0.18f);
    drawCube(-0.32f - boardStance, 0.11f + leftLift, leftFootZ + 0.33f, 0.43f, 0.045f, 0.08f, {0.86f, 0.82f, 0.74f}, 12.0f, 0.06f);
    drawCube(0.32f + boardStance, 0.11f + rightLift, rightFootZ + 0.33f, 0.43f, 0.045f, 0.08f, {0.86f, 0.82f, 0.74f}, 12.0f, 0.06f);
}

void drawHeadAndNeck() {
    // The neck touches both the rounded torso and the head.
    drawCylinderBetween(makeVec(0.0f, 2.58f, 0.0f), makeVec(0.0f, 3.02f, 0.0f), 0.16f, SKIN, 22.0f, 0.10f);
    drawSphere(0.0f, 3.55f, 0.0f, 0.62f, 0.70f, 0.57f, SKIN, 32.0f, 0.16f);
    drawHair();
    drawFaceDetails();
    drawHeadphones();
}

void drawCharacter() {
    // The main cartoon character is drawn large and close to the camera to represent the near plane.
    // The character is built from 3D OpenGL primitives: spheres, cylinders, cones, scaled cubes, and line strips.
    float hoverLift = gMountProgress * 0.10f;
    float hoverBob = std::sin(gHoverPhase) * 0.025f * gMountProgress;
    float mountBounce = (gHoverboardActive && gMountProgress < 1.0f)
        ? std::fabs(std::sin(gRunPhase * 2.0f)) * 0.025f * (1.0f - gMountProgress)
        : 0.0f;
    float bodyBob = hoverLift + hoverBob + mountBounce;

    glPushMatrix();
    glTranslatef(gCharacterX, bodyBob, gCharacterZ);
    glRotatef(gCharacterYaw, 0.0f, 1.0f, 0.0f);

    drawHoverboard();
    drawLegsAndShoes();
    drawTorso();
    drawArmsAndHands();
    drawBook();
    drawHeadAndNeck();

    glPopMatrix();
}

void setupLights() {
    GLfloat globalAmbient[] = {0.24f, 0.25f, 0.27f, 1.0f};
    GLfloat lightPosition[] = {-5.5f, 8.5f, 6.5f, 1.0f};
    GLfloat lightDiffuse[] = {0.95f, 0.92f, 0.84f, 1.0f};
    GLfloat lightSpecular[] = {0.58f, 0.56f, 0.52f, 1.0f};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Perspective camera: nearby objects appear larger and distant objects appear smaller.
    gluLookAt(0.0, 3.15, 8.4,
              0.0, 2.45, 0.7,
              0.0, 1.0, 0.0);
    setupLights();

    // Far plane: drawn smaller, lighter, higher, and behind the foreground character.
    drawSky();
    drawSun();
    drawClouds();
    drawMountains();
    drawFarBuildings();
    drawFarTrees();

    // Mid plane: ground and path support the depth illusion.
    drawGround();
    drawPath();

    // Near plane: the large connected 3D character is drawn last in the foreground.
    drawCharacterShadow();
    drawCharacter();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    gWindowWidth = width;
    gWindowHeight = height;
    float aspect = static_cast<float>(width) / static_cast<float>(height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // The near and far clipping planes are explicit here.
    // The character sits close to the near plane; mountains, clouds, trees, and sun sit toward the far plane.
    gluPerspective(48.0, aspect, 1.0, 80.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void saveScreenshot() {
    std::vector<unsigned char> pixels(static_cast<size_t>(gWindowWidth) * static_cast<size_t>(gWindowHeight) * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, gWindowWidth, gWindowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    FILE* file = std::fopen("part_b_3d_cartoon_scene.ppm", "wb");
    if (!file) {
        std::printf("Could not save screenshot.\n");
        return;
    }

    std::fprintf(file, "P6\n%d %d\n255\n", gWindowWidth, gWindowHeight);
    for (int y = gWindowHeight - 1; y >= 0; --y) {
        const unsigned char* row = pixels.data() + static_cast<size_t>(y) * static_cast<size_t>(gWindowWidth) * 3;
        std::fwrite(row, 1, static_cast<size_t>(gWindowWidth) * 3, file);
    }
    std::fclose(file);
    std::printf("Saved screenshot as part_b_3d_cartoon_scene.ppm\n");
}

void keyboard(unsigned char key, int, int) {
    gKeyDown[key] = true;

    if (key == 27 || key == 'q' || key == 'Q') {
        std::exit(0);
    }
    if (key == 'p' || key == 'P') {
        saveScreenshot();
    }
    if (key == 'r' || key == 'R') {
        gCharacterX = 0.0f;
        gCharacterZ = 1.20f;
        gCharacterYaw = 0.0f;
        gRunPhase = 0.0f;
        gIsRunning = false;
        gHoverboardActive = false;
        gMountProgress = 0.0f;
        gHoverPhase = 0.0f;
        gWorldTravel = 0.0f;
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int, int) {
    gKeyDown[key] = false;
}

void specialKeyboard(int key, int, int) {
    if (key >= 0 && key < 512) {
        gSpecialKeyDown[key] = true;
    }
}

void specialKeyboardUp(int key, int, int) {
    if (key >= 0 && key < 512) {
        gSpecialKeyDown[key] = false;
    }
}

void updateAnimation(int) {
    const float dt = 0.016f;
    const float turnSpeed = 125.0f;
    const float hoverSpeed = 3.00f;

    gFaceTime += dt;
    float blinkCycle = std::fmod(gFaceTime, 4.2f);
    float blinkPulse = blinkCycle < 0.16f ? std::sin(PI * blinkCycle / 0.16f) : 0.0f;
    float secondBlinkCycle = std::fmod(gFaceTime + 1.8f, 7.1f);
    float secondBlinkPulse = secondBlinkCycle < 0.12f ? std::sin(PI * secondBlinkCycle / 0.12f) : 0.0f;
    gBlinkAmount = std::max(blinkPulse, secondBlinkPulse);

    bool forward = gKeyDown['w'] || gKeyDown['W'] || gSpecialKeyDown[GLUT_KEY_UP];
    bool backward = gKeyDown['s'] || gKeyDown['S'] || gSpecialKeyDown[GLUT_KEY_DOWN];
    bool turnLeft = gKeyDown['a'] || gKeyDown['A'] || gSpecialKeyDown[GLUT_KEY_LEFT];
    bool turnRight = gKeyDown['d'] || gKeyDown['D'] || gSpecialKeyDown[GLUT_KEY_RIGHT];
    bool controlPressed = forward || backward || turnLeft || turnRight;

    if (controlPressed) {
        gHoverboardActive = true;
    }

    if (gHoverboardActive && gMountProgress < 1.0f) {
        gMountProgress += 2.6f * dt;
        if (gMountProgress > 1.0f) {
            gMountProgress = 1.0f;
        }
    }

    if (turnLeft) {
        gCharacterYaw += turnSpeed * dt;
    }
    if (turnRight) {
        gCharacterYaw -= turnSpeed * dt;
    }

    float movement = 0.0f;
    if (forward) {
        movement += 1.0f;
    }
    if (backward) {
        movement -= 1.0f;
    }

    gIsRunning = std::fabs(movement) > 0.001f;
    if (gIsRunning) {
        float radians = gCharacterYaw * PI / 180.0f;
        float mountSpeedMultiplier = 0.40f + 0.60f * gMountProgress;
        gWorldTravel += movement * hoverSpeed * mountSpeedMultiplier * dt;

        // Keep the avatar near the camera while the looping environment moves around it.
        gCharacterX += std::sin(radians) * movement * 0.28f * dt;
        gCharacterZ += (1.20f - gCharacterZ) * 0.08f;

        // Keep the playable area in the foreground so the character remains the near-plane object.
        gCharacterX = clampValue(gCharacterX, -1.15f, 1.15f);
        gCharacterZ = clampValue(gCharacterZ, 0.95f, 1.55f);
    } else {
        gCharacterX += (0.0f - gCharacterX) * 0.025f;
        gCharacterZ += (1.20f - gCharacterZ) * 0.08f;
    }

    if (gHoverboardActive && gMountProgress < 1.0f) {
        gRunPhase += 10.0f * dt;
    } else {
        gRunPhase *= 0.90f;
    }

    if (gHoverboardActive) {
        gHoverPhase += (gIsRunning ? 5.4f : 2.8f) * dt;
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateAnimation, 0);
}

void init() {
    glClearColor(0.62f, 0.80f, 0.94f, 1.0f);

    // 3D rendering settings required for the improved Part B scene.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    gQuadric = gluNewQuadric();
    gluQuadricNormals(gQuadric, GLU_SMOOTH);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(gWindowWidth, gWindowHeight);
    glutInitWindowPosition(90, 40);
    glutCreateWindow("Part B - 3D Cartoon Character Near and Far Plane");
    glutIgnoreKeyRepeat(1);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutTimerFunc(16, updateAnimation, 0);
    glutMainLoop();
    return 0;
}
