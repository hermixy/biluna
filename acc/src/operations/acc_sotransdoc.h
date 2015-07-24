/*****************************************************************
 * $Id: acc_sotransdoc.h 1932 2013-05-02 12:18:08Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SOTRANSDOC_H
#define ACC_SOTRANSDOC_H

#include <QObject>
#include "acc_operation.h"

/**
 * Sales order transaction class, creating the document and transactions
 * of the sales order. This operation is called from the sales order window
 * and sets the default taxes as per setting before previewing the invoice
 * or processing the invoice.
 */
class ACC_SoTransDoc : public QObject, public ACC_Operation  {

    Q_OBJECT

public:
    ACC_SoTransDoc();
    virtual ~ACC_SoTransDoc();

/*    virtual bool execute(RB_ObjectBase* transDoc,
                         RB_ObjectContainer* dtList,
                         bool isPost =  false); */
    // new
    virtual bool preparePreview(RB_ObjectBase* transDoc,
                                RB_ObjectContainer* dtList);
    virtual bool preparePost(RB_ObjectBase* transDoc);

private:

};

#endif // ACC_SOTRANSDOC_H
