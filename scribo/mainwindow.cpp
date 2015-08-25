#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preference.h"
#include "aes.h"
#include "regex.h"
#include "database.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->textEdit_mainWindow_surface);

    updatePreferences();

    db.transaction();
    query.prepare("CREATE TABLE IF NOT EXISTS `password` ( "
                  "`PasswordID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
                  "`Password` TEXT NOT NULL UNIQUE );");
    if ( query.exec() ) {
        db.commit();
        qDebug() << "The table of the database exists";
    } else {
        db.rollback();
        qDebug() << "The table of the database does not exist";
        qDebug() << query.lastError();
    }

    connect(this, SIGNAL(backgroundChanged()), this, SLOT(updatePreferences()));
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
    //ui->textEdit_mainWindow_surface->cut();
    QSettings setting("rk", "scribo");
    setting.beginGroup("writing");
    QString style = "#textEdit_mainWindow_surface { margin: 0 "+
            setting.value("marginLeft", 0).toString() +
            " 0 " +
            setting.value("marginRight", 0).toString() + "; }";
    ui->textEdit_mainWindow_surface->setStyleSheet(style);
    setting.endGroup();
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
    preference = new Preference();
    connect(preference, SIGNAL(preferencesChanged()), this, SLOT(updatePreferences()));

    preference->setModal(true);
    preference->setAttribute(Qt::WA_DeleteOnClose, true);
    preference->exec();
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

void MainWindow::on_actionFont_triggered()
{
    color = QColorDialog::getColor(Qt::white, this);
    ui->textEdit_mainWindow_surface->setTextColor(color);
}

void MainWindow::on_actionHighlighting_triggered()
{
    color = QColorDialog::getColor(Qt::white, this);
    ui->textEdit_mainWindow_surface->setTextBackgroundColor(color);
}

void MainWindow::on_actionBackground_triggered()
{
    QPalette* palette = new QPalette();
    color = QColorDialog::getColor(Qt::white, this);

    QSettings setting("rk", "scribo");
    setting.beginGroup("writing");
    setting.setValue( "red", color.red() );
    setting.setValue( "green", color.green() );
    setting.setValue( "blue", color.blue() );
    setting.endGroup();

    emit backgroundChanged();
}

void MainWindow::on_actionFont_2_triggered()
{
    bool ok;
    font = QFontDialog::getFont(&ok, this);
    if (!ok) {
        return;
    } else {
        cursor = ui->textEdit_mainWindow_surface->textCursor();
        QTextCharFormat textCharFormat = cursor.charFormat();
        textCharFormat.setFont(font);
        cursor.setCharFormat(textCharFormat);
    }
}

void MainWindow::updatePreferences()
{
    QSettings setting("rk", "scribo");
    setting.beginGroup("writing");

    QString style = "#textEdit_mainWindow_surface { margin: " +
            setting.value("marginTop", 35).toString() +
            " " +
            setting.value("marginRight", 35).toString() +
            " " +
            setting.value("marginBottom", 0).toString() +
            " " +
            setting.value("marginLeft", 35).toString() + "; " +
            "padding: " +
            setting.value("paddingTop", 20).toString() +
            " " +
            setting.value("paddingRight", 20).toString() +
            " " +
            setting.value("paddingBottom", 0).toString() +
            " " +
            setting.value("paddingLeft", 20).toString() +
            "; background: " +
            "rgb(" +
                setting.value("red", 255).toString() + "," +
                setting.value("green", 255).toString() + "," +
                setting.value("blue", 255).toString() + "); " +
            "}";
    ui->textEdit_mainWindow_surface->setStyleSheet(style);
    passwordUsed = setting.value("passwordUsed", false).toBool();
    setting.endGroup();
}

void MainWindow::on_actionEncrypt_triggered()
{
    if (passwordUsed)
    {
        query.prepare("SELECT `Password` FROM `password`;");
        query.exec();
        QString passwordBase;
        while ( query.next() ) {
            passwordBase = query.value(0).toString();
        }
        QByteArray passwordTemp = QByteArray::fromBase64( passwordBase.toUtf8() );
        password = QString(passwordTemp);

        Regex regex;
        QRegExp rePassword( regex.getPassword() );

        if ( !password.isEmpty() && password.contains(rePassword) )
        {
            encrypted = encrypt(password);
            ui->textEdit_mainWindow_surface->setPlainText( encrypted.toBase64() );
        } else
        {
            QMessageBox::critical(this, tr("Error"), tr("An error has ocurred. Please revise your entries and note the following:\n"
                                                        "A Password must contain at least one upper case letter.\n"
                                                        "A Password must contain at least one lower case letter.\n"
                                                        "A Password must contain at least one numeric digit."));
        }
    } else
    {
        bool ok = false;

        QInputDialog* inputDialog = new QInputDialog();
        inputDialog->setAttribute(Qt::WA_DeleteOnClose, true);

        inputDialog->setOptions(QInputDialog::NoButtons);

        password = inputDialog->getText(NULL ,"Encryption",
                                                  "Password:", QLineEdit::Password,
                                                  "", &ok);

        if (ok)
        {
            Regex regex;
            QRegExp rePassword( regex.getPassword() );

            if ( !password.isEmpty() && password.contains(rePassword) )
            {
                encrypted = encrypt(password);
                ui->textEdit_mainWindow_surface->setPlainText( encrypted.toBase64() );
            } else
            {
                QMessageBox::critical(this, tr("Error"), tr("An error has ocurred. Please revise your entries and note the following:\n"
                                                            "A Password must contain at least one upper case letter.\n"
                                                            "A Password must contain at least one lower case letter.\n"
                                                            "A Password must contain at least one numeric digit."));
            }
        } else
        {

            inputDialog->reject();
        }
        inputDialog->close();
    }
}

void MainWindow::on_actionDecrypt_triggered()
{
    if (passwordUsed)
    {
        query.prepare("SELECT `Password` FROM `password`;");
        query.exec();
        QString passwordBase;
        while ( query.next() ) {
            passwordBase = query.value(0).toString();
        }
        QByteArray passwordTemp = QByteArray::fromBase64( passwordBase.toUtf8() );
        password = QString(passwordTemp);

        Regex regex;
        QRegExp rePassword( regex.getPassword() );

        if ( !password.isEmpty() && password.contains(rePassword) )
        {
            decrypted = decrypt(password);
            ui->textEdit_mainWindow_surface->setHtml( QString(decrypted) );
        } else
        {
            QMessageBox::critical(this, tr("Error"), tr("An error has ocurred. Please revise your entries and note the following:\n"
                                                        "A Password must contain at least one upper case letter.\n"
                                                        "A Password must contain at least one lower case letter.\n"
                                                        "A Password must contain at least one numeric digit."));
        }
    } else
    {
        bool ok = false;

        QInputDialog* inputDialog = new QInputDialog();
        inputDialog->setAttribute(Qt::WA_DeleteOnClose, true);

        inputDialog->setOptions(QInputDialog::NoButtons);

        password = inputDialog->getText(NULL ,"Decryption",
                                                  "Password:", QLineEdit::Password,
                                                  "", &ok);

        if (ok)
        {
            Regex regex;
            QRegExp rePassword( regex.getPassword() );

            if ( !password.isEmpty() && password.contains(rePassword) )
            {
                decrypted = decrypt(password);
                ui->textEdit_mainWindow_surface->setHtml( QString(decrypted) );
            } else
            {
                QMessageBox::critical(this, tr("Error"), tr("An error has ocurred. Please revise your entries and note the following:\n"
                                                            "A Password must contain at least one upper case letter.\n"
                                                            "A Password must contain at least one lower case letter.\n"
                                                            "A Password must contain at least one numeric digit."));
            }
        } else
        {

            inputDialog->reject();
        }
        inputDialog->close();
    }
}

QByteArray MainWindow::encrypt(QString password)
{
    AES crypto;

    QString hashedPassword = QString( QCryptographicHash::hash(( password.toUtf8() ),QCryptographicHash::Md5).toHex() );

    QByteArray key = crypto.hexStringToByte(hashedPassword);
    QByteArray data = ui->textEdit_mainWindow_surface->toHtml().toUtf8();

    QByteArray encrypted = crypto.encrypt(data, key);

    return encrypted;
}

QByteArray MainWindow::decrypt(QString password)
{
    AES crypto;

    QString hashedPassword = QString( QCryptographicHash::hash(( password.toUtf8() ),QCryptographicHash::Md5).toHex() );

    QByteArray key = crypto.hexStringToByte(hashedPassword);
    QByteArray data = QByteArray::fromBase64( ui->textEdit_mainWindow_surface->toPlainText().toUtf8() );

    QByteArray decrypted = crypto.decrypt(data, key);

    return decrypted;
}
