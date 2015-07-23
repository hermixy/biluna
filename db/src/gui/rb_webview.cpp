/*****************************************************************
 * $Id: rb_webview.cpp 2012 2013-10-16 16:14:10Z rutger $
 * Created: 2013-10-08 rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_webview.h"

#include <QKeyEvent>

/**
 * Constructor
 */
RB_WebView::RB_WebView(QWidget *parent) : QWebView(parent) {
    mParentWidget = parent;
    mMaxMode = false;
    mGridLayout = NULL;
    mOrigZoomFactor = 1.0;
}

/**
 * Destructor
 */
RB_WebView::~RB_WebView() {
    // nothing
}

/**
 * Add this widget to the (grid)layout with
 * layout->addWidget(this,0,0,1,1);
 * @brief RB_WebView::setGridLayout
 * @param layout
 */
void RB_WebView::setGridLayout(QGridLayout *layout) {
    mGridLayout = layout;
}

/**
 * @brief RB_WebView::isWidgetMaximized
 * @returns true if widget has been maximized with F5 (full screen)
 */
bool RB_WebView::isWidgetMaximized() {
    return mMaxMode;
}

//void RB_WebView::keyPressEvent(QKeyEvent* e) {
//    switch(e->key()) {
//    case Qt::Key_F5 :
//        e->accept();
//        break;
//    default :
//        QWebView::keyReleaseEvent(e);
//        break;
//    }
//}

/**
 * @brief RB_WebView::keyReleaseEvent
 * @param e
 */
void RB_WebView::keyReleaseEvent(QKeyEvent* e) {
    switch(e->key()) {
    case Qt::Key_F5 :
        if (!mMaxMode) {
            mOrigWindowFlags = this->windowFlags();
            mOrigSize = this->size();
            mOrigZoomFactor = this->zoomFactor();
            this->setParent(NULL);
            this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
            this->setZoomFactor(2.0);
            this->showMaximized();
            mMaxMode = true;
            e->accept();
        } else {
            this->setParent(mParentWidget);
            this->resize(mOrigSize);
            this->overrideWindowFlags(mOrigWindowFlags);

            if (mGridLayout) {
                mGridLayout->addWidget(this,0,0,1,1);
            }

            this->setZoomFactor(mOrigZoomFactor);
            this->show();
            mMaxMode = false;
            e->accept();
        }
        break;
    case Qt::Key_Escape :
        if (mMaxMode) {
            this->setParent(mParentWidget);
            this->resize(mOrigSize);
            this->overrideWindowFlags(mOrigWindowFlags);

            if (mGridLayout) {
                mGridLayout->addWidget(this,0,0,1,1);
            }

            this->setZoomFactor(mOrigZoomFactor);
            this->show();
            mMaxMode = false;
            e->accept();
        } else {
            QWebView::keyReleaseEvent(e);
        }
        break;
    default :
        QWebView::keyReleaseEvent(e);
        break;
    }

}

