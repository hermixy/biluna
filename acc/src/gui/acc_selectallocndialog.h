/*****************************************************************
 * $Id: acc_selectallocndialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Oct 4, 2010 21:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTALLOCNDIALOG_H
#define ACC_SELECTALLOCNDIALOG_H

#include <QtWidgets>
#include "acc.h"
#include "acc_modelfactory.h"
#include "db_tabledialog.h"

class ACC_SelectAllocnDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_SelectAllocnDialog(QWidget* parent);
    virtual ~ACC_SelectAllocnDialog();

    virtual void init();
    virtual void setFilterType(ACC2::AllocnType type);
    virtual void updateAllocns(RB_ObjectContainer* transDocList);

public slots:
    virtual void slotSourceFilterChange(int number);

private:
    void setFilter(ACC2::AllocnType type);

    RB_String mCustomerId;
    RB_String mSupplierId;

};

#endif // ACC_SELECTALLOCNDIALOG_H

