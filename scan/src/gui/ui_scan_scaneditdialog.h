/********************************************************************************
** Form generated from reading UI file 'scan_scaneditdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCAN_SCANEDITDIALOG_H
#define UI_SCAN_SCANEDITDIALOG_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCAN_ScanEditDialog
{
public:
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pbHelp;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pbOk;
    QPushButton *pbCancel;
    QSplitter *splitter;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *pbAddScan;
    QToolButton *pbDeleteScan;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *pbSaveScan;
    QToolButton *pbRevertScan;
    QTableView *tvScan;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_4;
    QSplitter *splVertical;
    QGroupBox *gbQuestions;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *pbAddQuestion;
    QToolButton *pbDeleteQuestion;
    QSpacerItem *horizontalSpacer_3;
    QTableView *tvQuestion;
    QHBoxLayout *horizontalLayout;
    QLineEdit *leQuestionNumber;
    QLabel *label_5;
    QComboBox *cbQuestionType;
    QCheckBox *chbActive;
    QPlainTextEdit *teQuestionText;
    QLabel *label;
    QLabel *label_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QFrame *line;
    QGroupBox *gbAnswers;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *pbAddAnswer;
    QToolButton *pbDeleteAnswer;
    QSpacerItem *horizontalSpacer_4;
    QTableView *tvAnswer;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *leAnswerNumber;
    QSpacerItem *horizontalSpacer;
    QLabel *label_4;
    QPlainTextEdit *teAnswerText;
    QWidget *tab_2;
    QGridLayout *gridLayout_8;
    QSplitter *splitter_2;
    QGroupBox *gbTextBlock;
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout_7;
    QToolButton *pbAddBlock;
    QToolButton *pbAddSubBlock;
    QToolButton *pbDeleteBlock;
    QToolButton *pbPromoteBlock;
    QToolButton *pbDemoteBlock;
    QSpacerItem *horizontalSpacer_6;
    QTreeView *trvBlock;
    QWidget *layoutWidget_1;
    QGridLayout *gridLayout_7;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_6;
    QLineEdit *leBlockNumber;
    QSpacerItem *horizontalSpacer_7;
    QToolButton *pbAlignLeft;
    QToolButton *pbAlignRight;
    QToolButton *pbAlignCenter;
    QToolButton *pbAlignJustify;
    QToolButton *pbBold;
    QToolButton *pbItalic;
    QToolButton *pbUnderline;
    QToolButton *pbStrikethrough;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_8;
    QToolButton *pbIndent;
    QToolButton *pbOutdent;
    QToolButton *pbNumberList;
    QToolButton *pbBulletList;
    QToolButton *pbFontName;
    QToolButton *pbFontSize;
    QToolButton *pbColor;
    QToolButton *pbBgColor;
    QTabWidget *twBlock;
    QWidget *gvBloc;
    QGridLayout *gridLayout_9;
    QWebView *wvBlock;
    QWidget *tab_6;
    QGridLayout *gridLayout_10;
    QPlainTextEdit *teBlock;
    QLabel *label_9;
    QComboBox *cbContentType;
    QSpacerItem *horizontalSpacer_12;
    QLabel *label_8;
    QComboBox *cbValidity1;
    QLabel *label_12;
    QLineEdit *leVal1Min;
    QLabel *label_13;
    QLineEdit *leVal1Max;
    QComboBox *cbVal1AndOr;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_14;
    QComboBox *cbValidity2;
    QLabel *label_15;
    QLineEdit *leVal2Min;
    QLabel *label_16;
    QLineEdit *leVal2Max;
    QComboBox *cbVal2AndOr;
    QSpacerItem *horizontalSpacer_10;
    QLabel *label_17;
    QComboBox *cbValidity3;
    QLabel *label_18;
    QLineEdit *leVal3Min;
    QLabel *label_19;
    QLineEdit *leVal3Max;
    QSpacerItem *horizontalSpacer_11;

    void setupUi(QDialog *SCAN_ScanEditDialog)
    {
        if (SCAN_ScanEditDialog->objectName().isEmpty())
            SCAN_ScanEditDialog->setObjectName(QStringLiteral("SCAN_ScanEditDialog"));
        SCAN_ScanEditDialog->resize(759, 522);
        gridLayout_5 = new QGridLayout(SCAN_ScanEditDialog);
        gridLayout_5->setSpacing(1);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(3);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        pbHelp = new QPushButton(SCAN_ScanEditDialog);
        pbHelp->setObjectName(QStringLiteral("pbHelp"));
        pbHelp->setAutoDefault(false);

        horizontalLayout_6->addWidget(pbHelp);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        pbOk = new QPushButton(SCAN_ScanEditDialog);
        pbOk->setObjectName(QStringLiteral("pbOk"));

        horizontalLayout_6->addWidget(pbOk);

        pbCancel = new QPushButton(SCAN_ScanEditDialog);
        pbCancel->setObjectName(QStringLiteral("pbCancel"));

        horizontalLayout_6->addWidget(pbCancel);


        gridLayout_5->addLayout(horizontalLayout_6, 1, 0, 1, 1);

        splitter = new QSplitter(SCAN_ScanEditDialog);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        groupBox = new QGroupBox(splitter);
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
        pbAddScan = new QToolButton(groupBox);
        pbAddScan->setObjectName(QStringLiteral("pbAddScan"));
        pbAddScan->setMinimumSize(QSize(24, 24));
        pbAddScan->setMaximumSize(QSize(24, 24));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAddScan->setIcon(icon);
        pbAddScan->setIconSize(QSize(16, 16));

        horizontalLayout_2->addWidget(pbAddScan);

        pbDeleteScan = new QToolButton(groupBox);
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

        pbSaveScan = new QToolButton(groupBox);
        pbSaveScan->setObjectName(QStringLiteral("pbSaveScan"));
        pbSaveScan->setMinimumSize(QSize(24, 24));
        pbSaveScan->setMaximumSize(QSize(24, 24));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/icons/database_save.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbSaveScan->setIcon(icon2);

        horizontalLayout_2->addWidget(pbSaveScan);

        pbRevertScan = new QToolButton(groupBox);
        pbRevertScan->setObjectName(QStringLiteral("pbRevertScan"));
        pbRevertScan->setMinimumSize(QSize(24, 24));
        pbRevertScan->setMaximumSize(QSize(24, 24));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/icons/arrow_undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbRevertScan->setIcon(icon3);

        horizontalLayout_2->addWidget(pbRevertScan);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        tvScan = new QTableView(groupBox);
        tvScan->setObjectName(QStringLiteral("tvScan"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(tvScan->sizePolicy().hasHeightForWidth());
        tvScan->setSizePolicy(sizePolicy1);
        tvScan->setMinimumSize(QSize(0, 0));
        tvScan->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(tvScan, 1, 0, 1, 1);

        splitter->addWidget(groupBox);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_4 = new QGridLayout(tab);
        gridLayout_4->setSpacing(1);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(3, 3, 3, 3);
        splVertical = new QSplitter(tab);
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
        pbAddQuestion = new QToolButton(gbQuestions);
        pbAddQuestion->setObjectName(QStringLiteral("pbAddQuestion"));
        pbAddQuestion->setMinimumSize(QSize(24, 24));
        pbAddQuestion->setMaximumSize(QSize(24, 24));
        pbAddQuestion->setIcon(icon);
        pbAddQuestion->setIconSize(QSize(16, 16));

        horizontalLayout_3->addWidget(pbAddQuestion);

        pbDeleteQuestion = new QToolButton(gbQuestions);
        pbDeleteQuestion->setObjectName(QStringLiteral("pbDeleteQuestion"));
        pbDeleteQuestion->setMinimumSize(QSize(24, 24));
        pbDeleteQuestion->setMaximumSize(QSize(24, 24));
        pbDeleteQuestion->setIcon(icon1);
        pbDeleteQuestion->setIconSize(QSize(16, 16));

        horizontalLayout_3->addWidget(pbDeleteQuestion);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        gridLayout_2->addLayout(horizontalLayout_3, 0, 0, 1, 3);

        tvQuestion = new QTableView(gbQuestions);
        tvQuestion->setObjectName(QStringLiteral("tvQuestion"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(4);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(tvQuestion->sizePolicy().hasHeightForWidth());
        tvQuestion->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(tvQuestion, 1, 0, 1, 3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(9);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        leQuestionNumber = new QLineEdit(gbQuestions);
        leQuestionNumber->setObjectName(QStringLiteral("leQuestionNumber"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(leQuestionNumber->sizePolicy().hasHeightForWidth());
        leQuestionNumber->setSizePolicy(sizePolicy3);
        leQuestionNumber->setMinimumSize(QSize(75, 0));
        leQuestionNumber->setMaximumSize(QSize(75, 16777215));
        leQuestionNumber->setSizeIncrement(QSize(75, 0));

        horizontalLayout->addWidget(leQuestionNumber);

        label_5 = new QLabel(gbQuestions);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);

        cbQuestionType = new QComboBox(gbQuestions);
        cbQuestionType->setObjectName(QStringLiteral("cbQuestionType"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(cbQuestionType->sizePolicy().hasHeightForWidth());
        cbQuestionType->setSizePolicy(sizePolicy4);
        cbQuestionType->setSizeIncrement(QSize(0, 0));

        horizontalLayout->addWidget(cbQuestionType);

        chbActive = new QCheckBox(gbQuestions);
        chbActive->setObjectName(QStringLiteral("chbActive"));

        horizontalLayout->addWidget(chbActive);


        gridLayout_2->addLayout(horizontalLayout, 2, 1, 1, 2);

        teQuestionText = new QPlainTextEdit(gbQuestions);
        teQuestionText->setObjectName(QStringLiteral("teQuestionText"));
        sizePolicy4.setHeightForWidth(teQuestionText->sizePolicy().hasHeightForWidth());
        teQuestionText->setSizePolicy(sizePolicy4);
        teQuestionText->setMinimumSize(QSize(0, 50));
        teQuestionText->setMaximumSize(QSize(16777215, 50));

        gridLayout_2->addWidget(teQuestionText, 3, 1, 1, 2);

        label = new QLabel(gbQuestions);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 2, 0, 1, 1);

        label_2 = new QLabel(gbQuestions);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 3, 0, 1, 1);

        gridLayout_2->setColumnStretch(0, 1);
        gridLayout_2->setColumnStretch(1, 1);
        gridLayout_2->setColumnStretch(2, 10);
        splVertical->addWidget(gbQuestions);
        layoutWidget = new QWidget(splVertical);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        line = new QFrame(layoutWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        gbAnswers = new QGroupBox(layoutWidget);
        gbAnswers->setObjectName(QStringLiteral("gbAnswers"));
        gridLayout_3 = new QGridLayout(gbAnswers);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setHorizontalSpacing(3);
        gridLayout_3->setVerticalSpacing(1);
        gridLayout_3->setContentsMargins(3, 1, 3, 3);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(1);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pbAddAnswer = new QToolButton(gbAnswers);
        pbAddAnswer->setObjectName(QStringLiteral("pbAddAnswer"));
        pbAddAnswer->setMinimumSize(QSize(24, 24));
        pbAddAnswer->setMaximumSize(QSize(24, 24));
        pbAddAnswer->setIcon(icon);
        pbAddAnswer->setIconSize(QSize(16, 16));

        horizontalLayout_4->addWidget(pbAddAnswer);

        pbDeleteAnswer = new QToolButton(gbAnswers);
        pbDeleteAnswer->setObjectName(QStringLiteral("pbDeleteAnswer"));
        pbDeleteAnswer->setMinimumSize(QSize(24, 24));
        pbDeleteAnswer->setMaximumSize(QSize(24, 24));
        pbDeleteAnswer->setIcon(icon1);
        pbDeleteAnswer->setIconSize(QSize(16, 16));

        horizontalLayout_4->addWidget(pbDeleteAnswer);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        gridLayout_3->addLayout(horizontalLayout_4, 0, 0, 1, 2);

        tvAnswer = new QTableView(gbAnswers);
        tvAnswer->setObjectName(QStringLiteral("tvAnswer"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(4);
        sizePolicy5.setHeightForWidth(tvAnswer->sizePolicy().hasHeightForWidth());
        tvAnswer->setSizePolicy(sizePolicy5);

        gridLayout_3->addWidget(tvAnswer, 1, 0, 1, 2);

        label_3 = new QLabel(gbAnswers);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_3->addWidget(label_3, 2, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        leAnswerNumber = new QLineEdit(gbAnswers);
        leAnswerNumber->setObjectName(QStringLiteral("leAnswerNumber"));
        sizePolicy3.setHeightForWidth(leAnswerNumber->sizePolicy().hasHeightForWidth());
        leAnswerNumber->setSizePolicy(sizePolicy3);
        leAnswerNumber->setMinimumSize(QSize(75, 0));
        leAnswerNumber->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_5->addWidget(leAnswerNumber);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        gridLayout_3->addLayout(horizontalLayout_5, 2, 1, 1, 1);

        label_4 = new QLabel(gbAnswers);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_3->addWidget(label_4, 3, 0, 1, 1);

        teAnswerText = new QPlainTextEdit(gbAnswers);
        teAnswerText->setObjectName(QStringLiteral("teAnswerText"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(teAnswerText->sizePolicy().hasHeightForWidth());
        teAnswerText->setSizePolicy(sizePolicy6);
        teAnswerText->setMinimumSize(QSize(0, 50));
        teAnswerText->setMaximumSize(QSize(16777215, 50));

        gridLayout_3->addWidget(teAnswerText, 3, 1, 1, 1);


        verticalLayout->addWidget(gbAnswers);

        splVertical->addWidget(layoutWidget);

        gridLayout_4->addWidget(splVertical, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_8 = new QGridLayout(tab_2);
        gridLayout_8->setSpacing(1);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(3, 3, 3, 3);
        splitter_2 = new QSplitter(tab_2);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        gbTextBlock = new QGroupBox(splitter_2);
        gbTextBlock->setObjectName(QStringLiteral("gbTextBlock"));
        gridLayout_6 = new QGridLayout(gbTextBlock);
        gridLayout_6->setSpacing(1);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(1);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        pbAddBlock = new QToolButton(gbTextBlock);
        pbAddBlock->setObjectName(QStringLiteral("pbAddBlock"));
        pbAddBlock->setMinimumSize(QSize(24, 24));
        pbAddBlock->setMaximumSize(QSize(24, 24));
        pbAddBlock->setIcon(icon);

        horizontalLayout_7->addWidget(pbAddBlock);

        pbAddSubBlock = new QToolButton(gbTextBlock);
        pbAddSubBlock->setObjectName(QStringLiteral("pbAddSubBlock"));
        QSizePolicy sizePolicy7(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(pbAddSubBlock->sizePolicy().hasHeightForWidth());
        pbAddSubBlock->setSizePolicy(sizePolicy7);
        pbAddSubBlock->setMinimumSize(QSize(24, 24));
        pbAddSubBlock->setMaximumSize(QSize(24, 24));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/icons/add_child.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAddSubBlock->setIcon(icon4);

        horizontalLayout_7->addWidget(pbAddSubBlock);

        pbDeleteBlock = new QToolButton(gbTextBlock);
        pbDeleteBlock->setObjectName(QStringLiteral("pbDeleteBlock"));
        pbDeleteBlock->setMinimumSize(QSize(24, 24));
        pbDeleteBlock->setMaximumSize(QSize(24, 24));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbDeleteBlock->setIcon(icon5);

        horizontalLayout_7->addWidget(pbDeleteBlock);

        pbPromoteBlock = new QToolButton(gbTextBlock);
        pbPromoteBlock->setObjectName(QStringLiteral("pbPromoteBlock"));
        pbPromoteBlock->setMinimumSize(QSize(24, 24));
        pbPromoteBlock->setMaximumSize(QSize(24, 24));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/icons/arrow_promote.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbPromoteBlock->setIcon(icon6);

        horizontalLayout_7->addWidget(pbPromoteBlock);

        pbDemoteBlock = new QToolButton(gbTextBlock);
        pbDemoteBlock->setObjectName(QStringLiteral("pbDemoteBlock"));
        pbDemoteBlock->setMinimumSize(QSize(24, 24));
        pbDemoteBlock->setMaximumSize(QSize(24, 24));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/icons/arrow_demote.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbDemoteBlock->setIcon(icon7);

        horizontalLayout_7->addWidget(pbDemoteBlock);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        gridLayout_6->addLayout(horizontalLayout_7, 0, 0, 1, 1);

        trvBlock = new QTreeView(gbTextBlock);
        trvBlock->setObjectName(QStringLiteral("trvBlock"));

        gridLayout_6->addWidget(trvBlock, 1, 0, 1, 1);

        splitter_2->addWidget(gbTextBlock);
        layoutWidget_1 = new QWidget(splitter_2);
        layoutWidget_1->setObjectName(QStringLiteral("layoutWidget_1"));
        gridLayout_7 = new QGridLayout(layoutWidget_1);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setHorizontalSpacing(3);
        gridLayout_7->setVerticalSpacing(1);
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        line_2 = new QFrame(layoutWidget_1);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_7->addWidget(line_2, 0, 0, 1, 8);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(1);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_6 = new QLabel(layoutWidget_1);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_8->addWidget(label_6);

        leBlockNumber = new QLineEdit(layoutWidget_1);
        leBlockNumber->setObjectName(QStringLiteral("leBlockNumber"));

        horizontalLayout_8->addWidget(leBlockNumber);

        horizontalSpacer_7 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_7);

        pbAlignLeft = new QToolButton(layoutWidget_1);
        pbAlignLeft->setObjectName(QStringLiteral("pbAlignLeft"));
        pbAlignLeft->setMinimumSize(QSize(24, 24));
        pbAlignLeft->setMaximumSize(QSize(24, 24));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/icons/text_align_left.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAlignLeft->setIcon(icon8);

        horizontalLayout_8->addWidget(pbAlignLeft);

        pbAlignRight = new QToolButton(layoutWidget_1);
        pbAlignRight->setObjectName(QStringLiteral("pbAlignRight"));
        pbAlignRight->setMinimumSize(QSize(24, 24));
        pbAlignRight->setMaximumSize(QSize(24, 24));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/icons/text_align_right.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAlignRight->setIcon(icon9);

        horizontalLayout_8->addWidget(pbAlignRight);

        pbAlignCenter = new QToolButton(layoutWidget_1);
        pbAlignCenter->setObjectName(QStringLiteral("pbAlignCenter"));
        pbAlignCenter->setMinimumSize(QSize(24, 24));
        pbAlignCenter->setMaximumSize(QSize(24, 24));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/icons/text_align_center.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAlignCenter->setIcon(icon10);

        horizontalLayout_8->addWidget(pbAlignCenter);

        pbAlignJustify = new QToolButton(layoutWidget_1);
        pbAlignJustify->setObjectName(QStringLiteral("pbAlignJustify"));
        pbAlignJustify->setMinimumSize(QSize(24, 24));
        pbAlignJustify->setMaximumSize(QSize(24, 24));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/images/icons/text_align_justify.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAlignJustify->setIcon(icon11);

        horizontalLayout_8->addWidget(pbAlignJustify);

        pbBold = new QToolButton(layoutWidget_1);
        pbBold->setObjectName(QStringLiteral("pbBold"));
        pbBold->setMinimumSize(QSize(24, 24));
        pbBold->setMaximumSize(QSize(24, 24));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/images/icons/text_bold.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbBold->setIcon(icon12);

        horizontalLayout_8->addWidget(pbBold);

        pbItalic = new QToolButton(layoutWidget_1);
        pbItalic->setObjectName(QStringLiteral("pbItalic"));
        pbItalic->setMinimumSize(QSize(24, 24));
        pbItalic->setMaximumSize(QSize(24, 24));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/images/icons/text_italic.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbItalic->setIcon(icon13);

        horizontalLayout_8->addWidget(pbItalic);

        pbUnderline = new QToolButton(layoutWidget_1);
        pbUnderline->setObjectName(QStringLiteral("pbUnderline"));
        pbUnderline->setMinimumSize(QSize(24, 24));
        pbUnderline->setMaximumSize(QSize(24, 24));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/images/icons/text_underline.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbUnderline->setIcon(icon14);

        horizontalLayout_8->addWidget(pbUnderline);

        pbStrikethrough = new QToolButton(layoutWidget_1);
        pbStrikethrough->setObjectName(QStringLiteral("pbStrikethrough"));
        pbStrikethrough->setMinimumSize(QSize(24, 24));
        pbStrikethrough->setMaximumSize(QSize(24, 24));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/images/icons/text_strikethrough.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbStrikethrough->setIcon(icon15);

        horizontalLayout_8->addWidget(pbStrikethrough);


        gridLayout_7->addLayout(horizontalLayout_8, 1, 0, 1, 8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(1);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_7 = new QLabel(layoutWidget_1);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_9->addWidget(label_7);

        horizontalSpacer_8 = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_8);

        pbIndent = new QToolButton(layoutWidget_1);
        pbIndent->setObjectName(QStringLiteral("pbIndent"));
        pbIndent->setMinimumSize(QSize(24, 24));
        pbIndent->setMaximumSize(QSize(24, 24));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/images/icons/text_indent.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbIndent->setIcon(icon16);

        horizontalLayout_9->addWidget(pbIndent);

        pbOutdent = new QToolButton(layoutWidget_1);
        pbOutdent->setObjectName(QStringLiteral("pbOutdent"));
        pbOutdent->setMinimumSize(QSize(24, 24));
        pbOutdent->setMaximumSize(QSize(24, 24));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/images/icons/text_indent_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbOutdent->setIcon(icon17);

        horizontalLayout_9->addWidget(pbOutdent);

        pbNumberList = new QToolButton(layoutWidget_1);
        pbNumberList->setObjectName(QStringLiteral("pbNumberList"));
        pbNumberList->setMinimumSize(QSize(24, 24));
        pbNumberList->setMaximumSize(QSize(24, 24));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/images/icons/text_list_numbers.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbNumberList->setIcon(icon18);

        horizontalLayout_9->addWidget(pbNumberList);

        pbBulletList = new QToolButton(layoutWidget_1);
        pbBulletList->setObjectName(QStringLiteral("pbBulletList"));
        pbBulletList->setMinimumSize(QSize(24, 24));
        pbBulletList->setMaximumSize(QSize(24, 24));
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/images/icons/text_list_bullets.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbBulletList->setIcon(icon19);

        horizontalLayout_9->addWidget(pbBulletList);

        pbFontName = new QToolButton(layoutWidget_1);
        pbFontName->setObjectName(QStringLiteral("pbFontName"));
        pbFontName->setMinimumSize(QSize(24, 24));
        pbFontName->setMaximumSize(QSize(24, 24));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/images/icons/font.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFontName->setIcon(icon20);

        horizontalLayout_9->addWidget(pbFontName);

        pbFontSize = new QToolButton(layoutWidget_1);
        pbFontSize->setObjectName(QStringLiteral("pbFontSize"));
        pbFontSize->setMinimumSize(QSize(24, 24));
        pbFontSize->setMaximumSize(QSize(24, 24));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/images/icons/text_smallcaps.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFontSize->setIcon(icon21);

        horizontalLayout_9->addWidget(pbFontSize);

        pbColor = new QToolButton(layoutWidget_1);
        pbColor->setObjectName(QStringLiteral("pbColor"));
        pbColor->setMinimumSize(QSize(24, 24));
        pbColor->setMaximumSize(QSize(24, 24));
        QIcon icon22;
        icon22.addFile(QStringLiteral(":/images/icons/text_color.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbColor->setIcon(icon22);

        horizontalLayout_9->addWidget(pbColor);

        pbBgColor = new QToolButton(layoutWidget_1);
        pbBgColor->setObjectName(QStringLiteral("pbBgColor"));
        pbBgColor->setMinimumSize(QSize(24, 24));
        pbBgColor->setMaximumSize(QSize(24, 24));
        QIcon icon23;
        icon23.addFile(QStringLiteral(":/images/icons/text_background_color.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbBgColor->setIcon(icon23);

        horizontalLayout_9->addWidget(pbBgColor);


        gridLayout_7->addLayout(horizontalLayout_9, 2, 0, 1, 8);

        twBlock = new QTabWidget(layoutWidget_1);
        twBlock->setObjectName(QStringLiteral("twBlock"));
        twBlock->setTabPosition(QTabWidget::South);
        gvBloc = new QWidget();
        gvBloc->setObjectName(QStringLiteral("gvBloc"));
        gridLayout_9 = new QGridLayout(gvBloc);
        gridLayout_9->setSpacing(1);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        wvBlock = new QWebView(gvBloc);
        wvBlock->setObjectName(QStringLiteral("wvBlock"));
        wvBlock->setUrl(QUrl(QStringLiteral("about:blank")));

        gridLayout_9->addWidget(wvBlock, 0, 0, 1, 1);

        twBlock->addTab(gvBloc, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        gridLayout_10 = new QGridLayout(tab_6);
        gridLayout_10->setSpacing(1);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        teBlock = new QPlainTextEdit(tab_6);
        teBlock->setObjectName(QStringLiteral("teBlock"));

        gridLayout_10->addWidget(teBlock, 0, 0, 1, 1);

        twBlock->addTab(tab_6, QString());

        gridLayout_7->addWidget(twBlock, 3, 0, 1, 8);

        label_9 = new QLabel(layoutWidget_1);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_7->addWidget(label_9, 4, 0, 1, 1);

        cbContentType = new QComboBox(layoutWidget_1);
        cbContentType->setObjectName(QStringLiteral("cbContentType"));
        cbContentType->setMinimumSize(QSize(150, 0));

        gridLayout_7->addWidget(cbContentType, 4, 1, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(108, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_12, 4, 6, 1, 2);

        label_8 = new QLabel(layoutWidget_1);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_7->addWidget(label_8, 5, 0, 1, 1);

        cbValidity1 = new QComboBox(layoutWidget_1);
        cbValidity1->setObjectName(QStringLiteral("cbValidity1"));
        cbValidity1->setMinimumSize(QSize(150, 0));

        gridLayout_7->addWidget(cbValidity1, 5, 1, 1, 1);

        label_12 = new QLabel(layoutWidget_1);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_7->addWidget(label_12, 5, 2, 1, 1);

        leVal1Min = new QLineEdit(layoutWidget_1);
        leVal1Min->setObjectName(QStringLiteral("leVal1Min"));
        leVal1Min->setMinimumSize(QSize(50, 0));
        leVal1Min->setMaximumSize(QSize(50, 16777215));

        gridLayout_7->addWidget(leVal1Min, 5, 3, 1, 1);

        label_13 = new QLabel(layoutWidget_1);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_7->addWidget(label_13, 5, 4, 1, 1);

        leVal1Max = new QLineEdit(layoutWidget_1);
        leVal1Max->setObjectName(QStringLiteral("leVal1Max"));
        leVal1Max->setMinimumSize(QSize(50, 0));
        leVal1Max->setMaximumSize(QSize(50, 16777215));

        gridLayout_7->addWidget(leVal1Max, 5, 5, 1, 1);

        cbVal1AndOr = new QComboBox(layoutWidget_1);
        cbVal1AndOr->setObjectName(QStringLiteral("cbVal1AndOr"));
        cbVal1AndOr->setMinimumSize(QSize(75, 0));

        gridLayout_7->addWidget(cbVal1AndOr, 5, 6, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_9, 5, 7, 1, 1);

        label_14 = new QLabel(layoutWidget_1);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_7->addWidget(label_14, 6, 0, 1, 1);

        cbValidity2 = new QComboBox(layoutWidget_1);
        cbValidity2->setObjectName(QStringLiteral("cbValidity2"));
        cbValidity2->setMinimumSize(QSize(150, 0));

        gridLayout_7->addWidget(cbValidity2, 6, 1, 1, 1);

        label_15 = new QLabel(layoutWidget_1);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_7->addWidget(label_15, 6, 2, 1, 1);

        leVal2Min = new QLineEdit(layoutWidget_1);
        leVal2Min->setObjectName(QStringLiteral("leVal2Min"));
        leVal2Min->setMinimumSize(QSize(50, 0));
        leVal2Min->setMaximumSize(QSize(50, 16777215));

        gridLayout_7->addWidget(leVal2Min, 6, 3, 1, 1);

        label_16 = new QLabel(layoutWidget_1);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_7->addWidget(label_16, 6, 4, 1, 1);

        leVal2Max = new QLineEdit(layoutWidget_1);
        leVal2Max->setObjectName(QStringLiteral("leVal2Max"));
        leVal2Max->setMinimumSize(QSize(50, 0));
        leVal2Max->setMaximumSize(QSize(50, 16777215));

        gridLayout_7->addWidget(leVal2Max, 6, 5, 1, 1);

        cbVal2AndOr = new QComboBox(layoutWidget_1);
        cbVal2AndOr->setObjectName(QStringLiteral("cbVal2AndOr"));
        cbVal2AndOr->setMinimumSize(QSize(75, 0));

        gridLayout_7->addWidget(cbVal2AndOr, 6, 6, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_10, 6, 7, 1, 1);

        label_17 = new QLabel(layoutWidget_1);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_7->addWidget(label_17, 7, 0, 1, 1);

        cbValidity3 = new QComboBox(layoutWidget_1);
        cbValidity3->setObjectName(QStringLiteral("cbValidity3"));
        cbValidity3->setMinimumSize(QSize(150, 0));

        gridLayout_7->addWidget(cbValidity3, 7, 1, 1, 1);

        label_18 = new QLabel(layoutWidget_1);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_7->addWidget(label_18, 7, 2, 1, 1);

        leVal3Min = new QLineEdit(layoutWidget_1);
        leVal3Min->setObjectName(QStringLiteral("leVal3Min"));
        leVal3Min->setMinimumSize(QSize(50, 0));
        leVal3Min->setMaximumSize(QSize(50, 16777215));

        gridLayout_7->addWidget(leVal3Min, 7, 3, 1, 1);

        label_19 = new QLabel(layoutWidget_1);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_7->addWidget(label_19, 7, 4, 1, 1);

        leVal3Max = new QLineEdit(layoutWidget_1);
        leVal3Max->setObjectName(QStringLiteral("leVal3Max"));
        leVal3Max->setMinimumSize(QSize(50, 0));
        leVal3Max->setMaximumSize(QSize(50, 16777215));

        gridLayout_7->addWidget(leVal3Max, 7, 5, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(108, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_11, 7, 6, 1, 2);

        splitter_2->addWidget(layoutWidget_1);

        gridLayout_8->addWidget(splitter_2, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        splitter->addWidget(tabWidget);

        gridLayout_5->addWidget(splitter, 0, 0, 1, 1);

        QWidget::setTabOrder(pbAddScan, pbDeleteScan);
        QWidget::setTabOrder(pbDeleteScan, tvScan);
        QWidget::setTabOrder(tvScan, pbSaveScan);
        QWidget::setTabOrder(pbSaveScan, pbRevertScan);
        QWidget::setTabOrder(pbRevertScan, tabWidget);
        QWidget::setTabOrder(tabWidget, pbAddQuestion);
        QWidget::setTabOrder(pbAddQuestion, pbDeleteQuestion);
        QWidget::setTabOrder(pbDeleteQuestion, tvQuestion);
        QWidget::setTabOrder(tvQuestion, leQuestionNumber);
        QWidget::setTabOrder(leQuestionNumber, cbQuestionType);
        QWidget::setTabOrder(cbQuestionType, chbActive);
        QWidget::setTabOrder(chbActive, teQuestionText);
        QWidget::setTabOrder(teQuestionText, pbAddAnswer);
        QWidget::setTabOrder(pbAddAnswer, pbDeleteAnswer);
        QWidget::setTabOrder(pbDeleteAnswer, tvAnswer);
        QWidget::setTabOrder(tvAnswer, leAnswerNumber);
        QWidget::setTabOrder(leAnswerNumber, teAnswerText);
        QWidget::setTabOrder(teAnswerText, pbHelp);
        QWidget::setTabOrder(pbHelp, pbOk);
        QWidget::setTabOrder(pbOk, pbCancel);
        QWidget::setTabOrder(pbCancel, pbAddBlock);
        QWidget::setTabOrder(pbAddBlock, pbAddSubBlock);
        QWidget::setTabOrder(pbAddSubBlock, pbDeleteBlock);
        QWidget::setTabOrder(pbDeleteBlock, pbPromoteBlock);
        QWidget::setTabOrder(pbPromoteBlock, pbDemoteBlock);
        QWidget::setTabOrder(pbDemoteBlock, trvBlock);
        QWidget::setTabOrder(trvBlock, leBlockNumber);
        QWidget::setTabOrder(leBlockNumber, pbAlignLeft);
        QWidget::setTabOrder(pbAlignLeft, pbAlignRight);
        QWidget::setTabOrder(pbAlignRight, pbAlignCenter);
        QWidget::setTabOrder(pbAlignCenter, pbAlignJustify);
        QWidget::setTabOrder(pbAlignJustify, pbBold);
        QWidget::setTabOrder(pbBold, pbItalic);
        QWidget::setTabOrder(pbItalic, pbUnderline);
        QWidget::setTabOrder(pbUnderline, pbStrikethrough);
        QWidget::setTabOrder(pbStrikethrough, pbIndent);
        QWidget::setTabOrder(pbIndent, pbOutdent);
        QWidget::setTabOrder(pbOutdent, pbNumberList);
        QWidget::setTabOrder(pbNumberList, pbBulletList);
        QWidget::setTabOrder(pbBulletList, pbFontName);
        QWidget::setTabOrder(pbFontName, pbFontSize);
        QWidget::setTabOrder(pbFontSize, pbColor);
        QWidget::setTabOrder(pbColor, pbBgColor);
        QWidget::setTabOrder(pbBgColor, wvBlock);
        QWidget::setTabOrder(wvBlock, twBlock);
        QWidget::setTabOrder(twBlock, cbContentType);
        QWidget::setTabOrder(cbContentType, cbValidity1);
        QWidget::setTabOrder(cbValidity1, leVal1Min);
        QWidget::setTabOrder(leVal1Min, leVal1Max);
        QWidget::setTabOrder(leVal1Max, cbVal1AndOr);
        QWidget::setTabOrder(cbVal1AndOr, cbValidity2);
        QWidget::setTabOrder(cbValidity2, leVal2Min);
        QWidget::setTabOrder(leVal2Min, leVal2Max);
        QWidget::setTabOrder(leVal2Max, cbVal2AndOr);
        QWidget::setTabOrder(cbVal2AndOr, cbValidity3);
        QWidget::setTabOrder(cbValidity3, leVal3Min);
        QWidget::setTabOrder(leVal3Min, leVal3Max);
        QWidget::setTabOrder(leVal3Max, teBlock);

        retranslateUi(SCAN_ScanEditDialog);

        tabWidget->setCurrentIndex(0);
        twBlock->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SCAN_ScanEditDialog);
    } // setupUi

    void retranslateUi(QDialog *SCAN_ScanEditDialog)
    {
        SCAN_ScanEditDialog->setWindowTitle(QApplication::translate("SCAN_ScanEditDialog", "Dialog", 0));
        pbHelp->setText(QApplication::translate("SCAN_ScanEditDialog", "&Help", 0));
        pbOk->setText(QApplication::translate("SCAN_ScanEditDialog", "&OK", 0));
        pbCancel->setText(QApplication::translate("SCAN_ScanEditDialog", "&Cancel", 0));
        groupBox->setTitle(QApplication::translate("SCAN_ScanEditDialog", "Scan List", 0));
#ifndef QT_NO_TOOLTIP
        pbAddScan->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Add scan", 0));
#endif // QT_NO_TOOLTIP
        pbAddScan->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbDeleteScan->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Delete scan", 0));
#endif // QT_NO_TOOLTIP
        pbDeleteScan->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbSaveScan->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Save scan", 0));
#endif // QT_NO_TOOLTIP
        pbSaveScan->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbRevertScan->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Undo current changes", 0));
#endif // QT_NO_TOOLTIP
        pbRevertScan->setText(QString());
        gbQuestions->setTitle(QApplication::translate("SCAN_ScanEditDialog", "Questions", 0));
#ifndef QT_NO_TOOLTIP
        pbAddQuestion->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Add question", 0));
#endif // QT_NO_TOOLTIP
        pbAddQuestion->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbDeleteQuestion->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Delete question", 0));
#endif // QT_NO_TOOLTIP
        pbDeleteQuestion->setText(QString());
        label_5->setText(QApplication::translate("SCAN_ScanEditDialog", "Type", 0));
        chbActive->setText(QApplication::translate("SCAN_ScanEditDialog", "Active", 0));
        label->setText(QApplication::translate("SCAN_ScanEditDialog", "Number", 0));
        label_2->setText(QApplication::translate("SCAN_ScanEditDialog", "Question", 0));
        gbAnswers->setTitle(QApplication::translate("SCAN_ScanEditDialog", "Answers", 0));
#ifndef QT_NO_TOOLTIP
        pbAddAnswer->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Add answer", 0));
#endif // QT_NO_TOOLTIP
        pbAddAnswer->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbDeleteAnswer->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Delete answer", 0));
#endif // QT_NO_TOOLTIP
        pbDeleteAnswer->setText(QString());
#ifndef QT_NO_TOOLTIP
        label_3->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Sequence Number", 0));
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("SCAN_ScanEditDialog", "Number", 0));
#ifndef QT_NO_TOOLTIP
        label_4->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Question text", 0));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("SCAN_ScanEditDialog", "Answer", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SCAN_ScanEditDialog", "Content", 0));
        gbTextBlock->setTitle(QApplication::translate("SCAN_ScanEditDialog", "Text Block List", 0));
#ifndef QT_NO_TOOLTIP
        pbAddBlock->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Add text block", 0));
#endif // QT_NO_TOOLTIP
        pbAddBlock->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbAddSubBlock->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Add text subblock", 0));
#endif // QT_NO_TOOLTIP
        pbAddSubBlock->setText(QString());
        pbDeleteBlock->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbPromoteBlock->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Promote text block", 0));
#endif // QT_NO_TOOLTIP
        pbPromoteBlock->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbDemoteBlock->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Demote text block", 0));
#endif // QT_NO_TOOLTIP
        pbDemoteBlock->setText(QString());
        label_6->setText(QApplication::translate("SCAN_ScanEditDialog", "Number", 0));
#ifndef QT_NO_TOOLTIP
        pbAlignLeft->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Align left", 0));
#endif // QT_NO_TOOLTIP
        pbAlignLeft->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbAlignRight->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Align right", 0));
#endif // QT_NO_TOOLTIP
        pbAlignRight->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbAlignCenter->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Align center", 0));
#endif // QT_NO_TOOLTIP
        pbAlignCenter->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbAlignJustify->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Align justify", 0));
#endif // QT_NO_TOOLTIP
        pbAlignJustify->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbBold->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Bold", 0));
#endif // QT_NO_TOOLTIP
        pbBold->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbItalic->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Italic", 0));
#endif // QT_NO_TOOLTIP
        pbItalic->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbUnderline->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Underline", 0));
#endif // QT_NO_TOOLTIP
        pbUnderline->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbStrikethrough->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Strikethrough", 0));
#endif // QT_NO_TOOLTIP
        pbStrikethrough->setText(QString());
        label_7->setText(QApplication::translate("SCAN_ScanEditDialog", "Block Text", 0));
#ifndef QT_NO_TOOLTIP
        pbIndent->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Increase indent", 0));
#endif // QT_NO_TOOLTIP
        pbIndent->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbOutdent->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Decrease indent", 0));
#endif // QT_NO_TOOLTIP
        pbOutdent->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbNumberList->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Numbered list", 0));
#endif // QT_NO_TOOLTIP
        pbNumberList->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbBulletList->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Bullet list", 0));
#endif // QT_NO_TOOLTIP
        pbBulletList->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbFontName->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Set font type", 0));
#endif // QT_NO_TOOLTIP
        pbFontName->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbFontSize->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Set font size", 0));
#endif // QT_NO_TOOLTIP
        pbFontSize->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbColor->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Text color", 0));
#endif // QT_NO_TOOLTIP
        pbColor->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbBgColor->setToolTip(QApplication::translate("SCAN_ScanEditDialog", "Text background color", 0));
#endif // QT_NO_TOOLTIP
        pbBgColor->setText(QString());
        twBlock->setTabText(twBlock->indexOf(gvBloc), QApplication::translate("SCAN_ScanEditDialog", "View", 0));
        twBlock->setTabText(twBlock->indexOf(tab_6), QApplication::translate("SCAN_ScanEditDialog", "Source", 0));
        label_9->setText(QApplication::translate("SCAN_ScanEditDialog", "Content Type", 0));
        label_8->setText(QApplication::translate("SCAN_ScanEditDialog", "Validity 1", 0));
        label_12->setText(QApplication::translate("SCAN_ScanEditDialog", "Min.", 0));
        label_13->setText(QApplication::translate("SCAN_ScanEditDialog", "Max.", 0));
        label_14->setText(QApplication::translate("SCAN_ScanEditDialog", "Validity 2", 0));
        label_15->setText(QApplication::translate("SCAN_ScanEditDialog", "Min.", 0));
        label_16->setText(QApplication::translate("SCAN_ScanEditDialog", "Max.", 0));
        label_17->setText(QApplication::translate("SCAN_ScanEditDialog", "Validity 3", 0));
        label_18->setText(QApplication::translate("SCAN_ScanEditDialog", "Min.", 0));
        label_19->setText(QApplication::translate("SCAN_ScanEditDialog", "Max.", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SCAN_ScanEditDialog", "Report", 0));
    } // retranslateUi

};

namespace Ui {
    class SCAN_ScanEditDialog: public Ui_SCAN_ScanEditDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCAN_SCANEDITDIALOG_H
