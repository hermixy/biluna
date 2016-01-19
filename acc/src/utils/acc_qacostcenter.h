/*****************************************************************
 * $Id: acc_qacostcenter.h 2202 2016-01-07 19:39:36Z rutger $
 * Created: Jan 07, 2016 5:34:25 PM - rutger
 *
 * Copyright (C) 2016 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef ACC_QACOSTCENTER_H
#define ACC_QACOSTCENTER_H

//#include <iostream>
//#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"
#include "rb_string.h"
#include "rb_utility.h"


#define ACC_QACOSTCENTER ACC_QaCostCenter::getInstance()


/**
 * Quick Access to cost center Instance (static) class,
 * this class is instantiated once and can be accessed with ACC_QACOSTCENTER
 * The cost center is queried many times for different purposes,
 * this class is used for example during processing of invoices and for
 * general accounting/booking of invoices
 * Utililities are handled independently with utility factory
 */
class ACC_QaCostCenter : public RB_Utility {

public:
    virtual ~ACC_QaCostCenter();
    static ACC_QaCostCenter* getInstance();

    RB_ObjectBase* getCostCenterObj(const RB_String& costCenterId);
    RB_ObjectContainer* getCostCenterList();
    void refresh();


private:
    ACC_QaCostCenter();
    void initMemberValues();
    void setCostCenterList();

    static ACC_QaCostCenter* mActiveUtility;

    //! Cost center default ID
    RB_String mCostCenterDefaultId;
    //! Cost center default name/description
    RB_String mCostCenterDefaultName;

    //! Cost Center list. NOTE: parent is modelroot (=ACC_Project)
    RB_ObjectContainer* mCostCenterList;
    //! Map of chartmaster (GL accounts) id and description
    std::map<RB_String, RB_ObjectBase*> mCostCenterMap;

};

#endif // ACC_QACOSTCENTER_H
