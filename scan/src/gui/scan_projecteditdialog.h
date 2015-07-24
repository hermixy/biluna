/*****************************************************************
 * $Id: scan_projecteditdialog.h 1832 2012-12-16 12:01:50Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef SCAN_PROJECTEDITDIALOG_H
#define SCAN_PROJECTEDITDIALOG_H

#include "rb_projecteditdialog.h"


/**
 * Edit DB project dialog
 */
class SCAN_ProjectEditDialog : public RB_ProjectEditDialog {

    Q_OBJECT

public:
    SCAN_ProjectEditDialog(QWidget* parent);
    virtual ~SCAN_ProjectEditDialog();

    virtual void init();

public slots:
    virtual void on_pbAdd_clicked();
};

#endif // SCAN_PROJECTEDITDIALOG_H
