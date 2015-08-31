/** @file mainwindow.cpp
 * Presents the main window of this program
 * Holds main features like text and document operations
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preference.h"
#include "aes.h"
#include "regex.h"
#include "database.h"
#include "about.h"
#include "help.h"
#include "update.h"

/**
 * Initializes the main window object
 * Updates preferences
 * Checks whether database tables are exists
 * Adds a context menu and sets accepted drops
 * Moreover, registers signals and slots
 * @param parent Pointer to the super class of objects
 */
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

/**
 * Destroys the about object
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Creates a new document
 */
void MainWindow::on_actionNew_triggered()
{
    fileName = "";
    ui->textEdit_mainWindow_surface->setHtml("");
}

/**
 * Opens a document and checks its format
 */
void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Document"), QDir::home().absolutePath(), "text files (*.sb)");
    if ( !file.isEmpty() ) {
        QString text = getFileContent(file);
        ui->textEdit_mainWindow_surface->setHtml(text);
    } else {
        return;
    }

}

/**
 * Saves a document and draws an attention to existing documents
 */
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

/**
 * Saves a document in a specific location and draws an attention to existing documents
 */
void MainWindow::on_actionSave_As_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::home().absolutePath(), "text files (*.sb)");

    if ( !file.isEmpty() )
    {
        fileName = file;
        on_actionSave_triggered();
    }
}

/**
 * Copies selected text
 */
void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit_mainWindow_surface->copy();
}

/**
 * Cuts out selected text
 */
void MainWindow::on_actionCut_triggered()
{
    ui->textEdit_mainWindow_surface->cut();
}

/**
 * Undoes an action
 */
void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit_mainWindow_surface->undo();
}

/**
 * Redoes an action
 */
void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit_mainWindow_surface->redo();
}

/**
 * Pastes selected text
 */
void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit_mainWindow_surface->paste();
}

/**
 * Opens the help dialog with a video
 */
void MainWindow::on_actionScribo_Help_triggered()
{
    Help help;
    help.setModal(true);
    help.exec();
}

/**
 * Checks for potential updates
 */
void MainWindow::on_actionCheck_for_updates_triggered()
{
    Update update;
    update.setModal(true);
    update.exec();
}

/**
 * Opens the about dialog with program infos
 */
void MainWindow::on_actionAbout_triggered()
{
    About about;
    about.setModal(true);
    about.exec();
}

/**
 * Opens the preferences dialog
 */
void MainWindow::on_actionSettings_triggered()
{
    preference = new Preference();
    connect(preference, SIGNAL(preferencesChanged()), this, SLOT(updatePreferences()));

    preference->setModal(true);
    preference->setAttribute(Qt::WA_DeleteOnClose, true);
    preference->exec();
}

/**
 * Quits the program after a confirmation dialog
 */
void MainWindow::on_actionClose_triggered()
{
    if ( !ui->textEdit_mainWindow_surface->toPlainText().isEmpty() ) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Confirmation"), tr("The content is not empty. Are you sure?"),
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

/**
 * Standardizes a selected text
 */
void MainWindow::on_actionNormal_triggered()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Normal);
    fmt.setFontItalic(false);
    fmt.setFontUnderline(false);
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Makes a selected text bold
 */
void MainWindow::on_actionBold_triggered()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Bold);
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Makes a selected text italic
 */
void MainWindow::on_actionItalic_triggered()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(true);
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Makes a selected text underlined
 */
void MainWindow::on_actionUnderline_triggered()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(true);
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Merges a format on a word or selection
 * Sets the cursor to its right position
 * @param format Format
 */
void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    if ( !cursor.hasSelection() )
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->textEdit_mainWindow_surface->mergeCurrentCharFormat(format);
}

/**
 * Aligns a selected text left
 */
void MainWindow::on_actionAlign_text_left_triggered()
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignLeft);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit_mainWindow_surface->setTextCursor(cursor);
}

/**
 * Centers a selected text
 */
void MainWindow::on_actionCenter_text_triggered()
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit_mainWindow_surface->setTextCursor(cursor);
}

/**
 * Aligns a selected text right
 */
void MainWindow::on_actionAlign_text_right_triggered()
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit_mainWindow_surface->setTextCursor(cursor);
}

/**
 * Creates a justification regarding a selected text
 */
void MainWindow::on_actionJustification_triggered()
{
    cursor = ui->textEdit_mainWindow_surface->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignJustify);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit_mainWindow_surface->setTextCursor(cursor);
}

/**
 * Opens the font dialog
 */
void MainWindow::on_actionFont_triggered()
{
    color = QColorDialog::getColor(Qt::white, this);
    ui->textEdit_mainWindow_surface->setTextColor(color);
}

/**
 * Highlights a selected text
 */
void MainWindow::on_actionHighlighting_triggered()
{
    color = QColorDialog::getColor(Qt::white, this);
    ui->textEdit_mainWindow_surface->setTextBackgroundColor(color);
}

/**
 * Sets the background color with a dialog
 */
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

/**
 * Opens a submenu regarding font settings
 */
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

/**
 * Updates preferences
 */
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

/**
 * Encrypts a text using plaintext and cipher key
 * Chooses between a permanent or a individual password using the preferences
 */
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
            qDebug() << "Data could not be encrypted because the password requirements were not met.";
        }
    } else
    {
        bool ok = false;

        QInputDialog* inputDialog = new QInputDialog();
        inputDialog->setAttribute(Qt::WA_DeleteOnClose, true);

        inputDialog->setOptions(QInputDialog::NoButtons);

        password = inputDialog->getText(NULL ,tr("Encryption"),
                                                  tr("Password:"), QLineEdit::Password,
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
                qDebug() << "Data could not be encrypted because the password requirements were not met.";
            }
        } else
        {

            inputDialog->reject();
        }
        inputDialog->close();
    }
}

/**
 * Decrypts a text using cipher text and cipher key
 * Chooses between a permanent or a individual password using the preferences
 */
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
            qDebug() << "Data could not be decrypted because the password requirements were not met.";
        }
    } else
    {
        bool ok = false;

        QInputDialog* inputDialog = new QInputDialog();
        inputDialog->setAttribute(Qt::WA_DeleteOnClose, true);

        inputDialog->setOptions(QInputDialog::NoButtons);

        password = inputDialog->getText(NULL ,tr("Decryption"),
                                                  tr("Password:"), QLineEdit::Password,
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
                qDebug() << "Data could not be decrypted because the password requirements were not met.";
            }
        } else
        {

            inputDialog->reject();
        }
        inputDialog->close();
    }
}

/**
 * Encrypts the actual text using the password
 * @param password Password
 * @return Encrypted text
 */
QByteArray MainWindow::encrypt(QString password)
{
    AES crypto;

    QString hashedPassword = QString( QCryptographicHash::hash(( password.toUtf8() ),QCryptographicHash::Md5).toHex() );

    QByteArray key = crypto.hexStringToByte(hashedPassword);
    QByteArray data = ui->textEdit_mainWindow_surface->toHtml().toUtf8();

    QByteArray encrypted = crypto.encrypt(data, key);

    return encrypted;
}

/**
 * Decrypts the actual text using the password
 * @param password Password
 * @return Decrypted text
 */
QByteArray MainWindow::decrypt(QString password)
{
    AES crypto;

    QString hashedPassword = QString( QCryptographicHash::hash(( password.toUtf8() ),QCryptographicHash::Md5).toHex() );

    QByteArray key = crypto.hexStringToByte(hashedPassword);
    QByteArray data = QByteArray::fromBase64( ui->textEdit_mainWindow_surface->toPlainText().toUtf8() );

    QByteArray decrypted = crypto.decrypt(data, key);

    return decrypted;
}

/**
 * Inserts an valid image
 * Copies the specific image in the program folder
 */
void MainWindow::on_actionImage_triggered()
{
    QString scriboDir = QDir::home().absolutePath() + QDir::separator() + "scribo";

    if ( !QDir(scriboDir + QDir::separator() + "img").exists() )
        QDir().mkdir(scriboDir + QDir::separator() + "img");

    QString filePath = QFileDialog::getOpenFileName(this, tr("Select an image"),
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

/**
 * Opens the printer dialog
 */
void MainWindow::on_actionPrint_triggered()
{
    QPrinter *printer;
    QPrintDialog pd(this);
    if (pd.exec() == QDialog::Rejected)
        return;
    printer = pd.printer();
    ui->textEdit_mainWindow_surface->print(printer);
}

/**
 * Adds a context menu with operations like copy and paste
 */
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

/**
 * Event which is sent to a widget when a drag and drop action enters it
 * @param event Event
 */
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

/**
 * Event that is sent to a widget when a drag and drop action leaves it
 * @param event Event
 */
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

/**
 * Event which is sent while a drag and drop action is in progress
 * @param event Event
 */
void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

/**
 * Event which is sent when a drag and drop action is completed
 * @param event Event
 */
void MainWindow::dropEvent(QDropEvent *event)
{
    if ( !ui->textEdit_mainWindow_surface->toPlainText().isEmpty() ) {
        QMessageBox::StandardButton reply;
          reply = QMessageBox::question(this, tr("Confirmation"), tr("The content is not empty. Are you sure?"),
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

/**
 * Gets the file content of a given file
 * @param file File
 * @return File content
 */
QString MainWindow::getFileContent(QString file)
{
    QStringList list = file.split( "/" );
    QString fileName = list.value(list.length() - 1 );
    QStringList list2 = fileName.split( "." );
    QString fileFormat = list2.value(list2.length() - 1);

    if ( !fileFormat.contains("sb") ) {
        QMessageBox::critical(this, tr("Error"), tr("There are only formats like 'sb' allowed."));
        qDebug() << "File content could not be loaded because the format is incorrect.";
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
