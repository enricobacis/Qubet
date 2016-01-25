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


#ifndef MENU_H
#define MENU_H

#include "skin.h"
#include "level.h"
#include "actionlist.h"
#include "cubestringlist.h"

/**
 * @brief Menu class used to show and manage the game menu.
 *
 * @version 1.0
 * @author \#34
 */
class Menu : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Create a Menu object used to show and manage the game menu.
     *
     * @post You have to connect parent's signals to this object's slots.
     *       For Qubet check the connectInputEvents function.
     *
     * @param _skinsList is the reference to Qubet's skinsList.
     * @param _levelsList is the reference to Qubet's levelsList.
     * @param _iconsList is the reference to Qubet's iconsList.
     * @param _alphabet is the reference to Qubet's alphabet.
     * @param _parent is a callback variable to the parent of the parent.
     * @param _audioEnabled is a variable to states if the audio is enabled or not.
     * @param showIntro states if you want to show the intro. [default = true]
     */
    explicit Menu(QMap<GLint,Skin*> &_skinsList, QMap<GLint,Level*> &_levelsList, QMap<GLint,GLuint> &_iconsList, Alphabet *_alphabet, QObject *_parent, bool _audioEnabled = true, bool showIntro = true);

    /**
     * @brief Disconnect and Destroy a Menu object.
     */
    ~Menu();

    /**
     * @brief Method to draw the menu.
     *
     * @param simplifyForPicking [default = false] is used to draw a simplified scene
     *        used for the picking function.
     */
    GLvoid draw(GLboolean simplifyForPicking = false);


private:

    QObject *parent;  /**<  It is a callback variable to the parent of the widget. */
    GLint currentSkin; /**< It is the id of the current choosen skin. */
    GLint currentLevel;  /**< It is the id of the current choosen level. */
    QMap<GLint,Skin*> skinsList; /**< It is the skinsList provided by Qubet object. */
    QMap<GLint,Level*> levelsList; /**< It is the levelsList provided by Qubet object. */
    QMap<GLint,GLuint> iconsList;  /**< It is the iconsList provided by Qubet object. */
    Alphabet *alphabet; /**< It is the alphabet provided by Qubet. */
    GLint itemSelected; /**< It is the id of the current menu action selected. */
    GLboolean isMoving; /**< It is the variable that states if the menu is moving or not. */
    Vector3f *cameraOffset; /**< It is the camera offset for the menu. */
    ActionList *currentActions; /**< It is the list of the current menu steps. */
    GLint gameType; /**< It is the selected gameType. */
    GLfloat angleRotCube;  /**< It is the current rotation angle of the cube. */
    GLfloat spinCube;  /**< It is the spin step of the skin selection cube. */
    GLfloat angleRotVolumeCube;  /**< It is the current rotation angle of the volume icon. */
    GLboolean audioEnabled;  /**< It is the state of the audio. */
    Skin *volumeSkin; /**< It is the skin of the volume button */
    GLint currentView;  /**< It is the value of the current view */
    GLint currentSection; /**< It is used to states the menu to draw the introduction or the menu.*/
    GLint waitCounter; /**< It is an incremental variable to wait from intro screens. */

    CubeStringList *mainMenuButtons; /**< It is the main menu buttons stringList. */
    CubeString *skinName;  /**< It is the skinName CubeString. */
    CubeStringList *levelName; /**< It is the levelName CubeString. */
    CubeString *backButton;  /**< It is the backButton CubeString. */
    CubeString *playButton; /**< It is the playButton CubeString. */
    CubeString *levelsButton; /**< It is the levelsButton CubeString. */
    CubeString *editButton; /**< It is the editButton CubeString. */

    CubeStringList *cubeStudiosLabel; /**< It is the Cube studios distribution CubeStringList. */
    CubeStringList *team34Label; /**< It is the team 34 CubeStringList. */
    CubeString *qubetLabel; /**< It is the Qubet CubeString. */


    /**
     * @brief Change the current skin and select the previous one.
     */
    GLvoid previousSkin();

    /**
     * @brief Change the current skin and select the next one.
     */
    GLvoid nextSkin();

    /**
     * @brief Select the previous level.
     */
    GLvoid previousLevel();

    /**
     * @brief Select the next level.
     */
    GLvoid nextLevel();

    /**
     * @brief This function is invoked when playStoryButton is triggered.
    */
    GLvoid buttonPlayStoryTriggered();

    /**
     * @brief This function is invoked when playArcadeButton is triggered.
    */
    GLvoid buttonPlayArcadeTriggered();

    /**
     * @brief This function is invoked when editorButton is triggered.
    */
    GLvoid buttonEditorTriggered();

    /**
     * @brief This function is invoked when backButton is triggered.
     */
    GLvoid buttonBackTriggered();

    /**
     * @brief This function is invoked when nextButton is triggered.
     */
    GLvoid buttonNextTriggered();

    /**
     * @brief This function is invoked when previousSkinButton is triggered.
     */
    GLvoid buttonPreviousSkinTriggered();

    /**
     * @brief This function is invoked when nextSkinButton is triggered.
     */
    GLvoid buttonNextSkinTriggered();


private slots:

    /**
     * @brief Slot invoked when the user clicks on a item.
     *
     * @param event is the QMouseEvent.
     * @param listNames is the QList<GLuint> of item's names.
     */
    void itemClicked(QMouseEvent *event, QList<GLuint> listNames);

    /**
     * @brief Slot invoked when the user releases the mouse button.
     *
     * @param event is the QMouseEvent*.
     */
    void mouseReleased(QMouseEvent *event);

    /**
     * @brief Slot invoked when the user moves the mouse.
     *
     * @param event is the QMouseEvent*.
     * @param listNames is the QList<GLuint> of item's names.
     */
    void mouseMoved(QMouseEvent *event, QList<GLuint> listNames);

    /**
     * @brief Slot invoked when the user scrolls the wheel of the mouse button.
     *
     * @param event is the QWheelEvent*.
     */
    void wheelScrolled(QWheelEvent *event);

    /**
     * @brief Slot invoked when the user presses a key on the keyboard.
     *
     * @param event is the QKeyEvent*.
     */
    void keyPressed(QKeyEvent *event);


signals:
    /**
     * @brief Signal emitted to show the gameView in story mode.
     *
     * @param skinId
     */
    void playStory(GLint skinId);

    /**
     * @brief Signal emitted to show the gameView in arcade mode.
     *
     * @param skinId
     * @param levelId
     */
    void playArcade(GLint skinId, GLint levelId);

    /**
     * @brief Signal emitted to show the levelEditor
     *
     */
    void showLevelEditor(GLint levelId);

    /**
     * @brief This signal is emitted to set the Qubet's Mouse Move Tracking sensibility.
     *        The modes are:
     *          0 - no signal emitted.
     *          1 - the signal mouseMoved is emitted only when the mouse is moved
     *              and one of the mouse buttons are clicked.
     *          2 - the signal mouseMoved is emitted always when the mouse is
     *              moved.
     *
     * @param mode It is the selected mode.
     */
    void setMouseMovementTracking(int mode);

    /**
     * @brief Signal emitted to enable or disable the audio.
     *
     * @param enabled is the variable that states if the audio is to enable or not.
     */
    void enableAudio(bool enabled);

    /**
     * @brief Signal emitted to play the ambient music.
     *
     * @param filename is the filename to play.
     */
    void playAmbientMusic(QString filename);

    /**
     * @brief Signal emitted to stop the current ambient music.
     */
    void stopAmbientMusic();

    /**
     * @brief Signal emitted to play an effect.
     *
     * @param effectName is the name of the effect to play.
     */
    void playEffect(QString effectName);

    /**
     * @brief Signal emitted to set the Skybox.
     *
     * @param skyboxName is the name of the Skybox to apply.
     */
    void setSkybox(QString skyboxName);

};

#endif // MENU_H
