#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "gameData.h"

float pi = 3.14159;

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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();
    drawPlayer();
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