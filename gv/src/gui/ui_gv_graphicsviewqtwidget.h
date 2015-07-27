/********************************************************************************
** Form generated from reading UI file 'gv_graphicsviewqtwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_GRAPHICSVIEWQTWIDGET_H
#define UI_GV_GRAPHICSVIEWQTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QWidget>
#include "RGraphicsViewQt.h"
#include "RRulerQt.h"

QT_BEGIN_NAMESPACE

class Ui_GV_GraphicsViewQtWidget
{
public:
    QGridLayout *gridLayout;
    QFrame *cornerFrame;
    RRulerQt *HorizontalRuler;
    QScrollBar *VerticalScrollBar;
    RRulerQt *VerticalRuler;
    RGraphicsViewQt *GraphicsView;
    QHBoxLayout *horizontalLayout_2;
    QScrollBar *HorizontalScrollBar_2;
    QLabel *InfoLabel_2;

    void setupUi(QWidget *GV_GraphicsViewQtWidget)
    {
        if (GV_GraphicsViewQtWidget->objectName().isEmpty())
            GV_GraphicsViewQtWidget->setObjectName(QStringLiteral("GV_GraphicsViewQtWidget"));
        GV_GraphicsViewQtWidget->resize(602, 446);
        gridLayout = new QGridLayout(GV_GraphicsViewQtWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        cornerFrame = new QFrame(GV_GraphicsViewQtWidget);
        cornerFrame->setObjectName(QStringLiteral("cornerFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cornerFrame->sizePolicy().hasHeightForWidth());
        cornerFrame->setSizePolicy(sizePolicy);
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
        cornerFrame->setPalette(palette);
        cornerFrame->setAutoFillBackground(true);
        cornerFrame->setFrameShape(QFrame::Panel);
        cornerFrame->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(cornerFrame, 0, 0, 1, 1);

        HorizontalRuler = new RRulerQt(GV_GraphicsViewQtWidget);
        HorizontalRuler->setObjectName(QStringLiteral("HorizontalRuler"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(HorizontalRuler->sizePolicy().hasHeightForWidth());
        HorizontalRuler->setSizePolicy(sizePolicy1);
        HorizontalRuler->setSizeIncrement(QSize(0, 0));
        HorizontalRuler->setProperty("orientation", QVariant(1));

        gridLayout->addWidget(HorizontalRuler, 0, 1, 1, 1);

        VerticalScrollBar = new QScrollBar(GV_GraphicsViewQtWidget);
        VerticalScrollBar->setObjectName(QStringLiteral("VerticalScrollBar"));
        VerticalScrollBar->setOrientation(Qt::Vertical);

        gridLayout->addWidget(VerticalScrollBar, 0, 2, 2, 1);

        VerticalRuler = new RRulerQt(GV_GraphicsViewQtWidget);
        VerticalRuler->setObjectName(QStringLiteral("VerticalRuler"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(VerticalRuler->sizePolicy().hasHeightForWidth());
        VerticalRuler->setSizePolicy(sizePolicy2);
        VerticalRuler->setProperty("orientation", QVariant(2));

        gridLayout->addWidget(VerticalRuler, 1, 0, 1, 1);

        GraphicsView = new RGraphicsViewQt(GV_GraphicsViewQtWidget);
        GraphicsView->setObjectName(QStringLiteral("GraphicsView"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(GraphicsView->sizePolicy().hasHeightForWidth());
        GraphicsView->setSizePolicy(sizePolicy3);
        GraphicsView->setAcceptDrops(true);

        gridLayout->addWidget(GraphicsView, 1, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(20);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        HorizontalScrollBar_2 = new QScrollBar(GV_GraphicsViewQtWidget);
        HorizontalScrollBar_2->setObjectName(QStringLiteral("HorizontalScrollBar_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(HorizontalScrollBar_2->sizePolicy().hasHeightForWidth());
        HorizontalScrollBar_2->setSizePolicy(sizePolicy4);
        HorizontalScrollBar_2->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(HorizontalScrollBar_2);

        InfoLabel_2 = new QLabel(GV_GraphicsViewQtWidget);
        InfoLabel_2->setObjectName(QStringLiteral("InfoLabel_2"));
        InfoLabel_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(InfoLabel_2);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 3);


        retranslateUi(GV_GraphicsViewQtWidget);

        QMetaObject::connectSlotsByName(GV_GraphicsViewQtWidget);
    } // setupUi

    void retranslateUi(QWidget *GV_GraphicsViewQtWidget)
    {
        GV_GraphicsViewQtWidget->setWindowTitle(QApplication::translate("GV_GraphicsViewQtWidget", "Form", 0));
        InfoLabel_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GV_GraphicsViewQtWidget: public Ui_GV_GraphicsViewQtWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_GRAPHICSVIEWQTWIDGET_H
