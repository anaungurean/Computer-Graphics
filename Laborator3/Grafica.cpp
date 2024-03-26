#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "GL/glut.h"

// the size of the window measured in pixels
#define dim 500
#define NRITER_MS 1000
#define MODMAX_MS 2.0
#define RX_MS 0.005
#define RY_MS 0.005

unsigned char prevKey;

class CComplex {
public:
    CComplex() : re(0.0), im(0.0) {}
    CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
    CComplex(const CComplex& c) : re(c.re), im(c.im) {}
    ~CComplex() {}

    CComplex& operator=(const CComplex& c)
    {
        re = c.re;
        im = c.im;
        return *this;
    }

    double getRe() { return re; }
    void setRe(double re1) { re = re1; }

    double getIm() { return im; }
    void setIm(double im1) { im = im1; }

    double getModul() { return sqrt(re * re + im * im); }

    int operator==(CComplex& c1)
    {
        return ((re == c1.re) && (im == c1.im));
    }

    CComplex pow2()
    {
        CComplex rez;
        rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
        rez.im = 2.0 * re * im;
        return rez;
    }

    friend CComplex operator+(const CComplex& c1, const CComplex& c2);
    friend CComplex operator*(CComplex& c1, CComplex& c2);

    void print(FILE* f)
    {
        fprintf(f, "%.20f%+.20f i", re, im);
    }

private:
    double re, im;
};


CComplex operator+(const CComplex& c1, const CComplex& c2)
{
    CComplex rez(c1.re + c2.re, c1.im + c2.im);
    return rez;
}

CComplex operator*(CComplex& c1, CComplex& c2)
{
    CComplex rez(c1.re * c2.re - c1.im * c2.im,
        c1.re * c2.im + c1.im * c2.re);
    return rez;
}




class CMandelbrotSet {
public:
    CMandelbrotSet() {
        m.nriter = NRITER_MS;
        m.modmax = MODMAX_MS;
    }

    ~CMandelbrotSet() {}

    void setModmax(double v) { assert(v <= MODMAX_MS); m.modmax = v; }
    double getModmax() const { return m.modmax; }

    void setNriter(int v) { assert(v <= NRITER_MS); m.nriter = v; }
    int getNriter() const { return m.nriter; }

    // Test if the point (x, y) belongs to the Mandelbrot set
    int isIn(const CComplex& c) const {
        int result = 0;
        CComplex z0, z1;

        for (int i = 1; i < m.nriter; i++) {
            z1 = z0 * z0 + c;
            z0 = z1;
            if (z1.getModul() > m.modmax){
                // The point does not belong to the Mandelbrot set because the iterative process converges infinitely
                result = i;
                break;
		    }
            else{
                // The point does not belong to the Mandelbrot set because the iterative process converges finitely
                result = -1;
            }
		}
		return result;
    }

  
    void display(double xmin, double ymin, double xmax, double ymax) {
        glPushMatrix();
        glLoadIdentity();
        glScaled(0.7, 0.7, 1);
        glTranslated(0.5, 0.0, 0.0);
        glBegin(GL_POINTS);
        for (double x = xmin + RX_MS; x < xmax; x += RX_MS) {
            for (double y = ymin + RY_MS; y < ymax; y += RY_MS) {
                CComplex c(x, y);
                int iterations = isIn(c);

                if (iterations == -1) {
                    // Punctul aparține setului Mandelbrot
                    glColor3f(196, 228, 255); 
                    glVertex2d(x, y);
                }
                else {
                    // Punctul nu aparține setului Mandelbrot
                    // Colorăm punctele în funcție de numărul de iterații necesare pentru detectarea non-pertenenței
                    double r = 1.0 / (iterations % 256);
                    double g = 1.0 / (iterations % 256);
                    double b = 1.0 / (iterations % 256);
                    glColor3f(r, g, b);
                    glVertex2d(x, y);
                }
            }
        }
   
        glEnd();
        glPopMatrix();
    }

private:
    struct Data {
        // The maximum value of M
        double modmax;
        // Number of iterations
        int nriter;
    } m;
};


void Display4()
{
    CMandelbrotSet f;
    double minX = -2;
    double minY = -2;
    double maxX = 2;
    double maxY = 2;
    f.display(minX, minY, maxX, maxY);
    
}


void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    switch (prevKey) {
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        Display4();
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
    if (key == 27)
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


