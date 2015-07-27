/*****************************************************************
 * $Id: gv_mainwindow.h 1164 2010-05-20 11:20:24Z rutger $
 * Created: Mar 19, 2008 16:34:42 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_MAINWINDOW_H
#define GV_MAINWINDOW_H

#include <QtGui>
#include <QtSql>
#include "rb_mainwindow.h"



/**
 * ACC mainwindow
 */
class GV_MainWindow : public RB_MainWindow {

    Q_OBJECT

public:
    GV_MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~GV_MainWindow();

public slots:
    virtual void slotFileNew();
    virtual bool slotFileOpen();

    virtual void slotFilePrint();

protected:
    void initActions();
    void initViews();

};

#endif // GV_MAINWINDOW_H
