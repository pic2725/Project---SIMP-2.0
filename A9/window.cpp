#include "window.h"
#include "ui_window.h"


#include <QApplication>
#include <QMovie>
#include <QSound>


window::window(GameManager *gameManager) :
    QMainWindow(nullptr),
    ui(new Ui::window)
{
    ui->setupUi(this);

    playlist = new QMediaPlaylist;
    music= new QMediaPlayer;

    playlist->addMedia(QUrl("qrc:/ChipTune3.1.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    music->setPlaylist(playlist);
    music->setVolume(25);

    music->play();

    this->gameManager = gameManager;

    QMovie *dance = new QMovie(":/player_down.gif");
    dance->start();

    ui->gif->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->gif->setAttribute(Qt::WA_NoSystemBackground);
    ui->gif->setMovie(dance);
    ui->gif->setScaledContents(true);
}


window::~window()
{
    delete ui;
    gameManager = nullptr;
}


void window::on_startButton_clicked()
{
    QSound::play(":/key.wav");
    close();
    MainWindow *w = new MainWindow(nullptr, gameManager);
    w->show();
    music->stop();
}
