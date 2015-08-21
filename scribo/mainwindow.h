#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>

class Preference;

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

    void on_actionAlign_text_left_triggered();

    void on_actionCenter_text_triggered();

    void on_actionAlign_text_right_triggered();

    void on_actionJustification_triggered();

    void on_actionFont_triggered();

    void on_actionHighlighting_triggered();

    void on_actionBackground_triggered();

    void on_actionFont_2_triggered();

    void updatePreferences(); // const QString &name

private:
    Ui::MainWindow *ui;
    QString fileName;
    QTextCursor cursor;
    QColor color;
    QFont font;
    Preference *preference;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
};

#endif // MAINWINDOW_H
