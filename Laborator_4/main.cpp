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
            glColor3d(0, 1, 1); // Cyan for other pixels

        glBegin(GL_TRIANGLE_FAN); // Start drawing filled triangles
        glVertex2f(xgl, ygl); // Center of the circle
        for (int i = 0; i <= triangles; i++) {
            // Calculate vertices of the circle
            glVertex2d(xgl + (radius * cos(i * 2.0 * M_PI / triangles)),
                ygl + (radius * sin(i * 2.0 * M_PI / triangles)));
        }
        glEnd();
    }
};

void Display0() {
    GrilaCarteziana gc(16, 16);
    gc.drawGrid();
    gc.writePixel(5, 5); // Example usage of writePixel function
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
