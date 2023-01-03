#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "gameData.h"

#define pi 3.14159
#define pi2 pi/2
#define pi3 3*pi/2
#define degreeRad 0.0174533 //1 degree in radians

void drawMap2D() {
    int xo, yo;
    for(int y = 0; y < map.y; y++) {
        for(int x = 0; x < map.x; x++) {
            if(grid[y * map.x + x] == 1) glColor3f(1, 1, 1);
            else glColor3f(0, 0, 0);

            xo = x * map.s;
            yo = y * map.s;
            glBegin(GL_QUADS);
            glVertex2i(xo + 1, yo + 1);
            glVertex2i(xo + 1, yo + map.s - 1);
            glVertex2i(xo + map.s - 1, yo + map.s - 1);
            glVertex2i(xo + map.s - 1, yo + 1);
            glEnd();
        }
    }
}

void drawPlayer() {
    glColor3f(1, 1, 0); //yellow
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(player.x, player.y);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(player.x, player.y);
    glVertex2i(player.x + player.deltaX * 5, player.y + player.deltaY * 5);
    glEnd();
}

float dist(float ax, float ay, float bx, float by, float angle) {
    //Return hypoteneuse 
    return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}

void drawRays3D(int fov) {
    int mx, my, mp, depthOfField;
    float rayX, rayY, rayAngle, xoffset, yoffset, aTan, nTan;
    float disH, hX, hY, disV, vX, vY, distT;

    rayAngle = player.angle - degreeRad * (fov / 2);
    if(rayAngle < 0) rayAngle += 2 * pi;
    if(rayAngle > 2 * pi) rayAngle -= 2 * pi;

    for(int i = 0; i < fov; i++) {
        //Horizontal lines 
        depthOfField = 0;
        aTan = -1/tan(rayAngle);
        disH = 1000000;
        hX = player.x;
        hY = player.y;
        if(rayAngle > pi) { //If looking down...
            //We round the ray angle to the nearest 64th value
            //We do this by bit shifting down 6 bits, then bit shifting those
            //6 bits back 
            rayY = (((int)player.y>>6)<<6) - 0.0001;
            rayX = (player.y - rayY) * aTan + player.x;
            yoffset = -64;
            xoffset = -yoffset * aTan;
        }
        if(rayAngle < pi) { //If looking up...
            rayY = (((int)player.y>>6)<<6) + 64;
            rayX = (player.y - rayY) * aTan + player.x;
            yoffset = 64;
            xoffset = -yoffset * aTan;
        }
        if(rayAngle == 0 || rayAngle == pi) { //Looking straight ahead
            rayX = player.x;
            rayY = player.y;
            depthOfField = 8;
        }
        while(depthOfField < 8) {
            mx = (int) (rayX)>>6;
            my = (int) (rayY)>>6;
            mp = my * map.x + mx; 

            if(mp > 0 && mp < map.x * map.y && grid[mp] == 1) {
                //hit wall
                hX = rayX;
                hY = rayY; 
                disH = dist(player.x, player.y, hX, hY, rayAngle);
                depthOfField = 8; 
            }
            else {
                rayX += xoffset;
                rayY += yoffset; 
                depthOfField += 1;
            }
        }

        //Vertical lines 
        depthOfField = 0;
        nTan = -tan(rayAngle);
        disV = 1000000;
        vX = player.x;
        vY = player.y; 
        if(rayAngle > pi2 && rayAngle < pi3) { //If looking left...
            rayX = (((int)player.x>>6)<<6) - 0.0001;
            rayY = (player.x - rayX) * nTan + player.y;
            xoffset = -64;
            yoffset = -xoffset * nTan;
        }
        if(rayAngle < pi2 || rayAngle > pi3) { //If looking right...
            rayX = (((int)player.x>>6)<<6) + 64;
            rayY = (player.x - rayX) * nTan + player.y;
            xoffset = 64;
            yoffset = -xoffset * nTan;
        }
        if(rayAngle == 0 || rayAngle == pi) { //Looking straight ahead
            rayX = player.x;
            rayY = player.y;
            depthOfField = 8;
        }
        while(depthOfField < 8) {
            mx = (int) (rayX)>>6;
            my = (int) (rayY)>>6;
            mp = my * map.x + mx; 

            if(mp > 0 && mp < map.x * map.y && grid[mp] == 1) {
                //Hit wall
                vX = rayX;
                vY = rayY; 
                disV = dist(player.x, player.y, vX, vY, rayAngle);
                depthOfField = 8;
            }
            else { 
                rayX += xoffset;
                rayY += yoffset; 
                depthOfField += 1;
            }
        }

        //Draw shortest line
        if(disH < disV) {
            rayX = hX;
            rayY = hY; 
            distT = disH; 
            glColor3f(1, 0, 0);
        } else {
            rayX = vX;
            rayY = vY;
            distT = disV; 
            glColor3f(0.7, 0, 0);
        }
        //Draw rays
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(player.x, player.y);
        glVertex2i(rayX, rayY);
        glEnd();

        //Render 3d walls 
        float flatAngle = player.angle - rayAngle;
        if (flatAngle < 0) flatAngle += 2 * pi;
        if (flatAngle > 2 * pi) flatAngle -= 2 * pi;
        distT = distT * cos(flatAngle);
            //flatAngle fixes the fish eye effect

        float lineH = (map.s * 320)/distT;
        if(lineH > 320) lineH = 320;
        float lineO = 160 - lineH / 2;

        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i(i * 8 + 530, lineO);
        glVertex2i(i * 8 + 530, lineH + lineO);
        glEnd();

        //Move ray over
        rayAngle += degreeRad;
        if(rayAngle < 0) rayAngle += 2 * pi;
        if(rayAngle > 2 * pi) rayAngle -= 2 * pi;
    }

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();
    drawPlayer();
    drawRays3D(60);
    glutSwapBuffers();
}

void checkKeyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 27: //escape; close program
            glutDestroyWindow(glutGetWindow());
            exit(0);
            break;
        case 97: //a
            player.angle -= 0.1;
            if(player.angle < 0) player.angle += 2*pi;
            player.deltaX = cos(player.angle) * 5;
            player.deltaY = sin(player.angle) * 5;
            break;
        case 100: //d
            player.angle += 0.1;
            if(player.angle > 2 * pi) player.angle -= 2*pi;
            player.deltaX = cos(player.angle) * 5;
            player.deltaY = sin(player.angle) * 5;
            break;
        case 119: //w
            player.x += player.deltaX;
            player.y += player.deltaY; 
            break;
        case 115: //s
            player.x -= player.deltaX;
            player.y -= player.deltaY;
            break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, 1024, 512, 0);
    player.x = 300;
    player.y = 300;
    player.deltaX = cos(player.angle) * 5;
    player.deltaY = sin(player.angle) * 5;
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024, 512);
    glutCreateWindow("Window Test");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(checkKeyboard);
    glutMainLoop();

    return 0;
}