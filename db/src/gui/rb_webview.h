/*****************************************************************
 * $Id: rb_webview.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2013-10-08 rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_WEBVIEW_H
#define RB_WEBVIEW_H

#include <QGridLayout>
#include <QWebView>
#include "db_global.h"


class DB_EXPORT RB_WebView : public QWebView {

    Q_OBJECT

public:
    explicit RB_WebView(QWidget* parent = 0);
    virtual ~RB_WebView();

    void setGridLayout(QGridLayout* layout);
    bool isWidgetMaximized();

protected:
//    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
private:
    QWidget* mParentWidget;
    bool mMaxMode;
    Qt::WindowFlags mOrigWindowFlags;
    QSize mOrigSize;
    QGridLayout* mGridLayout;
    double mOrigZoomFactor;
};

#endif // RB_WEBVIEW_H
