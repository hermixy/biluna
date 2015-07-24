/*****************************************************************
 * $Id: scan_block.h 1407 2011-05-04 20:13:03Z rutger $
 * Created: 2011-05-04 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_BLOCK_H
#define SCAN_BLOCK_H

#include "rb_objectcontainer.h"


/**
 * Class that contains the block text for a report,
 * can be a single title, paragraph or part of a paragraph
 * with maximum one image.
 * The blocks will be shown depending the scan results
 */
class SCAN_Block : public RB_ObjectContainer {

public:
    SCAN_Block(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SCAN_Block(SCAN_Block* b);
    virtual ~SCAN_Block();

private:
    void createMembers();

};

#endif /*SCAN_BLOCK_H*/
