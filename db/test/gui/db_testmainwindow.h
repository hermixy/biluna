/*****************************************************************
 * $Id: db_testmainwindow.h 1050 2009-12-03 22:38:27Z rutger $
 * Created: Nov 5, 2009 12:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTMAINWINDOW_H
#define DB_TESTMAINWINDOW_H

#include <QtGui>
#include <QtSql>
#include "rb_mainwindow.h"
#include "rb_signalspydialog.h"
#include "rb_string.h"


/**
 * Main window test class
 */
class DB_TestMainWindow : public RB_MainWindow {

    Q_OBJECT

public:
    DB_TestMainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~DB_TestMainWindow();

public slots:
    virtual void slotFileNew();
    virtual bool slotFileOpen();

    virtual void slotFilePrint();

protected:
    void initActions();
    void initViews();

};

#endif // DB_TESTMAINWINDOW_H
