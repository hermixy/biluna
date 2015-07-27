/********************************************************************************
** Form generated from reading UI file 'gv_drawinglistwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_DRAWINGLISTWIDGET_H
#define UI_GV_DRAWINGLISTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GV_DrawingListWidget
{
public:
    QGridLayout *gridLayout;
    QLineEdit *leProjectNumber;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pbAddDrawing;
    QPushButton *pbDeleteDrawing;
    QSpacerItem *horizontalSpacer;
    QTableView *tvDrawingList;

    void setupUi(QWidget *GV_DrawingListWidget)
    {
        if (GV_DrawingListWidget->objectName().isEmpty())
            GV_DrawingListWidget->setObjectName(QStringLiteral("GV_DrawingListWidget"));
        GV_DrawingListWidget->resize(190, 400);
        gridLayout = new QGridLayout(GV_DrawingListWidget);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        leProjectNumber = new QLineEdit(GV_DrawingListWidget);
        leProjectNumber->setObjectName(QStringLiteral("leProjectNumber"));
        leProjectNumber->setBaseSize(QSize(150, 0));

        gridLayout->addWidget(leProjectNumber, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pbAddDrawing = new QPushButton(GV_DrawingListWidget);
        pbAddDrawing->setObjectName(QStringLiteral("pbAddDrawing"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pbAddDrawing->sizePolicy().hasHeightForWidth());
        pbAddDrawing->setSizePolicy(sizePolicy);
        pbAddDrawing->setMinimumSize(QSize(24, 24));
        pbAddDrawing->setMaximumSize(QSize(24, 24));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAddDrawing->setIcon(icon);
        pbAddDrawing->setIconSize(QSize(16, 16));

        horizontalLayout_2->addWidget(pbAddDrawing);

        pbDeleteDrawing = new QPushButton(GV_DrawingListWidget);
        pbDeleteDrawing->setObjectName(QStringLiteral("pbDeleteDrawing"));
        pbDeleteDrawing->setMinimumSize(QSize(24, 24));
        pbDeleteDrawing->setMaximumSize(QSize(24, 24));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbDeleteDrawing->setIcon(icon1);

        horizontalLayout_2->addWidget(pbDeleteDrawing);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        tvDrawingList = new QTableView(GV_DrawingListWidget);
        tvDrawingList->setObjectName(QStringLiteral("tvDrawingList"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tvDrawingList->sizePolicy().hasHeightForWidth());
        tvDrawingList->setSizePolicy(sizePolicy1);
        tvDrawingList->setBaseSize(QSize(186, 314));

        verticalLayout->addWidget(tvDrawingList);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);


        retranslateUi(GV_DrawingListWidget);

        QMetaObject::connectSlotsByName(GV_DrawingListWidget);
    } // setupUi

    void retranslateUi(QWidget *GV_DrawingListWidget)
    {
        GV_DrawingListWidget->setWindowTitle(QApplication::translate("GV_DrawingListWidget", "Form", 0));
#ifndef QT_NO_TOOLTIP
        leProjectNumber->setToolTip(QApplication::translate("GV_DrawingListWidget", "Project Number", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pbAddDrawing->setToolTip(QApplication::translate("GV_DrawingListWidget", "Add customer", 0));
#endif // QT_NO_TOOLTIP
        pbAddDrawing->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbDeleteDrawing->setToolTip(QApplication::translate("GV_DrawingListWidget", "Delete customer", 0));
#endif // QT_NO_TOOLTIP
        pbDeleteDrawing->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GV_DrawingListWidget: public Ui_GV_DrawingListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_DRAWINGLISTWIDGET_H
