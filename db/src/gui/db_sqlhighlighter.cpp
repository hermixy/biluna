#include <QtWidgets>

#include "db_sqlhighlighter.h"

DB_SqlHighlighter::DB_SqlHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QColor color;

    _config = new QSettings(QSettings::defaultFormat(), QSettings::UserScope,
                            qApp->organizationName(), "sql.syntax");

    //! numbers
    color = _config->value("number/color", QColor(Qt::darkMagenta).name()).toString();
    _numberFormat.setForeground(color);
    rule.pattern = QRegExp("\\b[0-9]+\\b");
    rule.format = _numberFormat;
    _highlightingRules.append(rule);

    //! keywords
    color = _config->value("keyword/color", QColor(Qt::darkYellow).name()).toString();
    _keywordFormat.setForeground(color);
    QVariantList keywords;
    keywords << "accessible" << "add" << "all" << "alter" << "analyze" << "as"
             << "asc" << "asensitive" << "before" << "both" << "by" << "call"
             << "cascade" << "case" << "change" << "check" << "collate"
             << "column" << "condition" << "constraint" << "continue"
             << "convert" << "create" << "cross" << "current_date"
             << "current_time" << "current_timestamp" << "current_user"
             << "cursor" << "database" << "databases" << "day_hour"
             << "day_microsecond" << "day_minute" << "day_second" << "declare"
             << "default" << "delayed" << "delete" << "desc" << "describe"
             << "deterministic" << "distinct" << "distinctrow" << "drop"
             << "dual" << "each" << "else" << "elseif" << "enclosed"
             << "escaped" << "exists" << "exit" << "explain" << "false"
             << "fetch" << "for" << "force" << "foreign" << "from"
             << "fulltext" << "grant" << "group" << "having" << "high_priority"
             << "hour_microsecond" << "hour_minute" << "hour_second" << "if"
             << "ignore" << "in" << "index" << "infile" << "inner" << "inout"
             << "insensitive" << "insert" << "interval" << "into" << "iterate"
             << "join" << "key" << "keys" << "kill" << "leading" << "leave"
             << "left" << "limit" << "linear" << "lines" << "load"
             << "localtime" << "localtimestamp" << "lock" << "loop"
             << "low_priority" << "master_ssl_verify_server_cert" << "match"
             << "minute_microsecond" << "minute_second" << "mod" << "modifies"
             << "natural" << "no_write_to_binlog" << "null" << "on" << "optimize"
             << "option" << "optionally" << "order" << "out" << "outer"
             << "outfile" << "precision" << "primary" << "procedure" << "purge"
             << "range" << "read" << "reads" << "read_only" << "read_write"
             << "references" << "release" << "rename" << "repeat" << "replace"
             << "require" << "restrict" << "return" << "revoke" << "right"
             << "schema" << "schemas" << "second_microsecond" << "select"
             << "sensitive" << "separator" << "set" << "show" << "spatial"
             << "specific" << "sql" << "sqlexception" << "sqlstate"
             << "sqlwarning" << "sql_big_result" << "sql_calc_found_rows"
             << "sql_small_result" << "ssl" << "starting" << "straight_join"
             << "table" << "terminated" << "then" << "to" << "trailing"
             << "trigger" << "true" << "undo" << "union" << "unique"
             << "unlock" << "unsigned" << "update" << "usage" << "use"
             << "using" << "utc_date" << "utc_time" << "utc_timestamp"
             << "values" << "when" << "where" << "while" << "with" << "write"
             << "x509" << "year_month" << "zerofill" << "begin" << "end"
             << "open" << "close" << "loop" << "delimiter" << "data"
             << "handler" << "until" << "returns" << "function" << "view"
             << "contains";
    _keywords = _config->value("keyword/list", keywords).toList();
    foreach (const QVariant& keyword, _keywords) {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(keyword.toString()));
        //! sql is case insensitive
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = _keywordFormat;
        _highlightingRules.append(rule);
    }

    //! operator
    color = _config->value("operator/color", QColor(Qt::darkRed).name()).toString();
    _operatorFormat.setForeground(color);
    QVariantList operators;
    operators << "and" << "&&" << "between" << "binary" << "&" << "\\|"
            << "\\^" << "/" << "div" << "<=>" << "=" << ">=" << ">"
            << "is null" << "null" << "is" << "<<" << "<=" << "<"
            << "like" << "-" << "%" << "\\!=" << "<>" << "in"
            << "rlike" << "regexp" << "not" << "\\!" << "\\|\\|"
            << "or" << "\\+" << ">>" << "sounds" << "\\~" << "\\*"
            << "-" << "xor";
    _operators = _config->value("operator/list", operators).toList();
    foreach (const QVariant& operatorWord, _operators) {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(operatorWord.toString()));
        //! sql is case insensitive
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = _operatorFormat;
        _highlightingRules.append(rule);
    }

    //! data type
    color = _config->value("data_type/color", QColor(Qt::darkBlue).name()).toString();
    _dataTypeFormat.setForeground(color);
    QVariantList dataTypes;
    dataTypes << "bigint" << "binary" << "blob" << "bool" << "boolean"
              << "char" << "character" << "dec" << "decimal" << "double"
              << "float" << "float4" << "float8" << "int" << "int1"
              << "int2" << "int3" << "int4" << "int8" << "integer"
              << "long" << "longblob" << "longtext" << "mediumblob"
              << "mediumint" << "mediumtext" << "middleint" << "numeric"
              << "real" << "smallint" << "spatial" << "tinyblob" << "tinyint"
              << "tinytext" << "varbinary" << "varchar" << "varcharacter"
              << "varying" << "bit" << "date" << "datetime" << "enum"
              << "text" << "time" << "timestamp";
    _dataTypes = _config->value("data_type/list", dataTypes).toList();
    foreach (const QVariant& dataType, _dataTypes) {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(dataType.toString()));
        //! sql is case insensitive
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = _dataTypeFormat;
        _highlightingRules.append(rule);
    }

    //! function
    color = _config->value("function/color", QColor(Qt::blue).name()).toString();
    _functionFormat.setForeground(color);
    QVariantList functions;
    functions << "abs" << "acos" << "adddate" << "addtime" << "aes_decrypt"
              << "aes_encrypt" << "ascii" << "asin" << "atan2" << "atan"
              << "avg" << "benchmark" << "bin" << "bit_and" << "bit_count"
              << "bit_length" << "bit_or" << "bit_xor" << "cast" << "ceiling"
              << "ceil" << "char_length" << "char" << "character_length"
              << "charset" << "coalesce" << "coercibility" << "collation"
              << "compress" << "concat_ws" << "concat" << "connection_id"
              << "conv" << "convert_tz" << "cos" << "cot" << "count"
              << "crc32" << "curdate" << "current_date" << "current_time"
              << "current_timestamp" << "current_user" << "curtime"
              << "database" << "date_add" << "date_format" << "date_sub"
              << "date" << "datediff" << "day" << "dayname" << "dayofmonth"
              << "dayofweek" << "dayofyear" << "decode" << "default"
              << "degrees" << "des_decrypt" << "des_encrypt" << "elt"
              << "encode" << "encrypt" << "exp" << "export_set" << "extract"
              << "field" << "find_in_set" << "floor" << "format" << "found_rows"
              << "from_days" << "from_unixtime" << "get_format" << "get_lock"
              << "greatest" << "group_concat" << "hex" << "hour" << "if"
              << "ifnull" << "inet_aton" << "inet_ntoa" << "instr" << "interval"
              << "is_free_lock" << "is_used_lock" << "isnull" << "last_day"
              << "last_insert_id" << "lcase" << "least" << "left" << "length"
              << "ln" << "load_file" << "localtime" << "localtimestamp"
              << "locate" << "log10" << "log2" << "log" << "lower" << "lpad"
              << "ltrim" << "make_set" << "makedate" << "maketime"
              << "master_pos_wait" << "max" << "md5" << "microsecond" << "mid"
              << "min" << "minute" << "mod" << "month" << "monthname"
              << "name_const" << "now" << "nullif" << "oct" << "octet_length"
              << "old_password" << "ord" << "password" << "period_add"
              << "period_diff" << "pi" << "position" << "pow" << "power"
              << "procedure analyse" << "quarter" << "quote" << "radians"
              << "rand" << "release_lock" << "repeat" << "replace" << "reverse"
              << "right" << "round" << "row_count" << "rpad" << "rtrim"
              << "schema" << "sec_to_time" << "second" << "session_user"
              << "sha1" << "sha" << "sign" << "sin" << "sleep" << "soundex"
              << "space" << "sqrt" << "std" << "stddev" << "stddev_pop"
              << "stddev_samp" << "str_to_date" << "strcmp" << "subdate"
              << "substring_index" << "substring" << "substr" << "subtime"
              << "sum" << "sysdate" << "system_user" << "tan" << "time_format"
              << "time_to_sec" << "time" << "timediff" << "timestamp"
              << "timestampadd" << "timestampdiff" << "to_days" << "trim"
              << "truncate" << "ucase" << "uncompress" << "uncompressed_length"
              << "unhex" << "unix_timestamp" << "upper" << "user" << "utc_date"
              << "utc_time" << "utc_timestamp" << "uuid" << "values"
              << "var_pop" << "var_samp" << "variance" << "week" << "weekday"
              << "weekofyear" << "year" << "yearweek";
    _functions = _config->value("function/list", functions).toList();
    foreach (const QVariant& function, _functions) {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(function.toString()));
        //! sql is case insensitive
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = _functionFormat;
        _highlightingRules.append(rule);
    }

    //! quotation
    color = _config->value("quotation/color", QColor(Qt::darkGreen).name()).toString();
    _quotationFormat.setForeground(color);
    // rule.pattern = QRegExp("(\".*\"|\'.*\')"); original not good
    rule.pattern = QRegExp("(\'[^\'^\"]+\')|(\"[^\'^\"]+\")");
    rule.format = _quotationFormat;
    _highlightingRules.append(rule);

    //! singleline comment
    color = _config->value("singleline_comment/color", QColor(Qt::darkGreen).name()).toString();
    _singleLineCommentFormat.setForeground(color);
    rule.pattern = QRegExp("--[^\n]*");
    rule.format = _singleLineCommentFormat;
    _highlightingRules.append(rule);

    /*!
     * multiline comment
     * \sa further process goes to highlightBlock()
     */
    color = _config->value("multiline_comment/color", QColor(Qt::darkGreen).name()).toString();
    _multiLineCommentFormat.setForeground(color);
    _commentStartExpression = QRegExp("/\\*");
    _commentEndExpression = QRegExp("\\*/");

    saveSettings();
}

DB_SqlHighlighter::~DB_SqlHighlighter()
{
    delete _config;
}

void DB_SqlHighlighter::saveSettings()
{
    _config->setValue("number/color", _numberFormat.foreground().color().name());
    _config->setValue("keyword/color", _keywordFormat.foreground().color().name());
    _config->setValue("keyword/list", QVariant(_keywords));
    _config->setValue("operator/color", _operatorFormat.foreground().color().name());
    _config->setValue("operator/list", QVariant(_operators));
    _config->setValue("data_type/color", _dataTypeFormat.foreground().color().name());
    _config->setValue("data_type/list", QVariant(_dataTypes));
    _config->setValue("function/color", _functionFormat.foreground().color().name());
    _config->setValue("function/list", QVariant(_functions));
    _config->setValue("quotation/color", _quotationFormat.foreground().color().name());
    _config->setValue("singleline_comment/color", _singleLineCommentFormat.foreground().color().name());
    _config->setValue("multiline_comment/color", _multiLineCommentFormat.foreground().color().name());
    _config->sync();
}

void DB_SqlHighlighter::highlightBlock(const QString &text)
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
