#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <cmath>
#include "GL/glut.h"

using namespace std;

// the size of the window measured in pixels
#define dim 300
#define inf 99999
unsigned char prevKey;

// the graph of the Conchoid of Nicomedes
void Example1()
{
    double xmax, ymax, xmin, ymin;
    double a = 1, b = 2;
    double pi = 4 * atan(1.0);
    double ratia = 0.05;
    double t;

    /**********************************
       Maximum/minimum values of x and y are computed.
       These values will be further used in scaling the
       original graph of the curve.
       These values are stored in the variables
       xmax, ymax, xmin, ymin: e.g., xmax is initialized
       with a value smaller than any of possible values
       of x; if in doubt or you cannot estimate it, use
       DBL_MIN instead (or DBL_MAX for minimum values).
       These DBL_* constants are found in <float.h> which
       need to be included.
       E.g., xmax is initialized with a - b - 1 because
       x(t) = a +/- b * cos(t) and for t in (-pi/2, pi/2),
       cos(t) is in (0, 1), so b * cos(t) is in (0, b),
       so +/- b * cos(t) is in (-b, b), so x(t) is in
       (a-b, a+b) and one can safely choose a-b-1 because
       a-b-1 < a-b.
       For y(t) we see that in its expression appears
       tan(t) which varies in (-inf,+inf) when t
       varies in (-pi/2, pi/2).
      **********************************/
    xmax = a - b - 1;
    xmin = a + b + 1;
    ymax = ymin = 0;
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia)
    {
        double x1, y1, x2, y2;
        x1 = a + b * cos(t);
        xmax = (xmax < x1) ? x1 : xmax;
        xmin = (xmin > x1) ? x1 : xmin;

        x2 = a - b * cos(t);
        xmax = (xmax < x2) ? x2 : xmax;
        xmin = (xmin > x2) ? x2 : xmin;

        y1 = a * tan(t) + b * sin(t);
        ymax = (ymax < y1) ? y1 : ymax;
        ymin = (ymin > y1) ? y1 : ymin;

        y2 = a * tan(t) - b * sin(t);
        ymax = (ymax < y2) ? y2 : ymax;
        ymin = (ymin > y2) ? y2 : ymin;
    }

    /**********************************
       At this line, we have found that the graph of the Conchoid
       is included in the rectangle having the edges x = xmin,
       x = xmax, y = ymin and y = ymax.
       We would like that the rectangle should be symmetric with
       respect to the Ox and Oy axes.
       We store in xmax and ymax the maximum of absolute values
       max(|xmax|,|xmin|) and max(|ymax|,|ymin|).
       Now we know that the graph of the Conchoid is included in
       the rectangle [-xmax, xmax] x [-ymax, ymax].
      **********************************/

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    /**********************************
       At this line, we can perform the scaling. All the points
       scaled are visible (i.e., in the rectangle [-1,1]x[-1,1]).
       Because we have -xmax <= x <= xmax we get
       -1 <= x / xmax <= 1. Idem for y.
       In order to correctly display the graph of the Conchoid,
       one should use the exact same points that were used in
       the computation of the scaling factor.
      **********************************/

    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia)
    {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia)
    {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x2, y2);
    }
    glEnd();
}

// The graph of the function
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$,
void Example2()
{
    double pi = 4 * atan(1.0);
    double xmax = 8 * pi;
    double ymax = exp(1.1);
    double ratia = 0.05;

    /**********************************
       For this example, the computation of the scaling factors
       is not needed. Because x is in the interval [0, 8pi]
       the scaling factor for x is 8pi because x/(8pi) is in
       [0,1]. In the case of the exponential function we know
       that if x is [0,8pi] then sin x is in [-1,1] so
       e^(-sin x) is in [1/e, e] and thus it is safe to use
       e^(1.1) as a scaling factor.
      **********************************/
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < xmax; x += ratia)
    {
        double x1, y1;
        x1 = x / xmax;
        y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();
}

void Display1()
{
    double ratia = 0.5;
    double xmax = 30;
    double ymax = 1;

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x <= xmax; x += ratia)
    {
        double x1 = x / xmax;
        double y1 = 0;
        if (x == 0)
        {
            glVertex2f(0, 1);
        }
        else
        {
            y1 = fabs(x - round(x)) / x;
            glVertex2f(x1, y1 / ymax);
        }
    }
    glEnd();
}

void Display2()
{
    double a = 0.2;
    double pi = 4 * atan(1);
    double ratia1 = 0.00001;
    double ratia2 = 0.005;

    double xStart = -0.99;
    double yStart = 0.99;
    int count = 0;

    // linia de sus
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(xStart, yStart);
    glVertex2f(a / (4 * cos(-pi / 2) * cos(-pi / 2) - 3), yStart);
    glEnd();

    // linia din stanga
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(xStart, yStart);
    glVertex2f(xStart, (a * tan(-pi / 5.41) / (4 * cos(-pi / 5.41) * cos(-pi / 5.41) - 3)));
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    glVertex2f(xStart, yStart);
    glVertex2f(a / (4 * cos(-pi / 2) * cos(-pi / 2) - 3), yStart);
    // am dat o ratie foarte mica pentru a desena curba din puncte dand impresia ca este o linie continua
    for (double t = -(pi / 2); t < -(pi / 6); t += ratia1)
    {
        double x = a / (4 * cos(t) * cos(t) - 3);
        double y = (a * tan(t)) / (4 * cos(t) * cos(t) - 3);
        if (x >= xStart && y <= yStart)
        {
            glVertex2f(x, y);
        }
    }
    glEnd();

    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_TRIANGLES);
    glVertex2f(xStart, yStart);
    // triunghiurile albe si cele rosii
    for (double t = -pi / 2 + ratia2; t < -pi / 6; t += ratia2, count++)
    {

        double x = a / (4 * cos(t) * cos(t) - 3);
        double y = (a * tan(t)) / (4 * cos(t) * cos(t) - 3);

        double xNext = a / (4 * cos(t + ratia2) * cos(t + ratia2) - 3);
        double yNext = (a * tan(t + ratia2)) / (4 * cos(t + ratia2) * cos(t + ratia2) - 3);

        if (x >= xStart && y <= yStart && y > 0.2)
        {
            if (count % 4 == 2)
            {
                glVertex2f(x, y);
                glVertex2f(xNext, yNext);
                glVertex2f(xStart, yStart);
            }
        }
    }
    glEnd();
}

void Display3()
{
    double a = 0.1, b = 0.2;
    double ratia = 0.05;
    bool isRed = true;

    glBegin(GL_LINE_STRIP);
    for (double t = -10; t < 10; t += ratia)
    {
        double x, y;
        x = a * t - b * sin(t);
        y = a - b * cos(t);

        if (isRed)
            glColor3f(1, 0.1, 0.1);
        else
            glColor3f(0, 1, 0); // verde

        glVertex2f(x, y);

        isRed = !isRed;
    }
    glEnd();
}

void Display4()
{
    double pi = 4 * atan(1.0);
    double a = 0.4;
    double ratia = 0.001; // am redus pasul de incrementare pentru a apropia mai mult întâlnirea

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);

    // desenez partea stanga
    for (double t = (pi / 4) - ratia; t > -(pi / 4); t -= ratia)
    {
        double r = a * sqrt(2 * cos(2 * t));
        double x = r * cos(t);
        double y = r * sin(t);

        glVertex2f(-x, -y);
    }
    // desenez partea dreapta
    for (double t = -(pi / 4) + ratia; t < (pi / 4); t += ratia)
    {
        double r = a * sqrt(2 * cos(2 * t));
        double x = r * cos(t);
        double y = r * sin(t);

        glVertex2f(x, y);
    }

    glEnd();
}

void Display5()
{
    double a = 0.02;
    double ratia = 0.01;

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double t = 0; t < 3; t += ratia)
    {
        double r, x, y;
        r = a * exp(1 + t);
        x = r * cos(t);
        y = r * sin(t);
        glVertex2f(x, y);
    }
    glEnd();
}

void Init(void)
{

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    //   glPointSize(4);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey)
    {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    case '3':
        Display3();
        break;
    case '4':
        Display4();
        break;
    case '5':
        Display5();
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

int main(int argc, char **argv)
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
