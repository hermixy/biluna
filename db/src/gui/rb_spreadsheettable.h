/*****************************************************************
 * $Id: rb_spreadsheettable.h 2248 2016-08-08 09:13:00Z rutger $
 *
 * Copyright (C) 2016 Biluna. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_SPREADSHEETTABLE_H
#define RB_SPREADSHEETTABLE_H

#include <QTableWidget>
#include "db_global.h"
class RB_SpreadSheetCell;
class RB_SpreadSheetCompare;

/**
 * @brief Base class for QTableWidget with spreadsheet functionality
 */
class DB_EXPORT RB_SpreadSheetTable : public QTableWidget {

    Q_OBJECT

public:
    RB_SpreadSheetTable(QWidget* parent = 0);

    bool autoRecalculate() const { return autoRecalc; }
    QString currentLocation() const;
    QString currentFormula() const;
    QTableWidgetSelectionRange selectedRange() const;
    void clear();
    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);
    void sort(const RB_SpreadSheetCompare& compare);

public slots:
    void cut();
    void copy();
    void paste();
    void del();
    void selectCurrentRow();
    void selectCurrentColumn();
    void recalculate();
    void setAutoRecalculate(bool recalc);
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

signals:
    void modified();

private slots:
    void somethingChanged();

private:
    enum { MagicNumber = 0x7F51C883, RowCount = 999, ColumnCount = 26 };
    RB_SpreadSheetCell* cell(int row, int column) const;
    QString text(int row, int column) const;
    QString formula(int row, int column) const;
    void setFormula(int row, int column, const QString &formula);
    bool autoRecalc;
};

class DB_EXPORT RB_SpreadSheetCompare {

public:
    bool operator()(const QStringList &row1,
                    const QStringList &row2) const;

    enum { KeyCount = 3 };
    int keys[KeyCount];
    bool ascending[KeyCount];
};


#endif // RB_SPREADSHEETTABLE_H
