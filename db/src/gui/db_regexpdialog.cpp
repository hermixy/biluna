/*****************************************************************
 * $Id: db_regexpdialog.cpp 1965 2013-08-08 16:33:15Z rutger $
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project. Based on Qt Example.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_regexpdialog.h"

#include <QtWidgets>
#include "rb_settings.h"

/**
 * Constructor
 */
DB_RegExpDialog::DB_RegExpDialog(QWidget *parent) : RB_Dialog(parent) {

    setObjectName("DB_RegExpDialog"); // because no setupUi()

    patternComboBox = new QComboBox;
    patternComboBox->setEditable(true);
    patternComboBox->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Preferred);

    patternLabel = new QLabel(tr("&Pattern:"));
    patternLabel->setBuddy(patternComboBox);

    escapedPatternLineEdit = new QLineEdit;
    escapedPatternLineEdit->setReadOnly(true);
    QPalette palette = escapedPatternLineEdit->palette();
    palette.setBrush(QPalette::Base,
                     palette.brush(QPalette::Disabled, QPalette::Base));
    escapedPatternLineEdit->setPalette(palette);

    escapedPatternLabel = new QLabel(tr("&Escaped Pattern:"));
    escapedPatternLabel->setBuddy(escapedPatternLineEdit);

    syntaxComboBox = new QComboBox;
    syntaxComboBox->addItem(tr("Regular expression v1"), QRegExp::RegExp);
    syntaxComboBox->addItem(tr("Regular expression v2"), QRegExp::RegExp2);
    syntaxComboBox->addItem(tr("Wildcard"), QRegExp::Wildcard);
    syntaxComboBox->addItem(tr("Fixed string"), QRegExp::FixedString);
    syntaxComboBox->addItem(tr("W3C Xml Schema 1.1"), QRegExp::W3CXmlSchema11);

    syntaxLabel = new QLabel(tr("&Pattern Syntax:"));
    syntaxLabel->setBuddy(syntaxComboBox);

    textComboBox = new QComboBox;
    textComboBox->setEditable(true);
    textComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    textLabel = new QLabel(tr("&Text:"));
    textLabel->setBuddy(textComboBox);

    caseSensitiveCheckBox = new QCheckBox(tr("Case &Sensitive"));
    caseSensitiveCheckBox->setChecked(true);
    minimalCheckBox = new QCheckBox(tr("&Minimal"));

    indexLabel = new QLabel(tr("Index of Match:"));
    indexEdit = new QLineEdit;
    indexEdit->setReadOnly(true);

    matchedLengthLabel = new QLabel(tr("Matched Length:"));
    matchedLengthEdit = new QLineEdit;
    matchedLengthEdit->setReadOnly(true);

    for (int i = 0; i < MaxCaptures; ++i) {
        captureLabels[i] = new QLabel(tr("Capture %1:").arg(i));
        captureEdits[i] = new QLineEdit;
        captureEdits[i]->setReadOnly(true);
    }
    captureLabels[0]->setText(tr("Match:"));

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(caseSensitiveCheckBox);
    checkBoxLayout->addWidget(minimalCheckBox);
    checkBoxLayout->addStretch(1);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(patternLabel, 0, 0);
    mainLayout->addWidget(patternComboBox, 0, 1);
    mainLayout->addWidget(escapedPatternLabel, 1, 0);
    mainLayout->addWidget(escapedPatternLineEdit, 1, 1);
    mainLayout->addWidget(syntaxLabel, 2, 0);
    mainLayout->addWidget(syntaxComboBox, 2, 1);
    mainLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
    mainLayout->addWidget(textLabel, 4, 0);
    mainLayout->addWidget(textComboBox, 4, 1);
    mainLayout->addWidget(indexLabel, 5, 0);
    mainLayout->addWidget(indexEdit, 5, 1);
    mainLayout->addWidget(matchedLengthLabel, 6, 0);
    mainLayout->addWidget(matchedLengthEdit, 6, 1);

    for (int j = 0; j < MaxCaptures; ++j) {
        mainLayout->addWidget(captureLabels[j], 7 + j, 0);
        mainLayout->addWidget(captureEdits[j], 7 + j, 1);
    }
    setLayout(mainLayout);

    connect(patternComboBox, SIGNAL(editTextChanged(QString)),
            this, SLOT(refresh()));
    connect(textComboBox, SIGNAL(editTextChanged(QString)),
            this, SLOT(refresh()));
    connect(caseSensitiveCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(refresh()));
    connect(minimalCheckBox, SIGNAL(toggled(bool)), this, SLOT(refresh()));
    connect(syntaxComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(refresh()));

    patternComboBox->addItem(tr("[A-Za-z_]+([A-Za-z_0-9]*)"));
    textComboBox->addItem(tr("(10 + delta4) * 32"));

    setWindowTitle(tr("Regular Expression"));
    setFixedHeight(sizeHint().height());
    refresh();
    readSettings();
}

/**
 * Destructor
 */
DB_RegExpDialog::~DB_RegExpDialog() {
    // othing
}

/**
 * Slot refresh regular expression results
 */
void DB_RegExpDialog::refresh() {
    setUpdatesEnabled(false);

    QString pattern = patternComboBox->currentText();
    QString text = textComboBox->currentText();

    QString escaped = pattern;
    escaped.replace("\\", "\\\\");
    escaped.replace("\"", "\\\"");
    escaped.prepend("\"");
    escaped.append("\"");
    escapedPatternLineEdit->setText(escaped);

    QRegExp rx(pattern);
    Qt::CaseSensitivity cs = Qt::CaseInsensitive;
    if (caseSensitiveCheckBox->isChecked())
        cs = Qt::CaseSensitive;
    rx.setCaseSensitivity(cs);
    rx.setMinimal(minimalCheckBox->isChecked());
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(
            syntaxComboBox->itemData(syntaxComboBox->currentIndex()).toInt());
    rx.setPatternSyntax(syntax);

    QPalette palette = patternComboBox->palette();
    if (rx.isValid()) {
        palette.setColor(QPalette::Text,
                         textComboBox->palette().color(QPalette::Text));
    } else {
        palette.setColor(QPalette::Text, Qt::red);
    }
    patternComboBox->setPalette(palette);

    indexEdit->setText(QString::number(rx.indexIn(text)));
    matchedLengthEdit->setText(QString::number(rx.matchedLength()));
    for (int i = 0; i < MaxCaptures; ++i) {
        captureLabels[i]->setEnabled(i <= rx.captureCount());
        captureEdits[i]->setEnabled(i <= rx.captureCount());
        captureEdits[i]->setText(rx.cap(i));
    }

    setUpdatesEnabled(true);
}
