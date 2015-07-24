/*****************************************************************
 * $Id: crm_document.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Jan 27, 2014 14:24:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_DOCUMENT_H
#define CRM_DOCUMENT_H

#include "rb_objectatomic.h"

/**
 * Class for text document such as a regular letter
 */
class CRM_Document : public RB_ObjectAtomic {

public:
    CRM_Document(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_Document(CRM_Document* obj);
    virtual ~CRM_Document();

private:
    void createMembers();

};

#endif // CRM_DOCUMENT_H
