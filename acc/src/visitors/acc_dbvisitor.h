/*****************************************************************
 * $Id: acc_dbvisitor.h 1932 2013-05-02 12:18:08Z rutger $
 * Created: Jan 16, 2012 12:53:43 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_DBVISITOR_H
#define ACC_DBVISITOR_H

#include <map>
#include "rb_dbvisitor.h"
#include "rb_string.h"

/**
 * Visitor class for reading and writing from/to the database.
 * NOTE: if the system settings already exist the ACC_SysSetting children
 * will not be written to database, if the project settings (ACC_ChartMaster)
 * exist for this project also the chart master records will not be
 * written to the database
 */
class ACC_DbVisitor : public RB_DbVisitor {
	
public:
    ACC_DbVisitor();
    virtual ~ACC_DbVisitor() {}

    virtual void visitObject(RB_ObjectBase* obj);
    virtual void leaveObject(RB_ObjectBase* obj);

    void init();
    bool isSystemSettingExisting();
    bool isProjectSettingExisting();

protected:
    virtual bool dbUpdateList();

private:
    bool isInitDone();

    RB_String mProjectId;
    bool mIsInitDone;
    bool mIsSysSettingExisting;
    bool mIsProjectSettingExisting;

    RB2::ResolveLevel mMemoryResolveLevel;

//    std::map<RB_String, RB_String> mGroupMap;
};

#endif /*ACC_DBVISITOR_H*/
