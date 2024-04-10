#ifndef SPICEHIGHLIGHTER_HPP
#define SPICEHIGHLIGHTER_HPP
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QDebug>
class SpiceHighlighter : public QSyntaxHighlighter
{
public:
    SpiceHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent)
    {
        HighlightRule* rule;

        rule = new HighlightRule();
        rule->pattern = QRegularExpression("\\.[a-zA-Z]*");
        rule->format.setForeground(Qt::red);
        rule->format.setFontWeight(75);
        m_rules.append(*rule);

        rule = new HighlightRule();
        rule->pattern = QRegularExpression("\\b[0-9]+\\b");
        rule->format.setForeground(Qt::blue);
        m_rules.append(*rule);

        rule = new HighlightRule();
        rule->pattern = QRegularExpression("^\\*.*");
        rule->format.setForeground(Qt::gray);
        m_rules.append(*rule);

//        rule.pattern = QRegularExpression("\\b(void|int|float|double|char|bool)\\b");
//        rule.format.setForeground(Qt::darkMagenta);
//        m_rules.append(rule);
    }

protected:
    void highlightBlock(const QString& text) override
    {
        for (const HighlightRule& rule : m_rules) {
            QRegularExpressionMatchIterator it = rule.pattern.globalMatch(text);
            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }

private:
    struct HighlightRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightRule> m_rules;
};

#endif // SPICEHIGHLIGHTER_HPP
