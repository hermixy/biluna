/********************************************************************************
** Form generated from reading UI file 'gv_legendwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_LEGENDWIDGET_H
#define UI_GV_LEGENDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GV_LegendWidget
{
public:
    QGridLayout *gridLayout;
    QComboBox *cbLegend;
    QSpinBox *sbZoom;
    QFrame *frLegend;
    QGridLayout *frLayout;

    void setupUi(QWidget *GV_LegendWidget)
    {
        if (GV_LegendWidget->objectName().isEmpty())
            GV_LegendWidget->setObjectName(QStringLiteral("GV_LegendWidget"));
        GV_LegendWidget->resize(250, 350);
        GV_LegendWidget->setMinimumSize(QSize(100, 100));
        gridLayout = new QGridLayout(GV_LegendWidget);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        cbLegend = new QComboBox(GV_LegendWidget);
        cbLegend->setObjectName(QStringLiteral("cbLegend"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cbLegend->sizePolicy().hasHeightForWidth());
        cbLegend->setSizePolicy(sizePolicy);

        gridLayout->addWidget(cbLegend, 0, 0, 1, 1);

        sbZoom = new QSpinBox(GV_LegendWidget);
        sbZoom->setObjectName(QStringLiteral("sbZoom"));
        sbZoom->setMinimum(10);
        sbZoom->setMaximum(1000);
        sbZoom->setValue(100);

        gridLayout->addWidget(sbZoom, 0, 1, 1, 1);

        frLegend = new QFrame(GV_LegendWidget);
        frLegend->setObjectName(QStringLiteral("frLegend"));
        frLegend->setFrameShape(QFrame::NoFrame);
        frLegend->setFrameShadow(QFrame::Raised);
        frLayout = new QGridLayout(frLegend);
        frLayout->setContentsMargins(0, 0, 0, 0);
        frLayout->setObjectName(QStringLiteral("frLayout"));

        gridLayout->addWidget(frLegend, 1, 0, 1, 2);


        retranslateUi(GV_LegendWidget);

        QMetaObject::connectSlotsByName(GV_LegendWidget);
    } // setupUi

    void retranslateUi(QWidget *GV_LegendWidget)
    {
        GV_LegendWidget->setWindowTitle(QApplication::translate("GV_LegendWidget", "Symbol Legend", 0));
    } // retranslateUi

};

namespace Ui {
    class GV_LegendWidget: public Ui_GV_LegendWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_LEGENDWIDGET_H
