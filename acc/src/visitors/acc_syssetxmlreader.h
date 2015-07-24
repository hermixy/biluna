/*****************************************************************
 * $Id: acc_syssetxmlreader.h 1517 2012-01-16 16:04:02Z rutger $
 * Created: Jan 16, 2012 9:53:43 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SYSSETXMLREADER_H
#define ACC_SYSSETXMLREADER_H

#include <stack>
#include <QXmlStreamReader>

class RB_ObjectBase;
class RB_ObjectFactory;

/**
 * Reader for XML system settings files.
 * If no system settings exist the settings will stored in the database,
 * the stettings will not be stored if settings exist already in the database.
 * Other ACC_Project settings will always be stored in the current project,
 * for example the ACC_ChartMaster.
 */
class ACC_SysSetXmlReader {

public:
    ACC_SysSetXmlReader(RB_ObjectBase* obj);
    virtual ~ACC_SysSetXmlReader() { }

    virtual bool read(QIODevice *device);
    virtual QString errorString() const;
    virtual void setNewId(bool isNewId);

protected:
    virtual void readXml();
    virtual void readListContainer();
    virtual void readObject();
    virtual void readMember();

    QXmlStreamReader mReader;
    QString mPerspective;
    QString mVersion;
    bool mIsNewId;
    bool mIsRootRead;

    RB_ObjectBase* mRoot;
    RB_ObjectFactory* mObjectFactory;

    //! Stack (memory) container of parent objects
    std::stack<RB_ObjectBase*> mObjectStack;

};

#endif // ACC_SYSSETXMLREADER_H
