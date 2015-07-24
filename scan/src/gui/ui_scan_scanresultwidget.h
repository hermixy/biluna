/********************************************************************************
** Form generated from reading UI file 'scan_scanresultwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCAN_SCANRESULTWIDGET_H
#define UI_SCAN_SCANRESULTWIDGET_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCAN_ScanResultWidget
{
public:
    QGridLayout *gridLayout_8;
    QSplitter *splHorizontal;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pbAddScan;
    QPushButton *pbDeleteScan;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbSaveScan;
    QPushButton *pbRevertScan;
    QTableView *tvScan;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_6;
    QLineEdit *leResultName;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_7;
    QComboBox *cbScanType;
    QPushButton *pbSelectScanType;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_8;
    QComboBox *cbPerson;
    QPushButton *pbSelectPerson;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QGridLayout *gridLayout_4;
    QSplitter *splVertical;
    QGroupBox *gbQuestions;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pbAddQuestion;
    QPushButton *pbDeleteQuestion;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QTableView *tvQuestion;
    QWidget *layoutWidget_1;
    QVBoxLayout *verticalLayout;
    QFrame *line;
    QGroupBox *gbAnswers;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_2;
    QTableView *tvAnswer;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *leAnswerNumber;
    QLabel *label_5;
    QLineEdit *leValue;
    QSpacerItem *horizontalSpacer_7;
    QWidget *tab2;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbRefreshScore;
    QSpacerItem *horizontalSpacer_5;
    QWebView *wvScoreResult;
    QWidget *tab3;
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pbRefreshReport;
    QSpacerItem *horizontalSpacer_6;
    QWebView *wvReport;

    void setupUi(QWidget *SCAN_ScanResultWidget)
    {
        if (SCAN_ScanResultWidget->objectName().isEmpty())
            SCAN_ScanResultWidget->setObjectName(QStringLiteral("SCAN_ScanResultWidget"));
        SCAN_ScanResultWidget->resize(744, 520);
        gridLayout_8 = new QGridLayout(SCAN_ScanResultWidget);
        gridLayout_8->setSpacing(1);
        gridLayout_8->setContentsMargins(1, 1, 1, 1);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        splHorizontal = new QSplitter(SCAN_ScanResultWidget);
        splHorizontal->setObjectName(QStringLiteral("splHorizontal"));
        splHorizontal->setOrientation(Qt::Horizontal);
        groupBox = new QGroupBox(splHorizontal);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(3);
        gridLayout->setVerticalSpacing(1);
        gridLayout->setContentsMargins(3, 1, 3, 3);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pbAddScan = new QPushButton(groupBox);
        pbAddScan->setObjectName(QStringLiteral("pbAddScan"));
        pbAddScan->setMinimumSize(QSize(24, 24));
        pbAddScan->setMaximumSize(QSize(24, 24));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAddScan->setIcon(icon);
        pbAddScan->setIconSize(QSize(16, 16));

        horizontalLayout_2->addWidget(pbAddScan);

        pbDeleteScan = new QPushButton(groupBox);
        pbDeleteScan->setObjectName(QStringLiteral("pbDeleteScan"));
        pbDeleteScan->setMinimumSize(QSize(24, 24));
        pbDeleteScan->setMaximumSize(QSize(24, 24));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/icons/delete.png"), QSize(), QIcon::Normal, QIcon::On);
        pbDeleteScan->setIcon(icon1);
        pbDeleteScan->setIconSize(QSize(16, 16));

        horizontalLayout_2->addWidget(pbDeleteScan);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pbSaveScan = new QPushButton(groupBox);
        pbSaveScan->setObjectName(QStringLiteral("pbSaveScan"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/icons/database_save.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbSaveScan->setIcon(icon2);

        horizontalLayout_2->addWidget(pbSaveScan);

        pbRevertScan = new QPushButton(groupBox);
        pbRevertScan->setObjectName(QStringLiteral("pbRevertScan"));
        pbRevertScan->setMinimumSize(QSize(24, 24));
        pbRevertScan->setMaximumSize(QSize(24, 24));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/icons/arrow_undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbRevertScan->setIcon(icon3);

        horizontalLayout_2->addWidget(pbRevertScan);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 2);

        tvScan = new QTableView(groupBox);
        tvScan->setObjectName(QStringLiteral("tvScan"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(tvScan->sizePolicy().hasHeightForWidth());
        tvScan->setSizePolicy(sizePolicy1);
        tvScan->setMinimumSize(QSize(0, 0));
        tvScan->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(tvScan, 1, 0, 1, 2);

        splHorizontal->addWidget(groupBox);
        layoutWidget = new QWidget(splHorizontal);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        gridLayout_7 = new QGridLayout(layoutWidget);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setVerticalSpacing(1);
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_9->addWidget(label_6);

        leResultName = new QLineEdit(layoutWidget);
        leResultName->setObjectName(QStringLiteral("leResultName"));

        horizontalLayout_9->addWidget(leResultName);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_9->addWidget(label_7);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(3);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        cbScanType = new QComboBox(layoutWidget);
        cbScanType->setObjectName(QStringLiteral("cbScanType"));

        horizontalLayout_7->addWidget(cbScanType);

        pbSelectScanType = new QPushButton(layoutWidget);
        pbSelectScanType->setObjectName(QStringLiteral("pbSelectScanType"));
        pbSelectScanType->setMinimumSize(QSize(24, 24));
        pbSelectScanType->setMaximumSize(QSize(24, 24));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/icons/wrench.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbSelectScanType->setIcon(icon4);

        horizontalLayout_7->addWidget(pbSelectScanType);


        horizontalLayout_9->addLayout(horizontalLayout_7);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_9->addWidget(label_8);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(3);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        cbPerson = new QComboBox(layoutWidget);
        cbPerson->setObjectName(QStringLiteral("cbPerson"));

        horizontalLayout_8->addWidget(cbPerson);

        pbSelectPerson = new QPushButton(layoutWidget);
        pbSelectPerson->setObjectName(QStringLiteral("pbSelectPerson"));
        pbSelectPerson->setMinimumSize(QSize(24, 24));
        pbSelectPerson->setMaximumSize(QSize(24, 24));
        pbSelectPerson->setIcon(icon4);

        horizontalLayout_8->addWidget(pbSelectPerson);


        horizontalLayout_9->addLayout(horizontalLayout_8);


        gridLayout_7->addLayout(horizontalLayout_9, 0, 0, 1, 1);

        tabWidget = new QTabWidget(layoutWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab1 = new QWidget();
        tab1->setObjectName(QStringLiteral("tab1"));
        gridLayout_4 = new QGridLayout(tab1);
        gridLayout_4->setSpacing(1);
        gridLayout_4->setContentsMargins(3, 3, 3, 3);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        splVertical = new QSplitter(tab1);
        splVertical->setObjectName(QStringLiteral("splVertical"));
        splVertical->setFrameShape(QFrame::NoFrame);
        splVertical->setFrameShadow(QFrame::Plain);
        splVertical->setLineWidth(1);
        splVertical->setMidLineWidth(0);
        splVertical->setOrientation(Qt::Vertical);
        gbQuestions = new QGroupBox(splVertical);
        gbQuestions->setObjectName(QStringLiteral("gbQuestions"));
        gridLayout_2 = new QGridLayout(gbQuestions);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(3);
        gridLayout_2->setVerticalSpacing(1);
        gridLayout_2->setContentsMargins(3, 1, 3, 3);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(1);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pbAddQuestion = new QPushButton(gbQuestions);
        pbAddQuestion->setObjectName(QStringLiteral("pbAddQuestion"));
        pbAddQuestion->setMinimumSize(QSize(24, 24));
        pbAddQuestion->setMaximumSize(QSize(24, 24));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/icons/paintcan.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAddQuestion->setIcon(icon5);
        pbAddQuestion->setIconSize(QSize(16, 16));

        horizontalLayout_3->addWidget(pbAddQuestion);

        pbDeleteQuestion = new QPushButton(gbQuestions);
        pbDeleteQuestion->setObjectName(QStringLiteral("pbDeleteQuestion"));
        pbDeleteQuestion->setMinimumSize(QSize(24, 24));
        pbDeleteQuestion->setMaximumSize(QSize(24, 24));
        pbDeleteQuestion->setIcon(icon1);
        pbDeleteQuestion->setIconSize(QSize(16, 16));

        horizontalLayout_3->addWidget(pbDeleteQuestion);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        label = new QLabel(gbQuestions);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);


        gridLayout_2->addLayout(horizontalLayout_3, 0, 0, 1, 2);

        tvQuestion = new QTableView(gbQuestions);
        tvQuestion->setObjectName(QStringLiteral("tvQuestion"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(4);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(tvQuestion->sizePolicy().hasHeightForWidth());
        tvQuestion->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(tvQuestion, 1, 0, 1, 2);

        gridLayout_2->setColumnStretch(0, 1);
        splVertical->addWidget(gbQuestions);
        layoutWidget_1 = new QWidget(splVertical);
        layoutWidget_1->setObjectName(QStringLiteral("layoutWidget_1"));
        verticalLayout = new QVBoxLayout(layoutWidget_1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        line = new QFrame(layoutWidget_1);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        gbAnswers = new QGroupBox(layoutWidget_1);
        gbAnswers->setObjectName(QStringLiteral("gbAnswers"));
        gridLayout_3 = new QGridLayout(gbAnswers);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setHorizontalSpacing(3);
        gridLayout_3->setVerticalSpacing(1);
        gridLayout_3->setContentsMargins(3, 1, 3, 3);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(1);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        label_2 = new QLabel(gbAnswers);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);


        gridLayout_3->addLayout(horizontalLayout_4, 0, 0, 1, 2);

        tvAnswer = new QTableView(gbAnswers);
        tvAnswer->setObjectName(QStringLiteral("tvAnswer"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(4);
        sizePolicy3.setHeightForWidth(tvAnswer->sizePolicy().hasHeightForWidth());
        tvAnswer->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(tvAnswer, 1, 0, 1, 2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_3 = new QLabel(gbAnswers);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_5->addWidget(label_3);

        leAnswerNumber = new QLineEdit(gbAnswers);
        leAnswerNumber->setObjectName(QStringLiteral("leAnswerNumber"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(leAnswerNumber->sizePolicy().hasHeightForWidth());
        leAnswerNumber->setSizePolicy(sizePolicy4);
        leAnswerNumber->setMinimumSize(QSize(75, 0));
        leAnswerNumber->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_5->addWidget(leAnswerNumber);

        label_5 = new QLabel(gbAnswers);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        leValue = new QLineEdit(gbAnswers);
        leValue->setObjectName(QStringLiteral("leValue"));
        leValue->setMinimumSize(QSize(75, 0));
        leValue->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_5->addWidget(leValue);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);


        gridLayout_3->addLayout(horizontalLayout_5, 2, 1, 1, 1);


        verticalLayout->addWidget(gbAnswers);

        splVertical->addWidget(layoutWidget_1);

        gridLayout_4->addWidget(splVertical, 0, 0, 1, 1);

        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QStringLiteral("tab2"));
        gridLayout_5 = new QGridLayout(tab2);
        gridLayout_5->setContentsMargins(3, 3, 3, 3);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setHorizontalSpacing(3);
        gridLayout_5->setVerticalSpacing(1);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pbRefreshScore = new QPushButton(tab2);
        pbRefreshScore->setObjectName(QStringLiteral("pbRefreshScore"));
        pbRefreshScore->setMinimumSize(QSize(24, 24));
        pbRefreshScore->setMaximumSize(QSize(24, 24));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/icons/arrow_refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbRefreshScore->setIcon(icon6);

        horizontalLayout->addWidget(pbRefreshScore);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);


        gridLayout_5->addLayout(horizontalLayout, 0, 0, 1, 1);

        wvScoreResult = new QWebView(tab2);
        wvScoreResult->setObjectName(QStringLiteral("wvScoreResult"));
        wvScoreResult->setUrl(QUrl(QStringLiteral("about:blank")));

        gridLayout_5->addWidget(wvScoreResult, 1, 0, 1, 1);

        tabWidget->addTab(tab2, QString());
        tab3 = new QWidget();
        tab3->setObjectName(QStringLiteral("tab3"));
        gridLayout_6 = new QGridLayout(tab3);
        gridLayout_6->setSpacing(1);
        gridLayout_6->setContentsMargins(3, 3, 3, 3);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        pbRefreshReport = new QPushButton(tab3);
        pbRefreshReport->setObjectName(QStringLiteral("pbRefreshReport"));
        pbRefreshReport->setMinimumSize(QSize(24, 24));
        pbRefreshReport->setMaximumSize(QSize(24, 24));
        pbRefreshReport->setIcon(icon6);

        horizontalLayout_6->addWidget(pbRefreshReport);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        gridLayout_6->addLayout(horizontalLayout_6, 0, 0, 1, 1);

        wvReport = new QWebView(tab3);
        wvReport->setObjectName(QStringLiteral("wvReport"));
        wvReport->setUrl(QUrl(QStringLiteral("about:blank")));

        gridLayout_6->addWidget(wvReport, 1, 0, 1, 1);

        tabWidget->addTab(tab3, QString());

        gridLayout_7->addWidget(tabWidget, 1, 0, 1, 1);

        splHorizontal->addWidget(layoutWidget);

        gridLayout_8->addWidget(splHorizontal, 0, 0, 1, 1);


        retranslateUi(SCAN_ScanResultWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SCAN_ScanResultWidget);
    } // setupUi

    void retranslateUi(QWidget *SCAN_ScanResultWidget)
    {
        SCAN_ScanResultWidget->setWindowTitle(QApplication::translate("SCAN_ScanResultWidget", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("SCAN_ScanResultWidget", "Result List", 0));
#ifndef QT_NO_TOOLTIP
        pbAddScan->setToolTip(QApplication::translate("SCAN_ScanResultWidget", "Add scan", 0));
#endif // QT_NO_TOOLTIP
        pbAddScan->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbDeleteScan->setToolTip(QApplication::translate("SCAN_ScanResultWidget", "Delete scan", 0));
#endif // QT_NO_TOOLTIP
        pbDeleteScan->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbSaveScan->setToolTip(QApplication::translate("SCAN_ScanResultWidget", "Save scan", 0));
#endif // QT_NO_TOOLTIP
        pbSaveScan->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbRevertScan->setToolTip(QApplication::translate("SCAN_ScanResultWidget", "Undo current changes", 0));
#endif // QT_NO_TOOLTIP
        pbRevertScan->setText(QString());
        label_6->setText(QApplication::translate("SCAN_ScanResultWidget", "Result Name", 0));
        label_7->setText(QApplication::translate("SCAN_ScanResultWidget", "Scan Type", 0));
        pbSelectScanType->setText(QString());
        label_8->setText(QApplication::translate("SCAN_ScanResultWidget", "Person", 0));
        pbSelectPerson->setText(QString());
        gbQuestions->setTitle(QApplication::translate("SCAN_ScanResultWidget", "Questions", 0));
#ifndef QT_NO_TOOLTIP
        pbAddQuestion->setToolTip(QApplication::translate("SCAN_ScanResultWidget", "Add questions", 0));
#endif // QT_NO_TOOLTIP
        pbAddQuestion->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbDeleteQuestion->setToolTip(QApplication::translate("SCAN_ScanResultWidget", "Delete questions", 0));
#endif // QT_NO_TOOLTIP
        pbDeleteQuestion->setText(QString());
        label->setText(QApplication::translate("SCAN_ScanResultWidget", "Click row to select", 0));
        gbAnswers->setTitle(QApplication::translate("SCAN_ScanResultWidget", "Answers", 0));
        label_2->setText(QApplication::translate("SCAN_ScanResultWidget", "Click row to select", 0));
#ifndef QT_NO_TOOLTIP
        label_3->setToolTip(QApplication::translate("SCAN_ScanResultWidget", "Sequence Number", 0));
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("SCAN_ScanResultWidget", "Number", 0));
        label_5->setText(QApplication::translate("SCAN_ScanResultWidget", "Value", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("SCAN_ScanResultWidget", "Answer Result", 0));
#ifndef QT_NO_TOOLTIP
        pbRefreshScore->setToolTip(QApplication::translate("SCAN_ScanResultWidget", "Refresh score result", 0));
#endif // QT_NO_TOOLTIP
        pbRefreshScore->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("SCAN_ScanResultWidget", "Score Result", 0));
#ifndef QT_NO_TOOLTIP
        pbRefreshReport->setToolTip(QApplication::translate("SCAN_ScanResultWidget", "Refresh report", 0));
#endif // QT_NO_TOOLTIP
        pbRefreshReport->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab3), QApplication::translate("SCAN_ScanResultWidget", "Report", 0));
    } // retranslateUi

};

namespace Ui {
    class SCAN_ScanResultWidget: public Ui_SCAN_ScanResultWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCAN_SCANRESULTWIDGET_H
