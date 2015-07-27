/********************************************************************************
** Form generated from reading UI file 'gv_onetimedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_ONETIMEDIALOG_H
#define UI_GV_ONETIMEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GV_OneTimeDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *lblText;
    QCheckBox *cbDontShowAgain;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *bOK;
    QPushButton *bCancel;

    void setupUi(QDialog *GV_OneTimeDialog)
    {
        if (GV_OneTimeDialog->objectName().isEmpty())
            GV_OneTimeDialog->setObjectName(QStringLiteral("GV_OneTimeDialog"));
        GV_OneTimeDialog->resize(323, 144);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GV_OneTimeDialog->sizePolicy().hasHeightForWidth());
        GV_OneTimeDialog->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral("../../../../qcad2/qcadguiqt/src/dialogs/ui"), QSize(), QIcon::Normal, QIcon::Off);
        GV_OneTimeDialog->setWindowIcon(icon);
        vboxLayout = new QVBoxLayout(GV_OneTimeDialog);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        lblText = new QLabel(GV_OneTimeDialog);
        lblText->setObjectName(QStringLiteral("lblText"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(7);
        sizePolicy1.setHeightForWidth(lblText->sizePolicy().hasHeightForWidth());
        lblText->setSizePolicy(sizePolicy1);
        lblText->setTextFormat(Qt::AutoText);
        lblText->setScaledContents(false);
        lblText->setWordWrap(false);

        vboxLayout->addWidget(lblText);

        cbDontShowAgain = new QCheckBox(GV_OneTimeDialog);
        cbDontShowAgain->setObjectName(QStringLiteral("cbDontShowAgain"));

        vboxLayout->addWidget(cbDontShowAgain);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        spacerItem = new QSpacerItem(91, 26, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        bOK = new QPushButton(GV_OneTimeDialog);
        bOK->setObjectName(QStringLiteral("bOK"));

        hboxLayout->addWidget(bOK);

        bCancel = new QPushButton(GV_OneTimeDialog);
        bCancel->setObjectName(QStringLiteral("bCancel"));

        hboxLayout->addWidget(bCancel);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(GV_OneTimeDialog);
        QObject::connect(bOK, SIGNAL(clicked()), GV_OneTimeDialog, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), GV_OneTimeDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(GV_OneTimeDialog);
    } // setupUi

    void retranslateUi(QDialog *GV_OneTimeDialog)
    {
        GV_OneTimeDialog->setWindowTitle(QApplication::translate("GV_OneTimeDialog", "Warning", 0));
        lblText->setText(QString());
        cbDontShowAgain->setText(QApplication::translate("GV_OneTimeDialog", "Do not ask me about this again", 0));
        bOK->setText(QApplication::translate("GV_OneTimeDialog", "OK", 0));
        bCancel->setText(QApplication::translate("GV_OneTimeDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class GV_OneTimeDialog: public Ui_GV_OneTimeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_ONETIMEDIALOG_H
