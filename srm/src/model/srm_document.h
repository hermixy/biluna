/*****************************************************************
 * $Id: srm_document.h 2071 2014-01-22 21:56:32Z rutger $
 * Created: Jan 22, 2014 14:24:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_DOCUMENT_H
#define SRM_DOCUMENT_H

#include "rb_objectatomic.h"

/**
 * Class for text document such as a regular letter
 */
class SRM_Document : public RB_ObjectAtomic {

public:
    SRM_Document(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_Document(SRM_Document* obj);
    virtual ~SRM_Document();

private:
    void createMembers();

};

#endif // SRM_DOCUMENT_H
