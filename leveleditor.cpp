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


#include "leveleditor.h"
#include "defines.h"
#include "leveleditor_defines.h"
#include "effects_defines.h"

LevelEditor::LevelEditor(QMap<GLint,GLuint> &_iconsList, Alphabet *_alphabet, QObject *_parent, Level *_level, bool _audioEnabled, Skin *_asphaltSkin):
    parent(_parent),
    iconsList(_iconsList),
    audioEnabled(_audioEnabled),
    isMoving(false),
    level(_level),
    currentView(SET_NAME_VIEW),
    currentLength(LEVEL_LENGTH_DEFAULT),
    currentWidth(LEVEL_WIDTH_DEFAULT),
    currentGravity(LEVEL_GRAVITY_DEFAULT),
    alphabet(_alphabet),
    volumeSkin(NULL),
    asphaltSkin(_asphaltSkin),
    visible(true),
    visibleTime(0),
    currentName(""),
    currentError(""),
    movingObject(-1),
    positionValid(false),
    xCell(0),
    yCell(0),
    zCell(0),
    lastMouseX(0),
    lastMouseY(0)
{
    connect(this, SIGNAL(playAmbientMusic(QString)), parent, SIGNAL(playAmbientMusic(QString)));
    connect(this, SIGNAL(setMouseMovementTracking(int)), parent, SLOT(setMouseMovementTracking(int)));
    connect(this, SIGNAL(setSkybox(QString)), parent, SLOT(setSkybox(QString)));

    emit playAmbientMusic("editor.mp3");
    emit setSkybox("stars");

    currentActions = new ActionList(INITIAL_MOVEMENT);

    cameraOffset = new Vector3f(-30.0f, -30.0f, 0.0f);
    levelOffset  = new Vector3f(0.0f, -3.0f, 0.0f);

    if (level != NULL)
    {
        // Carica il livello
        level->load();
        currentGravity = level->getGravity();
        currentLength = level->getLength();
        currentWidth = level->getWidth();

        cameraOffset->x = 30.0f;
        currentView = SET_GRAVITY_VIEW;
    }

    angleRotVolumeCube = (audioEnabled ? 0.0f : 90.0f);

    lengthLabel = new CubeString(QString::number(currentLength / 3), 3.0f, alphabet, LENGTH_LABEL);
    widthLabel = new CubeString(QString::number(currentWidth / 3), 3.0f, alphabet, WIDTH_LABEL);
    gravityLabel = new CubeString(QString::number(currentGravity), 3.0f, alphabet, GRAVITY_LABEL);

    lengthString = new CubeString("length", 1.5f, alphabet, LENGTH_STRING);
    widthString = new CubeString("width", 1.5f, alphabet, WIDTH_STRING);
    gravityString = new CubeString("gravity", 1.5f, alphabet, GRAVITY_STRING);

    back = new CubeString("back", 1.0f, alphabet, BUTTON_BACK);
    menu = new CubeString("menu", 1.0f, alphabet, BUTTON_EXIT);
    create = new CubeString("create", 1.0f, alphabet, BUTTON_NEXT);
    next = new CubeString("next", 1.0f, alphabet, BUTTON_NEXT);
    save = new CubeString("save", 0.8f, alphabet, BUTTON_SAVE);
    cancel = new CubeString("cancel", 0.8f, alphabet, BUTTON_CANCEL);
    clear = new CubeString("clear", 0.8f, alphabet, BUTTON_CLEAR);
    exit = new CubeString("exit", 0.8f, alphabet, BUTTON_EXIT);

    labelSetLevelName = new CubeString("level name", 1.3f, alphabet, LABEL_SET_LEVEL_NAME);

    formSetLevelName = new CubeStringList("", 14.0f, 6.0f, alphabet, 2.0f, FORM_SET_LEVEL_NAME);
    GLuint volume_on = iconsList.value(VOLUME_ON);
    GLuint volume_off = iconsList.value(VOLUME_OFF);
    volumeSkin = new Skin(0, 0, volume_off, volume_off, volume_on, volume_on);

    toolbarObstacleCentres.append(new Vector3f(80.0f,  6.0f, 0.0f));
    toolbarObstacleCentres.append(new Vector3f(80.0f,  3.2f, 0.0f));
    toolbarObstacleCentres.append(new Vector3f(80.0f,  0.4f, 0.0f));
    toolbarObstacleCentres.append(new Vector3f(80.0f, -2.4f, 0.0f));
}

LevelEditor::~LevelEditor()
{
    this->disconnect(parent);
    parent->disconnect(this);

    if (lengthLabel != NULL)
        lengthLabel->~CubeString();

    if (widthLabel != NULL)
        widthLabel->~CubeString();

    if (gravityLabel != NULL)
        gravityLabel->~CubeString();

    if (lengthString != NULL)
        lengthString->~CubeString();

    if (widthString != NULL)
        widthString->~CubeString();

    if (gravityString != NULL)
        gravityString->~CubeString();

    if (labelSetLevelName != NULL)
        labelSetLevelName->~CubeString();

    if (formSetLevelName != NULL)
        formSetLevelName->~CubeStringList();

    if (next != NULL)
        next->~CubeString();

    if (back != NULL)
        back->~CubeString();

    if (menu != NULL)
        menu->~CubeString();

    if (create != NULL)
        create->~CubeString();

    if (save != NULL)
        save->~CubeString();

    if (cancel != NULL)
        cancel->~CubeString();

    if (clear != NULL)
        clear->~CubeString();

    if (exit != NULL)
        exit->~CubeString();
}

void LevelEditor::draw(GLboolean simplifyForPicking)
{
    // Calcolo nuovo Frame

    if (!simplifyForPicking)
    {
       QList<int> actions = currentActions->getAllActions();

        while (!actions.isEmpty())
        {
            GLint step = actions.takeFirst();
            switch (step)
            {
            // Primary Actions

            case INITIAL_MOVEMENT:
                if (cameraOffset->y < 0.0f)
                {
                    cameraOffset->y += 2.0f;
                }
                else
                {
                    currentActions->setPrimaryAction(DO_NOTHING);
                    isMoving = false;
                }

                break;

            case GO_TO_SET_NAME_VIEW:
                if(currentView == SET_PARAM_VIEW)
                {
                    cameraOffset->x -= 0.5f;
                    cameraAngle -= 6.0f;
                    if(cameraOffset->x == -30.0f)
                    {
                        currentView = SET_NAME_VIEW;
                        currentActions->setPrimaryAction(DO_NOTHING);
                        isMoving = false;
                        cameraAngle = 0.0f;
                    }
                }

                break;

            case GO_TO_SET_PARAM_VIEW:
                if (currentView == SET_NAME_VIEW)
                {
                    cameraOffset->x += 0.5f;
                    cameraAngle += 6.0f;

                    if (cameraOffset->x == 0.0f)
                    {
                        currentView = SET_PARAM_VIEW;
                        currentActions->setPrimaryAction(DO_NOTHING);
                        isMoving = false;
                        cameraAngle = 0.0f;
                    }
                }
                else if (currentView == SET_GRAVITY_VIEW)
                {
                    cameraOffset->x -= 0.5f;
                    cameraAngle -= 6.0f;

                    if (cameraOffset->x == 0.0f)
                    {
                        currentView = SET_PARAM_VIEW;
                        currentActions->setPrimaryAction(DO_NOTHING);
                        isMoving = false;
                        cameraAngle = 0.0f;
                    }
                }

                break;

            case GO_TO_SET_GRAVITY_VIEW:
                cameraOffset->x += 0.5f;
                cameraAngle += 6.0f;

                if (cameraOffset->x == 30.0f)
                {
                    currentView = SET_GRAVITY_VIEW;
                    currentActions->setPrimaryAction(DO_NOTHING);
                    isMoving = false;
                    cameraAngle = 0.0f;
                }

                break;

             case GO_TO_EDITING_LEVEL_VIEW:
                cameraOffset->x += 2.0f;

                if (cameraOffset->x == 60.0f)
                {
                    currentView = EDITING_LEVEL_VIEW;
                    emit setMouseMovementTracking(MOUSE_MOVED_DOWN);
                }
                else if (cameraOffset->x == 90.0f)
                {
                    currentActions->setPrimaryAction(DO_NOTHING);
                    isMoving = false;
                }

                break;

            case EXIT_TO_MENU:
                cameraOffset->x += (currentView == EDITING_LEVEL_VIEW) ? 2.0f : 5.0f;

                if (cameraOffset->x == 150.0f)
                {
                    currentActions->setPrimaryAction(DO_NOTHING);
                    isMoving = false;
                    emit levelEditorClosed();
                    return;
                }

                break;

            // Secondary Actions

            case ROTATE_VOLUMECUBE:
                angleRotVolumeCube += 5.0f;

                if ((GLint(angleRotVolumeCube) % 360) == 0)
                    angleRotVolumeCube = 0.0f;

                if ((GLint(angleRotVolumeCube) % 90) == 0)
                    currentActions->removeSecondaryAction(ROTATE_VOLUMECUBE);

                break;
            }
        }
    }

    // Disegno l' editor

    if (!(isMoving && simplifyForPicking))
    {
        dynamic_cast<QGLWidget*>(parent)->renderText(-currentError.length()*0.1225f, -6.0f, 0.0, currentError);

        glPushName(BUTTON_VOLUME);
        glPushMatrix();
            glTranslatef(9.5f, 6.0f, 3.0f);
            glRotatef(angleRotVolumeCube, -1.0f, 0.0f, 0.0f);
            drawPrism(0.8f, 0.8f, 0.8f, volumeSkin, true);
        glPopMatrix();
        glPopName();

        glTranslatef(-cameraOffset->x, -cameraOffset->y, -cameraOffset->z);
        glRotatef(cameraAngle, 0.0f, 0.0f, 1.0f);

        glPushMatrix();

        if(currentView == EDITING_LEVEL_VIEW)
        {
            glPushMatrix();

                glTranslatef(90.0f, 0.0f, 0.0f);

                glPushMatrix();
                    glTranslatef(7.0f, 4.0f, 3.0f);
                    save->draw(simplifyForPicking);
                    glTranslatef(0.0f, -1.5f, 0.0f);
                    cancel->draw(simplifyForPicking);
                    glTranslatef(0.0f, -1.5f, 0.0f);
                    clear->draw(simplifyForPicking);
                    glTranslatef(0.0f, -1.5f, 0.0f);
                    exit->draw(simplifyForPicking);
                glPopMatrix();

                glPushMatrix();

                    glTranslatef(levelOffset->x, levelOffset->y, levelOffset->z);
                    glScalef(0.4f, 0.4f, 0.4f);
                    level->draw(simplifyForPicking);

                glPopMatrix();

                glScalef(0.4f, 0.4f, 0.4f);
                glTranslatef(-25.0f, 15.0f, 0.0f);

                glPushMatrix();
                if (movingObject == 0)
                {
                    glTranslatef(currentDelta.x, currentDelta.y, currentDelta.z);
                    setMaterialColor(positionValid ? COLOR_GREEN : COLOR_RED);
                }
                else
                {
                    setMaterialColor(COLOR_DISABLED);
                }

                glPushName(OBSTACLE_0);
                drawObstacle(0);
                glPopName();
                glPopMatrix();

                glTranslatef(0.0f, -7.0f, 0.0f);

                glPushMatrix();
                if (movingObject == 1)
                {
                    glTranslatef(currentDelta.x, currentDelta.y, currentDelta.z);
                    setMaterialColor(positionValid ? COLOR_GREEN : COLOR_RED);
                }
                else
                {
                    setMaterialColor(COLOR_DISABLED);
                }

                glPushName(OBSTACLE_1);
                drawObstacle(1);
                glPopName();
                glPopMatrix();

                if (currentWidth >= 6)
                {
                    glTranslatef(0.0f, -7.0f, 0.0f);

                    glPushMatrix();
                    if (movingObject == 2)
                    {
                        glTranslatef(currentDelta.x, currentDelta.y, currentDelta.z);
                        setMaterialColor(positionValid ? COLOR_GREEN : COLOR_RED);
                    }
                    else
                    {
                        setMaterialColor(COLOR_DISABLED);
                    }

                    glPushName(OBSTACLE_2);
                    drawObstacle(2);
                    glPopName();
                    glPopMatrix();

                    glTranslatef(0.0f, -7.0f, 0.0f);

                    glPushMatrix();
                    if (movingObject == 3)
                    {
                        glTranslatef(currentDelta.x, currentDelta.y, currentDelta.z);
                        setMaterialColor(positionValid ? COLOR_GREEN : COLOR_RED);
                    }
                    else
                    {
                        setMaterialColor(COLOR_DISABLED);
                    }

                    glPushName(OBSTACLE_3);
                    drawObstacle(3);
                    glPopName();
                    glPopMatrix();
                }

            glPopMatrix();
            setMaterialColor(COLOR_DISABLED);
        }
        else
        {
            // Set Name View
            glPushMatrix();

                glTranslatef(-30.0f, 6.0f, 0.0f);
                labelSetLevelName->draw(simplifyForPicking);

                glTranslatef(8.5f, -13.0f, 0.0f);
                next->draw(simplifyForPicking);

                glTranslatef(-17.0f, 0.0f, 0.0f);
                menu->draw(simplifyForPicking);

                glTranslatef(8.5f, 6.5f, 0.0f);

                if (!simplifyForPicking)
                {
                    if (formSetLevelName->getLabel(0) == "" && visible)
                    {
                        visibleTime++;
                        drawPrism(2.0f, 2.0f, 2.0f);
                        if(visibleTime == 20)
                            visible = false;
                    }
                    else if (formSetLevelName->getLabel(0) == "" && !visible)
                    {
                        visibleTime++;
                        if(visibleTime == 33)
                        {
                            visibleTime = 0;
                            visible = true;
                        }
                    }
                    else
                    {
                        formSetLevelName->draw(simplifyForPicking);
                    }
                }

            glPopMatrix();

            // Set Parameters View

            glPushMatrix();

                glTranslatef(9.0f, -7.0f, 0.0f);
                next->draw(simplifyForPicking);

                glTranslatef(-18.5f, 0.0f, 0.0f);
                back->draw(simplifyForPicking);

                glTranslatef(3.5f, 11.5f, 0.0f);
                lengthString->draw(simplifyForPicking);

                glTranslatef(12.0f, 0.0f, 0.0f);
                widthString->draw(simplifyForPicking);

                glTranslatef(-12.0f, -2.5f, 1.5f);
                glPushName(BUTTON_LENGTHEN);
                glBegin(GL_TRIANGLES);
                    glVertex3f( 0.0f,  1.0f,  0.0f);
                    glVertex3f( 1.0f, -1.0f,  0.0f);
                    glVertex3f(-1.0f, -1.0f,  0.0f);
                glEnd();
                glPopName();

                glTranslatef(0.0f, -3.5f, -1.5f);
                lengthLabel->draw(simplifyForPicking);

                glTranslatef(7.0f, 0.0f, 1.5f);
                glPushName(BUTTON_REDUCE);
                glBegin(GL_TRIANGLES);
                    glVertex3f(-1.0f,  0.0f,  0.0f);
                    glVertex3f( 1.0f,  1.0f,  0.0f);
                    glVertex3f( 1.0f, -1.0f,  0.0f);
                glEnd();
                glPopName();

                glTranslatef(4.5f, 0.0f, -1.5f);
                widthLabel->draw(simplifyForPicking);

                glTranslatef(4.5f, 0.0f, 1.5f);
                glPushName(BUTTON_ENLARGE);
                glBegin(GL_TRIANGLES);
                    glVertex3f( 1.0f,  0.0f,  0.0f);
                    glVertex3f(-1.0f,  1.0f,  0.0f);
                    glVertex3f(-1.0f, -1.0f,  0.0f);
                glEnd();
                glPopName();

                glTranslatef(-16.0f, -3.5f, 0.0f);
                glPushName(BUTTON_SHORTEN);
                glBegin(GL_TRIANGLES);
                    glVertex3f(-1.0f,  1.0f,  0.0f);
                    glVertex3f( 1.0f,  1.0f,  0.0f);
                    glVertex3f( 0.0f, -1.0f,  0.0f);
                glEnd();
                glPopName();

            glPopMatrix();

            // Set Gravity

            glPushMatrix();

                glTranslatef(30.0f, 4.0f, 0.0f);
                gravityString->draw(simplifyForPicking);

                glTranslatef(0.0f, -4.0f, 0.0f);
                gravityLabel->draw(simplifyForPicking);

                glTranslatef(+6.0f, 0.0f, 0.0f);
                glPushName(BUTTON_GRAVITY_PLUS);
                glBegin(GL_TRIANGLES);
                    glVertex3f( 1.0f,  0.0f,  0.0f);
                    glVertex3f(-1.0f,  1.0f,  0.0f);
                    glVertex3f(-1.0f, -1.0f,  0.0f);
                glEnd();
                glPopName();

                glTranslatef(-12.0f, 0.0f, 0.0f);
                glPushName(BUTTON_GRAVITY_MINUS);
                glBegin(GL_TRIANGLES);
                    glVertex3f(-1.0f,  0.0f,  0.0f);
                    glVertex3f( 1.0f,  1.0f,  0.0f);
                    glVertex3f( 1.0f, -1.0f,  0.0f);
                glEnd();
                glPopName();

                glTranslatef(-2.0f, -7.0f, 0.0f);

                if (level == NULL)
                    back->draw(simplifyForPicking);

                glTranslatef(15.0f, -0.0f, 0.0f);
                create->draw(simplifyForPicking);

            glPopMatrix();

        }
        glPopMatrix();
    }
}

GLvoid LevelEditor::lengthen()
{
    if (currentLength + 3 <= MAX_LEVEL_LENGTH)
    {
        emit playEffect(EFFECT_COIN);
        lengthLabel->~CubeString();
        currentLength += 3;
        lengthLabel = new CubeString(QString::number(currentLength / 3), 3, alphabet, LENGTH_LABEL);
   }
}

GLvoid LevelEditor::shorten()
{
    if (currentLength - 3 >= MIN_LEVEL_LENGTH)
    {
        emit playEffect(EFFECT_COIN);
        lengthLabel->~CubeString();
        currentLength -= 3;
        lengthLabel = new CubeString(QString::number(currentLength / 3), 3, alphabet, LENGTH_LABEL);
    }
}

GLvoid LevelEditor::enlarge()
{
    if (currentWidth + 3 <= MAX_LEVEL_WIDTH)
    {
        emit playEffect(EFFECT_COIN);
        widthLabel->~CubeString();
        currentWidth += 3;
        widthLabel = new CubeString(QString::number(currentWidth / 3), 3, alphabet, WIDTH_LABEL);
    }
}

GLvoid LevelEditor::reduce()
{
    if (currentWidth - 3 >= MIN_LEVEL_WIDTH)
    {
        emit playEffect(EFFECT_COIN);
        widthLabel->~CubeString();
        currentWidth -= 3;
        widthLabel = new CubeString(QString::number(currentWidth / 3), 3, alphabet, WIDTH_LABEL);
    }
}

GLvoid LevelEditor::gravityMinus()
{
    if (currentGravity > MIN_GRAVITY)
    {
        emit playEffect(EFFECT_COIN);
        gravityLabel->~CubeString();
        currentGravity--;
        gravityLabel = new CubeString(QString::number(currentGravity), 3, alphabet, GRAVITY_LABEL);
    }
}

GLvoid LevelEditor::gravityPlus()
{
    if (currentGravity < MAX_GRAVITY)
    {
        emit playEffect(EFFECT_COIN);
        gravityLabel->~CubeString();
        currentGravity++;
        gravityLabel = new CubeString(QString::number(currentGravity), 3, alphabet, GRAVITY_LABEL);
    }
}

GLvoid LevelEditor::buttonBackTriggered()
{
    if (currentView == SET_PARAM_VIEW)
    {
        isMoving = true;
        emit playEffect(EFFECT_JUMPSMALL);
        currentActions->setPrimaryAction(GO_TO_SET_NAME_VIEW);
    }
    else if (currentView == SET_GRAVITY_VIEW)
    {
        if (level == NULL)
        {
            isMoving = true;
            emit playEffect(EFFECT_JUMPSMALL);
            currentActions->setPrimaryAction(GO_TO_SET_PARAM_VIEW);
        }
    }
}

GLvoid LevelEditor::buttonNextTriggered()
{
    if (currentView == SET_NAME_VIEW)
    {
        if (currentName.endsWith(' '))
            currentName.chop(1);

        if (currentName.startsWith(' '))
            currentName.remove(0, 1);

        if (currentName != "")
        {
            currentName = currentName.toLower();

            if (QFile::exists("resources/levels/xml/" + currentName + ".xml"))
            {
                currentError = "Name already taken.";
                return;
            }

            isMoving = true;
            emit playEffect(EFFECT_JUMPSMALL);
            currentActions->setPrimaryAction(GO_TO_SET_PARAM_VIEW);
        }
    }
    else if (currentView == SET_PARAM_VIEW)
    {
        isMoving = true;
        emit playEffect(EFFECT_JUMPSMALL);
        currentActions->setPrimaryAction(GO_TO_SET_GRAVITY_VIEW);
    }
    else if (currentView == SET_GRAVITY_VIEW)
    {
        emit playEffect(EFFECT_JUMPSMALL);
        isMoving = true;

        if (level == NULL)
            level = new Level(currentName, currentLength, currentWidth, this, asphaltSkin);

        level->setGravity(currentGravity);

        levelOffset->z -= ((currentLength * 0.4f) / 2.0f) - 10.0f;
        currentActions->setPrimaryAction(GO_TO_EDITING_LEVEL_VIEW);
    }
    else
    {
        saveLevel();
    }
}

GLvoid LevelEditor::letterTyped(int key)
{
    if (currentView == SET_NAME_VIEW)
    {
        bool isSpace = false;

        if (key != Qt::Key_Space)
        {
            currentName += key;
        }
        else
        {
            if (currentName.endsWith(' '))
                return;

            isSpace = true;
            currentName.append(" ");
        }

        emit playEffect(EFFECT_COIN);

        if (!isSpace)
        {
            formSetLevelName->~CubeStringList();
            formSetLevelName = new CubeStringList(currentName, 14.0f, 6.0f, alphabet, 2.0f, FORM_SET_LEVEL_NAME);

            GLuint stringName = formSetLevelName->getLabelCount() - 1;
            GLuint letterName = formSetLevelName->getLabel(stringName).size() - 1;

            if (!formSetLevelName->isRotating(stringName, letterName))
                formSetLevelName->startLetterRotation(stringName, letterName, 30, 4);
        }
    }
}

GLvoid LevelEditor::checkMousePosition(GLint x, GLint y)
{
    if (movingObject != -1)
    {
        Vector3f* P0 = new Vector3f(x, y, 0);
        Vector3f* P1 = new Vector3f(x, y, 1);

        Vector3f* M0 = getModelViewPos(P0, false);
        Vector3f* M1 = getModelViewPos(P1, false);

        // Calcolo del punto sul piano
        GLfloat t = (levelOffset->y + (LEVEL_HEIGHT/2.0f) - M0->y)/(M1->y - M0->y);
        Vector3f *newPos = getPointFromParametricLine(M0, M1, t);

        // Se non e' sul piano uso l'altra parametrizzazione
        if (  (newPos->x < (90.0f - (currentWidth * 0.4f)/2.0f))
           || (newPos->x > (90.0f + (currentWidth * 0.4f)/2.0f))
           || (newPos->z >  20.0f)
           || (newPos->z > levelOffset->z + (currentLength * 0.4f)/2.0f)
           || (newPos->z < levelOffset->z - (currentLength * 0.4f)/2.0f))
        {
            t = (lastCentre.z + deltaFromCentre.z - M0->z)/(M1->z - M0->z);
            newPos = getPointFromParametricLine(M0, M1, t);
            currentDelta = *newPos - lastCentre - deltaFromCentre;
            currentDelta = currentDelta * 2.5f;
            positionValid = false;
        }
        else
        {
            xCell = floor((((newPos->x - 90.f) / 0.4f) + currentWidth / 2.0f) / 3.0f);
            zCell = floor((-(newPos->z - levelOffset->z) / 0.4f + currentLength / 2.0f) / 3.0f);

            int x_cellMax = floor(currentWidth / 3.0f) - ((movingObject == 0) || (movingObject == 1) ? 1 : 2);
            xCell = qMin(xCell, x_cellMax);

            Vector3f *bounding = getObstacleBoundingBox(movingObject);

            Vector3f *newCentre = new Vector3f();
            newCentre->x = ((bounding->x / 2.0f) + (xCell * 3.0f) - (currentWidth / 2.0f)) * 0.4f + 90.0f;
            newCentre->y = ((bounding->y / 2.0f) + (LEVEL_HEIGHT / 2.0f)) * 0.4f + levelOffset->y;
            newCentre->z = -((bounding->z / 2.0f) + (zCell * 3.0f) - (currentLength / 2.0f)) * 0.4f + levelOffset->z;

            currentDelta = *newCentre - lastCentre;
            currentDelta = currentDelta * 2.5f;

            positionValid = true;
        }
    }
}

GLvoid LevelEditor::moveIn()
{
    if (levelOffset->z <= ((currentLength * 0.4f) / 2.0f) + 10.0f)
        levelOffset->z += 1.0f;
}

GLvoid LevelEditor::moveOut()
{
    if (levelOffset->z > -((currentLength * 0.4f) / 2.0f) + 10.0f)
        levelOffset->z -= 1.0f;
}

GLvoid LevelEditor::saveLevel()
{
    bool newlyCreated = false;
    level->save(&newlyCreated);

    emit playEffect(EFFECT_COIN);

    if (newlyCreated)
        emit addLevelToLevelsList(level);
}

GLvoid LevelEditor::quitEditor()
{
    isMoving = true;
    emit stopAmbientMusic();
    emit playEffect(EFFECT_STOMP);
    currentActions->setPrimaryAction(EXIT_TO_MENU);
}

void LevelEditor::itemClicked(QMouseEvent *event, QList<GLuint> listNames)
{
    if (isMoving)
        return;

    Vector3f *pos = NULL;

    if (!listNames.isEmpty())
    {
        switch (listNames.at(0))
        {
        case BUTTON_VOLUME:
            if ((GLint(angleRotVolumeCube) % 90) == 0)
            {
                audioEnabled = !audioEnabled;
                emit enableAudio(audioEnabled);
                currentActions->appendSecondaryAction(ROTATE_VOLUMECUBE);
            }
            break;

        case BUTTON_SHORTEN:
            shorten();
            break;

        case BUTTON_LENGTHEN:
            lengthen();
            break;

        case BUTTON_REDUCE:
            reduce();
            break;

        case BUTTON_ENLARGE:
            enlarge();
            break;

        case BUTTON_GRAVITY_MINUS:
            gravityMinus();
            break;

        case BUTTON_GRAVITY_PLUS:
            gravityPlus();
            break;

        case BUTTON_BACK:
            buttonBackTriggered();
            break;

        case BUTTON_NEXT:
            buttonNextTriggered();
            break;

        case OBSTACLE_0:
            lastCentre = toolbarObstacleCentres.at(0);
            pos = getModelViewPos(new Vector3f(event->x(), event->y(), 0.0f), true);
            currentDelta = Vector3f();
            deltaFromCentre = *pos - lastCentre;
            movingObject = 0;
            break;

        case OBSTACLE_1:
            lastCentre = toolbarObstacleCentres.at(1);
            pos = getModelViewPos(new Vector3f(event->x(), event->y(), 0.0f), true);
            currentDelta = Vector3f();
            deltaFromCentre = *pos - lastCentre;
            movingObject = 1;
            break;

        case OBSTACLE_2:
            lastCentre = toolbarObstacleCentres.at(2);
            pos = getModelViewPos(new Vector3f(event->x(), event->y(), 0.0f), true);
            currentDelta = Vector3f();
            deltaFromCentre = *pos - lastCentre;
            movingObject = 2;
            break;

        case OBSTACLE_3:
            lastCentre = toolbarObstacleCentres.at(3);
            pos = getModelViewPos(new Vector3f(event->x(), event->y(), 0.0f), true);
            currentDelta = Vector3f();
            deltaFromCentre = *pos - lastCentre;
            movingObject = 3;
            break;

        case BUTTON_SAVE:
            saveLevel();
            break;

        case BUTTON_CANCEL:
            level->clearTempObstaclesList();
            emit playEffect(EFFECT_JUMP);
            break;

        case BUTTON_CLEAR:
            level->clearObstaclesList();
            emit playEffect(EFFECT_JUMP);
            break;

        case BUTTON_EXIT:
            quitEditor();
            break;

        case OBSTACLES:
            level->deleteObstacle(listNames.at(1));
            break;
        }
    }
}

void LevelEditor::mouseReleased(QMouseEvent *event)
{
    Q_UNUSED(event);

    if(currentView == EDITING_LEVEL_VIEW)
    {
        if ((movingObject != -1) && (positionValid == true))
        {
            Obstacle *obstacle = new Obstacle(movingObject, new Vector3f(GLfloat(xCell), GLfloat(yCell), GLfloat(zCell)));
            obstacle->setColor(QColor(0, 0, 255, 255));
            level->addObstacle(obstacle);

            emit playEffect(EFFECT_JUMP);
        }

        positionValid = false;
        currentDelta = new Vector3f();
        lastMouseX = 0;
        lastMouseY = 0;
        movingObject = -1;
    }
}

void LevelEditor::mouseMoved(QMouseEvent *event, QList<GLuint> listNames)
{
    if (isMoving)
        return;

    if (currentView == EDITING_LEVEL_VIEW)
    {
        lastMouseX = event->x();
        lastMouseY = event->y();

        checkMousePosition(event->x(), event->y());
    }

    if (!listNames.isEmpty())
    {
        switch (listNames.at(0))
        {
        case LENGTH_LABEL:
            if (!lengthLabel->isRotating(listNames.at(1)))
                lengthLabel->startLetterRotation(listNames.at(1), 6, 1);
            break;

        case LENGTH_STRING:
            if (!lengthString->isRotating(listNames.at(1)))
            {
                if ((listNames.at(1) % 2) == 0)
                    lengthString->startLetterRotation(listNames.at(1), 6, 2);
                else
                    lengthString->startLetterRotation(listNames.at(1), 12, 4);
            }
            break;

        case WIDTH_LABEL:
            if (!widthLabel->isRotating(listNames.at(1)))
                widthLabel->startLetterRotation(listNames.at(1), 6, 1);
            break;

        case WIDTH_STRING:
            if (!widthString->isRotating(listNames.at(1)))
            {
                if ((listNames.at(1) % 2) == 0)
                    widthString->startLetterRotation(listNames.at(1), 6, 2);
                else
                    widthString->startLetterRotation(listNames.at(1), 12, 4);
            }
            break;

        case LABEL_SET_LEVEL_NAME:
            if (!labelSetLevelName->isRotating(listNames.at(1)))
                labelSetLevelName->startLetterRotation(listNames.at(1), 6, 1);
            break;

        }
    }
}

void LevelEditor::wheelScrolled(QWheelEvent *event)
{
    if (currentView == EDITING_LEVEL_VIEW)
    {
        if (event->delta() < 0)
            moveIn();
        else
            moveOut();

        checkMousePosition(lastMouseX, lastMouseY);
    }
}

void LevelEditor::keyPressed(QKeyEvent *event)
{
    if (isMoving)
        return;

    currentError.clear();
    int key = event->key();

    switch (key)
    {
    case Qt::Key_Escape:
        buttonBackTriggered();
        break;

    case Qt::Key_Backspace:
        if (currentView == SET_NAME_VIEW)
        {
            emit playEffect(EFFECT_JUMP);
            currentName = currentName.left(currentName.length() - 1);

            formSetLevelName->~CubeStringList();
            formSetLevelName = new CubeStringList(currentName, 14.0f, 6.0f, alphabet, 2.0f, FORM_SET_LEVEL_NAME);
        }
        else
        {
            buttonBackTriggered();
        }

        break;

    case Qt::Key_Enter:
    case Qt::Key_Return:
        buttonNextTriggered();
        break;

    case Qt::Key_Up:
        if (currentView == SET_PARAM_VIEW)
        {
            lengthen();
        }
        else if (currentView == EDITING_LEVEL_VIEW)
        {
            moveIn();
            checkMousePosition(lastMouseX, lastMouseY);
        }

        break;

    case Qt::Key_Down:
        if (currentView == SET_PARAM_VIEW)
        {
            shorten();
        }
        else if (currentView == EDITING_LEVEL_VIEW)
        {
            moveOut();
            checkMousePosition(lastMouseX, lastMouseY);
        }

        break;

    case Qt::Key_Left:

        if (currentView == SET_PARAM_VIEW)
            reduce();
        else if (currentView == SET_GRAVITY_VIEW)
            gravityMinus();
        break;

    case Qt::Key_Right:


        if (currentView == SET_PARAM_VIEW)
            enlarge();
        else if (currentView == SET_GRAVITY_VIEW)
            gravityPlus();
        break;

    default:
        if ((key >= Qt::Key_A && key <= Qt::Key_Z) || (key >= Qt::Key_0 && key <= Qt::Key_9) || key == Qt::Key_Space)
            letterTyped(key);
        break;
    }
}
