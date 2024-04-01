#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "ui_mainwindow2.h"
#include <qss/QtAdvancedStylesheet.h>
#include <QPushButton>
#include "SpiceHighlighter.hpp"

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
};
#endif // CMAINWINDOW_H



