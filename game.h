#ifndef GAME_H
#define GAME_H

#include <QThread>
#include <QImage>
#include <QKeyEvent>

#include <GL/gl.h>

#include "vector3f.h"
#include "cube.h"
#include "level.h"
#include "positioncontroller.h"

class Game : public QThread
{
    Q_OBJECT

public:
    explicit Game(GLint _gameType, QImage *_skin, QMap<Vector3f*, GLint> &_obstacleModelsList, QObject *parent = 0);
    ~Game();

    void newGameStory(QMap<QString, GLint> &_levelsList);
    void newGameArcade(QString filename);
    void draw();

private:
    QMap<QString, GLint> levelsList;
    QMap<Vector3f*, GLint> obstacleModelsList;
    QImage *skin;
    Cube *cube;
    Level *level;
    PositionController *positionController;
    GLint state;
    GLint gameType;
    GLint currentLevel;
    GLint deaths;

    void playLevel(GLint levelId);
    void nextLevel(GLint currentLevelId);
    void pauseGame();
    void continueGame();
    void quitGame();

    void run();

private slots:
    void collided();
    void levelCompleted();
    void keyPressed(QKeyEvent *e);
    void itemClicked(GLint id);

signals:
    void pauseGameSignal();
    void continueGameSignal();
    void enableAudio(bool enabled);
    void playAmbientMusic(QString filename);
    void pauseAmbientMusic();
    void continueAmbientMusic();
    void playEffect(GLint effectId);
    void gameClosed();

};

#endif // GAME_H
