#include <QtWidgets>

#include "db_cpphighlighter.h"

DB_CppHighlighter::DB_CppHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QColor color;

    _config = new QSettings(QSettings::defaultFormat(), QSettings::UserScope,
                            qApp->organizationName(), "cpp.syntax");

    //! compiler directives
    color = QColor(_config->value("compiler_directive/color", QColor(Qt::darkBlue).name()).toString());
    _compilerDirectiveFormat.setForeground(color);
    rule.pattern = QRegExp("#[A-Za-z]+");
    rule.format = _compilerDirectiveFormat;
    _highlightingRules.append(rule);

    //! numbers
    color = QColor(_config->value("number/color", QColor(Qt::darkBlue).name()).toString());
    _numberFormat.setForeground(color);
    rule.pattern = QRegExp("\\b[0-9]+\\b");
    rule.format = _numberFormat;
    _highlightingRules.append(rule);

    //! keywords
    color = QColor(_config->value("keyword/color", QColor(Qt::darkYellow).name()).toString());
    _keywordFormat.setForeground(color);
    QVariantList keywords;
    keywords << "char" << "class" << "const" << "double" << "enum"
             << "explicit" << "friend" << "inline" << "int" << "long"
             << "namespace" << "operator" << "private" << "protected"
             << "public" << "short" << "signals" << "signed" << "slots"
             << "static" << "struct" << "template" << "typedef"
             << "typename" << "union" << "unsigned" << "virtual"
             << "void" << "volatile" << "new" << "delete";
    _keywords = _config->value("keyword/list", keywords).toList();
    foreach (const QVariant& keyword, _keywords) {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(keyword.toString()));
        rule.format = _keywordFormat;
        _highlightingRules.append(rule);
    }

    /*!
     * function
     * function goes before class, otherwise class constructor will be
     * recognized as function.
     */
    color = QColor(_config->value("function/color", QColor(Qt::blue).name()).toString());
    _functionFormat.setForeground(color);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = _functionFormat;
    _highlightingRules.append(rule);

    //! class
    color = QColor(_config->value("class/color", QColor(Qt::darkMagenta).name()).toString());
    _classFormat.setForeground(color);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = _classFormat;
    _highlightingRules.append(rule);

    //! quotation
    color = QColor(_config->value("quotation/color", QColor(Qt::darkGreen).name()).toString());
    _quotationFormat.setForeground(color);
    rule.pattern = QRegExp("(\".*\"|<.*>)");
    rule.format = _quotationFormat;
    _highlightingRules.append(rule);

    //! singleline comment
    color = QColor(_config->value("singleline_comment/color", QColor(Qt::darkGreen).name()).toString());
    _singleLineCommentFormat.setForeground(color);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = _singleLineCommentFormat;
    _highlightingRules.append(rule);

    /*!
     * multiline comment
     * \sa further process goes to highlightBlock()
     */
    color = QColor(_config->value("multiline_comment/color", QColor(Qt::darkGreen).name()).toString());
    _multiLineCommentFormat.setForeground(color);
    _commentStartExpression = QRegExp("/\\*");
    _commentEndExpression = QRegExp("\\*/");

    saveSettings();
}

DB_CppHighlighter::~DB_CppHighlighter()
{
    delete _config;
}

void DB_CppHighlighter::saveSettings()
{
    _config->setValue("compiler_directive/color", _compilerDirectiveFormat.foreground().color().name());
    _config->setValue("number/color", _numberFormat.foreground().color().name());
    _config->setValue("keyword/color", _keywordFormat.foreground().color().name());
    _config->setValue("keyword/list", QVariant(_keywords));
    _config->setValue("function/color", _functionFormat.foreground().color().name());
    _config->setValue("class/color", _classFormat.foreground().color().name());
    _config->setValue("quotation/color", _quotationFormat.foreground().color().name());
    _config->setValue("singleline_comment/color", _singleLineCommentFormat.foreground().color().name());
    _config->setValue("multiline_comment/color", _multiLineCommentFormat.foreground().color().name());
    _config->sync();
}

void DB_CppHighlighter::highlightBlock(const QString &text)
{
    // singleline matches
    foreach (const HighlightingRule &rule, _highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    /* multiline matches
     * block-state:
     * 0 = comment-block-start-or-end, 1 = comment-block-not-end
     */
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = _commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = _commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + _commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, _multiLineCommentFormat);
        startIndex = _commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
