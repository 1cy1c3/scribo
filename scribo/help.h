#ifndef HELP_H
#define HELP_H

#include <QDialog>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QProgressBar>
#include <QStatusBar>
#include <QAction>
#include <QApplication>
#include <QCoreApplication>

class Help : public QDialog
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = 0);
    ~Help();

signals:

private slots:
    void on_actionPlay_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();

private:
    QMediaPlayer *player;
    QVideoWidget *vw;
    QSlider *slider;
    QProgressBar *bar;
    void setHelpHeight();
};

#endif // HELP_H
