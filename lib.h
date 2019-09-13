#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

typedef struct v4
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} v4;

typedef struct m4
{
    v4 a;
    v4 b;
    v4 c;
    v4 d;
} m4;

void v4_def(v4*, GLfloat, GLfloat, GLfloat, GLfloat);
void v4_print(v4*);
void v4_copy(v4*, v4*);
void v4_norm(v4*, v4*);
float v4_angleBetween(v4*, v4*);
void v4_cross(v4*, v4*, v4*);
GLfloat v4_dot(v4*, v4*);
void v4_scalar(v4*, GLfloat, v4*);
void v4_add(v4*, v4*, v4*);
void v4_sub(v4*, v4*, v4*);
void v4_mult(v4*, v4*, v4*);
int v4_equals(v4*, v4*, float);

void m4_def(m4*, v4*, v4*, v4*, v4*);
void m4_copy(m4*, m4*);
void m4_print(m4*);
void m4_scalar(m4*, GLfloat, m4*);
void m4_add(m4*, m4*, m4*);
void m4_sub(m4*, m4*, m4*);
void m4_mult(m4*, m4*, m4*);
void m4_inverse(m4*, m4*);
void m4_transpose(m4*, m4*);
void m4v4(v4*, m4*, v4*);

void m4_scale(m4*, m4*, GLfloat f);
void m4_translate(m4*, m4*, GLfloat, GLfloat, GLfloat);
void m4_rotate(m4*, m4*, float, float, float);

void apply_tm(v4*, m4*);

void cube_generate(v4[], GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

int sphere_get_num_vertices(int);
void sphere_generate(v4[], int, float, float, float, float);

void look_at(m4*, v4*, v4*, v4*);


void frustum(m4*,
             GLfloat, GLfloat, GLfloat,
             GLfloat, GLfloat, GLfloat);
