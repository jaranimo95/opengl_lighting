#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>


#include <stdio.h>
#include <string.h>
#include <math.h>

#include "lib.h"


void v4_def(v4* v, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
}

int v4_equals(v4* a, v4* b, float threshold)
{
    if(fabs(a->x - b->x) > threshold)
        return 0;
    if(fabs(a->y - b->y) > threshold)
        return 0;
    if(fabs(a->z - b->z) > threshold)
        return 0;
    if(fabs(a->w - b->w) > threshold)
        return 0;
    return 1;
}

void v4_print(v4* v)
{
  printf("[ %f %f %f %f ]\n", v->x, v->y, v->z, v->w);
}

void v4_copy(v4* d, v4* s)
{
    d->x = s->x;
    d->y = s->y;
    d->z = s->z;
    d->w = s->w;
}

void v4_norm(v4* u, v4* v)
{
    GLfloat d = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    u->x = v->x / d;
    u->y = v->y / d;
    u->z = v->z / d;
    u->w = 1;
}

float v4_angleBetween(v4* v1, v4* v2)
{
    float dot = v1->x * v2->x + v1->y * v2->y +v1->z * v2->z + v1->w * v2->w;
    float a = sqrt(v1->x * v1->x + v1->y * v1->y + v1->z * v1->z + v1->w * v1->w);
    float b = sqrt(v2->x * v2->x + v2->y * v2->y + v2->z * v2->z + v2->w * v2->w);
    return acos(dot / (a * b));
}

void v4_cross(v4* r, v4* v1, v4* v2)
{
    r->x = v1->y * v2->z - v1->z * v2->y;
    r->y = v1->z * v2->x - v1->x * v2->z;
    r->z = v1->x * v2->y - v1->y * v2->x;
    r->w = 0;
}

GLfloat v4_dot(v4* v1, v4* v2)
{
    return v1->x * v2->x +
        v1->y * v2->y +
        v1->z * v2->z +
        v1->w * v2->w;
}

void v4_scalar(v4* r, GLfloat s, v4* v)
{
    r->x = s * v->x;
    r->y = s * v->y;
    r->z = s * v->z;
    r->w = s * v->w;
}

void v4_add(v4* r, v4* v1, v4* v2)
{
    r->x = v1->x + v2->x;
    r->y = v1->y + v2->y;
    r->z = v1->z + v2->z;
    r->w = v1->w + v2->w;
}

void v4_sub(v4* r, v4* v1, v4* v2)
{
    r->x = v1->x - v2->x;
    r->y = v1->y - v2->y;
    r->z = v1->z - v2->z;
    r->w = v1->w - v2->w;
}

void v4_mult(v4* r, v4* v1, v4* v2)
{
    r->x = v1->x * (v2->x + v2->y + v2->z + v2->w);
    r->y = v1->y * (v2->x + v2->y + v2->z + v2->w);
    r->z = v1->z * (v2->x + v2->y + v2->z + v2->w);
	r->w = v1->w * (v2->x + v2->y + v2->z + v2->w);
}

void m4_copy(m4* d, m4* s)
{
    d->a.x = s->a.x;
    d->a.y = s->a.y;
    d->a.z = s->a.z;
    d->a.w = s->a.w;

    d->b.x = s->b.x;
    d->b.y = s->b.y;
    d->b.z = s->b.z;
    d->b.w = s->b.w;

    d->c.x = s->c.x;
    d->c.y = s->c.y;
    d->c.z = s->c.z;
    d->c.w = s->c.w;

    d->d.x = s->d.x;
    d->d.y = s->d.y;
    d->d.z = s->d.z;
    d->d.w = s->d.w;
}

void m4_print(m4* m)
{
    printf("[\n");
    printf("[ %5.2f %5.2f %5.2f %5.2f ]\n", m->a.x, m->a.y, m->a.z, m->a.w);
    printf("[ %5.2f %5.2f %5.2f %5.2f ]\n", m->b.x, m->b.y, m->b.z, m->b.w);
    printf("[ %5.2f %5.2f %5.2f %5.2f ]\n", m->c.x, m->c.y, m->c.z, m->c.w);
    printf("[ %5.2f %5.2f %5.2f %5.2f ]\n", m->d.x, m->d.y, m->d.z, m->d.w);
    printf("]\n");
}

void m4_scalar(m4* r, GLfloat s, m4* m)
{
    r->a.x = m->a.x * s;
    r->a.y = m->a.y * s;
    r->a.z = m->a.z * s;
    r->a.w = m->a.w * s;

    r->b.x = m->b.x * s;
    r->b.y = m->b.y * s;
    r->b.z = m->b.z * s;
    r->b.w = m->b.w * s;

    r->c.x = m->c.x * s;
    r->c.y = m->c.y * s;
    r->c.z = m->c.z * s;
    r->c.w = m->c.w * s;

    r->d.x = m->d.x * s;
    r->d.y = m->d.y * s;
    r->d.z = m->d.z * s;
    r->d.w = m->d.w * s;
}

void m4_add(m4* r, m4* m1, m4* m2)
{
    r->a.x = m1->a.x + m2->a.x;
    r->a.y = m1->a.y + m2->a.x;
    r->a.z = m1->a.z + m2->a.x;
    r->a.w = m1->a.w + m2->a.x;

    r->b.x = m1->b.x + m2->b.x;
    r->b.y = m1->b.y + m2->b.y;
    r->b.z = m1->b.z + m2->b.z;
    r->b.w = m1->b.w + m2->b.w;

    r->c.x = m1->c.x + m2->c.x;
    r->c.y = m1->c.y + m2->c.y;
    r->c.z = m1->c.z + m2->c.z;
    r->c.w = m1->c.w + m2->c.w;

    r->d.x = m1->d.x + m2->d.x;
    r->d.y = m1->d.y + m2->d.y;
    r->d.z = m1->d.z + m2->d.z;
    r->d.w = m1->d.w + m2->d.w;
}

void m4_sub(m4* r, m4* m1, m4* m2)
{
    r->a.x = m1->a.x - m2->a.x;
    r->a.y = m1->a.y - m2->a.y;
    r->a.z = m1->a.z - m2->a.z;
    r->a.w = m1->a.w - m2->a.w;

    r->b.x = m1->b.x - m2->b.x;
    r->b.y = m1->b.y - m2->b.y;
    r->b.z = m1->b.z - m2->b.z;
    r->b.w = m1->b.w - m2->b.w;

    r->c.x = m1->c.x - m2->c.x;
    r->c.y = m1->c.y - m2->c.y;
    r->c.z = m1->c.z - m2->c.z;
    r->c.w = m1->c.w - m2->c.w;

    r->d.x = m1->d.x - m2->d.x;
    r->d.y = m1->d.y - m2->d.y;
    r->d.z = m1->d.z - m2->d.z;
    r->d.w = m1->d.w - m2->d.w;
}

void m4_mult(m4* r, m4* m1, m4* m2)
{
    r->a.x = m1->a.x * m2->a.x + m1->a.y * m2->b.x + m1->a.z * m2->c.x + m1->a.w * m2->d.x;
    r->a.y = m1->a.x * m2->a.y + m1->a.y * m2->b.y + m1->a.z * m2->c.y + m1->a.w * m2->d.y;
    r->a.z = m1->a.x * m2->a.z + m1->a.y * m2->b.z + m1->a.z * m2->c.z + m1->a.w * m2->d.z;
    r->a.w = m1->a.x * m2->a.w + m1->a.y * m2->b.w + m1->a.z * m2->c.w + m1->a.w * m2->d.w;

    r->b.x = m1->b.x * m2->a.x + m1->b.y * m2->b.x + m1->b.z * m2->c.x + m1->b.w * m2->d.x;
    r->b.y = m1->b.x * m2->a.y + m1->b.y * m2->b.y + m1->b.z * m2->c.y + m1->b.w * m2->d.y;
    r->b.z = m1->b.x * m2->a.z + m1->b.y * m2->b.z + m1->b.z * m2->c.z + m1->b.w * m2->d.z;
    r->b.w = m1->b.x * m2->a.w + m1->b.y * m2->b.w + m1->b.z * m2->c.w + m1->b.w * m2->d.w;

    r->c.x = m1->c.x * m2->a.x + m1->c.y * m2->b.x + m1->c.z * m2->c.x + m1->c.w * m2->d.x;
    r->c.y = m1->c.x * m2->a.y + m1->c.y * m2->b.y + m1->c.z * m2->c.y + m1->c.w * m2->d.y;
    r->c.z = m1->c.x * m2->a.z + m1->c.y * m2->b.z + m1->c.z * m2->c.z + m1->c.w * m2->d.z;
    r->c.w = m1->c.x * m2->a.w + m1->c.y * m2->b.w + m1->c.z * m2->c.w + m1->c.w * m2->d.w;

    r->d.x = m1->d.x * m2->a.x + m1->d.y * m2->b.x + m1->d.z * m2->c.x + m1->d.w * m2->d.x;
    r->d.y = m1->d.x * m2->a.y + m1->d.y * m2->b.y + m1->d.z * m2->c.y + m1->d.w * m2->d.y;
    r->d.z = m1->d.x * m2->a.z + m1->d.y * m2->b.z + m1->d.z * m2->c.z + m1->d.w * m2->d.z;
    r->d.w = m1->d.x * m2->a.w + m1->d.y * m2->b.w + m1->d.z * m2->c.w + m1->d.w * m2->d.w;
}

void m4_inverse(m4* r, m4* A)
{
    GLfloat detA = A->a.x * A->b.y * A->c.z * A->d.w
                + A->a.x * A->b.z * A->c.w * A->d.y
                + A->a.x * A->b.w * A->c.y * A->d.z
                + A->a.y * A->b.x * A->c.w * A->d.z
                + A->a.y * A->b.z * A->c.x * A->d.w
                + A->a.y * A->b.w * A->c.z * A->d.x
                + A->a.z * A->b.x * A->c.y * A->d.w
                + A->a.z * A->b.y * A->c.w * A->d.x
                + A->a.z * A->b.w * A->c.x * A->d.y
                + A->a.w * A->b.x * A->c.z * A->d.y
                + A->a.w * A->b.y * A->c.x * A->d.z
                + A->a.w * A->b.z * A->c.y * A->d.x
                - A->a.x * A->b.y * A->c.w * A->d.z
                - A->a.x * A->b.z * A->c.y * A->d.w
                - A->a.x * A->b.w * A->c.z * A->d.y
                - A->a.y * A->b.x * A->c.z * A->d.w
                - A->a.y * A->b.z * A->c.w * A->d.x
                - A->a.y * A->b.w * A->c.x * A->d.z
                - A->a.z * A->b.x * A->c.w * A->d.y
                - A->a.z * A->b.y * A->c.x * A->d.w
                - A->a.z * A->b.w * A->c.y * A->d.x
                - A->a.w * A->b.x * A->c.y * A->c.w
                - A->a.w * A->b.y * A->c.z * A->d.x
                - A->a.w * A->b.z * A->c.x * A->d.y;

    r->a.x = 1 / detA * (A->b.y * A->c.z * A->d.w + A->b.z * A->c.w * A->d.y + A->b.w * A->c.y * A->d.z - A->b.y * A->c.w * A->d.z - A->b.z * A->c.y * A->d.w - A->b.w * A->c.z * A->d.y);
    r->a.y = 1 / detA * (A->a.y * A->c.w * A->d.z + A->a.z * A->c.y * A->d.w + A->a.w * A->c.z * A->d.y - A->a.y * A->c.z * A->d.w - A->a.z * A->c.w * A->c.y - A->a.w * A->c.y * A->d.z);
    r->a.z = 1 / detA * (A->a.y * A->b.z * A->d.w + A->a.z * A->b.w * A->d.y + A->a.w * A->b.y * A->d.z - A->a.y * A->b.w * A->d.z - A->a.z * A->b.y * A->d.w - A->a.w * A->b.z * A->d.y);
    r->a.w = 1 / detA * (A->a.y * A->b.w * A->c.z + A->a.z * A->b.y * A->c.w + A->a.w * A->b.z * A->c.y - A->a.y * A->b.z * A->c.w - A->a.z * A->b.w * A->c.y - A->a.w * A->b.y * A->c.z);
    r->b.x = 1 / detA * (A->b.x * A->c.w * A->d.z + A->b.z * A->c.x * A->d.w + A->b.w * A->c.z * A->d.x - A->b.x * A->c.z * A->d.w - A->b.z * A->c.w * A->d.x - A->b.w * A->c.x * A->d.z);
    r->b.y = 1 / detA * (A->a.x * A->c.z * A->d.w + A->a.z * A->c.w * A->d.x + A->a.w * A->c.x * A->d.z - A->a.x * A->c.w * A->d.z - A->a.z * A->c.x * A->d.w - A->a.w * A->c.z * A->d.x);
    r->b.z = 1 / detA * (A->a.x * A->b.w * A->d.z + A->a.z * A->b.x * A->d.w + A->a.w * A->b.z * A->d.x - A->a.x * A->b.z * A->d.w - A->a.z * A->b.w * A->d.x - A->a.w * A->b.x * A->d.z);
    r->b.w = 1 / detA * (A->a.x * A->b.z * A->c.w + A->a.z * A->b.w * A->c.x + A->a.w * A->b.x * A->c.z - A->a.x * A->b.w * A->c.z - A->a.z * A->b.x * A->c.w - A->a.w * A->b.z * A->c.x);
    r->c.x = 1 / detA * (A->b.x * A->c.y * A->d.w + A->b.y * A->c.w * A->d.x + A->b.w * A->c.x * A->d.y - A->b.x * A->c.w * A->d.y - A->b.y * A->c.x * A->d.w - A->b.w * A->c.y * A->d.x);
    r->c.y = 1 / detA * (A->a.x * A->c.w * A->d.y + A->a.y * A->c.x * A->d.w + A->a.w * A->c.y * A->d.x - A->a.x * A->c.y * A->d.w - A->a.y * A->c.w * A->d.x - A->a.w * A->c.x * A->d.y);
    r->c.z = 1 / detA * (A->a.x * A->b.y * A->d.w + A->a.y * A->b.w * A->d.x + A->a.w * A->b.x * A->d.y - A->a.x * A->b.w * A->d.y - A->a.y * A->b.x * A->d.w - A->a.w * A->b.y * A->d.x);
    r->c.w = 1 / detA * (A->a.x * A->b.w * A->c.y + A->a.y * A->b.x * A->c.w + A->a.w * A->b.y * A->c.x - A->a.x * A->b.y * A->c.w - A->a.y * A->b.w * A->c.x - A->a.w * A->b.x * A->c.y);
    r->d.x = 1 / detA * (A->b.x * A->c.z * A->d.z + A->b.y * A->c.x * A->c.w + A->b.z * A->c.y * A->d.x - A->b.x * A->c.y * A->d.z - A->b.y * A->c.z * A->d.x - A->b.z * A->c.x * A->d.y);
    r->d.y = 1 / detA * (A->a.x * A->c.y * A->d.z + A->a.y * A->c.z * A->d.x + A->a.z * A->d.x * A->d.y - A->a.x * A->c.z * A->d.y - A->a.y * A->c.x * A->c.w - A->a.z * A->c.y * A->d.x);
    r->d.z = 1 / detA * (A->a.x * A->b.z * A->d.y + A->a.y * A->b.x * A->d.z + A->a.z * A->b.y * A->d.x - A->a.x * A->b.y * A->d.z - A->a.y * A->b.z * A->d.x - A->a.z * A->b.x * A->d.y);
    r->d.w = 1 / detA * (A->a.x * A->b.y * A->c.z + A->a.y * A->b.z * A->c.x + A->a.z * A->b.x * A->c.y - A->a.x * A->b.z * A->c.y - A->a.y * A->b.x * A->c.z - A->a.z * A->b.y * A->c.x);
}

void m4_transpose(m4* r, m4* m)
{
    r->a.x = m->a.x;
    r->a.y = m->b.x;
    r->a.z = m->c.x;
    r->a.w = m->d.x;

    r->b.x = m->a.y;
    r->b.y = m->b.y;
    r->b.z = m->c.y;
    r->b.w = m->d.y;

    r->c.x = m->a.z;
    r->c.y = m->b.z;
    r->c.z = m->c.z;
    r->c.w = m->d.z;

    r->d.x = m->a.w;
    r->d.y = m->b.w;
    r->d.z = m->c.w;
    r->d.w = m->d.w;
}

void m4v4(v4* r, m4* m, v4* v)
{
    r->x = m->a.x * v->x + m->a.y * v->y + m->a.z * v->z + m->a.w * v->w;
    r->y = m->b.x * v->x + m->b.y * v->y + m->b.z * v->z + m->b.w * v->w;
    r->z = m->c.x * v->x + m->c.y * v->y + m->c.z * v->z + m->c.w * v->w;
    r->w = m->d.x * v->x + m->d.y * v->y + m->d.z * v->z + m->d.w * v->w;
}

void m4_scale(m4* r, m4* m, GLfloat f)
{
    m4 sm = {
        f, 0, 0, 0,
        0, f, 0, 0,
        0, 0, f, 0,
        0, 0, 0, 1
    };
    m4_mult(r, m, &sm);
}

void m4_translate(m4* r, m4* m, GLfloat x, GLfloat y, GLfloat z)
{
    m4 tm = {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };
    m4_mult(r, m, &tm);
}

void m4_rotate(m4* r, m4* m, float x, float y, float z)
{
    m4 temp;
    m4 rm;
    m4 xm = {
              1,       0,       0, 0,
              0,  cos(x), -sin(x), 0,
              0,  sin(x),  cos(x), 0,
              0,       0,       0, 1,
    };
    m4 ym = {
         cos(y),       0,  sin(y), 0,
              0,       1,       0, 0,
        -sin(y),       0,  cos(y), 0,
              0,       0,       0, 1,
    };
    m4 zm = {
         cos(z), -sin(z),       0, 0,
         sin(z),  cos(z),       0, 0,
              0,       0,       1, 0,
              0,       0,       0, 1,
    };

    m4_mult(&temp, &ym, &xm);
    rm = temp;
    m4_mult(&temp, &zm, &rm);
    rm = temp;
    m4_mult(r, m, &rm);
}

void apply_tm(v4* v, m4* m)
{
    v4 temp;
    m4v4(&temp, m, v);
    *v = temp;
}

void cube_generate(v4 c[], GLfloat xs, GLfloat ys, GLfloat zs, GLfloat x, GLfloat y, GLfloat z)
{
    v4 t[36] = {
         xs + x, ys + y, zs + z, 1, // front
         xs + x,-ys + y, zs + z, 1,
        -xs + x, ys + y, zs + z, 1,
        -xs + x,-ys + y, zs + z, 1,
        -xs + x, ys + y, zs + z, 1,
         xs + x,-ys + y, zs + z, 1,

         xs + x, ys + y,-zs + z, 1, // back
         xs + x,-ys + y,-zs + z, 1,
        -xs + x, ys + y,-zs + z, 1,
        -xs + x,-ys + y,-zs + z, 1,
        -xs + x, ys + y,-zs + z, 1,
         xs + x,-ys + y,-zs + z, 1,

         xs + x, ys + y, zs + z, 1, // right
         xs + x,-ys + y, zs + z, 1,
         xs + x, ys + y,-zs + z, 1,
         xs + x,-ys + y,-zs + z, 1,
         xs + x, ys + y,-zs + z, 1,
         xs + x,-ys + y, zs + z, 1,

        -xs + x, ys + y, zs + z, 1, // left
        -xs + x,-ys + y, zs + z, 1,
        -xs + x, ys + y,-zs + z, 1,
        -xs + x,-ys + y,-zs + z, 1,
        -xs + x, ys + y,-zs + z, 1,
        -xs + x,-ys + y, zs + z, 1,

         xs + x, ys + y, zs + z, 1, // top
         xs + x, ys + y,-zs + z, 1,
        -xs + x, ys + y, zs + z, 1,
        -xs + x, ys + y,-zs + z, 1,
        -xs + x, ys + y, zs + z, 1,
         xs + x, ys + y,-zs + z, 1,

         xs + x,-ys + y, zs + z, 1, // bottom
         xs + x,-ys + y,-zs + z, 1,
        -xs + x,-ys + y, zs + z, 1,
        -xs + x,-ys + y,-zs + z, 1,
        -xs + x,-ys + y, zs + z, 1,
         xs + x,-ys + y,-zs + z, 1
    };

    memcpy(c, t, sizeof(v4) * 36);
}

void look_at(m4* m, v4* ve, v4* vt, v4* up)
{
    v4 vtemp;

    v4 f;
    v4_sub(&f, ve, vt);
    v4_norm(&vtemp, &f);
    f = vtemp;

    v4 l;
    v4_cross(&l, up, &f);
    v4_norm(&vtemp, &l);
    l = vtemp;

    v4 u;
    v4_cross(&u, &f, &l);

    m4 mtemp = {
        l.x, l.y, l.z, -l.x * ve->x - l.y * ve->y - l.z * ve->z,
        u.x, u.y, u.z, -u.x * ve->x - u.y * ve->y - u.z * ve->z,
        f.x, f.y, f.z, -f.x * ve->x - f.y * ve->y - f.z * ve->z,
        0, 0, 0, 1
    };


    m4_copy(m, &mtemp);
}


void frustum(m4* p,
             GLfloat left, GLfloat right, GLfloat bottom,
             GLfloat top, GLfloat near, GLfloat far)
{
    m4 tempp = {
        {-2 * near / (right - left), 0, (left + right) / (right - left), 0},
        {0, -2 * near / (top - bottom), (bottom + top) / (top - bottom), 0},
        {0, 0, (near + far) / (far - near), (-2 * near * far) / (far - near)},
        {0, 0, -1, 0}
    };

    m4_copy(p, &tempp);
}

void get_mid(v4* c, v4* a, v4* b)
{
    c->x = (a->x + b->x) / 2;
    c->y = (a->y + b->y) / 2;
    c->z = (a->z + b->z) / 2;
    c->w = 1;

    v4 temp;
    v4_norm(&temp, c);
    v4_copy(c, &temp);
}

int sphere_get_num_vertices(int sd)
{
    return 60 * pow(4, sd);
}

void sphere_generate(v4 s[], int sd, float r, float x, float y, float z)
{
    float PHI = 1.6180339887498948482;

    v4 pos[] = {
        -1,   0,   PHI,   1,
         1,   0,   PHI,   1,
        -1,   0,  -PHI,   1,
         1,   0,  -PHI,   1,
         0,   PHI,   1,   1,
         0,   PHI,  -1,   1,
         0,  -PHI,   1,   1,
         0,  -PHI,  -1,   1,
         PHI,   1,   0,   1,
        -PHI,   1,   0,   1,
         PHI,  -1,   0,   1,
        -PHI,  -1,   0,   1
    };

		int k[] = {
         0,  6,  1,
         0,  1,  4,
         0,  4,  9,
         0,  9, 11,
         0, 11,  6,
         6, 10,  1,
         1,  8,  4,
         4,  5,  9,
         9,  2, 11,
        11,  7,  6,
        10,  8,  1,
         8,  5,  4,
         5,  2,  9,
         2,  7, 11,
         7, 10,  6,
        10,  3,  8,
         8,  3,  5,
         5,  3,  2,
         2,  3,  7,
         7,  3, 10
    };


    int num_vertices = 60;

    for(int i = 0; i < num_vertices; i++)
    {
        v4 temp;
        v4_norm(&temp, &pos[k[i]]);
        s[i] = temp;
    }

    for(int i = 0; i < sd; i++)
    {
        num_vertices = sub_divide(s, num_vertices);
    }

    m4 identity = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
    m4 temp;
    m4 tm;
    m4_translate(&tm, &identity, x, y, z);
    m4_scale(&temp, &tm, r);
    tm = temp;
    for(int i = 0; i < num_vertices; i++)
    {
        apply_tm(&s[i], &tm);
    }

    // return num_vertices;
}

int sub_divide(v4 s[], int num_vertices)
{
    v4* temp_s = malloc(sizeof(v4) * num_vertices * 4);

    for(int i = 0, j = 0; i < num_vertices; i += 3)
    {
        v4 temp;
        v4 a;
        get_mid(&a, &s[i], &s[i + 1]);
        v4 b;
        get_mid(&b, &s[i], &s[i + 2]);
        v4 c;
        get_mid(&c, &s[i + 1], &s[i + 2]);

        temp_s[j++] = s[i];
        temp_s[j++] = a;
        temp_s[j++] = b;

        temp_s[j++] = s[i + 1];
        temp_s[j++] = c;
        temp_s[j++] = a;

        temp_s[j++] = s[i + 2];
        temp_s[j++] = c;
        temp_s[j++] = b;

        temp_s[j++] = a;
        temp_s[j++] = b;
        temp_s[j++] = c;
    }

    memcpy(s, temp_s, sizeof(v4) * num_vertices * 4);

    free(temp_s);


    return num_vertices * 4;
}
