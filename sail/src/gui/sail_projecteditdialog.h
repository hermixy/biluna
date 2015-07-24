/*****************************************************************
 * $Id: sail_projecteditdialog.h 1831 2012-12-15 15:07:57Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef SAIL_PROJECTEDITDIALOG_H
#define SAIL_PROJECTEDITDIALOG_H

#include "rb_projecteditdialog.h"


/**
 * Edit DB project dialog
 */
class SAIL_ProjectEditDialog : public RB_ProjectEditDialog {

    Q_OBJECT

public:
    SAIL_ProjectEditDialog(QWidget* parent);
    virtual ~SAIL_ProjectEditDialog();

    virtual void init();

public slots:
    virtual void on_pbAdd_clicked();
};

#endif // SAIL_PROJECTEDITDIALOG_H
