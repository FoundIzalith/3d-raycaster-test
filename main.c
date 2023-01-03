#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "gameData.h"

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
    glVertex2i(player.px, player.py);
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
        case 27:
            glutDestroyWindow(glutGetWindow());
            exit(0);
            break;
        case 97: //a
            player.px -= 5;
            break;
        case 100: //d
            player.px += 5;
            break;
        case 119: //w
            player.py -= 5;
            break;
        case 115: //s
            player.py += 5;
            break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, 1024, 512, 0);
    player.px = 300;
    player.py = 300;
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