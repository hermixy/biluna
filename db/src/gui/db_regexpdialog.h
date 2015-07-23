/*****************************************************************
 * $Id: db_regexpdialog.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project. Based on Qt Example.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_REGEXPDIALOG_H
#define DB_REGEXPDIALOG_H

#include <QDialog>
#include "rb_dialog.h"

QT_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class DB_EXPORT DB_RegExpDialog : public RB_Dialog {

    Q_OBJECT

public:
    DB_RegExpDialog(QWidget* parent = 0);
    virtual ~DB_RegExpDialog();

    void init() { }

private slots:
    void refresh();

private:
    QLabel *patternLabel;
    QLabel *escapedPatternLabel;
    QLabel *syntaxLabel;
    QLabel *textLabel;
    QComboBox *patternComboBox;
    QLineEdit *escapedPatternLineEdit;
    QComboBox *textComboBox;
    QCheckBox *caseSensitiveCheckBox;
    QCheckBox *minimalCheckBox;
    QComboBox *syntaxComboBox;

    QLabel *indexLabel;
    QLabel *matchedLengthLabel;
    QLineEdit *indexEdit;
    QLineEdit *matchedLengthEdit;

    enum { MaxCaptures = 6 };
    QLabel *captureLabels[MaxCaptures];
    QLineEdit *captureEdits[MaxCaptures];
};

#endif // DB_REGEXPDIALOG_H
