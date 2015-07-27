/********************************************************************************
** Form generated from reading UI file 'gv_previewwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_PREVIEWWIDGET_H
#define UI_GV_PREVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GV_PreviewWidget
{
public:
    QGridLayout *gridLayout;
    QGraphicsView *gvPreview;

    void setupUi(QWidget *GV_PreviewWidget)
    {
        if (GV_PreviewWidget->objectName().isEmpty())
            GV_PreviewWidget->setObjectName(QStringLiteral("GV_PreviewWidget"));
        GV_PreviewWidget->resize(300, 300);
        GV_PreviewWidget->setMinimumSize(QSize(100, 100));
        gridLayout = new QGridLayout(GV_PreviewWidget);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gvPreview = new QGraphicsView(GV_PreviewWidget);
        gvPreview->setObjectName(QStringLiteral("gvPreview"));

        gridLayout->addWidget(gvPreview, 0, 0, 1, 1);


        retranslateUi(GV_PreviewWidget);

        QMetaObject::connectSlotsByName(GV_PreviewWidget);
    } // setupUi

    void retranslateUi(QWidget *GV_PreviewWidget)
    {
        GV_PreviewWidget->setWindowTitle(QApplication::translate("GV_PreviewWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class GV_PreviewWidget: public Ui_GV_PreviewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_PREVIEWWIDGET_H
