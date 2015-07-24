/*****************************************************************
 * $Id: acc_stockmovetax.cpp 1800 2012-11-13 20:22:10Z rutger $
 * Created: Mar 25, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_stockmovetax.h"

ACC_StockMoveTax::ACC_StockMoveTax (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_StockMoveTax::ACC_StockMoveTax(ACC_StockMoveTax* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_StockMoveTax::~ACC_StockMoveTax() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void ACC_StockMoveTax::createMembers() {
    RB_String uuid = "";
    // conform webErp, except stkmoveno which is parent
    addMember(uuid, this, "taxauth_id", "-", "0", RB2::MemberChar40);      // 3 rel_id
    addMember(uuid, this, "taxrate", "-", 0.0, RB2::MemberDouble);
    addMember(uuid, this, "taxontax", "-", 0, RB2::MemberInteger);
    addMember(uuid, this, "taxcalculationorder", "-", 0, RB2::MemberInteger);
    // added:
    addMember(uuid, this, "salestaxglacct_id", "-", "0", RB2::MemberChar40);      // rel_id

/*
CREATE TABLE `stockmovestaxes` (
  `stkmoveno` int(11) NOT NULL DEFAULT '0',
  `taxauth_id` tinyint(4) NOT NULL DEFAULT '0',
  `taxrate` double NOT NULL DEFAULT '0',
  `taxontax` tinyint(4) NOT NULL DEFAULT '0',
  `taxcalculationorder` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`stkmoveno`,`taxauth_id`),
  KEY `taxauth_id` (`taxauth_id`),
  KEY `calculationorder` (`taxcalculationorder`),
  CONSTRAINT `stockmovestaxes_ibfk_1` FOREIGN KEY (`taxauth_id`) REFERENCES `taxauthorities` (`taxid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
*/
}
