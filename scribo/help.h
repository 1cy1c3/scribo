/**
 * @file help.h
 * @brief Represents the help about this program
 */

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

/**
 * Realises the help dialog
 *
 * @author Rune Krauss
 */
class Help : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Initializes the about object
     * @param parent Pointer to the super class of objects
     */
    explicit Help(QWidget *parent = 0);
    /**
     * @brief Destroys the about object
     */
    ~Help();

signals:

private slots:
    /**
     * @brief Plays a video
     */
    void on_actionPlay_triggered();
    /**
     * @brief Pauses a video
     */
    void on_actionPause_triggered();
    /**
     * @brief Stops a video
     */
    void on_actionStop_triggered();

private:
    /**
     * @brief Video player
     */
    QMediaPlayer *player;
    /**
     * @brief Video widget
     */
    QVideoWidget *vw;
    /**
     * @brief Slider
     */
    QSlider *slider;
    /**
     * @brief Progress bar
     */
    QProgressBar *bar;
    /**
     * @brief Refreshes the help size
     */
    void setHelpHeight();
};

#endif // HELP_H
