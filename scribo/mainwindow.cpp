#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preference.h"
#include "aes.h"
#include "regex.h"
#include "database.h"
#include "about.h"
#include "help.h"

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

    addContextMenu();

    ui->textEdit_mainWindow_surface->setAcceptDrops(false);
    setAcceptDrops(true);

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
    QString file = QFileDialog::getOpenFileName(this, "Open Document", QDir::home().absolutePath(), "text files (*.sb)");
    if ( !file.isEmpty() ) {
        QString text = getFileContent(file);
        ui->textEdit_mainWindow_surface->setHtml(text);
    } else {
        return;
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
    QString file = QFileDialog::getSaveFileName(this, "Save Document", QDir::home().absolutePath(), "text files (*.sb)");

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
    Help help;
    help.setModal(true);
    help.exec();
}

void MainWindow::on_actionCheck_for_updates_triggered()
{

}

void MainWindow::on_actionAbout_triggered()
{
    About about;
    about.setModal(true);
    about.exec();
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
    if ( !ui->textEdit_mainWindow_surface->toPlainText().isEmpty() ) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "The content is not empty. Are you sure?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QApplication::quit();
        } else {
          return;
        }
    } else {
        QApplication::quit();
    }
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

        ui->textEdit_mainWindow_surface->setFontPointSize( font.pointSizeF() );
        ui->textEdit_mainWindow_surface->setFontFamily( font.family() );
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

void MainWindow::on_actionImage_triggered()
{
    QString scriboDir = QDir::home().absolutePath() + QDir::separator() + "scribo";

    if ( !QDir(scriboDir + QDir::separator() + "img").exists() )
        QDir().mkdir(scriboDir + QDir::separator() + "img");

    QString filePath = QFileDialog::getOpenFileName(this, "Select an image",
                                      QDir::home().absolutePath(), "Bitmap Files (*.bmp)\n"
                                        "JPEG (*.jpg *jpeg)\n"
                                        "GIF (*.gif)\n"
                                        "PNG (*.png)");
    QStringList list = filePath.split( "/" );
    QString imageName = list.value(list.length() - 1 );
    QString imagePath = scriboDir + QDir::separator() + "img" + QDir::separator() + imageName;

    QFile::copy(filePath, imagePath);
    QUrl Uri ( QString ( "file://%1" ).arg ( imagePath ) );
    QImage image = QImageReader ( imagePath ).read();

    QTextDocument * textDocument = ui->textEdit_mainWindow_surface->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter *printer;
    QPrintDialog pd(this);
    if (pd.exec() == QDialog::Rejected)
        return;
    printer = pd.printer();
    ui->textEdit_mainWindow_surface->print(printer);
}

void MainWindow::addContextMenu()
{
    QAction *copy, *cut, *paste, *font, *color;
    copy = new QAction("Copy", this);
    cut = new QAction("Cut", this);
    paste = new QAction("Paste", this);
    font = new QAction("Font", this);
    color = new QAction("Color", this);
    connect(copy, SIGNAL(triggered()), this, SLOT(on_actionCopy_triggered()));
    connect(cut, SIGNAL(triggered()), this, SLOT(on_actionCut_triggered()));
    connect(paste, SIGNAL(triggered()), this, SLOT(on_actionPaste_triggered()));
    connect(font, SIGNAL(triggered()), this, SLOT(on_actionFont_2_triggered()));
    connect(color, SIGNAL(triggered()), this, SLOT(on_actionFont_triggered()));
    ui->textEdit_mainWindow_surface->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->textEdit_mainWindow_surface->addAction(copy);
    ui->textEdit_mainWindow_surface->addAction(cut);
    ui->textEdit_mainWindow_surface->addAction(paste);
    ui->textEdit_mainWindow_surface->addAction(font);
    ui->textEdit_mainWindow_surface->addAction(color);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if ( !ui->textEdit_mainWindow_surface->toPlainText().isEmpty() ) {
        QMessageBox::StandardButton reply;
          reply = QMessageBox::question(this, "Confirmation", "The content is not empty. Are you sure?",
                                        QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::Yes) {
              QString file;
              QList<QUrl> urls;
              QList<QUrl>::Iterator i;
              urls = event->mimeData()->urls();
              for (i = urls.begin(); i != urls.end(); ++i)
              {
                  file = i->toLocalFile();
                  QString text = getFileContent(file);
                  ui->textEdit_mainWindow_surface->setHtml(text);
              }
          } else {
            return;
          }
    } else {
        QString file;
        QList<QUrl> urls;
        QList<QUrl>::Iterator i;
        urls = event->mimeData()->urls();
        for (i = urls.begin(); i != urls.end(); ++i)
        {
            file = i->toLocalFile();
            QString text = getFileContent(file);
            ui->textEdit_mainWindow_surface->setHtml(text);
        }
    }
}

QString MainWindow::getFileContent(QString file)
{
    QStringList list = file.split( "/" );
    QString fileName = list.value(list.length() - 1 );
    QStringList list2 = fileName.split( "." );
    QString fileFormat = list2.value(list2.length() - 1);

    if ( !fileFormat.contains("sb") ) {
        QMessageBox::critical(this, tr("Error"), tr("There are only formats like 'sb' and 'txt' allowed."));
        return ui->textEdit_mainWindow_surface->toHtml();
    }

    QString text = "";
    QFile sFile(file);
    if (sFile.open(QFile::ReadOnly | QFile::Text) )
    {
        fileName = file;
        QTextStream in(&sFile);
        text = in.readAll();
        sFile.close();
    }
    return text;
}
