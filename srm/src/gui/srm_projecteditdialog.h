/*****************************************************************
 * $Id: srm_projecteditdialog.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef SRM_PROJECTEDITDIALOG_H
#define SRM_PROJECTEDITDIALOG_H

#include "rb_projecteditdialog.h"


/**
 * Edit CRM project dialog
 */
class SRM_ProjectEditDialog : public RB_ProjectEditDialog {

    Q_OBJECT

public:
    SRM_ProjectEditDialog(QWidget* parent);
    virtual ~SRM_ProjectEditDialog();

    virtual void init();

public slots:
    virtual void on_pbAdd_clicked();
};

#endif // SRM_PROJECTEDITDIALOG_H
