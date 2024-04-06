#include <GL/glut.h>
#include <cmath>

#define dim 500
#define M_PI 3.14159265358979323846


class GrilaCarteziana {
private:
    int numRows, numColumns; // Dimensions of the grid
    double centerX, centerY; // Coordinates of the center of the Cartesian system
    double deltaX, deltaY; // Distance between grid points

public:
    GrilaCarteziana(int numRows, int numColumns) : numRows(numRows), numColumns(numColumns) {
        // Set origin to (0,0) and calculate grid parameters
        setOrigin(0, 0);
    }

    void setOrigin(int x = 0, int y = 0) {
        // Calculate distance between grid points on x and y axes
        deltaX = 1.8 / (numColumns - 1);
        deltaY = 1.8 / (numRows - 1);

        // Set grid center coordinates
        centerX = -0.9 + x * deltaX;
        centerY = -0.9 + y * deltaY;
    }

    void drawGrid() {
        // Set color and line width for grid lines
        glColor3d(0, 0, 0);
        glLineWidth(1);

        // Draw vertical lines
        double startX = -0.9;
        for (int i = 0; i < numColumns; i++) {
            glBegin(GL_LINES);
            glVertex2d(startX, 0.9);
            glVertex2d(startX, -0.9);
            glEnd();
            startX += deltaX;
        }

        // Draw horizontal lines
        double startY = 0.9;
        for (int i = 0; i < numRows; i++) {
            glBegin(GL_LINES);
            glVertex2d(-0.9, startY);
            glVertex2d(0.9, startY);
            glEnd();
            startY -= deltaY;
        }
    }

    void writePixel(int x, int y) {
        double xgl = centerX + x * deltaX;
        double ygl = centerY + y * deltaY;

        int triangles = 50; // Number of triangles to approximate circle
        double radius = 0.03;

        if (xgl == centerX && ygl == centerY)
            glColor3d(0, 0, 1); // Blue for the center pixel
        else
            glColor3d(0, 0, 0); // Black for other pixels

        glBegin(GL_TRIANGLE_FAN); // Start drawing filled triangles
        glVertex2f(xgl, ygl); // Center of the circle
        for (int i = 0; i <= triangles; i++) {
            // Calculate vertices of the circle
            glVertex2d(xgl + (radius * cos(i * 2.0 * M_PI / triangles)),
                ygl + (radius * sin(i * 2.0 * M_PI / triangles)));
        }
        glEnd();
    }


    void cerc(int R) {
        // Calculate the center coordinates of the circle in grid space
        double xgl = centerX;
        double ygl = centerY;

        // Number of triangles used to approximate the circle
        int triangles = 360;

        // Set color and line width for the circle
        glColor3d(1, 0, 0); // Red color
        glLineWidth(5);     // Line width

        // Begin drawing the circle
        glBegin(GL_LINE_STRIP);

        // Draw vertices of the circle
        for (int i = 0; i <= triangles; i++) {
            // Calculate each vertex of the circle based on its angle
            glVertex2d(
                xgl + (R * deltaX * cos(i * 2.0 * M_PI / triangles)), // x-coordinate
                ygl + (R * deltaY * sin(i * 2.0 * M_PI / triangles))  // y-coordinate
            );
        }

        // End drawing the circle
        glEnd();

        // Algorithm for drawing pixels in the circle
        int x = R, y = 0;          // Starting point
        int d = 1 - R;             // Initial decision parameter
        int dN = 3;                // Change in decision parameter for N direction
        int dNV = -2 * R + 5;      // Change in decision parameter for NV direction

        // Draw the pixels in the circle using Bresenham's algorithm
        while (x > y) {
            // Update the decision parameter
            if (d < 0) {
                d += dN;
                dN += 2;
                dNV += 2;
            }
            else {
                d += dNV;
                dN += 2;
                dNV += 4;
                x--;
            }
            y++;

            // Draw pixels symmetrically in all octants
            if (x + 1 < numColumns) writePixel(x + 1, y); // Right
            writePixel(x, y);                              // East
            if (x - 1 >= 0) writePixel(x - 1, y);         // Left
        }
    }

};

void Display0() {
    GrilaCarteziana gc(16, 16);
    gc.drawGrid();
    gc.cerc(13); // Example usage of cerc function
    //gc.writePixel(5, 5); // Example usage of writePixel function
}

void Init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(1);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    Display0();
    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {
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
