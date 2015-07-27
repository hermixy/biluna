/********************************************************************************
** Form generated from reading UI file 'gv_drawingwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_DRAWINGWIDGET_H
#define UI_GV_DRAWINGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GV_DrawingWidget
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;

    void setupUi(QWidget *GV_DrawingWidget)
    {
        if (GV_DrawingWidget->objectName().isEmpty())
            GV_DrawingWidget->setObjectName(QStringLiteral("GV_DrawingWidget"));
        GV_DrawingWidget->resize(640, 480);
        gridLayout = new QGridLayout(GV_DrawingWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        frame = new QFrame(GV_DrawingWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(GV_DrawingWidget);

        QMetaObject::connectSlotsByName(GV_DrawingWidget);
    } // setupUi

    void retranslateUi(QWidget *GV_DrawingWidget)
    {
        GV_DrawingWidget->setWindowTitle(QApplication::translate("GV_DrawingWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class GV_DrawingWidget: public Ui_GV_DrawingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_DRAWINGWIDGET_H
