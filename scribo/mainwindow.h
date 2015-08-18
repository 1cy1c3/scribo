#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionScribo_Help_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionPaste_triggered();

    void on_actionCheck_for_updates_triggered();

    void on_actionAbout_triggered();

    void on_actionSettings_triggered();

    void on_actionClose_triggered();

    void on_actionNormal_triggered();

    void on_actionBold_triggered();

    void on_actionItalic_triggered();

    void on_actionUnderline_triggered();

private:
    Ui::MainWindow *ui;
    QString fileName;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
};

#endif // MAINWINDOW_H
