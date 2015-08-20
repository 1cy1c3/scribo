#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit_mainWindow_surface);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    fileName = "";
    ui->textEdit_mainWindow_surface->setHtml("");
}

void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Document", QDir::currentPath(), "text files (*.txt)");

    if ( !file.isEmpty() )
    {
        QFile sFile(file);
        if (sFile.open(QFile::ReadOnly | QFile::Text) )
        {
            fileName = file;
            QTextStream in(&sFile);
            QString text = in.readAll();
            sFile.close();

            ui->textEdit_mainWindow_surface->setHtml(text);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if ( fileName.isEmpty() )
    {
        on_actionSave_As_triggered();
    } else
    {
        QFile sFile(fileName);
        if ( sFile.open(QFile::WriteOnly | QFile::Text) )
        {
            QTextStream out(&sFile);

            out << ui->textEdit_mainWindow_surface->toHtml();

            sFile.flush();
            sFile.close();
        }
    }
}


void MainWindow::on_actionSave_As_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, "Save Document", QDir::currentPath(), "text files (*.txt)");

    if ( !file.isEmpty() )
    {
        fileName = file;
        on_actionSave_triggered();
    }
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit_mainWindow_surface->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit_mainWindow_surface->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit_mainWindow_surface->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit_mainWindow_surface->redo();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit_mainWindow_surface->paste();
}

void MainWindow::on_actionScribo_Help_triggered()
{

}

void MainWindow::on_actionCheck_for_updates_triggered()
{

}

void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::on_actionSettings_triggered()
{

}

void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionNormal_triggered()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Normal);
    fmt.setFontItalic(false);
    fmt.setFontUnderline(false);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::on_actionBold_triggered()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Bold);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::on_actionItalic_triggered()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(true);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::on_actionUnderline_triggered()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(true);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    if ( !cursor.hasSelection() )
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->textEdit_mainWindow_surface->mergeCurrentCharFormat(format);
}

void MainWindow::on_actionAlign_text_left_triggered()
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignLeft);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit_mainWindow_surface->setTextCursor(cursor);
}

void MainWindow::on_actionCenter_text_triggered()
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit_mainWindow_surface->setTextCursor(cursor);
}

void MainWindow::on_actionAlign_text_right_triggered()
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit_mainWindow_surface->setTextCursor(cursor);
}

void MainWindow::on_actionJustification_triggered()
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignJustify);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit_mainWindow_surface->setTextCursor(cursor);
}
