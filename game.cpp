#include "game.h"

Game::Game(GLint _gameType, QImage *_skin, QMap<GLint,Vector3f*> &_obstacleModelsList, QObject *parent)
{

}

Game::~Game()
{

}

void Game::newGameStory(QMap<GLint,QString> &_levelsList)
{

}

void Game::newGameArcade(QString filename)
{

}

void Game::draw()
{

}

void Game::playLevel(GLint levelId)
{

}

void Game::nextLevel(GLint currentLevelId)
{

}

void Game::pauseGame()
{

}

void Game::continueGame()
{

}

void Game::quitGame()
{

}

void Game::run()
{

}

void Game::collided()
{

}

void Game::levelCompleted()
{

}

void Game::keyPressed(QKeyEvent *e)
{

}

void Game::itemClicked(GLint id)
{

}
