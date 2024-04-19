#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include "ui_mainwindow2.h"

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>

#include "qss/QtAdvancedStylesheet.h"
#include "utils/SpiceHighlighter.hpp"
#include "global/global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyMainWindow; }
QT_END_NAMESPACE

struct MyMainWindowPrivate;

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyMainWindow(QWidget *parent = nullptr);
    virtual ~MyMainWindow();

private:
    void updateThemeColorButtons();
    Ui::MainWindow2 ui;
    acss::QtAdvancedStylesheet* AdvancedStyleSheet;
    QVector<QPushButton*> ThemeColorButtons;

    QString fileName = "";
    SpiceHighlighter* highlighter;
    void openDialogAndSave();

private slots:
    void on_actionOpen_triggered();
    void onStyleManagerStylesheetChanged();
    void on_actionNew_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_triggered();
    void on_actionParse_triggered();
    void on_actionDebug_triggered();
    void on_actionAbout_triggered();
};
#endif // CMAINWINDOW_H



