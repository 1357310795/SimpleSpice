#include "myconsole.h"
#include <QString>

void MyConsole::log(std::string str)
{
    console->appendPlainText(QString::fromStdString(str));
}
