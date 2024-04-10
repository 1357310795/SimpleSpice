#include "console/myconsole.h"
#include <QString>

MyConsole* console;

void MyConsole::log(std::string str)
{
    console->appendPlainText(QString::fromStdString(str));
}
