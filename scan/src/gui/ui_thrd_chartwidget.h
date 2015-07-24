/********************************************************************************
** Form generated from reading UI file 'thrd_chartwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THRD_CHARTWIDGET_H
#define UI_THRD_CHARTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_THRD_ChartWidget
{
public:
    QGridLayout *gridLayout_4;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout_2;
    QLabel *label;
    QComboBox *cbChartType;
    QLabel *label_2;
    QComboBox *cbLegendType;
    QLabel *label_3;
    QComboBox *cbShadow;
    QLabel *label_19;
    QComboBox *cbInsideValues;
    QGroupBox *groupBox_4;
    QFormLayout *formLayout_4;
    QLabel *label_11;
    QLineEdit *leImageWidth;
    QLabel *label_12;
    QLineEdit *leImageHeight;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label_4;
    QLineEdit *leGraphX;
    QLabel *label_5;
    QLineEdit *leGraphY;
    QLabel *label_6;
    QLineEdit *leGraphWidth;
    QLabel *label_7;
    QLineEdit *leGraphHeight;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_3;
    QLabel *label_8;
    QLineEdit *leLegendX;
    QLabel *label_9;
    QLineEdit *leLegendY;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_2;
    QLabel *label_10;
    QLineEdit *leFont;
    QToolButton *pbFont;
    QLabel *label_13;
    QLineEdit *leStartAngle;
    QLabel *label_14;
    QLineEdit *leLabelBefore;
    QLabel *label_15;
    QLineEdit *leLabelAfter;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *leLengthYaxis;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
    QPushButton *pbAddStackName;
    QPushButton *pbDeleteStackName;
    QSpacerItem *verticalSpacer;
    QTableWidget *twStacks;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pbAddItem;
    QPushButton *pbDeleteItem;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbRefresh;
    QTableWidget *twItems;
    QTextEdit *textEdit;

    void setupUi(QWidget *THRD_ChartWidget)
    {
        if (THRD_ChartWidget->objectName().isEmpty())
            THRD_ChartWidget->setObjectName(QStringLiteral("THRD_ChartWidget"));
        THRD_ChartWidget->resize(720, 575);
        gridLayout_4 = new QGridLayout(THRD_ChartWidget);
        gridLayout_4->setSpacing(3);
        gridLayout_4->setContentsMargins(3, 3, 3, 3);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        splitter = new QSplitter(THRD_ChartWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        gridLayout_3 = new QGridLayout(layoutWidget);
        gridLayout_3->setSpacing(3);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setHorizontalSpacing(3);
        formLayout_2->setVerticalSpacing(3);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        cbChartType = new QComboBox(layoutWidget);
        cbChartType->setObjectName(QStringLiteral("cbChartType"));
        cbChartType->setMinimumSize(QSize(100, 0));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, cbChartType);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_2);

        cbLegendType = new QComboBox(layoutWidget);
        cbLegendType->setObjectName(QStringLiteral("cbLegendType"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, cbLegendType);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_3);

        cbShadow = new QComboBox(layoutWidget);
        cbShadow->setObjectName(QStringLiteral("cbShadow"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, cbShadow);

        label_19 = new QLabel(layoutWidget);
        label_19->setObjectName(QStringLiteral("label_19"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_19);

        cbInsideValues = new QComboBox(layoutWidget);
        cbInsideValues->setObjectName(QStringLiteral("cbInsideValues"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, cbInsideValues);


        verticalLayout->addLayout(formLayout_2);

        groupBox_4 = new QGroupBox(layoutWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        formLayout_4 = new QFormLayout(groupBox_4);
        formLayout_4->setContentsMargins(3, 3, 3, 3);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setHorizontalSpacing(3);
        formLayout_4->setVerticalSpacing(3);
        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QStringLiteral("label_11"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_11);

        leImageWidth = new QLineEdit(groupBox_4);
        leImageWidth->setObjectName(QStringLiteral("leImageWidth"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leImageWidth->sizePolicy().hasHeightForWidth());
        leImageWidth->setSizePolicy(sizePolicy);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, leImageWidth);

        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QStringLiteral("label_12"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_12);

        leImageHeight = new QLineEdit(groupBox_4);
        leImageHeight->setObjectName(QStringLiteral("leImageHeight"));
        sizePolicy.setHeightForWidth(leImageHeight->sizePolicy().hasHeightForWidth());
        leImageHeight->setSizePolicy(sizePolicy);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, leImageHeight);


        verticalLayout->addWidget(groupBox_4);

        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setContentsMargins(3, 3, 3, 3);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setHorizontalSpacing(3);
        formLayout->setVerticalSpacing(3);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_4);

        leGraphX = new QLineEdit(groupBox);
        leGraphX->setObjectName(QStringLiteral("leGraphX"));

        formLayout->setWidget(0, QFormLayout::FieldRole, leGraphX);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

        leGraphY = new QLineEdit(groupBox);
        leGraphY->setObjectName(QStringLiteral("leGraphY"));

        formLayout->setWidget(1, QFormLayout::FieldRole, leGraphY);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_6);

        leGraphWidth = new QLineEdit(groupBox);
        leGraphWidth->setObjectName(QStringLiteral("leGraphWidth"));

        formLayout->setWidget(2, QFormLayout::FieldRole, leGraphWidth);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_7);

        leGraphHeight = new QLineEdit(groupBox);
        leGraphHeight->setObjectName(QStringLiteral("leGraphHeight"));

        formLayout->setWidget(3, QFormLayout::FieldRole, leGraphHeight);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        formLayout_3 = new QFormLayout(groupBox_2);
        formLayout_3->setContentsMargins(3, 3, 3, 3);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_3->setHorizontalSpacing(3);
        formLayout_3->setVerticalSpacing(3);
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_8);

        leLegendX = new QLineEdit(groupBox_2);
        leLegendX->setObjectName(QStringLiteral("leLegendX"));
        sizePolicy.setHeightForWidth(leLegendX->sizePolicy().hasHeightForWidth());
        leLegendX->setSizePolicy(sizePolicy);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, leLegendX);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_9);

        leLegendY = new QLineEdit(groupBox_2);
        leLegendY->setObjectName(QStringLiteral("leLegendY"));
        sizePolicy.setHeightForWidth(leLegendY->sizePolicy().hasHeightForWidth());
        leLegendY->setSizePolicy(sizePolicy);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, leLegendY);


        verticalLayout->addWidget(groupBox_2);


        gridLayout_3->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(3);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_2->addWidget(label_10, 0, 0, 1, 1);

        leFont = new QLineEdit(layoutWidget);
        leFont->setObjectName(QStringLiteral("leFont"));
        sizePolicy.setHeightForWidth(leFont->sizePolicy().hasHeightForWidth());
        leFont->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(leFont, 0, 1, 1, 1);

        pbFont = new QToolButton(layoutWidget);
        pbFont->setObjectName(QStringLiteral("pbFont"));

        gridLayout_2->addWidget(pbFont, 0, 2, 1, 1);

        label_13 = new QLabel(layoutWidget);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 1, 0, 1, 1);

        leStartAngle = new QLineEdit(layoutWidget);
        leStartAngle->setObjectName(QStringLiteral("leStartAngle"));
        sizePolicy.setHeightForWidth(leStartAngle->sizePolicy().hasHeightForWidth());
        leStartAngle->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(leStartAngle, 1, 1, 1, 1);

        label_14 = new QLabel(layoutWidget);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_2->addWidget(label_14, 2, 0, 1, 1);

        leLabelBefore = new QLineEdit(layoutWidget);
        leLabelBefore->setObjectName(QStringLiteral("leLabelBefore"));

        gridLayout_2->addWidget(leLabelBefore, 2, 1, 1, 1);

        label_15 = new QLabel(layoutWidget);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_2->addWidget(label_15, 3, 0, 1, 1);

        leLabelAfter = new QLineEdit(layoutWidget);
        leLabelAfter->setObjectName(QStringLiteral("leLabelAfter"));

        gridLayout_2->addWidget(leLabelAfter, 3, 1, 1, 1);

        label_16 = new QLabel(layoutWidget);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_2->addWidget(label_16, 2, 2, 1, 1);

        label_17 = new QLabel(layoutWidget);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_2->addWidget(label_17, 3, 2, 1, 1);

        label_18 = new QLabel(layoutWidget);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_2->addWidget(label_18, 4, 0, 1, 1);

        leLengthYaxis = new QLineEdit(layoutWidget);
        leLengthYaxis->setObjectName(QStringLiteral("leLengthYaxis"));

        gridLayout_2->addWidget(leLengthYaxis, 4, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);

        groupBox_5 = new QGroupBox(layoutWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setSpacing(3);
        gridLayout_5->setContentsMargins(3, 3, 3, 3);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        pbAddStackName = new QPushButton(groupBox_5);
        pbAddStackName->setObjectName(QStringLiteral("pbAddStackName"));

        gridLayout_5->addWidget(pbAddStackName, 0, 1, 1, 1);

        pbDeleteStackName = new QPushButton(groupBox_5);
        pbDeleteStackName->setObjectName(QStringLiteral("pbDeleteStackName"));

        gridLayout_5->addWidget(pbDeleteStackName, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_5->addItem(verticalSpacer, 2, 1, 1, 1);

        twStacks = new QTableWidget(groupBox_5);
        twStacks->setObjectName(QStringLiteral("twStacks"));

        gridLayout_5->addWidget(twStacks, 0, 0, 3, 1);


        verticalLayout_2->addWidget(groupBox_5);


        gridLayout_3->addLayout(verticalLayout_2, 0, 1, 1, 1);

        groupBox_3 = new QGroupBox(layoutWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pbAddItem = new QPushButton(groupBox_3);
        pbAddItem->setObjectName(QStringLiteral("pbAddItem"));

        horizontalLayout_2->addWidget(pbAddItem);

        pbDeleteItem = new QPushButton(groupBox_3);
        pbDeleteItem->setObjectName(QStringLiteral("pbDeleteItem"));

        horizontalLayout_2->addWidget(pbDeleteItem);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pbRefresh = new QPushButton(groupBox_3);
        pbRefresh->setObjectName(QStringLiteral("pbRefresh"));

        horizontalLayout_2->addWidget(pbRefresh);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        twItems = new QTableWidget(groupBox_3);
        twItems->setObjectName(QStringLiteral("twItems"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(twItems->sizePolicy().hasHeightForWidth());
        twItems->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(twItems, 1, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_3, 1, 0, 1, 2);

        splitter->addWidget(layoutWidget);
        textEdit = new QTextEdit(splitter);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(2);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy2);
        splitter->addWidget(textEdit);

        gridLayout_4->addWidget(splitter, 0, 0, 1, 1);

        QWidget::setTabOrder(cbChartType, cbLegendType);
        QWidget::setTabOrder(cbLegendType, cbShadow);
        QWidget::setTabOrder(cbShadow, leImageWidth);
        QWidget::setTabOrder(leImageWidth, leImageHeight);
        QWidget::setTabOrder(leImageHeight, leGraphX);
        QWidget::setTabOrder(leGraphX, leGraphY);
        QWidget::setTabOrder(leGraphY, leGraphWidth);
        QWidget::setTabOrder(leGraphWidth, leGraphHeight);
        QWidget::setTabOrder(leGraphHeight, leLegendX);
        QWidget::setTabOrder(leLegendX, leLegendY);
        QWidget::setTabOrder(leLegendY, leFont);
        QWidget::setTabOrder(leFont, pbFont);
        QWidget::setTabOrder(pbFont, leStartAngle);
        QWidget::setTabOrder(leStartAngle, leLabelBefore);
        QWidget::setTabOrder(leLabelBefore, leLabelAfter);
        QWidget::setTabOrder(leLabelAfter, leLengthYaxis);
        QWidget::setTabOrder(leLengthYaxis, twStacks);
        QWidget::setTabOrder(twStacks, pbAddStackName);
        QWidget::setTabOrder(pbAddStackName, pbDeleteStackName);
        QWidget::setTabOrder(pbDeleteStackName, twItems);
        QWidget::setTabOrder(twItems, pbAddItem);
        QWidget::setTabOrder(pbAddItem, pbDeleteItem);
        QWidget::setTabOrder(pbDeleteItem, pbRefresh);
        QWidget::setTabOrder(pbRefresh, textEdit);

        retranslateUi(THRD_ChartWidget);

        QMetaObject::connectSlotsByName(THRD_ChartWidget);
    } // setupUi

    void retranslateUi(QWidget *THRD_ChartWidget)
    {
        THRD_ChartWidget->setWindowTitle(QApplication::translate("THRD_ChartWidget", "Form", 0));
        label->setText(QApplication::translate("THRD_ChartWidget", "Chart Type", 0));
        label_2->setText(QApplication::translate("THRD_ChartWidget", "Legend Type", 0));
        label_3->setText(QApplication::translate("THRD_ChartWidget", "Shadow", 0));
        label_19->setText(QApplication::translate("THRD_ChartWidget", "Inside Values", 0));
        groupBox_4->setTitle(QApplication::translate("THRD_ChartWidget", "Image Dimension", 0));
        label_11->setText(QApplication::translate("THRD_ChartWidget", "Width", 0));
        label_12->setText(QApplication::translate("THRD_ChartWidget", "Height", 0));
        groupBox->setTitle(QApplication::translate("THRD_ChartWidget", "Graph Coordinates", 0));
        label_4->setText(QApplication::translate("THRD_ChartWidget", "X (left)", 0));
        label_5->setText(QApplication::translate("THRD_ChartWidget", "Y (top)", 0));
        label_6->setText(QApplication::translate("THRD_ChartWidget", "Width", 0));
        label_7->setText(QApplication::translate("THRD_ChartWidget", "Height", 0));
        groupBox_2->setTitle(QApplication::translate("THRD_ChartWidget", "Legend Coordinates", 0));
        label_8->setText(QApplication::translate("THRD_ChartWidget", "X (left)", 0));
        label_9->setText(QApplication::translate("THRD_ChartWidget", "Y (top)", 0));
        label_10->setText(QApplication::translate("THRD_ChartWidget", "Font", 0));
        pbFont->setText(QApplication::translate("THRD_ChartWidget", "...", 0));
        label_13->setText(QApplication::translate("THRD_ChartWidget", "Start Angle Pie", 0));
        label_14->setText(QApplication::translate("THRD_ChartWidget", "Before Label", 0));
        label_15->setText(QApplication::translate("THRD_ChartWidget", "After Label", 0));
        label_16->setText(QApplication::translate("THRD_ChartWidget", "(Unit)", 0));
        label_17->setText(QApplication::translate("THRD_ChartWidget", "(Unit)", 0));
        label_18->setText(QApplication::translate("THRD_ChartWidget", "Length Y Axis", 0));
        groupBox_5->setTitle(QApplication::translate("THRD_ChartWidget", "Stack Names", 0));
        pbAddStackName->setText(QApplication::translate("THRD_ChartWidget", "Add", 0));
        pbDeleteStackName->setText(QApplication::translate("THRD_ChartWidget", "Delete", 0));
        groupBox_3->setTitle(QApplication::translate("THRD_ChartWidget", "Chart Items", 0));
        pbAddItem->setText(QApplication::translate("THRD_ChartWidget", "Add", 0));
        pbDeleteItem->setText(QApplication::translate("THRD_ChartWidget", "Delete", 0));
        pbRefresh->setText(QApplication::translate("THRD_ChartWidget", "Refresh &NightChart   >>", 0));
        textEdit->setHtml(QApplication::translate("THRD_ChartWidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Click refresh button ...</span></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class THRD_ChartWidget: public Ui_THRD_ChartWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THRD_CHARTWIDGET_H
