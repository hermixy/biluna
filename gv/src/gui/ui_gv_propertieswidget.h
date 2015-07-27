/********************************************************************************
** Form generated from reading UI file 'gv_propertieswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GV_PROPERTIESWIDGET_H
#define UI_GV_PROPERTIESWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GV_PropertiesWidget
{
public:
    QGridLayout *gridLayout;
    QTableWidget *twProperties;

    void setupUi(QWidget *GV_PropertiesWidget)
    {
        if (GV_PropertiesWidget->objectName().isEmpty())
            GV_PropertiesWidget->setObjectName(QStringLiteral("GV_PropertiesWidget"));
        GV_PropertiesWidget->resize(250, 350);
        GV_PropertiesWidget->setMinimumSize(QSize(100, 100));
        gridLayout = new QGridLayout(GV_PropertiesWidget);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        twProperties = new QTableWidget(GV_PropertiesWidget);
        twProperties->setObjectName(QStringLiteral("twProperties"));

        gridLayout->addWidget(twProperties, 0, 0, 1, 1);


        retranslateUi(GV_PropertiesWidget);

        QMetaObject::connectSlotsByName(GV_PropertiesWidget);
    } // setupUi

    void retranslateUi(QWidget *GV_PropertiesWidget)
    {
        GV_PropertiesWidget->setWindowTitle(QApplication::translate("GV_PropertiesWidget", "Component Properties", 0));
    } // retranslateUi

};

namespace Ui {
    class GV_PropertiesWidget: public Ui_GV_PropertiesWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GV_PROPERTIESWIDGET_H
