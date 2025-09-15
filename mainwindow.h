#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QCloseEvent>
#include <QSettings>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextStream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void newFile();
    void openFile();
    bool saveFileSlot();
    bool saveAsFile();
    void exitApp();

    void cutText();
    void copyText();
    void pasteText();
    void selectAllText();

    void setFont();
    void setTextColor();
    void setBackgroundColor();

    void about();
    void aboutQt();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();

    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    QTextEdit *textEdit;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *selectAllAct;

    QAction *fontAct;
    QAction *textColorAct;
    QAction *backgroundColorAct;

    QAction *aboutAct;
    QAction *aboutQtAct;

    QString currentFile;
    bool isModified;
};

#endif // MAINWINDOW_H
