/*****************************************************************
 * $Id: bil_mainwindow.h 2142 2014-04-07 19:51:29Z rutger $
 * Created: Dec 16, 2010 16:34:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna BIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef BIL_MAINWINDOW_H
#define BIL_MAINWINDOW_H

#include <QtWidgets>
#include <QtSql>
#include <QUrl>
#include "rb_mainwindow.h"



/**
 * BIL mainwindow
 */
class BIL_MainWindow : public RB_MainWindow {

    Q_OBJECT

public:
    BIL_MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~BIL_MainWindow();

public slots:
    virtual void slotFileNew();
    virtual bool slotFileOpen();
    virtual void slotDashboard();
    virtual void slotDashboardAnchorClicked(const QUrl& url);
    virtual void slotFileDemoQuit();

protected:
    void initActions();
    void initViews();
    void initDebug();

    void changeEvent(QEvent* e);

};

#endif // BIL_MAINWINDOW_H
