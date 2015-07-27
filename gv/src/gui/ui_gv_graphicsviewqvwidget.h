/********************************************************************************
** Form generated from reading UI file 'gv_graphicsviewqvwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_GRAPHICSVIEWQVWIDGET_H
#define UI_GV_GRAPHICSVIEWQVWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "RGraphicsViewQV.h"
#include "RRulerQt.h"

QT_BEGIN_NAMESPACE

class Ui_GV_GraphicsViewQVWidget
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QFrame *leftTopFrame;
    RRulerQt *horizontalRuler;
    QFrame *rightTopFrame;
    QVBoxLayout *verticalLayout;
    RRulerQt *verticalRuler;
    QFrame *leftBottomFrame;
    RGraphicsViewQV *view;

    void setupUi(QWidget *GV_GraphicsViewQVWidget)
    {
        if (GV_GraphicsViewQVWidget->objectName().isEmpty())
            GV_GraphicsViewQVWidget->setObjectName(QStringLiteral("GV_GraphicsViewQVWidget"));
        GV_GraphicsViewQVWidget->resize(602, 446);
        gridLayout = new QGridLayout(GV_GraphicsViewQVWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        leftTopFrame = new QFrame(GV_GraphicsViewQVWidget);
        leftTopFrame->setObjectName(QStringLiteral("leftTopFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leftTopFrame->sizePolicy().hasHeightForWidth());
        leftTopFrame->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(199, 199, 199, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        leftTopFrame->setPalette(palette);
        leftTopFrame->setAutoFillBackground(true);
        leftTopFrame->setFrameShape(QFrame::Panel);
        leftTopFrame->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(leftTopFrame);

        horizontalRuler = new RRulerQt(GV_GraphicsViewQVWidget);
        horizontalRuler->setObjectName(QStringLiteral("horizontalRuler"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(horizontalRuler->sizePolicy().hasHeightForWidth());
        horizontalRuler->setSizePolicy(sizePolicy1);
        horizontalRuler->setMinimumSize(QSize(0, 10));
        horizontalRuler->setSizeIncrement(QSize(0, 0));
        horizontalRuler->setProperty("orientation", QVariant(1));

        horizontalLayout->addWidget(horizontalRuler);

        rightTopFrame = new QFrame(GV_GraphicsViewQVWidget);
        rightTopFrame->setObjectName(QStringLiteral("rightTopFrame"));
        sizePolicy.setHeightForWidth(rightTopFrame->sizePolicy().hasHeightForWidth());
        rightTopFrame->setSizePolicy(sizePolicy);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        rightTopFrame->setPalette(palette1);
        rightTopFrame->setAutoFillBackground(true);
        rightTopFrame->setFrameShape(QFrame::Panel);
        rightTopFrame->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(rightTopFrame);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalRuler = new RRulerQt(GV_GraphicsViewQVWidget);
        verticalRuler->setObjectName(QStringLiteral("verticalRuler"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(verticalRuler->sizePolicy().hasHeightForWidth());
        verticalRuler->setSizePolicy(sizePolicy2);
        verticalRuler->setMinimumSize(QSize(10, 0));
        verticalRuler->setProperty("orientation", QVariant(2));

        verticalLayout->addWidget(verticalRuler);

        leftBottomFrame = new QFrame(GV_GraphicsViewQVWidget);
        leftBottomFrame->setObjectName(QStringLiteral("leftBottomFrame"));
        sizePolicy.setHeightForWidth(leftBottomFrame->sizePolicy().hasHeightForWidth());
        leftBottomFrame->setSizePolicy(sizePolicy);
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        leftBottomFrame->setPalette(palette2);
        leftBottomFrame->setAutoFillBackground(true);
        leftBottomFrame->setFrameShape(QFrame::Panel);
        leftBottomFrame->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(leftBottomFrame);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

        view = new RGraphicsViewQV(GV_GraphicsViewQVWidget);
        view->setObjectName(QStringLiteral("view"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(view->sizePolicy().hasHeightForWidth());
        view->setSizePolicy(sizePolicy3);
        view->setAcceptDrops(true);
        view->setAutoFillBackground(false);

        gridLayout->addWidget(view, 1, 1, 1, 1);


        retranslateUi(GV_GraphicsViewQVWidget);

        QMetaObject::connectSlotsByName(GV_GraphicsViewQVWidget);
    } // setupUi

    void retranslateUi(QWidget *GV_GraphicsViewQVWidget)
    {
        GV_GraphicsViewQVWidget->setWindowTitle(QApplication::translate("GV_GraphicsViewQVWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class GV_GraphicsViewQVWidget: public Ui_GV_GraphicsViewQVWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_GRAPHICSVIEWQVWIDGET_H
