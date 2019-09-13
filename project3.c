#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lib.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

m4 identity = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

v4 zero = { 0, 0, 0, 0 };

GLuint ctm_location;

m4 sphere1_ctm;
m4 sphere2_ctm;
m4 sphere3_ctm;
m4 sphere4_ctm;
m4 sphere5_ctm;
m4 light_ctm;

m4 model_view;
m4 projection;

GLuint model_view_location;
GLuint projection_location;

int vertices_length;
int colors_length;
v4* vertices;
v4* normals;

v4 eye;
v4 at;
v4 up;

float eye_y_angle = M_PI/4;
float eye_xz_angle = 0;

float camera_distance = 10;

int sub_divisions = 2;
int num_sphere_vertices;
int num_vertices;

int index_board;
int index_sphere1;
int index_sphere2;
int index_sphere3;
int index_sphere4;
int index_sphere5;
int index_light;

GLuint light_position_location;
GLuint isShadow_location;

v4 light_start = { 0, 3, 0, 1 };
v4 light_position;

v4 light_diffuse = { 1.0, 1.0, 1.0, 1.0 };
v4 light_specular = { 1.0, 1.0, 1.0, 1.0 };
v4 light_ambient = { 0.2, 0.2, 0.2, 1.0 };

typedef struct
{
    v4 reflect_ambient;
    v4 reflect_diffuse;
    v4 reflect_specular;
    GLfloat shininess;
} material;

material material_board = {{0, 0.5, 0, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, 10};
material material_sphere1 = {{1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material material_sphere2 = {{0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material material_sphere3 = {{0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material material_sphere4 = {{1.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material material_sphere5 = {{1.0, 0.5, 0.0, 1.0}, {1.0, 0.5, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material material_light = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, 10};

GLuint ap_location;
GLuint dp_location;
GLuint sp_location;

GLuint ac_location;
GLuint al_location;
GLuint aq_location;

GLuint shininess_location;

void init(void)
{
    num_sphere_vertices = sphere_get_num_vertices(sub_divisions);

    num_vertices = 36 + 6 * num_sphere_vertices;
    vertices_length = sizeof(v4) * num_vertices;

    vertices = malloc(sizeof(v4) * num_vertices);
    normals = malloc(sizeof(v4) * num_vertices);

    index_board = 0;
    cube_generate(vertices + index_board, 20, 0, 20, 0, 0, 0);

    index_sphere1 = index_board + 36;
    sphere_generate(vertices + index_sphere1, sub_divisions, 0.5, 0, 0.5, 0);

    index_sphere2 = index_sphere1 + num_sphere_vertices;
    sphere_generate(vertices + index_sphere2, sub_divisions, 0.5, 0, 0.5, 0);

    index_sphere3 = index_sphere2 + num_sphere_vertices;
    sphere_generate(vertices + index_sphere3, sub_divisions, 0.5, 0, 0.5, 0);

    index_sphere4 = index_sphere3 + num_sphere_vertices;
    sphere_generate(vertices + index_sphere4, sub_divisions, 0.5, 0, 0.5, 0);

    index_sphere5 = index_sphere4 + num_sphere_vertices;
    sphere_generate(vertices + index_sphere5, sub_divisions, 0.5, 0, 0.5, 0);

    index_light = index_sphere5 + num_sphere_vertices;
    sphere_generate(vertices + index_light, sub_divisions, 0.3, 0, 0, 0);

    m4_translate(&sphere1_ctm, &identity, 0, 0, 0);
    m4_translate(&sphere2_ctm, &identity, 1, 0, 0);
    m4_translate(&sphere3_ctm, &identity, 2, 0, 0);
    m4_translate(&sphere4_ctm, &identity, 3, 0, 0);
    m4_translate(&sphere5_ctm, &identity, 4, 0, 0);
    m4_translate(&light_ctm, &identity, 0, 3, 0);
    light_position = light_start;

    v4_def(&eye, camera_distance * cos(eye_y_angle) * cos(eye_xz_angle), camera_distance * sin(eye_y_angle), camera_distance * cos(eye_y_angle) * sin(eye_xz_angle), 0);
    v4_def(&at, 0, 0, 0, 0);
    v4_def(&up, 0, 1, 0, 0);

    look_at(&model_view, &eye, &at, &up);
    frustum(&projection, -1, 1, -1, 1, -1, -200);

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices_length * 2, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_length, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, vertices_length, vertices_length, normals);

    model_view_location  = glGetUniformLocation(program, "model_view_matrix");
    projection_location  = glGetUniformLocation(program, "projection_matrix");

    ctm_location  = glGetUniformLocation(program, "ctm");

    light_position_location  = glGetUniformLocation(program, "light_position");
    isShadow_location  = glGetUniformLocation(program, "isShadow");

    ap_location  = glGetUniformLocation(program, "AmbientProduct");
    dp_location = glGetUniformLocation(program, "DiffuseProduct");
    sp_location = glGetUniformLocation(program, "SpecularProduct");

    ac_location  = glGetUniformLocation(program, "attenuation_constant");
    al_location  = glGetUniformLocation(program, "attenuation_linear");
    aq_location  = glGetUniformLocation(program, "attenuation_quadratic");


    shininess_location = glGetUniformLocation(program, "shininess");

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnable(GL_DEPTH_TEST);
    /* glEnable(GL_CULL_FACE); */
    glClearColor(0, 0, 0, 1.0);
    glDepthRange(1,0);
}

v4 product(v4 u, v4 v)
{
    v4 result;
    result.x = u.x * v.x;
    result.y = u.y * v.y;
    result.z = u.z * v.z;
    result.w = u.w * v.w;
    return result;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(projection_location, 1, GL_TRUE, (GLfloat *) &projection);
    glUniformMatrix4fv(model_view_location, 1, GL_TRUE, (GLfloat *) &model_view);

    glUniform4fv(light_position_location, 1, (GLfloat *) &light_position);

    for(int i = 0; i < num_vertices; i++)
    {
        normals[i] = vertices[i];
    }


    v4 AmbientProduct;
    v4 DiffuseProduct;
    v4 SpecularProduct;

    glUniform1f(ac_location, 1.f);
    glUniform1f(al_location, 0.01);
    glUniform1f(aq_location, 0.01);

    glUniform1i(isShadow_location, 0);
    glUniformMatrix4fv(ctm_location, 1, GL_TRUE, (GLfloat *) &identity);
    AmbientProduct = product(material_board.reflect_ambient, light_ambient);
    DiffuseProduct = product(material_board.reflect_diffuse, light_diffuse);
    SpecularProduct = product(material_board.reflect_specular, light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) &AmbientProduct);
    glUniform4fv(dp_location, 1, (GLfloat *) &DiffuseProduct);
    glUniform4fv(sp_location, 1, (GLfloat *) &SpecularProduct);
    glUniform1fv(shininess_location, 1, (GLfloat *) &material_board.shininess);
    glDrawArrays(GL_TRIANGLES, index_board, 36);

    glUniform1i(isShadow_location, 0);
    glUniformMatrix4fv(ctm_location, 1, GL_TRUE, (GLfloat *) &sphere1_ctm);
    AmbientProduct = product(material_sphere1.reflect_ambient, light_ambient);
    DiffuseProduct = product(material_sphere1.reflect_diffuse, light_diffuse);
    SpecularProduct = product(material_sphere1.reflect_specular, light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) &AmbientProduct);
    glUniform4fv(dp_location, 1, (GLfloat *) &DiffuseProduct);
    glUniform4fv(sp_location, 1, (GLfloat *) &SpecularProduct);
    glUniform1fv(shininess_location, 1, (GLfloat *) &material_sphere1.shininess);
    glDrawArrays(GL_TRIANGLES, index_sphere1, num_sphere_vertices);
    glUniform1i(isShadow_location, 1);
    glDrawArrays(GL_TRIANGLES, index_sphere1, num_sphere_vertices);

    glUniform1i(isShadow_location, 0);
    glUniformMatrix4fv(ctm_location, 1, GL_TRUE, (GLfloat *) &sphere2_ctm);
    AmbientProduct = product(material_sphere2.reflect_ambient, light_ambient);
    DiffuseProduct = product(material_sphere2.reflect_diffuse, light_diffuse);
    SpecularProduct = product(material_sphere2.reflect_specular, light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) &AmbientProduct);
    glUniform4fv(dp_location, 1, (GLfloat *) &DiffuseProduct);
    glUniform4fv(sp_location, 1, (GLfloat *) &SpecularProduct);
    glUniform1fv(shininess_location, 1, (GLfloat *) &material_sphere2.shininess);
    glDrawArrays(GL_TRIANGLES, index_sphere2, num_sphere_vertices);
    glUniform1i(isShadow_location, 1);
    glDrawArrays(GL_TRIANGLES, index_sphere2, num_sphere_vertices);

    glUniform1i(isShadow_location, 0);
    glUniformMatrix4fv(ctm_location, 1, GL_TRUE, (GLfloat *) &sphere3_ctm);
    AmbientProduct = product(material_sphere3.reflect_ambient, light_ambient);
    DiffuseProduct = product(material_sphere3.reflect_diffuse, light_diffuse);
    SpecularProduct = product(material_sphere3.reflect_specular, light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) &AmbientProduct);
    glUniform4fv(dp_location, 1, (GLfloat *) &DiffuseProduct);
    glUniform4fv(sp_location, 1, (GLfloat *) &SpecularProduct);
    glUniform1fv(shininess_location, 1, (GLfloat *) &material_sphere3.shininess);
    glDrawArrays(GL_TRIANGLES, index_sphere3, num_sphere_vertices);
    glUniform1i(isShadow_location, 1);
    glDrawArrays(GL_TRIANGLES, index_sphere3, num_sphere_vertices);

    glUniform1i(isShadow_location, 0);
    glUniformMatrix4fv(ctm_location, 1, GL_TRUE, (GLfloat *) &sphere4_ctm);
    AmbientProduct = product(material_sphere4.reflect_ambient, light_ambient);
    DiffuseProduct = product(material_sphere4.reflect_diffuse, light_diffuse);
    SpecularProduct = product(material_sphere4.reflect_specular, light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) &AmbientProduct);
    glUniform4fv(dp_location, 1, (GLfloat *) &DiffuseProduct);
    glUniform4fv(sp_location, 1, (GLfloat *) &SpecularProduct);
    glUniform1fv(shininess_location, 1, (GLfloat *) &material_sphere4.shininess);
    glDrawArrays(GL_TRIANGLES, index_sphere4, num_sphere_vertices);
    glUniform1i(isShadow_location, 1);
    glDrawArrays(GL_TRIANGLES, index_sphere4, num_sphere_vertices);

    glUniform1i(isShadow_location, 0);
    glUniformMatrix4fv(ctm_location, 1, GL_TRUE, (GLfloat *) &sphere5_ctm);
    AmbientProduct = product(material_sphere5.reflect_ambient, light_ambient);
    DiffuseProduct = product(material_sphere5.reflect_diffuse, light_diffuse);
    SpecularProduct = product(material_sphere5.reflect_specular, light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) &AmbientProduct);
    glUniform4fv(dp_location, 1, (GLfloat *) &DiffuseProduct);
    glUniform4fv(sp_location, 1, (GLfloat *) &SpecularProduct);
    glUniform1fv(shininess_location, 1, (GLfloat *) &material_sphere5.shininess);
    glDrawArrays(GL_TRIANGLES, index_sphere5, num_sphere_vertices);
    glUniform1i(isShadow_location, 1);
    glDrawArrays(GL_TRIANGLES, index_sphere5, num_sphere_vertices);

    glUniform1i(isShadow_location, 0);
    AmbientProduct = product(material_light.reflect_ambient, light_ambient);
    DiffuseProduct = product(material_light.reflect_diffuse, light_diffuse);
    SpecularProduct = product(material_light.reflect_specular, light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) &AmbientProduct);
    glUniform4fv(dp_location, 1, (GLfloat *) &DiffuseProduct);
    glUniform4fv(sp_location, 1, (GLfloat *) &SpecularProduct);
    glUniform1fv(shininess_location, 1, (GLfloat *) &material_light.shininess);
    glUniformMatrix4fv(ctm_location, 1, GL_TRUE, (GLfloat *) &light_ctm);
    glDrawArrays(GL_TRIANGLES, index_light, num_sphere_vertices);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    m4 temp = light_ctm;

    if(key == 'q')
    {
        exit(0);
    }
    else if(key == 'i')
    {
        m4_translate(&temp, &light_ctm, 0, 1, 0);
    }
    else if(key == 'k')
    {
        m4_translate(&temp, &light_ctm, 0, -1, 0);
    }
    else if(key == 'j')
    {
        m4_translate(&temp, &light_ctm, 1, 0, 0);
    }
    else if(key == 'l')
    {
        m4_translate(&temp, &light_ctm, -1, 0, 0);
    }
    else if(key == 'u')
    {
        m4_translate(&temp, &light_ctm, 0, 0, 1);
    }
    else if(key == 'o')
    {
        m4_translate(&temp, &light_ctm, 0, 0, -1);
    }
    else if(key == 'w')
    {
        if(eye_y_angle < M_PI / 2 - 0.05)
        {
            eye_y_angle += 0.05;
        }
    }
    else if(key == 's')
    {
        if(eye_y_angle > 0.05)
        {
            eye_y_angle -= 0.05;
        }
    }
    else if(key == 'a')
    {
        eye_xz_angle += 0.05;
    }
    else if(key == 'd')
    {
        eye_xz_angle -= 0.05;
    }

    light_ctm = temp;
    v4 vtemp;
    m4v4(&vtemp, &light_ctm, &light_start);
    light_position = vtemp;

    v4_def(&eye, camera_distance * cos(eye_y_angle) * cos(eye_xz_angle), camera_distance * sin(eye_y_angle), camera_distance * cos(eye_y_angle) * sin(eye_xz_angle), 0);
    look_at(&model_view, &eye, &at, &up);

    glutPostRedisplay();
}

float d;
void idle()
{
    d += 1.0 / 200;

    m4_translate(&sphere2_ctm, &identity, 1 * sin(d * 8), 0, 1 * cos(d * 8));
    m4_translate(&sphere3_ctm, &identity, 2 * sin(d * 4), 0, 2 * cos(d * 4));
    m4_translate(&sphere4_ctm, &identity, 3 * sin(d * 2), 0, 3 * cos(d * 2));
    m4_translate(&sphere5_ctm, &identity, 4 * sin(d), 0, 4 * cos(d));

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lighting Model");
    // glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
