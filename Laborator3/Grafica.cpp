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
#define M_PI 3.14159265358979323846
unsigned char prevKey;
int level_desen_2 = 0;
int level_desen_3 = 0;
int level_desen_4 = 0;

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
class C2coord
{
public:
    C2coord()
    {
        m.x = m.y = 0;
    }

    C2coord(double x, double y)
    {
        m.x = x;
        m.y = y;
    }

    C2coord(const C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
    }

    C2coord& operator=(C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(C2coord& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

protected:
    struct SDate
    {
        double x, y;
    } m;
};


class CPunct : public C2coord
{
public:
    CPunct() : C2coord(0.0, 0.0)
    {}

    CPunct(double x, double y) : C2coord(x, y)
    {}

    CPunct& operator=(const CPunct& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    void getxy(double& x, double& y)
    {
        x = m.x;
        y = m.y;
    }

    int operator==(CPunct& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    void marcheaza()
    {
        glBegin(GL_POINTS);
        glVertex2d(m.x, m.y);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "(%+f,%+f)", m.x, m.y);
    }

};

class CVector : public C2coord
{
public:
    CVector() : C2coord(0.0, 0.0)
    {
        normalizare();
    }

    CVector(double x, double y) : C2coord(x, y)
    {
        normalizare();
    }

    CVector& operator=(CVector& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(CVector& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    CPunct getDest(CPunct& orig, double lungime)
    {
        double x, y;
        orig.getxy(x, y);
        CPunct p(x + m.x * lungime, y + m.y * lungime);
        return p;
    }

    void rotatie(double grade)
    {
        double x = m.x;
        double y = m.y;
        double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
        m.x = x * cos(t) - y * sin(t);
        m.y = x * sin(t) + y * cos(t);
        normalizare();
    }

    void deseneaza(CPunct p, double lungime)
    {
        double x, y;
        p.getxy(x, y);
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        glVertex2d(x, y);
        glVertex2d(x + m.x * lungime, y + m.y * lungime);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
    }

private:
    // it is used for normalizing a vector
    void normalizare()
    {
        double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
        if (d != 0.0)
        {
            C2coord::m.x = C2coord::m.x * 1.0 / d;
            C2coord::m.y = C2coord::m.y * 1.0 / d;
        }
    }
};


class Sierpinskis_carpet 
{
public:

    void draw_square(CPunct &p, double &lungime)

    {
        CPunct p1;
        double x, y;
        CVector v(1.0, 0.0);
        v.deseneaza(p, lungime);

        p = v.getDest(p, lungime);
        v.rotatie(-90);
        v.deseneaza(p, lungime);

        p = v.getDest(p, lungime);
        v.rotatie(-90);
        v.deseneaza(p, lungime);

        p = v.getDest(p, lungime);
        v.rotatie(-90);
        v.deseneaza(p, lungime);

        v.rotatie(-90);

    }

    void draw_fracal_top_down(CPunct& p, double& lungime, int &level, CVector& v)
    { 
        for (int i = 0; i < 3; i++)
        {
            fractal(lungime / 3.0, level - 1, p, v);
            p = v.getDest(p, lungime );
        }

    }

    void draw_fracal_lateral(CPunct& p, double& lungime, int& level, CVector& v)
    {
        for (int i = 0; i < 2; i++)
        {
            fractal(lungime / 3.0, level - 1, p, v);
            p = v.getDest(p, lungime*2.0);
		}
	}

    void fractal(double lungime,
        int level,
        CPunct p,
        CVector v)
    {
        CPunct p1;
        double x, y;
        if (level == 0)
        {
            draw_square(p, lungime);
        }
        else
        {
           draw_square(p, lungime);
           
           p1 = p;
           p.getxy(x, y);
           p = CPunct(x - 2.0 * lungime / 3.0, y + lungime * 5.0 / 3.0);
           draw_fracal_top_down(p, lungime, level, v);

           p = p1;
           p.getxy(x, y);
           p = CPunct(x - 2.0 * lungime / 3.0, y - lungime / 3.0);

           draw_fracal_top_down(p, lungime, level, v);

           p = p1;
           p.getxy(x, y);
           p = CPunct(x - 2.0 * lungime / 3.0, y + lungime / 1.5);
           draw_fracal_lateral(p, lungime, level, v);

        }

       
    }

    void display(double lungime, int level)
    {
        CVector v(1.0, 0.0);
        CPunct p(-1.0, 1.0);

        v.deseneaza(p, lungime);

        p = v.getDest(p, lungime);
        v.rotatie(-90);
        v.deseneaza(p, lungime);

        p = v.getDest(p, lungime);
        v.rotatie(-90);
        v.deseneaza(p, lungime);

        p = v.getDest(p, lungime);
        v.rotatie(-90);
        v.deseneaza(p, lungime);

        p = CPunct(-1.0 / 3.0, 1.0 / 3.0);
        v.rotatie(-90);

        fractal(lungime / 3.0, level, p, v);
    }
};


class LSystemBranch {
public:

    void fractal(double lungime,
        int nivel,
        double factordiviziune,
        CPunct &p,
        CVector v)
    {
        assert(factordiviziune != 0);
        CPunct p1, p2;
        if (nivel == 0)
        {
        }
        else
        {
            drawFirstBranch(lungime, nivel, factordiviziune, p, v);
            drawSecondBranch(lungime, nivel, factordiviziune, p, v);
        }
    }

    void display(double lungime, int nivel)
    {
        CVector v(0.0, -1.0);
        CPunct p(0.0, 1.0);

        v.deseneaza(p, 0.25);
        p = v.getDest(p, 0.25);
        fractal(lungime, nivel, 0.4, p, v);
    }

private:
    void drawFirstBranch(double lungime, int nivel, double factordiviziune, CPunct &p, CVector v)
    {
        v.rotatie(-45);
        v.deseneaza(p, lungime);
        CPunct p1 = v.getDest(p, lungime);
        fractal(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
    }

    void drawSecondBranch(double lungime, int nivel, double factordiviziune, CPunct &p, CVector v)
    {
        v.rotatie(45);
        v.deseneaza(p, lungime);
        CPunct p1 = v.getDest(p, lungime);
        CPunct p2 = p1;

        v.rotatie(15);
        v.deseneaza(p1, lungime);
        p1 = v.getDest(p1, lungime);
        fractal(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

        p1 = p2;
        v.rotatie(-60);
        v.deseneaza(p1, lungime);
        p1 = v.getDest(p1, lungime);
        p2 = p1;
        double f = 0.5;

        v.rotatie(30);
        v.deseneaza(p1, f * lungime);
        p1 = v.getDest(p1, f * lungime);
        fractal(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

        p1 = p2;
        v.rotatie(-120);
        v.deseneaza(p1, f * lungime);
        p1 = v.getDest(p1, f * lungime);
        fractal(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
    }

};


class Sierpinskis_arrowhead   
{
public:
    struct DrawingState
    {
        double x;
        double y;
        int unghi;
    };


    void drawState(DrawingState* state, double lungime)
    {
       
        double theta = (M_PI * state->unghi) / 180.0;
        state->x += lungime * cos(theta);
        state->y += lungime * sin(theta);
        glVertex2f(state->x, state->y);
    }

    void fractal(int nivel, double lungime, DrawingState* state, int unghi)
    {
        if (nivel == 0)
        {
            drawState(state, lungime);
        }
        else
        {
            fractal(nivel - 1, lungime / 2, state, -unghi);
            state->unghi = (state->unghi + unghi) % 360;

            fractal(nivel - 1, lungime / 2, state, unghi);
            state->unghi = (state->unghi + unghi) % 360;

            fractal(nivel - 1, lungime / 2, state, -unghi);
        }
    }

    void display(double lungime, int nivel)
    {
        DrawingState state;
        state.unghi = 90;
        state.x = 0.0 - lungime * 0.5;
        state.y = 0.0 - lungime * 0.5;

        if (nivel % 2 != 0) 
            state.unghi = (state.unghi - 60) % 360;

        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        glVertex2f(state.x, state.y);
        fractal(nivel, lungime, &state, 60);
        glEnd();
    }
};

void Display1()
{
    CMandelbrotSet f;
    double minX = -2;
    double minY = -2;
    double maxX = 2;
    double maxY = 2;
    f.display(minX, minY, maxX, maxY);
    
}

void Display2()
{
    Sierpinskis_carpet f1;

    char c[3];
    sprintf_s(c, sizeof(c), "%2d", level_desen_2);
    glRasterPos2d(-0.98, -0.98);

    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.9, 0.9, 1);
    f1.display(2.0, level_desen_2);
    glPopMatrix();
    level_desen_2++;
}

void Display3()
{
    LSystemBranch f;
    char c[3];
    sprintf_s(c, sizeof(c), "%2d", level_desen_3);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.4, 0.4, 1);
    glTranslated(-0.5, 0.5, 0.0);
    f.display(1, level_desen_3);
    glPopMatrix();
    level_desen_3++;
}

void Display4()
{
    Sierpinskis_arrowhead f;
    char c[3];
    sprintf_s(c, sizeof(c), "%2d", level_desen_4);
    glRasterPos2d(-0.98,-0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);
    f.display(1.0, level_desen_4);

    level_desen_4++;
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
        Display1();
        break;
    case '2':
		glClear(GL_COLOR_BUFFER_BIT);
		Display2();
		break;
    case '3':
        glClear(GL_COLOR_BUFFER_BIT);
		Display3();
		break;
    case '4':
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


