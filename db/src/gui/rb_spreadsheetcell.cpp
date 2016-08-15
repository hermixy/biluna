/*****************************************************************
 * $Id: rb_spreadsheetcell.cpp 2248 2016-08-08 09:13:00Z rutger $
 *
 * Copyright (C) 2016 Biluna. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_spreadsheetcell.h"

RB_SpreadSheetCell::RB_SpreadSheetCell() : QTableWidgetItem() {
    setDirty();
}

QTableWidgetItem* RB_SpreadSheetCell::clone() const {
    return new RB_SpreadSheetCell(*this);
}

void RB_SpreadSheetCell::setData(int role, const QVariant& value) {
    QTableWidgetItem::setData(role, value);

    if (role == Qt::EditRole) {
        setDirty();
    }
}

QVariant RB_SpreadSheetCell::data(int role) const {
    if (role == Qt::DisplayRole) {
        if (value().isValid()) {
            return value().toString();
        } else {
            return "####";
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (value().type() == QVariant::String) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return int(Qt::AlignRight | Qt::AlignVCenter);
        }
    } else {
        return QTableWidgetItem::data(role);
    }
}

void RB_SpreadSheetCell::setFormula(const QString& formula) {
    setData(Qt::EditRole, formula);
}

QString RB_SpreadSheetCell::formula() const {
    return data(Qt::EditRole).toString();
}

void RB_SpreadSheetCell::setDirty() {
    cacheIsDirty = true;
}

const QVariant Invalid;

QVariant RB_SpreadSheetCell::value() const {
    if (cacheIsDirty) {
        cacheIsDirty = false;
        QString formulaStr = formula();

        if (formulaStr.startsWith('\'')) {
            cachedValue = formulaStr.mid(1);
        } else if (formulaStr.startsWith('=')) {
            cachedValue = Invalid;
            QString expr = formulaStr.mid(1);
            expr.replace(" ", "");
            expr.append(QChar::Null);

            int pos = 0;
            cachedValue = evalExpression(expr, pos);

            if (expr[pos] != QChar::Null) {
                cachedValue = Invalid;
            }
        } else {
            bool ok;
            double d = formulaStr.toDouble(&ok);

            if (ok) {
                cachedValue = d;
            } else {
                cachedValue = formulaStr;
            }
        }
    }

    return cachedValue;
}

QVariant RB_SpreadSheetCell::evalExpression(const QString& str, int& pos) const {
    QVariant result = evalTerm(str, pos);

    while (str[pos] != QChar::Null) {
        QChar op = str[pos];

        if (op != '+' && op != '-') {
            return result;
        }

        ++pos;
        QVariant term = evalTerm(str, pos);

        if (result.type() == QVariant::Double
                && term.type() == QVariant::Double) {
            if (op == '+') {
                result = result.toDouble() + term.toDouble();
            } else {
                result = result.toDouble() - term.toDouble();
            }
        } else {
            result = Invalid;
        }
    }

    return result;
}

QVariant RB_SpreadSheetCell::evalTerm(const QString& str, int& pos) const {
    QVariant result = evalFactor(str, pos);

    while (str[pos] != QChar::Null) {
        QChar op = str[pos];
        if (op != '*' && op != '/') {
            return result;
        }

        ++pos;
        QVariant factor = evalFactor(str, pos);

        if (result.type() == QVariant::Double
                && factor.type() == QVariant::Double) {
            if (op == '*') {
                result = result.toDouble() * factor.toDouble();
            } else {
                if (factor.toDouble() == 0.0) {
                    result = Invalid;
                } else {
                    result = result.toDouble() / factor.toDouble();
                }
           }
        } else {
            result = Invalid;
        }
    }

    return result;
}

QVariant RB_SpreadSheetCell::evalFactor(const QString &str, int &pos) const {
    QVariant result;
    bool negative = false;

    if (str[pos] == '-') {
        negative = true;
        ++pos;
    }

    if (str[pos] == '(') {
        ++pos;
        result = evalExpression(str, pos);

        if (str[pos] != ')') {
            result = Invalid;
        }

        ++pos;
    } else {
        QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
        // [\$]{0,1}[A-Za-z]{1,3}[\$]{0,1}[1-9][0-9]{1,5} for $ZZZ$999999 and zzz999999
        QString token;

        while (str[pos].isLetterOrNumber() || str[pos] == '.') {
            token += str[pos];
            ++pos;
        }

        if (regExp.exactMatch(token)) {
            int column = token[0].toUpper().unicode() - 'A';
            int row = token.mid(1).toInt() - 1;

            RB_SpreadSheetCell *c = static_cast<RB_SpreadSheetCell*>(
            tableWidget()->item(row, column));

            if (c) {
                result = c->value();
            } else {
                result = 0.0;
            }
        } else {
            bool ok;
            result = token.toDouble(&ok);

            if (!ok) {
                result = Invalid;
            }
        }
    }
    if (negative) {
        if (result.type() == QVariant::Double) {
            result = -result.toDouble();
        } else {
            result = Invalid;
        }
    }

    return result;
}
