/*****************************************************************
 * $Id: rb_spreadsheettable.h 2248 2016-08-08 09:13:00Z rutger $
 *
 * Copyright (C) 2016 Biluna. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_spreadsheettable.h"

#include <QtGui>
#include "db_dialogfactory.h"
#include "rb_spreadsheetcell.h"


RB_SpreadSheetTable::RB_SpreadSheetTable(QWidget* parent)
            : QTableWidget(parent) {
    autoRecalc = true;
    setItemPrototype(new RB_SpreadSheetCell);
    setSelectionMode(ContiguousSelection);
    horizontalHeader()->setDefaultSectionSize(80);
    verticalHeader()->setDefaultSectionSize(20);
    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(somethingChanged()));
    clear();
}

void RB_SpreadSheetTable::clear() {
    setRowCount(0);
    setColumnCount(0);
    setRowCount(RowCount);
    setColumnCount(ColumnCount);

    for (int i = 0; i < ColumnCount; ++i) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString(QChar('A' + i)));
        setHorizontalHeaderItem(i, item);
    }

    setCurrentCell(0, 0);
}

RB_SpreadSheetCell* RB_SpreadSheetTable::cell(int row, int column) const {
    return static_cast<RB_SpreadSheetCell*>(item(row, column));
}

QString RB_SpreadSheetTable::text(int row, int column) const {
    RB_SpreadSheetCell* c = cell(row, column);

    if (c) {
        return c->text();
    } else {
        return "";
    }
}
QString RB_SpreadSheetTable::formula(int row, int column) const {
    RB_SpreadSheetCell* c = cell(row, column);

    if (c) {
        return c->formula();
    } else {
        return "";
    }
}

void RB_SpreadSheetTable::setFormula(int row, int column,
                                      const QString &formula) {
    RB_SpreadSheetCell* c = cell(row, column);

    if (!c) {
        c = new RB_SpreadSheetCell;
        setItem(row, column, c);
    }

    c->setFormula(formula);
}

QString RB_SpreadSheetTable::currentLocation() const {
    return QChar('A' + currentColumn())
           + QString::number(currentRow() + 1);
}

QString RB_SpreadSheetTable::currentFormula() const {
    return formula(currentRow(), currentColumn());
}

void RB_SpreadSheetTable::somethingChanged() {
    if (autoRecalc) {
        recalculate();
    }

    emit modified();
}

bool RB_SpreadSheetTable::writeFile(const QString& fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("RB_SpreadSheetTable"),
        tr("Cannot write file %1:\n%2.")
        .arg(file.fileName())
        .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint32(MagicNumber);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    for (int row = 0; row < RowCount; ++row) {
        for (int column = 0; column < ColumnCount; ++column) {
            QString str = formula(row, column);

            if (!str.isEmpty())
                out << quint16(row) << quint16(column) << str;
        }
    }

    QApplication::restoreOverrideCursor();
    return true;
}

bool RB_SpreadSheetTable::readFile(const QString& fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Spreadsheet"),
                            tr("Cannot read file %1:\n%2.")
                            .arg(file.fileName())
                            .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_3);

    quint32 magic;
    in >> magic;

    if (magic != MagicNumber) {
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("The file is not a Spreadsheet file."));
        return false;
    }

    clear();

    quint16 row;
    quint16 column;
    QString str;
    QApplication::setOverrideCursor(Qt::WaitCursor);

    while (!in.atEnd()) {
        in >> row >> column >> str;
        setFormula(row, column, str);
    }

    QApplication::restoreOverrideCursor();
    return true;
}

void RB_SpreadSheetTable::cut() {
    copy();
    del();
}

void RB_SpreadSheetTable::copy() {
    QTableWidgetSelectionRange range = selectedRange();
    QString str;

    for (int i = 0; i < range.rowCount(); ++i) {
        if (i > 0) {
            str += "\n";
        }

        for (int j = 0; j < range.columnCount(); ++j) {
            if (j > 0) {
                str += "\t";
            }

            str += formula(range.topRow() + i, range.leftColumn() + j);
        }
    }

    QApplication::clipboard()->setText(str);}

QTableWidgetSelectionRange RB_SpreadSheetTable::selectedRange() const {
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();

    if (ranges.isEmpty()) {
        return QTableWidgetSelectionRange();
    }

    return ranges.first();
}

void RB_SpreadSheetTable::paste() {
    QTableWidgetSelectionRange range = selectedRange();
    QString str = QApplication::clipboard()->text();
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t') + 1;

    if (range.rowCount() * range.columnCount() != 1
            && (range.rowCount() != numRows
                || range.columnCount() != numColumns)) {
        QMessageBox::information(this, tr("Spreadsheet"),
                                 tr("The information cannot be pasted because the copy "
                                    "and paste areas aren't the same size."));
        return;
    }

    for (int i = 0; i < numRows; ++i) {
        QStringList columns = rows[i].split('\t');

        for (int j = 0; j < numColumns; ++j) {
            int row = range.topRow() + i;
            int column = range.leftColumn() + j;

            if (row < RowCount && column < ColumnCount) {
                setFormula(row, column, columns[j]);
            }
        }
    }

    somethingChanged();
}

void RB_SpreadSheetTable::del() {
    QList<QTableWidgetItem *> items = selectedItems();

    if (!items.isEmpty()) {
        foreach (QTableWidgetItem *item, items) {
            delete item;
        }

        somethingChanged();
    }
}

void RB_SpreadSheetTable::selectCurrentRow() {
    selectRow(currentRow());
}

void RB_SpreadSheetTable::selectCurrentColumn() {
    selectColumn(currentColumn());
}

void RB_SpreadSheetTable::findNext(const QString &str, Qt::CaseSensitivity cs) {
    int row = currentRow();
    int column = currentColumn() + 1;

    while (row < RowCount) {
        while (column < ColumnCount) {
            if (text(row, column).contains(str, cs)) {
                clearSelection();
                setCurrentCell(row, column);
                activateWindow();
                return;
            }

            ++column;
        }

        column = 0;
        ++row;
    }

    QApplication::beep();
}

void RB_SpreadSheetTable::findPrevious(const QString &str, Qt::CaseSensitivity cs) {
    int row = currentRow();
    int column = currentColumn() - 1;

    while (row >= 0) {
        while (column >= 0) {
            if (text(row, column).contains(str, cs)) {
                clearSelection();
                setCurrentCell(row, column);
                activateWindow();
                return;
            }

            --column;
        }

        column = ColumnCount - 1;
        --row;
    }

    QApplication::beep();
}

void RB_SpreadSheetTable::recalculate() {
    for (int row = 0; row < RowCount; ++row) {
        for (int column = 0; column < ColumnCount; ++column) {
            if (cell(row, column)) {
                cell(row, column)->setDirty();
            }
         }
    }

    viewport()->update();
}

void RB_SpreadSheetTable::setAutoRecalculate(bool recalc) {
    autoRecalc = recalc;

    if (autoRecalc) {
        recalculate();
    }
}


void RB_SpreadSheetTable::sort(const RB_SpreadSheetCompare& compare) {
    QList<QStringList> rows;
    QTableWidgetSelectionRange range = selectedRange();
    int i;

    for (i = 0; i < range.rowCount(); ++i) {
        QStringList row;
        for (int j = 0; j < range.columnCount(); ++j) {
            row.append(formula(range.topRow() + i, range.leftColumn() + j));
        }

        rows.append(row);
    }

    qStableSort(rows.begin(), rows.end(), compare);

    for (i = 0; i < range.rowCount(); ++i) {
        for (int j = 0; j < range.columnCount(); ++j) {
            setFormula(range.topRow() + i, range.leftColumn() + j, rows[i][j]);
        }
    }

    clearSelection();
    somethingChanged();
}

bool RB_SpreadSheetCompare::operator()(const QStringList &row1,
                                       const QStringList &row2) const {
    for (int i = 0; i < KeyCount; ++i) {
        int column = keys[i];

        if (column != -1) {
            if (row1[column] != row2[column]) {
                if (ascending[i]) {
                    return row1[column] < row2[column];
                } else {
                    return row1[column] > row2[column];
                }
            }
        }
    }

    return false;
}
