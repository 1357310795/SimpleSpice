#include "mymainwindow.h"
#include "ui_mainwindow2.h"

#include <QDir>
#include <QApplication>
#include <QAction>
#include <QListWidgetItem>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>
#include "parser/scanner.hpp"
#include "parser/parser.hpp"
#include "calc/analyze_manager.hpp"
#include "console/myconsole.h"
#include "utils/SpiceHighlighter.hpp"
#include "global/global.h"
#include "chart/mychart.h"
#include "qss/QtAdvancedStylesheet.h"

#define _STR(x) #x
#define STRINGIFY(x)  _STR(x)

void MyMainWindow::updateThemeColorButtons()
{
    for (auto Button : ThemeColorButtons)
    {
        auto Color = AdvancedStyleSheet->themeColor(Button->text());
        QString TextColor = (Color.value() < 128) ? "#ffffff" : "#000000";
        QString ButtonStylesheet = QString("background-color: %1; color: %2;"
            "border: none;").arg(Color.name()).arg(TextColor);
        Button->setStyleSheet(ButtonStylesheet);
    }
}

MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    qDebug() << STRINGIFY(STYLES_DIR);
    QString AppDir = qApp->applicationDirPath();
    QString StylesDir = STRINGIFY(STYLES_DIR);
    AdvancedStyleSheet = new acss::QtAdvancedStylesheet(this);
    AdvancedStyleSheet->setStylesDirPath(StylesDir);
    AdvancedStyleSheet->setOutputDirPath(AppDir + "/output");
    AdvancedStyleSheet->setCurrentStyle("qt_material");
    AdvancedStyleSheet->setDefaultTheme();
    AdvancedStyleSheet->updateStylesheet();
    setWindowIcon(AdvancedStyleSheet->styleIcon());
    qApp->setStyleSheet(AdvancedStyleSheet->styleSheet());
    connect(AdvancedStyleSheet, SIGNAL(stylesheetChanged()), this,
        SLOT(onStyleManagerStylesheetChanged()));
    setWindowTitle(tr("SimpleSpice"));

    console = new MyConsole(ui.consoleText);
    highlighter = new SpiceHighlighter(ui.textEdit->document());
    ui.textEdit->setFontPointSize(16);
}

MyMainWindow::~MyMainWindow()
{
    //delete d;
}

void MyMainWindow::onStyleManagerStylesheetChanged()
{
    qApp->setStyleSheet(AdvancedStyleSheet->styleSheet());
    updateThemeColorButtons();
}

void MyMainWindow::on_actionNew_triggered()
{
    fileName = "";
    ui.textEdit->clear();          /// Clear the text
    ui.textEdit->setHidden(false); /// Display the text.
}

void MyMainWindow::on_actionOpen_triggered()
{
    auto newFileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr(""), "All Files (*.*)");
    /// If the dialog is directly closed, the filename will be null.
    if (newFileName == "") {
        return;
    } else {
        QFile file(newFileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Error"), tr("Failed to open file!"));
            return;
        } else {
            if (!file.isReadable()) {
                QMessageBox::warning(this, tr("Error"), tr("The file is unreadable"));
            } else {
                QTextStream textStream(&file); // Use QTextStream to load text.
                while (!textStream.atEnd()) {
                    ui.textEdit->setPlainText(textStream.readAll());
                }
                fileName = newFileName;
                ui.textEdit->show();
                file.close();
            }
        }
    }
}

void MyMainWindow::on_actionSave_As_triggered()
{
    openDialogAndSave();
}

void MyMainWindow::openDialogAndSave()
{
    QFileDialog fileDialog;
    fileName = fileDialog.getSaveFileName(this, tr("Open File"), "./", "All Files(*.*)");
    if (fileName == "") {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file!"),
                             QMessageBox::Ok);
        return;
    } else {
        /// Create a text stream and pass text in.
        QTextStream textStream(&file);
        QString     str = ui.textEdit->toPlainText();
        textStream << str;
    }
    file.close();
}

void MyMainWindow::on_actionSave_triggered()
{
    statusBar()->showMessage(tr("Saving file..."));

    if (fileName == "") /// File has not been saved.
    {
        /// Text is empty.
        if (ui.textEdit->toPlainText() == "") {
            QMessageBox::warning(this, tr("Warning"), tr("Content cannot be empty!"),
                                 QMessageBox::Ok);
        } else {
            openDialogAndSave();
        }
    } else { /// File has been saved.
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Warning"), tr("Failed to open file!"));
            return;
        } else {
            QTextStream textStream(&file);
            QString     str = ui.textEdit->toPlainText();
            textStream << str;
            file.close();
        }
    }
}


void MyMainWindow::on_actionParse_triggered()
{
    if (fileName == "")
    {
        QMessageBox::warning(this, tr("Warning"), tr("filename cannot be empty!"),
                             QMessageBox::Ok);
        return;
    }
    ui.consoleText->clear();

    if (circuit)
        delete circuit;
    circuit = new Circuit();

    FILE *file;
    file = fopen(fileName.toLocal8Bit().data(), "rb");
    
    if (!file) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file!"),
                             QMessageBox::Ok);
        console->log(std::format("[SpParser] Can not open {}.", fileName.toLocal8Bit().data()));
        return;
    }

    // read title and move the pointer to second line
    char title[128];
    fgets(title, 128, file);
    yylineno++;
    console->log(std::format("[SpParser] Title: {}", title));
    circuit->title = title;

    yyset_in(file);
    int ret = yyparse();
    while (ret == 0) {
        ret = yyparse();
    }
    fclose(yyin);
    yylex_destroy();

    if (ret == -255)
    {
        console->log(std::format("[SpParser] node number: {}", circuit->nodemap.size()));
        console->log(std::format("[SpParser] Parse success."));
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Parse failed. Check the console for error message."),
                             QMessageBox::Ok);
        console->log(std::format("[SpParser] Parse failed."));
        return;
    }

    ui.treeWidget->clear();
    for (auto& device : circuit->devices) {
        auto title = std::format("{} [{}]", device->Name, device->getDeviceType());
        QTreeWidgetItem* topitem = new QTreeWidgetItem(QStringList()<<title.c_str());
        ui.treeWidget->addTopLevelItem(topitem);
        auto infos = device->getInfoList();
        for (auto& info : infos) {
            QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<info.c_str());
            topitem->addChild(item);
        }
        //item->addChild()
        //device->appendStamp(nodeCount, mat, rhs, nodes);
    }

    // if (circuit->command_PLOT.enabled)
    // {
    //     for (auto& plotnode : circuit->command_PLOT.nodes) {
    //         console->log(std::format("[SpParser] Plot: {}({})", plotnode.prefix, plotnode.nodeName));
    //     }
    // }

    if (analyze)
        delete analyze;
    analyze = new AnalyzeManager(circuit);
    analyze->StartAnalyze();
}

void MyMainWindow::on_actionDebug_triggered()
{
    ChartViewWindow* chart = new ChartViewWindow();
    chart->show();
}

