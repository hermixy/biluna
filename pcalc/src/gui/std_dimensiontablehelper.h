/*****************************************************************
 * $Id: std_dimensiontablehelper.h 2016-07-22T11:39:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_DIMENSIONTABLEHELPER_H
#define STD_DIMENSIONTABLEHELPER_H

#include "rb_mmproxy.h"

class STD_DimensionTableHelper {

public:
    explicit STD_DimensionTableHelper();
    virtual ~STD_DimensionTableHelper();

    void setBoltDetailTables(const QModelIndex& current,
                             const QModelIndex& /*previous*/);
    void setFlangeDetailTables(const QModelIndex& current,
                               const QModelIndex& /*previous*/);
    void setNutDetailTables(const QModelIndex& current,
                            const QModelIndex& /*previous*/);

protected:
    bool isCodeStartWith(const QString& startString, const QModelIndex& current);
    bool isCurrentStandardIndexValid(const QModelIndex& current);

    RB_MmProxy* mComponentModel;    // flange dimensions
    RB_MmProxy* mEndModel;          // flange facing or ends
    RB_MmProxy* mLimitModel;        // flange rating and size limits for selected type
    RB_MmProxy* mRatingModel;       // interface list of all possible flange ratings
    RB_MmProxy* mSerieModel;        // flange facing types
    RB_MmProxy* mStandardModel;     // dimension standards
    RB_MmProxy* mTypeModel;         // flange types

};

#endif // STD_DIMENSIONTABLEHELPER_H
