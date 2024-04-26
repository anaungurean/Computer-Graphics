#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "glut.h"

unsigned char prevKey;

const int lines = 16;
const int cols = 16;

class CartesianGrid {
    double margin_grid;
    double starting_point;

    int nr_lines, nr_cols;
    double distance_columns;
    double distance_lines;

public:

    CartesianGrid(int nr_lines, int nr_cols) {

        this->nr_lines = nr_lines;
        this->nr_cols = nr_cols;

        this->margin_grid = 0.1;
        this->starting_point = -1 + margin_grid;

        this->distance_columns = 2 * (1 - margin_grid) / (nr_cols - 1);
        this->distance_lines = 2 * (1 - margin_grid) / (nr_lines - 1);

    }


    void displayAxes() {

        glColor3d(0,0,0);
        glLineWidth(1);

        double x = -1 + margin_grid;

        // desenarea a coloanelor
        for (int i = 0; i < nr_cols; i++) {

            glBegin(GL_LINES);
            glVertex2d(x, -1 + margin_grid);
            glVertex2d(x, 1 - margin_grid);
            glEnd();

            x += distance_columns;
        }

        double y = 1 - margin_grid;

        // desenarea a liniilor
        for (int i = 0; i < nr_lines; i++) {

            glBegin(GL_LINES);
            glVertex2d(-1 + margin_grid, y);
            glVertex2d(1 - margin_grid, y);
            glEnd();

            y -= distance_lines;
        }

    }


    void writePixel(int i, int j) {

        if (!(i >= 0 && i < nr_lines))
            return;

        if (!(j >= 0 && j < nr_cols))
            return;

        const double pi = 4 * atan(1.0);

        int nr_triangles = 20;
        double radius = 0.03;


        double x_center = starting_point + i * distance_columns;
        double y_center = starting_point + j * distance_lines;

        printf("(%i,%i) %f, %f\n", i, j, x_center, y_center);

        glColor3f(0.3, 0.3, 0.3);
        glBegin(GL_TRIANGLE_FAN);

        glVertex2d(x_center, y_center);

        for (int k = 0; k <= nr_triangles; k++) {

            double theta = k * 2.0 * pi / nr_triangles;

            double x = x_center + (radius * cos(theta));
            double y = y_center + (radius * sin(theta));

            glVertex2d(x, y);


        }
        glEnd();

    }

    void ScanConvertSegments3SE(int x0, int y0, int xn, int yn, int size_pixel) {

        int dy = abs(yn - y0);
        int dx = abs(xn - x0);
        int d = 2 * dy - dx;
        int dE = 2 * dy;
        int dSE = 2 * (dy - dx);

        int x = x0, y = y0;

        for (int i = 1; i <= size_pixel / 2; i++) {

            writePixel(x, y - i);
            writePixel(x, y + i);
        }
        writePixel(x, y);


        while (x < xn)
        {
            if (d <= 0)
            {
                d += dE; // orizontal
                x++;
            }
            else
            {
                d += dSE; //diagonala
                x++;
                y--;
            }

            for (int i = 1; i <= size_pixel / 2; i++) {

                writePixel(x, y - i);
                writePixel(x, y + i);
            }
            writePixel(x, y);



        }
    }

    void ScanConvertSegments3NE(int x0, int y0, int xn, int yn, int size_pixel) {

        int dx = abs(xn - x0);
        int dy = abs(yn - y0);

        int d = 2 * dy - dx;
        int dE = 2 * dy;
        int dNE = 2 * (dy - dx);

        int x = x0, y = y0;

        for (int i = 1; i <= size_pixel / 2; i++) {

            writePixel(x, y - 1);
            writePixel(x, y + 1);
        }
        writePixel(x, y);


        while (x < xn) {

            if (d <= 0) {

                d += dE; // orizontal
                x++;
            }
            else {

                d += dNE; //diagonala
                x++;
                y++;
            }

            for (int i = 1; i <= size_pixel / 2; i++) {

                writePixel(x, y - 1);
                writePixel(x, y + 1);
            }
            writePixel(x, y);


        }

    }


    void ScanConvertSegments3SW(int x0, int y0, int xn, int yn, int size_pixel) {

        int dy = abs(yn - y0);
        int dx = abs(xn - x0);
        int d = 2 * dx - dy;
        int dS = 2 * dx;
        int dSW = 2 * (dx - dy);

        int x = x0, y = y0;

        for (int i = 1; i <= size_pixel / 2; i++) {

            writePixel(x - i, y);
            writePixel(x + i, y);
        }
        writePixel(x, y);


        while (y > yn)
        {

            if (d <= 0)
            {
                d += dS; // vertical
                y--;
            }
            else
            {
                d += dSW; //diagonala
                y--;
                x--;
            }

            for (int i = 1; i <= size_pixel / 2; i++) {

                writePixel(x - i, y);
                writePixel(x + i, y);
            }
            writePixel(x, y);
        }

    }

    void ScanConvertSegments3NW(int x0, int y0, int xn, int yn, int size_pixel) {

        int dy = abs(yn - y0);
        int dx = abs(xn - x0);

        int d = 2 * dx - dy;
        int dN = 2 * dx;
        int dNW = 2 * (dx - dy);


        int x = x0, y = y0;

        for (int i = 1; i <= size_pixel / 2; i++) {

            writePixel(x - i, y);
            writePixel(x + i, y);
        }
        writePixel(x, y);

        while (y < yn)
        {

            if (d <= 0)
            {
                d += dN; // vertical
                y++;
            }
            else
            {
                d += dNW;//diagonala
                y++;
                x--;
            }

            for (int i = 1; i <= size_pixel / 2; i++) {

                writePixel(x - i, y);
                writePixel(x + i, y);
            }
            writePixel(x, y);
        }


    }

    void ScanConvertSegments(int x0, int y0, int xn, int yn, int size_pixel) {

        // desenare linie rosie
        glColor3f(1, 0.1, 0.1);  // rosu
        glLineWidth(4);
        glBegin(GL_LINES);

        glVertex2d(-1 + margin_grid + x0 * distance_lines, -1 + margin_grid + y0 * distance_columns);
        glVertex2d(-1 + margin_grid + xn * distance_lines, -1 + margin_grid + yn * distance_columns);

        glEnd();

        // panta
        if (y0 < yn) {
            

            //proiectia
            if (abs(x0 - xn) > abs(y0 - yn)) {

                ScanConvertSegments3NE(x0, y0, xn, yn, size_pixel);
            }
            else {
                ScanConvertSegments3SW(xn, yn, x0, y0, size_pixel);
            }

        }
        else {

            if (abs(x0 - xn) > abs(y0 - yn)) {

                ScanConvertSegments3SE(x0, y0, xn, yn, size_pixel);
            }
            else {
                ScanConvertSegments3NW(xn, yn, x0, y0, size_pixel);
            }

        }
    }

    void ScanConvertCircle4(int x0, int y0, int radius, int size_pixel) {

        // desenare cerc rosu
        const double pi = 4 * atan(1.0);

        glColor3d(1, 0, 0);
        glLineWidth(8);
        glBegin(GL_LINE_STRIP);

        int vertices = 50;

        double Ogx = -1 + margin_grid + x0 * distance_lines;
        double Ogy = -1 + margin_grid + y0 * distance_columns;

        for (int i = 0; i <= vertices; i++)
        {
            glVertex2d(
                Ogx + (radius * distance_lines * cos(i * 2.0 * pi / vertices)),
                Ogy + (radius * distance_lines * sin(i * 2.0 * pi / vertices))
            );
        }
        glEnd();

        // desenare pixeli

        double d = 1 - radius;
        int dN = 3;
        int dNV = -2 * radius + 5;

        int x = radius, y = 0;

        for (int i = 1; i <= size_pixel / 2; i++) {

            writePixel(x - i, y);
            writePixel(x + i, y);
        }
        writePixel(x, y);

        while (x > y)
        {
            if (d < 0)
            {
                d += dN; // verticala N
                dN += 2;
                dNV += 2;
            }
            else
            {
                d += dNV; // diagonal NV 
                dN += 2;
                dNV += 4;
                x--;
            }
            y++;

            for (int i = 1; i <= size_pixel / 2; i++) {

                writePixel(x - i, y);
                writePixel(x + i, y);
            }
            writePixel(x, y);

        }

    }

};


void Display1() {

    CartesianGrid cartesianGrid = *new CartesianGrid(lines, cols);
    cartesianGrid.displayAxes();

    int x0, y0, xn, yn;
    int size_pixel = 3;

    x0 = 0;
    y0 = cols - 1;
    xn = lines - 1;
    yn = 10;
    size_pixel = 3;

    cartesianGrid.ScanConvertSegments(x0, y0, xn, yn, size_pixel);

    x0 = 0;
    y0 = 0;
    xn = lines - 1;
    yn = 7;
    size_pixel = 1;


    cartesianGrid.ScanConvertSegments(x0, y0, xn, yn, size_pixel);

}

void Display2() {
    CartesianGrid cartesianGrid = *new CartesianGrid(lines, cols);
    cartesianGrid.displayAxes();

    int x0, y0, radius;
    int pixel_size;

    x0 = 0;
    y0 = 0;
    radius = 13;
    pixel_size = 3;

    cartesianGrid.ScanConvertCircle4(x0, y0, radius, pixel_size);

}


void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(3);

    glPointSize(4);

    glPolygonMode(GL_FRONT, GL_FILL);
}

void Display(void) {
    printf("Call Display\n");

    // The buffer GL_COLOR_BUFFER_BIT is cleared
    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey) {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    default:
        break;
    }

    glFlush();
}


void Reshape(int w, int h) {

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {

    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(600, 600);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}
