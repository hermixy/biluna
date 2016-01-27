/*****************************************************************
 * $Id: db_createmodelobject.h 0001 2015-12-28 09:13:00Z rutger $
 * Created: Dec 28, 2015 10:14:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Database (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_CREATEMODELOBJECT_H
#define DB_CREATEMODELOBJECT_H

#include "rb_objectcontainer.h"
#include "rb_operation.h"

/**
 * @brief Create standard model object operation
 */
class DB_EXPORT DB_CreateModelObject : public RB_Operation {

public:
    DB_CreateModelObject();
    virtual ~DB_CreateModelObject() {}

    virtual bool execute(RB_ObjectBase* memberList);
    virtual bool execute(RB_ObjectBase* /*input*/,
                         RB_ObjectBase* /*output*/) { return false; }

    void setBaseObject(const QString& baseObjName);
    void setPerspective(const QString& perspCode);
    void setObjectName(const QString& objName);
    void setDescription(const QString& description);
    void setFilePath(const QString& filePath);

protected:
    void createHeaderContent();
    void createSourceContent(RB_ObjectBase* memberList);
    void setFileHeader(QString& content, const QString& fileExtension);
    void formatMemberName(QString& memberName);
    bool writeFiles();

    QString mBaseObjectName;
    QString mPerspectiveCode;
    QString mObjectName;
    QString mDescription;
    QString mFilePath;

    QString mHeaderContent;
    QString mSourceContent;

};

#endif // DB_CREATEMODELOBJECT_H
