#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <queue>
#include <map>

#include <GL/glut.h>

#define dim 500
#define M_PI 3.14159265358979323846

using namespace std;

const int n = 16;
const int m = 16;
unsigned char prevKey;


class GrilaCarteziana {
private:
    // Private member variables
    double centerX, centerY; // Coordinates of the center of the Cartesian system
    double epsilon;    // Small parameter to avoid boundary issues
    int minX, maxX, minY, maxY, gridCenterX, gridCenterY; // Min and max values for coordinates
    double deltaX, deltaY; // Distance between grid points

public:
    // Public member variables
    int numRows, numColumns; // Dimensions of the grid

    // Constructor
    GrilaCarteziana(int numRows, int numColumns) {
        // Initialize grid dimensions
        this->numRows = numRows;
        this->numColumns = numColumns;

        // Set a small epsilon value
        epsilon = 0.1;

        // Set origin to (0,0) and calculate grid parameters
        setOrigin(0, 0);
    }

    // Function to set the origin
    void setOrigin(int x = 0, int y = 0) {
        // Calculate distance between grid points on x and y axes
        deltaX = (2 - 2 * epsilon) / (numColumns - 1);
        deltaY = (2 - 2 * epsilon) / (numRows - 1);

        // Set grid origin coordinates
        gridCenterX = x;
        gridCenterY = y;

        // Calculate coordinates of the center of the Cartesian system
        centerX = -1 + epsilon + gridCenterX * deltaX;
        centerY = -1 + epsilon + gridCenterY * deltaY;

        // Calculate min and max values for x and y coordinates
        minX = -gridCenterX;
        maxX = numColumns - gridCenterX - 1;
        minY = -gridCenterY;
        maxY = numRows - gridCenterY - 1;
    }

    // Function to draw a pixel at given coordinates
    void drawPixel(int x, int y) {
        // Calculate OpenGL coordinates
        double xGL = centerX + x * deltaX;
        double yGL = centerY + y * deltaY;

        // Set color based on whether the pixel is at the center or not
        if (xGL == centerX && yGL == centerY)
            glColor3d(0, 0, 1); // Blue for the center pixel
        else
            glColor3d(0, 1, 1); // Cyan for other pixels

        // Draw a circle at the specified coordinates
        int triangles = 50; // Number of triangles to approximate circle
        double radius = 0.03;

        glBegin(GL_TRIANGLE_FAN); // Start drawing filled triangles
        glVertex2f(xGL, yGL); // Center of the circle
        for (int i = 0; i <= triangles; i++) {
            // Calculate vertices of the circle
            glVertex2d(xGL + (radius * cos(i * 2.0 * M_PI / triangles)),
                yGL + (radius * sin(i * 2.0 * M_PI / triangles)));
        }
        glEnd();
    }

    // Function to draw the grid
    void drawGrid() {
        // Set color and line width for grid lines
        glColor3d(0, 0, 0);
        glLineWidth(1);

        // Draw vertical lines
        double startX = -1 + epsilon; // Starting point for drawing columns
        for (int i = 0; i < numColumns; i++) {
            glBegin(GL_LINES);
            glVertex2d(startX, 1 - epsilon); // Top point of the line
            glVertex2d(startX, -1 + epsilon); // Bottom point of the line
            glEnd();

            startX += deltaX; // Move to the next column
        }

        // Draw horizontal lines
        double startY = 1 - epsilon; // Starting point for drawing rows
        for (int i = 0; i < numRows; i++) {
            glBegin(GL_LINES);
            glVertex2d(-1 + epsilon, startY); // Left point of the line
            glVertex2d(1 - epsilon, startY); // Right point of the line
            glEnd();

            startY -= deltaY; // Move to the next row
        }
    }
};


void Display0()
{
    GrilaCarteziana gc(n, m);
    gc.drawGrid();
}


void Init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPointSize(1);

    glPolygonMode(GL_FRONT, GL_FILL); // pt umplerea poligoanelor
}

void Display(void)
{
    switch (prevKey)
    {
    case '0':
        glClear(GL_COLOR_BUFFER_BIT);
        Display0();
        break;
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        //Display1();
        break;
    case '2':
        glClear(GL_COLOR_BUFFER_BIT);
        //Display2();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

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
