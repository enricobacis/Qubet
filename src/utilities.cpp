//    Qubet  -  Copyright (C) 2011
//    Enrico Bacis
//    Daniele Ciriello

//    Qubet is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "utilities.h"
#include "defines.h"

GLvoid drawRectangle(GLfloat x, GLfloat y, GLuint texture, bool repeat)
{
    x = x / 2;
    y = y / 2;

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture);
    setTextureRepeat(repeat);

    glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);                   glVertex3f(-x, -y,  0.0f);
        glTexCoord2f(1.0f, 0.0f);                   glVertex3f( x, -y,  0.0f);
        glTexCoord2f(1.0f, repeat ? y/x : 1.0f);    glVertex3f( x,  y,  0.0f);
        glTexCoord2f(0.0f, repeat ? y/x : 1.0f);    glVertex3f(-x,  y,  0.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

GLvoid drawPrism(GLfloat x, GLfloat y, GLfloat z, Skin *skin, bool invertBackTexture, bool repeat)
{
    x = x / 2;
    y = y / 2;
    z = z / 2;

    glEnable(GL_TEXTURE_2D);

    // Front Face (Z+)
    glBindTexture(GL_TEXTURE_2D, (skin == NULL ? 0 : skin->getTextureZPlus()));
    setTextureRepeat(repeat);

    glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y,  z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);

    glEnd();

    // Back Face (Z-)
    glBindTexture(GL_TEXTURE_2D, (skin == NULL ? 0 : skin->getTextureZMinus()));
    setTextureRepeat(repeat);

    glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f,-1.0f);

        if (invertBackTexture)
        {
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-x, -y, -z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-x,  y, -z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( x,  y, -z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( x, -y, -z);
        }
        else
        {
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);
        }

    glEnd();

    // Top Face (Y+)
    glBindTexture(GL_TEXTURE_2D, (skin == NULL ? 0 : skin->getTextureYPlus()));
    setTextureRepeat(repeat);

    glBegin(GL_QUADS);

        glNormal3f( 0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, repeat ? z/x : 1.0f);    glVertex3f(-x,  y, -z);
        glTexCoord2f(0.0f, 0.0f);                   glVertex3f(-x,  y,  z);
        glTexCoord2f(1.0f, 0.0f);                   glVertex3f( x,  y,  z);
        glTexCoord2f(1.0f, repeat ? z/x : 1.0f);    glVertex3f( x,  y, -z);

    glEnd();

    // Bottom Face (Y-)
    glBindTexture(GL_TEXTURE_2D, (skin == NULL ? 0 : skin->getTextureYMinus()));
    setTextureRepeat(repeat);

    glBegin(GL_QUADS);

        glNormal3f( 0.0f,-1.0f, 0.0f);

        if (invertBackTexture)
        {
            glTexCoord2f(0.0f, 0.0f);                   glVertex3f(-x, -y, -z);
            glTexCoord2f(1.0f, 0.0f);                   glVertex3f( x, -y, -z);
            glTexCoord2f(1.0f, repeat ? z/x : 1.0f);    glVertex3f( x, -y,  z);
            glTexCoord2f(0.0f, repeat ? z/x : 1.0f);    glVertex3f(-x, -y,  z);
        }
        else
        {
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
        }

    glEnd();

    // Right face (X+)
    glBindTexture(GL_TEXTURE_2D, (skin == NULL ? 0 : skin->getTextureXPlus()));
    setTextureRepeat(repeat);

    glBegin(GL_QUADS);

        glNormal3f( 1.0f, 0.0f, 0.0f);
        glTexCoord2f(repeat ? z/y : 1.0f, 0.0f);    glVertex3f( x, -y, -z);
        glTexCoord2f(repeat ? z/y : 1.0f, 1.0f);    glVertex3f( x,  y, -z);
        glTexCoord2f(0.0f, 1.0f);                   glVertex3f( x,  y,  z);
        glTexCoord2f(0.0f, 0.0f);                   glVertex3f( x, -y,  z);

    glEnd();

    // Left Face (X-)
    glBindTexture(GL_TEXTURE_2D, (skin == NULL ? 0 : skin->getTextureXMinus()));
    setTextureRepeat(repeat);

    glBegin(GL_QUADS);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);                   glVertex3f(-x, -y, -z);
        glTexCoord2f(repeat ? z/y : 1.0f, 0.0f);    glVertex3f(-x, -y,  z);
        glTexCoord2f(repeat ? z/y : 1.0f, 1.0f);    glVertex3f(-x,  y,  z);
        glTexCoord2f(0.0f, 1.0f);                   glVertex3f(-x,  y, -z);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

GLvoid setTextureRepeat(bool repeat)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
}

GLvoid drawObstacle(GLuint id)
{
    switch (id)
    {
    case 0:
        drawPrism(3.0f, 3.0f, 3.0f);
        break;

    case 1:
        drawPrism(3.0f, 6.0f, 3.0f);
        break;

    case 2:
        glBegin(GL_QUADS);

            glNormal3f(-1.0f,  0.0f,  0.0f);
            glVertex3f(-3.0f, -3.0f,  1.5f);
            glVertex3f(-3.0f, -3.0f, -1.5f);
            glVertex3f(-3.0f,  0.0f, -1.5f);
            glVertex3f(-3.0f,  0.0f,  1.5f);

            glNormal3f( 0.0f,  1.0f,  0.0f);
            glVertex3f(-3.0f,  0.0f,  1.5f);
            glVertex3f(-3.0f,  0.0f, -1.5f);
            glVertex3f( 0.0f,  0.0f, -1.5f);
            glVertex3f( 0.0f,  0.0f,  1.5f);

            glNormal3f(-1.0f,  0.0f,  0.0f);
            glVertex3f( 0.0f,  0.0f,  1.5f);
            glVertex3f( 0.0f,  0.0f, -1.5f);
            glVertex3f( 0.0f,  3.0f, -1.5f);
            glVertex3f( 0.0f,  3.0f,  1.5f);

            glNormal3f( 0.0f,  1.0f,  0.0f);
            glVertex3f( 0.0f,  3.0f,  1.5f);
            glVertex3f( 0.0f,  3.0f, -1.5f);
            glVertex3f( 3.0f,  3.0f, -1.5f);
            glVertex3f( 3.0f,  3.0f,  1.5f);

            glNormal3f( 1.0f,  0.0f,  0.0f);
            glVertex3f( 3.0f,  3.0f,  1.5f);
            glVertex3f( 3.0f,  3.0f, -1.5f);
            glVertex3f( 3.0f, -3.0f, -1.5f);
            glVertex3f( 3.0f, -3.0f,  1.5f);

            glNormal3f( 0.0f, -1.0f,  0.0f);
            glVertex3f( 3.0f, -3.0f,  1.5f);
            glVertex3f( 3.0f, -3.0f, -1.5f);
            glVertex3f(-3.0f, -3.0f, -1.5f);
            glVertex3f(-3.0f, -3.0f,  1.5f);

        glEnd();

        glBegin(GL_TRIANGLE_STRIP);

            glNormal3f( 0.0f,  0.0f,  1.0f);
            glVertex3f(-3.0f, -3.0f,  1.5f);
            glVertex3f(-3.0f,  0.0f,  1.5f);
            glVertex3f( 3.0f, -3.0f,  1.5f);
            glVertex3f( 0.0f,  0.0f,  1.5f);
            glVertex3f( 3.0f,  3.0f,  1.5f);
            glVertex3f( 0.0f,  3.0f,  1.5f);

        glEnd();

        glBegin(GL_TRIANGLE_STRIP);

            glNormal3f( 0.0f,  0.0f, -1.0f);
            glVertex3f(-3.0f, -3.0f, -1.5f);
            glVertex3f(-3.0f,  0.0f, -1.5f);
            glVertex3f( 3.0f, -3.0f, -1.5f);
            glVertex3f( 0.0f,  0.0f, -1.5f);
            glVertex3f( 3.0f,  3.0f, -1.5f);
            glVertex3f( 0.0f,  3.0f, -1.5f);

        glEnd();
        break;

    case 3:
        drawPrism(6.0f, 6.0f, 3.0f);
        break;
    }
}

Vector3f *getModelViewPos(Vector3f *vect, bool computeZDepth)
{
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (GLfloat)vect->x;
    winY = (GLfloat)viewport[3] - vect->y;

    if (computeZDepth)
        glReadPixels(GLint(winX), GLint(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    else
        winZ = vect->z;

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return new Vector3f(posX, posY, posZ);
}

Vector3f *getProjectionPos(Vector3f *vect)
{
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    GLfloat objX, objY, objZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    objX = (GLfloat)vect->x;
    objY = (GLfloat)vect->y;
    objZ = (GLfloat)vect->z;

    gluProject(objX, objY, objZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return new Vector3f(posX, posY, posZ);

    // c'e' da sistemare le coordinate per OpenGL se si vuole fare una funzione generale.
}

Vector3f *getPointFromParametricLine(Vector3f *p0, Vector3f *p1, GLfloat t)
{
    GLfloat xr, yr, zr;

    xr = p0->x + t*(p1->x - p0->x);
    yr = p0->y + t*(p1->y - p0->y);
    zr = p0->z + t*(p1->z - p0->z);

    return new Vector3f(xr, yr, zr);
}

Vector3f *getObstacleBoundingBox(GLuint id)
{
    switch (id)
    {
    case 0:
        return new Vector3f(3.0f, 3.0f, 3.0f);
        break;

    case 1:
        return new Vector3f(3.0f, 6.0f, 3.0f);
        break;

    case 2:
        return new Vector3f(6.0f, 6.0f, 3.0f);
        break;

    case 3:
        return new Vector3f(6.0f, 6.0f, 3.0f);
        break;
    }

    return NULL;
}

GLvoid setMaterialColor(int color)
{
    QVector<GLfloat> mat_ambient;
    QVector<GLfloat> mat_diffuse;
    QVector<GLfloat> mat_specular;
    QVector<GLfloat> mat_emission;

    switch (color)
    {
    case COLOR_DISABLED:
        mat_ambient  << 0.2f << 0.2f << 0.2f << 1.0f;
        mat_diffuse  << 0.8f << 0.8f << 0.8f << 1.0f;
        mat_specular << 0.0f << 0.0f << 0.0f << 1.0f;
        mat_emission << 0.0f << 0.0f << 0.0f << 1.0f;
        break;

    case COLOR_RED:
        mat_ambient  << 0.2f << 0.0f << 0.0f << 1.0f;
        mat_diffuse  << 0.8f << 0.0f << 0.0f << 1.0f;
        mat_specular << 0.1f << 0.0f << 0.0f << 1.0f;
        mat_emission << 0.1f << 0.0f << 0.0f << 1.0f;
        break;

    case COLOR_GREEN:
        mat_ambient  << 0.0f << 0.2f << 0.0f << 1.0f;
        mat_diffuse  << 0.0f << 0.8f << 0.0f << 1.0f;
        mat_specular << 0.0f << 0.1f << 0.0f << 1.0f;
        mat_emission << 0.0f << 0.1f << 0.0f << 1.0f;
        break;

    case COLOR_BLUE:
        mat_ambient  << 0.0f << 0.0f << 0.2f << 1.0f;
        mat_diffuse  << 0.0f << 0.0f << 0.8f << 1.0f;
        mat_specular << 0.0f << 0.0f << 0.1f << 1.0f;
        mat_emission << 0.0f << 0.0f << 0.1f << 1.0f;
        break;
    }

    glMaterialfv(GL_FRONT, GL_AMBIENT,  mat_ambient.data());
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat_diffuse.data());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular.data());
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission.data());
}

GLvoid setMaterialColor(QColor color)
{
    QVector<GLfloat> mat_ambient;
    QVector<GLfloat> mat_diffuse;
    QVector<GLfloat> mat_specular;
    QVector<GLfloat> mat_emission;

    mat_ambient  << color.redF() / 4.0f << color.greenF() / 4.0f << color.blueF() / 4.0f << 1.0f;
    mat_diffuse  << color.redF()        << color.greenF()        << color.blueF()        << 1.0f;
    mat_specular << color.redF() / 8.0f << color.greenF() / 8.0f << color.blueF() / 8.0f << 1.0f;
    mat_emission << color.redF() / 8.0f << color.greenF() / 8.0f << color.blueF() / 8.0f << 1.0f;

    glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat_ambient.data());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_diffuse.data());
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_specular.data());
    glMaterialfv(GL_FRONT, GL_AMBIENT,  mat_emission.data());
}

Vector3f *obstacleCellToPosition(Vector3f *cell, GLuint obstacleModelId)
{
    Vector3f *bounding = getObstacleBoundingBox(obstacleModelId);
    Vector3f *position = new Vector3f();

    position->x =  (bounding->x / 2.0f) + (cell->x * 3.0f);
    position->y =  (bounding->y / 2.0f) + (cell->y * 3.0f);
    position->z = -(bounding->z / 2.0f) - (cell->z * 3.0f);

    return position;
}

Vector3f *obstaclePositionToCell(Vector3f *position, GLuint obstacleModelId)
{
    Vector3f *bounding = getObstacleBoundingBox(obstacleModelId);
    Vector3f *cell = new Vector3f();

    cell->x = (int)(( position->x -(bounding->x / 2.0f)) / 3);
    cell->y = (int)(( position->y -(bounding->y / 2.0f)) / 3);
    cell->z = (int)((-position->z -(bounding->z / 2.0f)) / 3);

    return cell;
}
