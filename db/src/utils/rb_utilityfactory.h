/*****************************************************************
 * $Id: rb_utilityfactory.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 27, 2011 6:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_UTILITYFACTORY_H
#define RB_UTILITYFACTORY_H

#include <QVariant>
#include "db_global.h"
#include "rb_aesencryption.h"
#include "../../../biluna_private/db/src/utils/rb_utilityfactory_private.h"

class RB_Utility;


/**
 * Abstract utility factory class, for the creation and deletion
 * of all utility managers
 */
class DB_EXPORT RB_UtilityFactory
        : public RB_UtilityFactory_Private {

public:
    RB_UtilityFactory();
    virtual ~RB_UtilityFactory();
	
    QString encrypt(const QString& str);
    QString decrypt(const QVariant& var);

    virtual void addUtility(RB_Utility* util);
    virtual void removeUtility(RB_Utility* util);

    virtual void refresh();

protected:
    std::vector<RB_Utility*> mUtilityVector;

private:
    bool mIsEncryption;
    RB_AesEncryption mAes;

};

#endif // RB_UTILITYFACTORY_H
