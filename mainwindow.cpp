#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), textEdit(new QTextEdit(this)), isModified(false)
{
    textEdit->setAcceptRichText(true);
    setCentralWidget(textEdit);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), &QTextDocument::contentsChanged,
            this, [this]() {
                isModified = true;
                setWindowModified(true);
            });

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile("");
    }
}

void MainWindow::openFile()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"), "", tr("Text Files (*.txt);;All Files (*)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::saveFileSlot()
{
    if (currentFile.isEmpty()) {
        saveAsFile();
    } else {
        saveFile(currentFile);
    }
    return true;
}

bool MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save File"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty())
        saveFile(fileName);
    return true;
}

void MainWindow::exitApp()
{
    close();
}

void MainWindow::cutText()
{
    textEdit->cut();
}

void MainWindow::copyText()
{
    textEdit->copy();
}

void MainWindow::pasteText()
{
    textEdit->paste();
}

void MainWindow::selectAllText()
{
    textEdit->selectAll();
}

void MainWindow::setFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, textEdit->font(), this);
    if (ok)
        textEdit->setFont(font);
}

void MainWindow::setTextColor()
{
    QColor color = QColorDialog::getColor(textEdit->textColor(), this);
    if (color.isValid())
        textEdit->setTextColor(color);
}

void MainWindow::setBackgroundColor()
{
    QColor color = QColorDialog::getColor(textEdit->palette().color(QPalette::Base), this);
    if (color.isValid()) {
        QPalette palette = textEdit->palette();
        palette.setColor(QPalette::Base, color);
        textEdit->setPalette(palette);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Text Editor"),
        tr("<h2>Text Editor 1.0</h2>"
           "<p>Simple text editor built with Qt</p>"
           "<p>Copyright © 2025</p>"));
}

void MainWindow::aboutQt()
{
    QApplication::aboutQt();
}

void MainWindow::createActions()
{
    // File actions
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this,  [this]() { saveFileSlot(); });

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsFile);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::exitApp);

    // Edit actions
    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(cutAct, &QAction::triggered, this, &MainWindow::cutText);

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(copyAct, &QAction::triggered, this, &MainWindow::copyText);

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(pasteAct, &QAction::triggered, this, &MainWindow::pasteText);

    selectAllAct = new QAction(tr("Select &All"), this);
    selectAllAct->setShortcuts(QKeySequence::SelectAll);
    selectAllAct->setStatusTip(tr("Select all the text in the document"));
    connect(selectAllAct, &QAction::triggered, this, &MainWindow::selectAllText);

    // Format actions
    fontAct = new QAction(tr("&Font..."), this);
    fontAct->setStatusTip(tr("Change the font style"));
    connect(fontAct, &QAction::triggered, this, &MainWindow::setFont);

    textColorAct = new QAction(tr("Text &Color..."), this);
    textColorAct->setStatusTip(tr("Change the text color"));
    connect(textColorAct, &QAction::triggered, this, &MainWindow::setTextColor);

    backgroundColorAct = new QAction(tr("&Background Color..."), this);
    backgroundColorAct->setStatusTip(tr("Change the background color"));
    connect(backgroundColorAct, &QAction::triggered, this, &MainWindow::setBackgroundColor);

    // Help actions
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAct);

    formatMenu = menuBar()->addMenu(tr("F&ormat"));
    formatMenu->addAction(fontAct);
    formatMenu->addAction(textColorAct);
    formatMenu->addAction(backgroundColorAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

bool MainWindow::maybeSave()
{
    if (!isModified)
        return true;

    const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Text Editor"),
        tr("The document has been modified.\nDo you want to save your changes?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (ret) {
    case QMessageBox::Save:
        return saveFileSlot();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Text Editor"),
            tr("Cannot read file %1:\n%2.")
            .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Text Editor"),
            tr("Cannot write file %1:\n%2.")
            .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out << textEdit->toPlainText();
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    textEdit->document()->setModified(false);
    isModified = false;
    setWindowModified(false);

    QString shownName = currentFile;
    if (currentFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

void MainWindow::readSettings()
{
    QSettings settings("MyCompany", "TextEditor");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings("MyCompany", "TextEditor");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
