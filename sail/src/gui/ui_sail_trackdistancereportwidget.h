/********************************************************************************
** Form generated from reading UI file 'sail_trackdistancereportwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAIL_TRACKDISTANCEREPORTWIDGET_H
#define UI_SAIL_TRACKDISTANCEREPORTWIDGET_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SAIL_TrackDistanceReportWidget
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblWindFrom;
    QLineEdit *leWindFrom;
    QToolButton *pbRefresh;
    QSpacerItem *horizontalSpacer_2;
    QFrame *frame;
    QGridLayout *gridLayout;
    QWebView *wvReport;

    void setupUi(QWidget *SAIL_TrackDistanceReportWidget)
    {
        if (SAIL_TrackDistanceReportWidget->objectName().isEmpty())
            SAIL_TrackDistanceReportWidget->setObjectName(QStringLiteral("SAIL_TrackDistanceReportWidget"));
        SAIL_TrackDistanceReportWidget->resize(680, 500);
        gridLayout_2 = new QGridLayout(SAIL_TrackDistanceReportWidget);
        gridLayout_2->setSpacing(3);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lblWindFrom = new QLabel(SAIL_TrackDistanceReportWidget);
        lblWindFrom->setObjectName(QStringLiteral("lblWindFrom"));

        horizontalLayout_2->addWidget(lblWindFrom);

        leWindFrom = new QLineEdit(SAIL_TrackDistanceReportWidget);
        leWindFrom->setObjectName(QStringLiteral("leWindFrom"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leWindFrom->sizePolicy().hasHeightForWidth());
        leWindFrom->setSizePolicy(sizePolicy);
        leWindFrom->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(leWindFrom);

        pbRefresh = new QToolButton(SAIL_TrackDistanceReportWidget);
        pbRefresh->setObjectName(QStringLiteral("pbRefresh"));
        pbRefresh->setMinimumSize(QSize(24, 24));
        pbRefresh->setMaximumSize(QSize(24, 24));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icons/arrow_refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbRefresh->setIcon(icon);

        horizontalLayout_2->addWidget(pbRefresh);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        frame = new QFrame(SAIL_TrackDistanceReportWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(1, 1, 1, 1);
        wvReport = new QWebView(frame);
        wvReport->setObjectName(QStringLiteral("wvReport"));
        wvReport->setUrl(QUrl(QStringLiteral("about:blank")));

        gridLayout->addWidget(wvReport, 0, 0, 1, 1);


        gridLayout_2->addWidget(frame, 1, 0, 1, 1);


        retranslateUi(SAIL_TrackDistanceReportWidget);

        QMetaObject::connectSlotsByName(SAIL_TrackDistanceReportWidget);
    } // setupUi

    void retranslateUi(QWidget *SAIL_TrackDistanceReportWidget)
    {
        SAIL_TrackDistanceReportWidget->setWindowTitle(QApplication::translate("SAIL_TrackDistanceReportWidget", "Dialog", 0));
        lblWindFrom->setText(QApplication::translate("SAIL_TrackDistanceReportWidget", "Wind From [degree]", 0));
        leWindFrom->setPlaceholderText(QApplication::translate("SAIL_TrackDistanceReportWidget", "0.00", 0));
#ifndef QT_NO_TOOLTIP
        pbRefresh->setToolTip(QApplication::translate("SAIL_TrackDistanceReportWidget", "Refresh report", 0));
#endif // QT_NO_TOOLTIP
        pbRefresh->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SAIL_TrackDistanceReportWidget: public Ui_SAIL_TrackDistanceReportWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAIL_TRACKDISTANCEREPORTWIDGET_H
