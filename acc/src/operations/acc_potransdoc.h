/*****************************************************************
 * $Id: acc_potransdoc.h 1932 2013-05-02 12:18:08Z rutger $
 * Created: Sep 26, 2011 8:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_POTRANSDOC_H
#define ACC_POTRANSDOC_H

#include <QObject>
#include "acc_operation.h"

/**
 * Purchase order transaction class, creating the document and transactions
 * of the purchase order. This operation is called from the purchase order window
 * and sets the default taxes as per setting before previewing the invoice
 * or processing the invoice.
 */
class ACC_PoTransDoc : public QObject, public ACC_Operation  {

    Q_OBJECT

public:
    ACC_PoTransDoc();
    virtual ~ACC_PoTransDoc();

//    virtual bool execute(RB_ObjectBase* transDoc,
//                         RB_ObjectContainer* ctList,
//                         bool isPost =  false);

    // new
    virtual bool preparePreview(RB_ObjectBase* transDoc,
                                RB_ObjectContainer* ctList);
    virtual bool preparePost(RB_ObjectBase* transDoc);

private:

};

#endif // ACC_POTRANSDOC_H
