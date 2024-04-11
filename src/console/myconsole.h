#ifndef MYCONSOLE_H
#define MYCONSOLE_H

#include <QPlainTextEdit>
class MyConsole
{
private:
    QPlainTextEdit* console;

public:
    MyConsole(QPlainTextEdit* console): console(console) {};
    void log(std::string str);
};

extern MyConsole* console;
#endif // MYCONSOLE_H
