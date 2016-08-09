/*****************************************************************
 * $Id: rb_spreadsheetcell.h 2248 2016-08-08 09:13:00Z rutger $
 *
 * Copyright (C) 2016 Biluna. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_SPREADSHEETCELL_H
#define RB_SPREADSHEETCELL_H

#include <QTableWidgetItem>
#include "db_global.h"


class DB_EXPORT RB_SpreadSheetCell : public QTableWidgetItem {

public:
    RB_SpreadSheetCell();

    QTableWidgetItem* clone() const;
    void setData(int role, const QVariant &value);
    QVariant data(int role) const;
    void setFormula(const QString &formula);
    QString formula() const;
    void setDirty();

private:
    QVariant value() const;
    QVariant evalExpression(const QString &str, int &pos) const;
    QVariant evalTerm(const QString &str, int &pos) const;
    QVariant evalFactor(const QString &str, int &pos) const;

    mutable QVariant cachedValue;
    mutable bool cacheIsDirty;
};

#endif // RB_SPREADSHEETCELL_H
