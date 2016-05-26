// Draw an Icosahedron
// ECE4893/8893 Project 4
// Shivam Agarwal

#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

#define NFACE 20
#define NVERTEX 12

#define X .525731112119133606
#define Z .850650808352039932


static GLfloat rotX = 0.0;
static GLfloat rotY = 0.0;
static GLfloat rotZ = 0.0;
bool rotate;
int gDepth;


// These are the 12 vertices for the icosahedron
static GLfloat vdata[NVERTEX][3] = {
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

// These are the 20 faces.  Each of the three entries for each
// vertex gives the 3 vertices that make the face.
static GLint tindices[NFACE][3] = {
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
};

int testNumber; // Global variable indicating which test number is desired

void Test1()
{
    gDepth = 0;
    rotate = false;
}

void Test2()
{
    gDepth = 0;
    rotate = true;
}

void Test3()
{
    gDepth = 1;
    rotate = false;
}

void Test4()
{
    gDepth = 1;
    rotate = true;
}

void Test5(int depth)
{
    gDepth = depth;
    rotate = false;
}

void Test6(int depth)
{
    gDepth = depth;
    rotate = true;
}


void normalize(GLfloat v[3])
{
    GLfloat d = sqrt (v[0] * v[0] +
                      v[1] * v[1] +
                      v[2] * v[2]);
    if (d == 0.0) return;
    v[0] /=d;
    v[1] /=d;
    v[2] /=d;
}

void drawTriangle(GLfloat* v1, GLfloat* v2, GLfloat* v3)
{
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(1.0);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glEnd();
    
    
    glBegin(GL_TRIANGLES);
    glColor3f(0.0,0.0,1.0);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glEnd();
    
    
}


void subDivide(GLfloat* v1, GLfloat* v2, GLfloat* v3, int depth)
{
    if (depth == 0)
    {
        drawTriangle(v1, v2, v3);
        return;
    }
    
    GLfloat v12[3];
    GLfloat v23[3];
    GLfloat v31[3];
    
    for (int i = 0; i < 3; ++i)
    {
        v12[i] = (v1[i] + v2[i])/2.0;
        v23[i] = (v2[i] + v3[i])/2.0;
        v31[i] = (v3[i] + v1[i])/2.0;
    }
    
    normalize(v12);
    normalize(v23);
    normalize(v31);
    
    
    subDivide(v1, v12, v31, depth-1);
    subDivide(v2, v23, v12, depth-1);
    subDivide(v3, v31, v23, depth-1);
    subDivide(v12, v23, v31, depth-1);
    
}

void drawIcosahedron(){
    for(int i =0;i<NFACE; i++){
        subDivide(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], gDepth);
    }
}

void rotation(){
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
    rotX += 1.0;
    rotY += 1.0;
}
void display(void)
{
  
   
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 1.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    
    glTranslatef(250, 250, 250);
    glScalef(200, 200, 200);
    
    if (rotate) rotation();
    
    drawIcosahedron();
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble)w, (GLdouble)0.0, h, (GLdouble)w, (GLdouble)-w);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

void init()
{
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(100.0, timer, 0);
}



int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: icosahedron testnumber" << endl;
        exit(1);
    }
    // Set the global test number
    testNumber = atol(argv[1]);
    if(argc>2) gDepth= atoi(argv[2]);
    else gDepth = 2;
    // Initialize glut  and create your window here
    // Set your glut callbacks here
    // Enter the glut main loop here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Icosahedron");
    
    // Initialize GL
    init();
    
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(100.0, timer, 0);
    
    
    switch (testNumber)
    {
        case 1: Test1();
            break;
        case 2: Test2();
            break;
        case 3: Test3();
            break;
        case 4: Test4();
            break;
        case 5: Test5(gDepth);
            break;
        case 6: Test6(gDepth);
            break;
        default: Test1();
    }
    
    glutMainLoop();
    
    return 0;
}

