/*****************************************************************
 * $Id: peng_projecteditdialog.h 1831 2012-12-15 15:07:57Z rutger $
 * Created: Apr 29, 2015 8:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef PCALC_PROJECTEDITDIALOG_H
#define PCALC_PROJECTEDITDIALOG_H

#include "rb_projecteditdialog.h"


/**
 * Edit DB project dialog
 */
class PCALC_ProjectEditDialog : public RB_ProjectEditDialog {

    Q_OBJECT

public:
    PCALC_ProjectEditDialog(QWidget* parent);
    virtual ~PCALC_ProjectEditDialog();

    virtual void init();

public slots:
    virtual void on_pbAdd_clicked();
};

#endif // PCALC_PROJECTEDITDIALOG_H
