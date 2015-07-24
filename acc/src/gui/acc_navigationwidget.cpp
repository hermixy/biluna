/*****************************************************************
 * $Id: acc_navigationwidget.cpp 2146 2014-05-23 20:27:06Z rutger $
 * Created: Dec 20, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_navigationwidget.h"

#include "acc_assetmasterwidget.h"
#include "acc_custmasterwidget.h"
#include "acc_modelfactory.h"
#include "acc_stockmasterwidget.h"
#include "acc_suppliermasterwidget.h"

/**
 * Constructor
 */
ACC_NavigationWidget::ACC_NavigationWidget(QWidget *parent) : RB_Widget(parent) {

    setupUi(this);

    mAssetMasterWidget = NULL;
    mCustMasterWidget = NULL;
    mStockCategoryWidget = NULL;
    mSupplierMasterWidget = NULL;


    mSizeHint = QSize(200, 600);
}

/**
 * Destructor
 */
ACC_NavigationWidget::~ACC_NavigationWidget() {
    //nothing
}

/**
 * Initialize this widget
 */
void ACC_NavigationWidget::init() {
    RB_ObjectBase* root = ACC_MODELFACTORY->getRoot();
    leProject->setText(root->getValue("coyname").toString());

    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(slotShowTab(int)));
}

/**
 * Show tab and create widgets
 * @param tabType tab type enumerator
 */
void ACC_NavigationWidget::slotShowTab(int tabType) {
    switch (tabType) {
    case TabCustomer:
    {
        tabWidget->setCurrentIndex(0);

        if (!mCustMasterWidget) {
            mCustMasterWidget = new ACC_CustMasterWidget(this);
            mCustMasterWidget->init();

            QWidget* wdgt = tabWidget->currentWidget();
            QGridLayout* layout = new QGridLayout(wdgt);
            layout->setObjectName("layout-customer");
            layout->setMargin(0);
            layout->setSpacing(0);
            layout->addWidget(mCustMasterWidget, 0, 0, 1, 1);
        }

        break;
    }
    case TabSupplier:
    {
        tabWidget->setCurrentIndex(1);

        if (!mSupplierMasterWidget) {
            mSupplierMasterWidget = new ACC_SupplierMasterWidget(this);
            mSupplierMasterWidget->init();

            QWidget* wdgt = tabWidget->currentWidget();
            QGridLayout* layout = new QGridLayout(wdgt);
            layout->setObjectName("layout-supplier");
            layout->setMargin(0);
            layout->setSpacing(0);
            layout->addWidget(mSupplierMasterWidget, 0, 0, 1, 1);
        }

        break;
    }
    case TabAsset:
    {
        tabWidget->setCurrentIndex(2);

        if (!mAssetMasterWidget) {
            mAssetMasterWidget = new ACC_AssetMasterWidget(this);
            mAssetMasterWidget->init();

            QWidget* wdgt = tabWidget->currentWidget();
            QGridLayout* layout = new QGridLayout(wdgt);
            layout->setObjectName("layout-asset");
            layout->setMargin(0);
            layout->setSpacing(0);
            layout->addWidget(mAssetMasterWidget, 0, 0, 1, 1);
        }

        break;
    }
    case TabStockItem:
    {
        tabWidget->setCurrentIndex(3);

        if (!mStockCategoryWidget) {
            mStockCategoryWidget = new ACC_StockMasterWidget(this);
            mStockCategoryWidget->init();

            QWidget* wdgt = tabWidget->currentWidget();
            QGridLayout* layout = new QGridLayout(wdgt);
            layout->setObjectName("layout-stock");
            layout->setMargin(0);
            layout->setSpacing(0);
            layout->addWidget(mStockCategoryWidget, 0, 0, 1, 1);
        }
        break;
    }
    default:
        break;
    }
}

/**
 * @returns current selected model index
 */
QModelIndex ACC_NavigationWidget::getCurrentModelIndex(int modelType) const {
    switch (modelType) {
    case ACC_ModelFactory::ModelCustomer :
    {
        if (mCustMasterWidget) {
            return mCustMasterWidget->getCurrentModelIndex(modelType);
        }
        break;
    }
    case ACC_ModelFactory::ModelSupplier :
    {
        if (mSupplierMasterWidget) {
            return mSupplierMasterWidget->getCurrentModelIndex(modelType);
        }
        break;
    }
    case ACC_ModelFactory::ModelAsset :
    {
        if (mAssetMasterWidget) {
            return mAssetMasterWidget->getCurrentModelIndex(modelType);
        }
        break;
    }
    case ACC_ModelFactory::ModelStockCategory :
    case ACC_ModelFactory::ModelStockMaster :
    {
        if (mStockCategoryWidget) {
            return mStockCategoryWidget->getCurrentModelIndex(modelType);
        }
        break;
    }
    default:
        break;
    }
    return QModelIndex();
}


void ACC_NavigationWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
