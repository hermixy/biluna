/********************************************************************************
** Form generated from reading UI file 'gv_pipelinedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_PIPELINEDIALOG_H
#define UI_GV_PIPELINEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GV_PipeLineDialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *buttonLayout;
    QPushButton *pbHelp;
    QSpacerItem *spacerItem;
    QPushButton *pbOk;
    QPushButton *pbCancel;

    void setupUi(QDialog *GV_PipeLineDialog)
    {
        if (GV_PipeLineDialog->objectName().isEmpty())
            GV_PipeLineDialog->setObjectName(QStringLiteral("GV_PipeLineDialog"));
        GV_PipeLineDialog->resize(375, 375);
        GV_PipeLineDialog->setMinimumSize(QSize(375, 375));
        GV_PipeLineDialog->setBaseSize(QSize(0, 0));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/biluna16.png"), QSize(), QIcon::Normal, QIcon::Off);
        GV_PipeLineDialog->setWindowIcon(icon);
        layoutWidget = new QWidget(GV_PipeLineDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 330, 361, 33));
        buttonLayout = new QHBoxLayout(layoutWidget);
#ifndef Q_OS_MAC
        buttonLayout->setSpacing(6);
#endif
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        buttonLayout->setObjectName(QStringLiteral("buttonLayout"));
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        pbHelp = new QPushButton(layoutWidget);
        pbHelp->setObjectName(QStringLiteral("pbHelp"));

        buttonLayout->addWidget(pbHelp);

        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(spacerItem);

        pbOk = new QPushButton(layoutWidget);
        pbOk->setObjectName(QStringLiteral("pbOk"));

        buttonLayout->addWidget(pbOk);

        pbCancel = new QPushButton(layoutWidget);
        pbCancel->setObjectName(QStringLiteral("pbCancel"));

        buttonLayout->addWidget(pbCancel);


        retranslateUi(GV_PipeLineDialog);
        QObject::connect(pbCancel, SIGNAL(clicked()), GV_PipeLineDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(GV_PipeLineDialog);
    } // setupUi

    void retranslateUi(QDialog *GV_PipeLineDialog)
    {
        GV_PipeLineDialog->setWindowTitle(QApplication::translate("GV_PipeLineDialog", "Pipe Line Main Data", 0));
        pbHelp->setText(QApplication::translate("GV_PipeLineDialog", "&Help", 0));
        pbOk->setText(QApplication::translate("GV_PipeLineDialog", "OK", 0));
        pbCancel->setText(QApplication::translate("GV_PipeLineDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class GV_PipeLineDialog: public Ui_GV_PipeLineDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_PIPELINEDIALOG_H
