/********************************************************************************
** Form generated from reading UI file 'sail_mapnavigationwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAIL_MAPNAVIGATIONWIDGET_H
#define UI_SAIL_MAPNAVIGATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "rb_idxlineedit.h"
#include "rb_widget.h"

QT_BEGIN_NAMESPACE

class Ui_SAIL_MapNavigationWidget
{
public:
    QGridLayout *gridLayout_10;
    QTabWidget *tabWidget;
    QWidget *tabNavigation;
    QGridLayout *gridLayout_4;
    QGraphicsView *gv;
    QWidget *tabCoordinate;
    QGridLayout *gridLayout_3;
    QGroupBox *gbCoordinate;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QToolButton *pbAddCoordinate;
    QToolButton *pbDeleteCoordinate;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *pbSaveCoordinate;
    QLineEdit *leFilter;
    QToolButton *pbFilterOn;
    QToolButton *pbFilterOff;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QTableView *tvCoordinate;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *leCoordinateName;
    QLabel *label_3;
    QLineEdit *leCoordinateComment;
    QLabel *label_4;
    RB_IdxLineEdit *ileSymbol;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QLineEdit *leLatitude;
    QLabel *label_8;
    QLineEdit *leLongitude;
    QLineEdit *leLatDecimal;
    QLineEdit *leLongDecimal;
    QLabel *label_19;
    QLabel *label_20;
    QWidget *tabRoute;
    QGridLayout *gridLayout_8;
    QSplitter *splitter;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *pbAddRoute;
    QToolButton *pbDeleteRoute;
    QSpacerItem *horizontalSpacer_7;
    QToolButton *pbSaveRoute;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QLineEdit *leRouteName;
    QLabel *label_9;
    QLineEdit *leRouteComment;
    QTableView *tvRoute;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *pbAddRouteCoordinate;
    QToolButton *pbDeleteRouteCoordinate;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_10;
    QTableView *tvRouteCoordinate;
    QFormLayout *formLayout_2;
    QLabel *label_11;
    QLineEdit *leSequenceNumber;
    QLabel *label_13;
    RB_IdxLineEdit *ileRouteCoordinate;
    QLabel *label_12;
    QLineEdit *leRouteCoordinateComment;
    QGroupBox *gbRouteCalculation;
    QGridLayout *gridLayout_6;
    QLabel *label_22;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *leWindDirectionFrom;
    QToolButton *pbCalcRoute;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_23;
    QLineEdit *leCalculatedDistance;
    QLabel *label_24;
    QLineEdit *leCalculatedDuration;
    QFrame *line;
    QWidget *tabTrack;
    QGridLayout *gridLayout_12;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_9;
    QHBoxLayout *horizontalLayout_5;
    QToolButton *pbAddTrack;
    QToolButton *pbDeleteTrack;
    QSpacerItem *horizontalSpacer_8;
    QToolButton *pbSaveTrack;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_14;
    QTableView *tvTrack;
    QGridLayout *gridLayout_11;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_21;
    QLineEdit *leTrackName;
    QLineEdit *leTrackComment;
    RB_IdxLineEdit *ileTrackCoordinateFrom;
    QLineEdit *leWeightFactor;
    RB_IdxLineEdit *ileTrackCoordinateTo;

    void setupUi(RB_Widget *SAIL_MapNavigationWidget)
    {
        if (SAIL_MapNavigationWidget->objectName().isEmpty())
            SAIL_MapNavigationWidget->setObjectName(QStringLiteral("SAIL_MapNavigationWidget"));
        SAIL_MapNavigationWidget->resize(620, 467);
        gridLayout_10 = new QGridLayout(SAIL_MapNavigationWidget);
        gridLayout_10->setSpacing(3);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_10->setContentsMargins(3, 3, 3, 3);
        tabWidget = new QTabWidget(SAIL_MapNavigationWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabNavigation = new QWidget();
        tabNavigation->setObjectName(QStringLiteral("tabNavigation"));
        gridLayout_4 = new QGridLayout(tabNavigation);
        gridLayout_4->setSpacing(1);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(1, 1, 1, 1);
        gv = new QGraphicsView(tabNavigation);
        gv->setObjectName(QStringLiteral("gv"));

        gridLayout_4->addWidget(gv, 0, 0, 1, 1);

        tabWidget->addTab(tabNavigation, QString());
        tabCoordinate = new QWidget();
        tabCoordinate->setObjectName(QStringLiteral("tabCoordinate"));
        gridLayout_3 = new QGridLayout(tabCoordinate);
        gridLayout_3->setSpacing(3);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(3, 3, 3, 3);
        gbCoordinate = new QGroupBox(tabCoordinate);
        gbCoordinate->setObjectName(QStringLiteral("gbCoordinate"));
        gridLayout = new QGridLayout(gbCoordinate);
        gridLayout->setSpacing(3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(3, 3, 3, 3);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pbAddCoordinate = new QToolButton(gbCoordinate);
        pbAddCoordinate->setObjectName(QStringLiteral("pbAddCoordinate"));
        pbAddCoordinate->setMinimumSize(QSize(24, 24));
        pbAddCoordinate->setMaximumSize(QSize(24, 24));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAddCoordinate->setIcon(icon);

        horizontalLayout->addWidget(pbAddCoordinate);

        pbDeleteCoordinate = new QToolButton(gbCoordinate);
        pbDeleteCoordinate->setObjectName(QStringLiteral("pbDeleteCoordinate"));
        pbDeleteCoordinate->setMinimumSize(QSize(24, 24));
        pbDeleteCoordinate->setMaximumSize(QSize(24, 24));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbDeleteCoordinate->setIcon(icon1);

        horizontalLayout->addWidget(pbDeleteCoordinate);

        horizontalSpacer_3 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        pbSaveCoordinate = new QToolButton(gbCoordinate);
        pbSaveCoordinate->setObjectName(QStringLiteral("pbSaveCoordinate"));
        pbSaveCoordinate->setMinimumSize(QSize(24, 24));
        pbSaveCoordinate->setMaximumSize(QSize(24, 24));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/icons/database_save.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbSaveCoordinate->setIcon(icon2);

        horizontalLayout->addWidget(pbSaveCoordinate);

        leFilter = new QLineEdit(gbCoordinate);
        leFilter->setObjectName(QStringLiteral("leFilter"));

        horizontalLayout->addWidget(leFilter);

        pbFilterOn = new QToolButton(gbCoordinate);
        pbFilterOn->setObjectName(QStringLiteral("pbFilterOn"));
        pbFilterOn->setMinimumSize(QSize(24, 24));
        pbFilterOn->setMaximumSize(QSize(24, 24));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/icons/filter.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFilterOn->setIcon(icon3);

        horizontalLayout->addWidget(pbFilterOn);

        pbFilterOff = new QToolButton(gbCoordinate);
        pbFilterOff->setObjectName(QStringLiteral("pbFilterOff"));
        pbFilterOff->setMinimumSize(QSize(24, 24));
        pbFilterOff->setMaximumSize(QSize(24, 24));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/icons/filter_empty.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFilterOff->setIcon(icon4);

        horizontalLayout->addWidget(pbFilterOff);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(gbCoordinate);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tvCoordinate = new QTableView(gbCoordinate);
        tvCoordinate->setObjectName(QStringLiteral("tvCoordinate"));

        gridLayout->addWidget(tvCoordinate, 1, 0, 1, 1);


        gridLayout_3->addWidget(gbCoordinate, 0, 0, 1, 2);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout->setHorizontalSpacing(3);
        formLayout->setVerticalSpacing(3);
        label_2 = new QLabel(tabCoordinate);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        leCoordinateName = new QLineEdit(tabCoordinate);
        leCoordinateName->setObjectName(QStringLiteral("leCoordinateName"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leCoordinateName->sizePolicy().hasHeightForWidth());
        leCoordinateName->setSizePolicy(sizePolicy);
        leCoordinateName->setMinimumSize(QSize(0, 0));
        leCoordinateName->setMaximumSize(QSize(16777215, 16777215));

        formLayout->setWidget(0, QFormLayout::FieldRole, leCoordinateName);

        label_3 = new QLabel(tabCoordinate);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        leCoordinateComment = new QLineEdit(tabCoordinate);
        leCoordinateComment->setObjectName(QStringLiteral("leCoordinateComment"));
        leCoordinateComment->setMinimumSize(QSize(0, 0));
        leCoordinateComment->setMaximumSize(QSize(16777215, 16777215));

        formLayout->setWidget(1, QFormLayout::FieldRole, leCoordinateComment);

        label_4 = new QLabel(tabCoordinate);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_4);

        ileSymbol = new RB_IdxLineEdit(tabCoordinate);
        ileSymbol->setObjectName(QStringLiteral("ileSymbol"));

        formLayout->setWidget(2, QFormLayout::FieldRole, ileSymbol);


        gridLayout_3->addLayout(formLayout, 1, 0, 1, 1);

        groupBox = new QGroupBox(tabCoordinate);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(0, 0));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(3);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        leLatitude = new QLineEdit(groupBox);
        leLatitude->setObjectName(QStringLiteral("leLatitude"));
        leLatitude->setMinimumSize(QSize(90, 0));
        leLatitude->setMaximumSize(QSize(90, 16777215));
        leLatitude->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(leLatitude, 0, 1, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 1, 0, 1, 1);

        leLongitude = new QLineEdit(groupBox);
        leLongitude->setObjectName(QStringLiteral("leLongitude"));
        leLongitude->setMinimumSize(QSize(90, 0));
        leLongitude->setMaximumSize(QSize(90, 16777215));
        leLongitude->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(leLongitude, 1, 1, 1, 1);

        leLatDecimal = new QLineEdit(groupBox);
        leLatDecimal->setObjectName(QStringLiteral("leLatDecimal"));
        leLatDecimal->setMaximumSize(QSize(120, 16777215));
        leLatDecimal->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        leLatDecimal->setReadOnly(true);

        gridLayout_2->addWidget(leLatDecimal, 0, 3, 1, 1);

        leLongDecimal = new QLineEdit(groupBox);
        leLongDecimal->setObjectName(QStringLiteral("leLongDecimal"));
        leLongDecimal->setMaximumSize(QSize(120, 16777215));
        leLongDecimal->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        leLongDecimal->setReadOnly(true);

        gridLayout_2->addWidget(leLongDecimal, 1, 3, 1, 1);

        label_19 = new QLabel(groupBox);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_2->addWidget(label_19, 0, 2, 1, 1);

        label_20 = new QLabel(groupBox);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_2->addWidget(label_20, 1, 2, 1, 1);


        gridLayout_3->addWidget(groupBox, 1, 1, 1, 1);

        tabWidget->addTab(tabCoordinate, QString());
        tabRoute = new QWidget();
        tabRoute->setObjectName(QStringLiteral("tabRoute"));
        gridLayout_8 = new QGridLayout(tabRoute);
        gridLayout_8->setSpacing(3);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(3, 3, 3, 3);
        splitter = new QSplitter(tabRoute);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setFrameShape(QFrame::NoFrame);
        splitter->setFrameShadow(QFrame::Plain);
        splitter->setLineWidth(1);
        splitter->setMidLineWidth(1);
        splitter->setOrientation(Qt::Vertical);
        splitter->setOpaqueResize(true);
        splitter->setChildrenCollapsible(true);
        groupBox_2 = new QGroupBox(splitter);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_5 = new QGridLayout(groupBox_2);
        gridLayout_5->setSpacing(3);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pbAddRoute = new QToolButton(groupBox_2);
        pbAddRoute->setObjectName(QStringLiteral("pbAddRoute"));
        pbAddRoute->setMinimumSize(QSize(24, 24));
        pbAddRoute->setMaximumSize(QSize(24, 24));
        pbAddRoute->setIcon(icon);

        horizontalLayout_2->addWidget(pbAddRoute);

        pbDeleteRoute = new QToolButton(groupBox_2);
        pbDeleteRoute->setObjectName(QStringLiteral("pbDeleteRoute"));
        pbDeleteRoute->setMinimumSize(QSize(24, 24));
        pbDeleteRoute->setMaximumSize(QSize(24, 24));
        pbDeleteRoute->setIcon(icon1);

        horizontalLayout_2->addWidget(pbDeleteRoute);

        horizontalSpacer_7 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        pbSaveRoute = new QToolButton(groupBox_2);
        pbSaveRoute->setObjectName(QStringLiteral("pbSaveRoute"));
        pbSaveRoute->setMinimumSize(QSize(24, 24));
        pbSaveRoute->setMaximumSize(QSize(24, 24));
        pbSaveRoute->setIcon(icon2);

        horizontalLayout_2->addWidget(pbSaveRoute);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_2->addWidget(label_5);


        gridLayout_5->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_3->addWidget(label_6);

        leRouteName = new QLineEdit(groupBox_2);
        leRouteName->setObjectName(QStringLiteral("leRouteName"));

        horizontalLayout_3->addWidget(leRouteName);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_3->addWidget(label_9);

        leRouteComment = new QLineEdit(groupBox_2);
        leRouteComment->setObjectName(QStringLiteral("leRouteComment"));

        horizontalLayout_3->addWidget(leRouteComment);


        gridLayout_5->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        tvRoute = new QTableView(groupBox_2);
        tvRoute->setObjectName(QStringLiteral("tvRoute"));

        gridLayout_5->addWidget(tvRoute, 1, 0, 1, 1);

        splitter->addWidget(groupBox_2);
        groupBox_3 = new QGroupBox(splitter);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_7 = new QGridLayout(groupBox_3);
        gridLayout_7->setSpacing(3);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(1);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pbAddRouteCoordinate = new QToolButton(groupBox_3);
        pbAddRouteCoordinate->setObjectName(QStringLiteral("pbAddRouteCoordinate"));
        pbAddRouteCoordinate->setMinimumSize(QSize(24, 24));
        pbAddRouteCoordinate->setMaximumSize(QSize(24, 24));
        pbAddRouteCoordinate->setIcon(icon);

        horizontalLayout_4->addWidget(pbAddRouteCoordinate);

        pbDeleteRouteCoordinate = new QToolButton(groupBox_3);
        pbDeleteRouteCoordinate->setObjectName(QStringLiteral("pbDeleteRouteCoordinate"));
        pbDeleteRouteCoordinate->setMinimumSize(QSize(24, 24));
        pbDeleteRouteCoordinate->setMaximumSize(QSize(24, 24));
        pbDeleteRouteCoordinate->setIcon(icon1);

        horizontalLayout_4->addWidget(pbDeleteRouteCoordinate);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_4->addWidget(label_10);


        gridLayout_7->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        tvRouteCoordinate = new QTableView(groupBox_3);
        tvRouteCoordinate->setObjectName(QStringLiteral("tvRouteCoordinate"));

        gridLayout_7->addWidget(tvRouteCoordinate, 1, 0, 1, 2);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout_2->setHorizontalSpacing(3);
        formLayout_2->setVerticalSpacing(3);
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_11);

        leSequenceNumber = new QLineEdit(groupBox_3);
        leSequenceNumber->setObjectName(QStringLiteral("leSequenceNumber"));
        leSequenceNumber->setMinimumSize(QSize(50, 0));
        leSequenceNumber->setMaximumSize(QSize(100, 16777215));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, leSequenceNumber);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_13);

        ileRouteCoordinate = new RB_IdxLineEdit(groupBox_3);
        ileRouteCoordinate->setObjectName(QStringLiteral("ileRouteCoordinate"));
        ileRouteCoordinate->setMinimumSize(QSize(100, 0));
        ileRouteCoordinate->setMaximumSize(QSize(200, 16777215));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, ileRouteCoordinate);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_12);

        leRouteCoordinateComment = new QLineEdit(groupBox_3);
        leRouteCoordinateComment->setObjectName(QStringLiteral("leRouteCoordinateComment"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, leRouteCoordinateComment);


        gridLayout_7->addLayout(formLayout_2, 2, 0, 1, 1);

        gbRouteCalculation = new QGroupBox(groupBox_3);
        gbRouteCalculation->setObjectName(QStringLiteral("gbRouteCalculation"));
        sizePolicy1.setHeightForWidth(gbRouteCalculation->sizePolicy().hasHeightForWidth());
        gbRouteCalculation->setSizePolicy(sizePolicy1);
        gridLayout_6 = new QGridLayout(gbRouteCalculation);
        gridLayout_6->setSpacing(3);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(3, 3, 3, 3);
        label_22 = new QLabel(gbRouteCalculation);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_22, 0, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(3);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        leWindDirectionFrom = new QLineEdit(gbRouteCalculation);
        leWindDirectionFrom->setObjectName(QStringLiteral("leWindDirectionFrom"));
        leWindDirectionFrom->setMinimumSize(QSize(50, 0));
        leWindDirectionFrom->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(leWindDirectionFrom);

        pbCalcRoute = new QToolButton(gbRouteCalculation);
        pbCalcRoute->setObjectName(QStringLiteral("pbCalcRoute"));
        pbCalcRoute->setMinimumSize(QSize(24, 24));
        pbCalcRoute->setMaximumSize(QSize(24, 24));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/icons/calculator.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbCalcRoute->setIcon(icon5);

        horizontalLayout_6->addWidget(pbCalcRoute);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        gridLayout_6->addLayout(horizontalLayout_6, 0, 1, 1, 1);

        label_23 = new QLabel(gbRouteCalculation);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_23, 1, 0, 1, 1);

        leCalculatedDistance = new QLineEdit(gbRouteCalculation);
        leCalculatedDistance->setObjectName(QStringLiteral("leCalculatedDistance"));
        leCalculatedDistance->setMinimumSize(QSize(50, 0));
        leCalculatedDistance->setMaximumSize(QSize(100, 16777215));

        gridLayout_6->addWidget(leCalculatedDistance, 1, 1, 1, 1);

        label_24 = new QLabel(gbRouteCalculation);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_24, 2, 0, 1, 1);

        leCalculatedDuration = new QLineEdit(gbRouteCalculation);
        leCalculatedDuration->setObjectName(QStringLiteral("leCalculatedDuration"));
        leCalculatedDuration->setMinimumSize(QSize(50, 0));
        leCalculatedDuration->setMaximumSize(QSize(100, 16777215));

        gridLayout_6->addWidget(leCalculatedDuration, 2, 1, 1, 1);


        gridLayout_7->addWidget(gbRouteCalculation, 2, 1, 1, 1);

        splitter->addWidget(groupBox_3);

        gridLayout_8->addWidget(splitter, 0, 0, 1, 1);

        line = new QFrame(tabRoute);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_8->addWidget(line, 1, 0, 1, 1);

        tabWidget->addTab(tabRoute, QString());
        tabTrack = new QWidget();
        tabTrack->setObjectName(QStringLiteral("tabTrack"));
        gridLayout_12 = new QGridLayout(tabTrack);
        gridLayout_12->setSpacing(3);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        gridLayout_12->setContentsMargins(3, 3, 3, 3);
        groupBox_4 = new QGroupBox(tabTrack);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_9 = new QGridLayout(groupBox_4);
        gridLayout_9->setSpacing(3);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(1);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pbAddTrack = new QToolButton(groupBox_4);
        pbAddTrack->setObjectName(QStringLiteral("pbAddTrack"));
        pbAddTrack->setMinimumSize(QSize(24, 24));
        pbAddTrack->setMaximumSize(QSize(24, 24));
        pbAddTrack->setIcon(icon);

        horizontalLayout_5->addWidget(pbAddTrack);

        pbDeleteTrack = new QToolButton(groupBox_4);
        pbDeleteTrack->setObjectName(QStringLiteral("pbDeleteTrack"));
        pbDeleteTrack->setMinimumSize(QSize(24, 24));
        pbDeleteTrack->setMaximumSize(QSize(24, 24));
        pbDeleteTrack->setIcon(icon1);

        horizontalLayout_5->addWidget(pbDeleteTrack);

        horizontalSpacer_8 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        pbSaveTrack = new QToolButton(groupBox_4);
        pbSaveTrack->setObjectName(QStringLiteral("pbSaveTrack"));
        pbSaveTrack->setIcon(icon2);

        horizontalLayout_5->addWidget(pbSaveTrack);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);

        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_5->addWidget(label_14);


        gridLayout_9->addLayout(horizontalLayout_5, 0, 0, 1, 1);

        tvTrack = new QTableView(groupBox_4);
        tvTrack->setObjectName(QStringLiteral("tvTrack"));

        gridLayout_9->addWidget(tvTrack, 1, 0, 1, 1);

        gridLayout_11 = new QGridLayout();
        gridLayout_11->setSpacing(3);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        label_15 = new QLabel(groupBox_4);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_11->addWidget(label_15, 0, 0, 1, 1);

        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_11->addWidget(label_16, 0, 2, 1, 1);

        label_17 = new QLabel(groupBox_4);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_11->addWidget(label_17, 1, 0, 1, 1);

        label_18 = new QLabel(groupBox_4);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_11->addWidget(label_18, 1, 2, 1, 1);

        label_21 = new QLabel(groupBox_4);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_11->addWidget(label_21, 0, 4, 1, 1);

        leTrackName = new QLineEdit(groupBox_4);
        leTrackName->setObjectName(QStringLiteral("leTrackName"));

        gridLayout_11->addWidget(leTrackName, 0, 1, 1, 1);

        leTrackComment = new QLineEdit(groupBox_4);
        leTrackComment->setObjectName(QStringLiteral("leTrackComment"));

        gridLayout_11->addWidget(leTrackComment, 1, 1, 1, 1);

        ileTrackCoordinateFrom = new RB_IdxLineEdit(groupBox_4);
        ileTrackCoordinateFrom->setObjectName(QStringLiteral("ileTrackCoordinateFrom"));

        gridLayout_11->addWidget(ileTrackCoordinateFrom, 0, 3, 1, 1);

        leWeightFactor = new QLineEdit(groupBox_4);
        leWeightFactor->setObjectName(QStringLiteral("leWeightFactor"));

        gridLayout_11->addWidget(leWeightFactor, 0, 5, 1, 1);

        ileTrackCoordinateTo = new RB_IdxLineEdit(groupBox_4);
        ileTrackCoordinateTo->setObjectName(QStringLiteral("ileTrackCoordinateTo"));

        gridLayout_11->addWidget(ileTrackCoordinateTo, 1, 3, 1, 1);


        gridLayout_9->addLayout(gridLayout_11, 2, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_4, 0, 0, 1, 1);

        tabWidget->addTab(tabTrack, QString());

        gridLayout_10->addWidget(tabWidget, 0, 0, 1, 1);

        QWidget::setTabOrder(gv, tabWidget);
        QWidget::setTabOrder(tabWidget, tvCoordinate);
        QWidget::setTabOrder(tvCoordinate, pbAddCoordinate);
        QWidget::setTabOrder(pbAddCoordinate, pbDeleteCoordinate);
        QWidget::setTabOrder(pbDeleteCoordinate, pbSaveCoordinate);
        QWidget::setTabOrder(pbSaveCoordinate, leFilter);
        QWidget::setTabOrder(leFilter, pbFilterOn);
        QWidget::setTabOrder(pbFilterOn, pbFilterOff);
        QWidget::setTabOrder(pbFilterOff, leCoordinateName);
        QWidget::setTabOrder(leCoordinateName, leCoordinateComment);
        QWidget::setTabOrder(leCoordinateComment, ileSymbol);
        QWidget::setTabOrder(ileSymbol, leLatitude);
        QWidget::setTabOrder(leLatitude, leLongitude);
        QWidget::setTabOrder(leLongitude, leLatDecimal);
        QWidget::setTabOrder(leLatDecimal, leLongDecimal);
        QWidget::setTabOrder(leLongDecimal, tvRoute);
        QWidget::setTabOrder(tvRoute, pbAddRoute);
        QWidget::setTabOrder(pbAddRoute, pbDeleteRoute);
        QWidget::setTabOrder(pbDeleteRoute, pbSaveRoute);
        QWidget::setTabOrder(pbSaveRoute, leRouteName);
        QWidget::setTabOrder(leRouteName, leRouteComment);
        QWidget::setTabOrder(leRouteComment, tvRouteCoordinate);
        QWidget::setTabOrder(tvRouteCoordinate, pbAddRouteCoordinate);
        QWidget::setTabOrder(pbAddRouteCoordinate, pbDeleteRouteCoordinate);
        QWidget::setTabOrder(pbDeleteRouteCoordinate, leSequenceNumber);
        QWidget::setTabOrder(leSequenceNumber, ileRouteCoordinate);
        QWidget::setTabOrder(ileRouteCoordinate, leRouteCoordinateComment);
        QWidget::setTabOrder(leRouteCoordinateComment, leWindDirectionFrom);
        QWidget::setTabOrder(leWindDirectionFrom, pbCalcRoute);
        QWidget::setTabOrder(pbCalcRoute, leCalculatedDistance);
        QWidget::setTabOrder(leCalculatedDistance, leCalculatedDuration);
        QWidget::setTabOrder(leCalculatedDuration, tvTrack);
        QWidget::setTabOrder(tvTrack, pbAddTrack);
        QWidget::setTabOrder(pbAddTrack, pbDeleteTrack);
        QWidget::setTabOrder(pbDeleteTrack, pbSaveTrack);
        QWidget::setTabOrder(pbSaveTrack, leTrackName);
        QWidget::setTabOrder(leTrackName, leTrackComment);
        QWidget::setTabOrder(leTrackComment, ileTrackCoordinateFrom);
        QWidget::setTabOrder(ileTrackCoordinateFrom, ileTrackCoordinateTo);
        QWidget::setTabOrder(ileTrackCoordinateTo, leWeightFactor);

        retranslateUi(SAIL_MapNavigationWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SAIL_MapNavigationWidget);
    } // setupUi

    void retranslateUi(RB_Widget *SAIL_MapNavigationWidget)
    {
        SAIL_MapNavigationWidget->setWindowTitle(QApplication::translate("SAIL_MapNavigationWidget", "Form", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabNavigation), QApplication::translate("SAIL_MapNavigationWidget", "Navigation", 0));
        gbCoordinate->setTitle(QApplication::translate("SAIL_MapNavigationWidget", "Coordinate List", 0));
        pbAddCoordinate->setText(QString());
        pbDeleteCoordinate->setText(QString());
        pbSaveCoordinate->setText(QString());
        leFilter->setPlaceholderText(QApplication::translate("SAIL_MapNavigationWidget", "Filter", 0));
        pbFilterOn->setText(QString());
        pbFilterOff->setText(QString());
        label->setText(QApplication::translate("SAIL_MapNavigationWidget", "Click to select or [F2] to edit", 0));
        label_2->setText(QApplication::translate("SAIL_MapNavigationWidget", "Name", 0));
        label_3->setText(QApplication::translate("SAIL_MapNavigationWidget", "Comment", 0));
        label_4->setText(QApplication::translate("SAIL_MapNavigationWidget", "Symbol", 0));
        groupBox->setTitle(QApplication::translate("SAIL_MapNavigationWidget", "Coordinate", 0));
        label_7->setText(QApplication::translate("SAIL_MapNavigationWidget", "Latitude", 0));
        leLatitude->setInputMask(QString());
        leLatitude->setText(QString());
        leLatitude->setPlaceholderText(QApplication::translate("SAIL_MapNavigationWidget", "00\302\26000.0000N", 0));
        label_8->setText(QApplication::translate("SAIL_MapNavigationWidget", "Longitude", 0));
        leLongitude->setInputMask(QString());
        leLongitude->setText(QString());
        leLongitude->setPlaceholderText(QApplication::translate("SAIL_MapNavigationWidget", "000\302\26000.0000E", 0));
        leLatDecimal->setInputMask(QString());
        leLatDecimal->setText(QString());
        leLatDecimal->setPlaceholderText(QApplication::translate("SAIL_MapNavigationWidget", "+0.0", 0));
        leLongDecimal->setInputMask(QString());
        leLongDecimal->setText(QString());
        leLongDecimal->setPlaceholderText(QApplication::translate("SAIL_MapNavigationWidget", "+0.0", 0));
        label_19->setText(QApplication::translate("SAIL_MapNavigationWidget", "(in decimal)", 0));
        label_20->setText(QApplication::translate("SAIL_MapNavigationWidget", "(in decimal)", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabCoordinate), QApplication::translate("SAIL_MapNavigationWidget", "Coordinate", 0));
        groupBox_2->setTitle(QApplication::translate("SAIL_MapNavigationWidget", "Route List", 0));
        pbAddRoute->setText(QString());
        pbDeleteRoute->setText(QString());
        pbSaveRoute->setText(QString());
        label_5->setText(QApplication::translate("SAIL_MapNavigationWidget", "Click to select or [F2] to edit", 0));
        label_6->setText(QApplication::translate("SAIL_MapNavigationWidget", "Route Name", 0));
        label_9->setText(QApplication::translate("SAIL_MapNavigationWidget", "Comment", 0));
        groupBox_3->setTitle(QApplication::translate("SAIL_MapNavigationWidget", "Coordinate List", 0));
        pbAddRouteCoordinate->setText(QString());
        pbDeleteRouteCoordinate->setText(QString());
        label_10->setText(QApplication::translate("SAIL_MapNavigationWidget", "Click to select or [F2] to edit", 0));
        label_11->setText(QApplication::translate("SAIL_MapNavigationWidget", "Sequence No", 0));
        label_13->setText(QApplication::translate("SAIL_MapNavigationWidget", "Coordinate", 0));
        label_12->setText(QApplication::translate("SAIL_MapNavigationWidget", "Comment", 0));
        gbRouteCalculation->setTitle(QApplication::translate("SAIL_MapNavigationWidget", "Calculation (Select coordinates for detail route calculation)", 0));
        label_22->setText(QApplication::translate("SAIL_MapNavigationWidget", "Wind Direction From", 0));
#ifndef QT_NO_TOOLTIP
        pbCalcRoute->setToolTip(QApplication::translate("SAIL_MapNavigationWidget", "Calculate distance and duration", 0));
#endif // QT_NO_TOOLTIP
        pbCalcRoute->setText(QString());
        label_23->setText(QApplication::translate("SAIL_MapNavigationWidget", "Calculated Distance", 0));
        label_24->setText(QApplication::translate("SAIL_MapNavigationWidget", "Calculated Duration", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabRoute), QApplication::translate("SAIL_MapNavigationWidget", "Route", 0));
        groupBox_4->setTitle(QApplication::translate("SAIL_MapNavigationWidget", "Track List", 0));
        pbAddTrack->setText(QString());
        pbDeleteTrack->setText(QString());
        pbSaveTrack->setText(QString());
        label_14->setText(QApplication::translate("SAIL_MapNavigationWidget", "Click to select or [F2] to edit", 0));
        label_15->setText(QApplication::translate("SAIL_MapNavigationWidget", "Track Name", 0));
        label_16->setText(QApplication::translate("SAIL_MapNavigationWidget", "From", 0));
        label_17->setText(QApplication::translate("SAIL_MapNavigationWidget", "Comment", 0));
        label_18->setText(QApplication::translate("SAIL_MapNavigationWidget", "To", 0));
        label_21->setText(QApplication::translate("SAIL_MapNavigationWidget", "Weight Factor", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabTrack), QApplication::translate("SAIL_MapNavigationWidget", "Track", 0));
    } // retranslateUi

};

namespace Ui {
    class SAIL_MapNavigationWidget: public Ui_SAIL_MapNavigationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAIL_MAPNAVIGATIONWIDGET_H
