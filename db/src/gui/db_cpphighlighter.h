#ifndef DB_CPPHIGHLIGHTER_H
#define DB_CPPHIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>
#include "db_global.h"

QT_BEGIN_NAMESPACE
class QTextDocument;
class QSettings;
QT_END_NAMESPACE

class DB_EXPORT DB_CppHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    DB_CppHighlighter(QTextDocument *parent = 0);
    virtual ~DB_CppHighlighter();

public:
    void saveSettings();

protected:
    void highlightBlock(const QString &text);

private:
    QSettings* _config;

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> _highlightingRules;

    QRegExp _commentStartExpression;
    QRegExp _commentEndExpression;

    QTextCharFormat _compilerDirectiveFormat;
    QTextCharFormat _numberFormat;
    QTextCharFormat _keywordFormat;
    QVariantList _keywords;
    QTextCharFormat _functionFormat;
    QTextCharFormat _classFormat;
    QTextCharFormat _quotationFormat;
    QTextCharFormat _singleLineCommentFormat;
    QTextCharFormat _multiLineCommentFormat;
};

#endif
