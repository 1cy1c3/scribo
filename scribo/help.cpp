/** @file about.cpp
 * Represents the help about this program
 * Furthermore, presents a video with a status bar
 */

#include "help.h"

/**
 * Constructor of the help object
 * Creates a tool bar, surface and status bar
 * Moreover, creates a video widget within video player
 * @param parent Pointer to the super class of objects
 */
Help::Help(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Help"));
    this->setStyleSheet("#Help {height: 262px;}");

    player = new QMediaPlayer(this);
    vw = new QVideoWidget(this);
    vw->setMinimumHeight(262);
    vw->setMinimumWidth(420);
    player->setVideoOutput(vw);

    #ifdef WIN
        player->setMedia(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/video/help.mp4"));
    #endif

    #ifndef WIN
        player->setMedia(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/../Resources/video/help.mp4"));
    #endif

    player->setVolume(80);

    QBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setAlignment(Qt::AlignCenter);

    QToolBar *toolBar = new QToolBar();
    QAction *play = new QAction(tr("Play"), toolBar);
    play->setToolTip(tr("Play"));
    play->setIcon(QIcon(":/img/play.png"));
    QAction *pause = new QAction(tr("Pause"), toolBar);
    pause->setToolTip(tr("Pause"));
    pause->setIcon(QIcon(":/img/pause.png"));
    QAction *stop = new QAction(tr("Stop"), toolBar);
    stop->setToolTip(tr("Stop"));
    stop->setIcon(QIcon(":/img/stop.png"));
    toolBar->addAction(play);
    toolBar->addAction(pause);
    toolBar->addAction(stop);

    connect(play, SIGNAL(triggered()), this, SLOT(on_actionPlay_triggered()));
    connect(pause, SIGNAL(triggered()), this, SLOT(on_actionPause_triggered()));
    connect(stop, SIGNAL(triggered()), this, SLOT(on_actionStop_triggered()));

    boxLayout->addWidget(toolBar);

    boxLayout->addWidget(vw);

    slider = new QSlider(this);
    bar = new QProgressBar(this);

    slider->setOrientation(Qt::Horizontal);

    QStatusBar *statusBar = new QStatusBar;
    statusBar->addPermanentWidget(slider);
    statusBar->addPermanentWidget(bar);
    connect(player, &QMediaPlayer::durationChanged, slider, &QSlider::setMaximum);
    connect(player, &QMediaPlayer::positionChanged, slider, &QSlider::setValue);
    connect(slider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    connect(player, &QMediaPlayer::durationChanged, bar, &QProgressBar::setMaximum);
    connect(player, &QMediaPlayer::positionChanged, bar, &QProgressBar::setValue);

    boxLayout->addWidget(statusBar);

    setLayout(boxLayout);
}

/**
 * Destructor of the help object
 */
Help::~Help()
{
}

/**
 * Plays a video and refreshes the widget
 */
void Help::on_actionPlay_triggered()
{
    player->play();
    setHelpHeight();
}

/**
 * Pauses a video and refreshes the widget
 */
void Help::on_actionPause_triggered()
{
    player->pause();
    setHelpHeight();
}

/**
 * Stops a video and refreshes the widget
 */
void Help::on_actionStop_triggered(){
    player->stop();
    setHelpHeight();
}

/**
 * Sets the size of the help widget
 */
void Help::setHelpHeight()
{
    this->setStyleSheet("#Help {height: 262px;}");
}
