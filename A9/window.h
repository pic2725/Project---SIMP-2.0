#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "gamemanager.h"
namespace Ui {

class window;
}

/**
 * @brief Game Title Window
 */
class window : public QMainWindow
{
    Q_OBJECT

public:
    window(GameManager* gameManager);
    ~window();

    QMediaPlaylist *playlist;
    QMediaPlayer *music;



private slots:
    void on_startButton_clicked();


private:
    Ui::window *ui;
    GameManager *gameManager;
};

#endif // WINDOW_H
