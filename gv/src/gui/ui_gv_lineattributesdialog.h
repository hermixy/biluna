/********************************************************************************
** Form generated from reading UI file 'gv_lineattributesdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_LINEATTRIBUTESDIALOG_H
#define UI_GV_LINEATTRIBUTESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_GV_LineAttributesDialog
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *cbColor;
    QLabel *label_2;
    QComboBox *cbWidth;
    QLabel *label_3;
    QComboBox *cbLineType;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GV_LineAttributesDialog)
    {
        if (GV_LineAttributesDialog->objectName().isEmpty())
            GV_LineAttributesDialog->setObjectName(QStringLiteral("GV_LineAttributesDialog"));
        GV_LineAttributesDialog->resize(270, 168);
        gridLayout = new QGridLayout(GV_LineAttributesDialog);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(6, 6, 6, 6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox = new QGroupBox(GV_LineAttributesDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setContentsMargins(6, 6, 6, 6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setVerticalSpacing(3);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        cbColor = new QComboBox(groupBox);
        cbColor->setObjectName(QStringLiteral("cbColor"));

        formLayout->setWidget(0, QFormLayout::FieldRole, cbColor);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        cbWidth = new QComboBox(groupBox);
        cbWidth->setObjectName(QStringLiteral("cbWidth"));

        formLayout->setWidget(1, QFormLayout::FieldRole, cbWidth);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        cbLineType = new QComboBox(groupBox);
        cbLineType->setObjectName(QStringLiteral("cbLineType"));

        formLayout->setWidget(2, QFormLayout::FieldRole, cbLineType);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(GV_LineAttributesDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(GV_LineAttributesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), GV_LineAttributesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GV_LineAttributesDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(GV_LineAttributesDialog);
    } // setupUi

    void retranslateUi(QDialog *GV_LineAttributesDialog)
    {
        GV_LineAttributesDialog->setWindowTitle(QApplication::translate("GV_LineAttributesDialog", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("GV_LineAttributesDialog", "Pen", 0));
        label->setText(QApplication::translate("GV_LineAttributesDialog", "Color", 0));
        label_2->setText(QApplication::translate("GV_LineAttributesDialog", "Width", 0));
        label_3->setText(QApplication::translate("GV_LineAttributesDialog", "Line Type", 0));
    } // retranslateUi

};

namespace Ui {
    class GV_LineAttributesDialog: public Ui_GV_LineAttributesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_LINEATTRIBUTESDIALOG_H
