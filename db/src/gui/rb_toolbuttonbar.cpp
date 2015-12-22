/*****************************************************************
* $Id: rb_toolbuttonbar.cpp 2234 2015-04-30 19:23:20Z rutger $
* Created: Mar 18, 2014 8:27 PM - rutger
*
* Copyright (C) 2014 Red-Bag. All rights reserved.
* This file is part of the Biluna DB project.
*
* See http://www.red-bag.com for further details.
*****************************************************************/

#include "rb_toolbuttonbar.h"


RB_ToolButtonBar::RB_ToolButtonBar(QWidget* parent)
               : QWidget(parent) {
   // setupUi(this); in initEdit() and initDetailEdit()
}

RB_ToolButtonBar::~RB_ToolButtonBar() {

}

void RB_ToolButtonBar::initSlimTable(bool isEdit,
                                     bool isFindFilter,
                                     bool isCopyItem) {
    setupSlimTable(isEdit, isFindFilter, isCopyItem);
}

void RB_ToolButtonBar::initEdit(bool isTreeModel, bool isSaveRevert,
                                bool isFindFilter, bool isShowAll) {
    setupUi(this);

    // pbAdd->setVisible(true);
    pbAddChild->setVisible(isTreeModel);
    // pbDelete->setVisible(true);

    pbEdit->setVisible(false /*isSaveRevert*/);
    pbCopy->setVisible(false);
    pbSave->setVisible(isSaveRevert);
    pbRevert->setVisible(isSaveRevert);

    pbPrevious->setVisible(false);
    pbNext->setVisible(false);

    pbFind->setVisible(isFindFilter);
    leFilter->setVisible(isFindFilter);
    pbFilterOn->setVisible(isFindFilter);
    pbFilterOff->setVisible(isFindFilter);
    pbFilterOff->setEnabled(false);
    pbAll->setVisible(isShowAll);

    pbDemote->setVisible(isTreeModel);
    pbPromote->setVisible(isTreeModel);

    pbUp->setVisible(false);
    pbDown->setVisible(false);
}

void RB_ToolButtonBar::initDetailEdit(bool isSaveRevert) {
    setupUi(this);

    pbAdd->setVisible(false);
    pbAddChild->setVisible(false);
    pbDelete->setVisible(false);

    pbEdit->setVisible(false /*isSaveRevert*/);
    pbCopy->setVisible(false);
    pbSave->setVisible(isSaveRevert);
    pbRevert->setVisible(isSaveRevert);

    pbPrevious->setVisible(true);
    pbNext->setVisible(true);

    pbFind->setVisible(false);
    leFilter->setVisible(false);
    pbFilterOn->setVisible(false);
    pbFilterOff->setVisible(false);
    pbFilterOff->setEnabled(false);
    pbAll->setVisible(false);

    pbDemote->setVisible(false);
    pbPromote->setVisible(false);

    pbUp->setVisible(false);
    pbDown->setVisible(false);
}

void RB_ToolButtonBar::initSelect(bool isFindFilter, bool isShowAll) {
    setupUi(this);

    pbAdd->setVisible(false);
    pbAddChild->setVisible(false);
    pbDelete->setVisible(false);

    pbEdit->setVisible(false);
    pbCopy->setVisible(false);
    pbSave->setVisible(false);
    pbRevert->setVisible(false);

    pbPrevious->setVisible(false);
    pbNext->setVisible(false);

    pbFind->setVisible(isFindFilter);
    leFilter->setVisible(isFindFilter);
    pbFilterOn->setVisible(isFindFilter);
    pbFilterOff->setVisible(isFindFilter);
    pbFilterOff->setEnabled(isFindFilter);
    pbAll->setVisible(isShowAll);

    pbDemote->setVisible(false);
    pbPromote->setVisible(false);

    pbUp->setVisible(false);
    pbDown->setVisible(false);
}

/**
* Required by retranslate during .ui to .h file generation
* @brief RB_ToolButtonBar::setText
* @param txt
*/
void RB_ToolButtonBar::setText(const RB_String& /*txt*/) {

}

RB_String RB_ToolButtonBar::getFindFilterText() {
   return this->leFilter->text();
}

void RB_ToolButtonBar::setFilterOffEnabled(bool enabled) {
    pbFilterOff->setEnabled(enabled);
}

void RB_ToolButtonBar::on_pbAdd_clicked() {
   emit addClicked();
}

void RB_ToolButtonBar::on_pbAddChild_clicked() {
   emit addChildClicked();
}

void RB_ToolButtonBar::on_pbDelete_clicked() {
   emit deleteClicked();
}

void RB_ToolButtonBar::on_pbEdit_clicked() {
    emit editClicked();
}

void RB_ToolButtonBar::on_pbCopy_clicked() {
    emit copyClicked();
}

void RB_ToolButtonBar::on_pbSave_clicked() {
   emit saveClicked();
}

void RB_ToolButtonBar::on_pbRevert_clicked() {
   emit revertClicked();
}

void RB_ToolButtonBar::on_pbPrevious_clicked() {
   emit previousClicked();
}

void RB_ToolButtonBar::on_pbNext_clicked() {
   emit nextClicked();
}

void RB_ToolButtonBar::on_pbFind_clicked() {
   emit findClicked();
}

void RB_ToolButtonBar::on_pbFilterOn_clicked() {
   emit filterOnClicked();
}

void RB_ToolButtonBar::on_pbFilterOff_clicked() {
    emit filterOffClicked();
}

void RB_ToolButtonBar::on_pbAll_clicked() {
    emit allClicked();
}

void RB_ToolButtonBar::on_pbDemote_clicked() {
   emit demoteClicked();
}

void RB_ToolButtonBar::on_pbPromote_clicked() {
   emit promoteClicked();
}

void RB_ToolButtonBar::on_pbUp_clicked() {
   emit upClicked();
}

void RB_ToolButtonBar::on_pbDown_clicked() {
   emit downClicked();
}

void RB_ToolButtonBar::changeEvent(QEvent* /*e*/) {

}

void RB_ToolButtonBar::setupSlimTable(bool isEdit,
                                      bool isFindFilter,
                                      bool isCopyItem) {
    // copied (partly) from ui_rb_toolbuttonbar.h

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("RB_ToolButtonBar"));

    if (isEdit && !isFindFilter) {
        this->resize(150, 28);
    } else if (!isEdit && isFindFilter) {
        this->resize(150, 28);
    } else {
        this->resize(250, 28);
    }

    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(1);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setContentsMargins(1, 1, 1, 1);

    int col = -1;

    if (isEdit) {
        pbAdd = new QToolButton(this);
        pbAdd->setObjectName(QStringLiteral("pbAdd"));
        pbAdd->setMinimumSize(QSize(24, 24));
        pbAdd->setMaximumSize(QSize(24, 24));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbAdd->setIcon(icon);
        pbAdd->setIconSize(QSize(16, 16));

        gridLayout->addWidget(pbAdd, 0, ++col);

        pbDelete = new QToolButton(this);
        pbDelete->setObjectName(QStringLiteral("pbDelete"));
        pbDelete->setMinimumSize(QSize(24, 24));
        pbDelete->setMaximumSize(QSize(24, 24));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbDelete->setIcon(icon2);
        pbDelete->setIconSize(QSize(16, 16));

        gridLayout->addWidget(pbDelete, 0, ++col);

        if (isCopyItem) {
            pbCopy = new QToolButton(this);
            pbCopy->setObjectName(QStringLiteral("pbCopy"));
            pbCopy->setMinimumSize(QSize(24, 24));
            pbCopy->setMaximumSize(QSize(24, 24));
            QIcon icon3;
            icon3.addFile(QStringLiteral(":/images/icons/page_copy.png"), QSize(), QIcon::Normal, QIcon::Off);
            pbCopy->setIcon(icon3);
            pbCopy->setIconSize(QSize(16, 16));

            gridLayout->addWidget(pbCopy, 0, ++col);
            connect(pbCopy, SIGNAL(clicked()), this, SLOT(on_pbCopy_clicked()));
        }

        if (isFindFilter) {
            hsAddDelete = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
        } else {
            hsAddDelete = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        }

        gridLayout->addItem(hsAddDelete, 0, ++col);
        connect(pbAdd, SIGNAL(clicked()), this, SLOT(on_pbAdd_clicked()));
        connect(pbDelete, SIGNAL(clicked()), this, SLOT(on_pbDelete_clicked()));
    }

    if (isFindFilter) {
        pbFind = new QToolButton(this);
        pbFind->setObjectName(QStringLiteral("pbFind"));
        pbFind->setMinimumSize(QSize(24, 24));
        pbFind->setMaximumSize(QSize(24, 24));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/icons/find.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFind->setIcon(icon8);

        gridLayout->addWidget(pbFind, 0, ++col);

        leFilter = new QLineEdit(this);
        leFilter->setObjectName(QStringLiteral("leFilter"));
        leFilter->setMinimumSize(QSize(75, 0));
        leFilter->setMaximumSize(QSize(150, 16777215));
        leFilter->setPlaceholderText(QApplication::translate("RB_ToolButtonBar", "Search/Filter", 0));

        gridLayout->addWidget(leFilter, 0, ++col);

        pbFilterOn = new QToolButton(this);
        pbFilterOn->setObjectName(QStringLiteral("pbFilterOn"));
        pbFilterOn->setMinimumSize(QSize(24, 24));
        pbFilterOn->setMaximumSize(QSize(24, 24));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/icons/filter.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFilterOn->setIcon(icon9);

        gridLayout->addWidget(pbFilterOn, 0, ++col);

        pbFilterOff = new QToolButton(this);
        pbFilterOff->setObjectName(QStringLiteral("pbFilterOff"));
        pbFilterOff->setMinimumSize(QSize(24, 24));
        pbFilterOff->setMaximumSize(QSize(24, 24));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/icons/filter_empty.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFilterOff->setIcon(icon10);

        gridLayout->addWidget(pbFilterOff, 0, ++col);

        hsFilter = new QSpacerItem(10, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        gridLayout->addItem(hsFilter, 0, ++col);
        connect(pbFind, SIGNAL(clicked()), this, SLOT(on_pbFind_clicked()));
        connect(pbFilterOn, SIGNAL(clicked()), this, SLOT(on_pbFilterOn_clicked()));
        connect(pbFilterOff, SIGNAL(clicked()), this, SLOT(on_pbFilterOff_clicked()));
    }
}
