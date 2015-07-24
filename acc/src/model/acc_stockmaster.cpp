/*****************************************************************
 * $Id: acc_stockmaster.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: Jan 16, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_stockmaster.h"

ACC_StockMaster::ACC_StockMaster (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_StockMaster::ACC_StockMaster(ACC_StockMaster* uom) : RB_ObjectContainer(uom) {
    createMembers();
    *this = *uom;
}


ACC_StockMaster::~ACC_StockMaster() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - stockcode stock code usually capitals
 * - description description of the stock item
 * - longdescription long description
 * - unitofmeasurement unit of measurement
 * - mbflag assembly, kit, manufactured, service/labor flag
 * - lastcurcostdate last current cost date
 * - actualcost actual cost
 * - lastcost last cost price
 * - materialcost material cost, price used when creating a purchase order
 * - labourcost labor cost
 * - overheadcost overhead cost
 * - lowestlevel lowest stock level
 * - discontinued discontinued stock item
 * - controlled batch serial or lot controlled
 * - eoq economic order quantities
 * - volume volume of stock item
 * - kgs packaged weight in kilograms
 * - barcode bar code
 * - discountcategory discount category
 * - serialised 1 if stock item has an automtic serial number,
 *    the serial number (integer) will be generated automatically
 * - appendfile file with information about the stock item
 * - perishable 1 if stock item is perishable (can rot or decay like fresh food)
 * - decimalplaces number of decimal places for quantity
 * - serialno serial number (string) to be used instead of automatic updated if serialized
 * - pansize size of pan (container) in automated warehouse
 * - shrinkfactor shrink factor when in vacuum
 * - netweight net weight
 * - costcenter_idx cost center ID and description
 */
void ACC_StockMaster::createMembers() {
    addMember(this, "stockcode", "-", "<NEW>", RB2::MemberChar20);      // 3 or stockcode
    addMember(this, "description", "-", "", RB2::MemberChar125);
    addMember(this, "longdescription", "-", "", RB2::MemberChar2500);
    addMember(this, "unitofmeasurement", "-", "", RB2::MemberChar40);
    addMember(this, "mbflag", "-", "", RB2::MemberChar10);
    addMember(this, "lastcurcostdate", "-", "", RB2::MemberDateTime);
    addMember(this, "actualcost", "-", 0.0, RB2::MemberDouble);
    addMember(this, "lastcost", "-", 0.0, RB2::MemberDouble);
    addMember(this, "materialcost", "-", 0.0, RB2::MemberDouble);
    addMember(this, "labourcost", "-", 0.0, RB2::MemberDouble);
    addMember(this, "overheadcost", "-", 0.0, RB2::MemberDouble);
    addMember(this, "lowestlevel", "-", 0, RB2::MemberInteger);
    addMember(this, "discontinued", "-", 0, RB2::MemberInteger);
    addMember(this, "controlled", "-", 0, RB2::MemberInteger);
    addMember(this, "eoq", "-", 0.0, RB2::MemberDouble); // economic order quantity
    addMember(this, "volume", "-", 0.0, RB2::MemberDouble);
    addMember(this, "kgs", "-", 0.0, RB2::MemberDouble);
    addMember(this, "barcode", "-", "", RB2::MemberChar125);
    addMember(this, "discountcategory", "-", "", RB2::MemberChar10);
//    addMember(this, "taxcat_id", "-", "0", RB2::MemberChar40);
    addMember(this, "serialised", "-", 0, RB2::MemberInteger);
    addMember(this, "stockimage", "-", "", RB2::MemberByteArray);
    addMember(this, "perishable", "-", 0, RB2::MemberInteger);
    addMember(this, "decimalplaces", "-", 0, RB2::MemberInteger);
    addMember(this, "serialno", "-", "", RB2::MemberChar40);
    addMember(this, "pansize", "-", 0.0, RB2::MemberDouble);
    addMember(this, "shrinkfactor", "-", 0.0, RB2::MemberDouble);
    addMember(this, "netweight", "-", 0.0, RB2::MemberDouble);
    addMember(this, "costcenter_idx", "-", "0", RB2::MemberChar165);
}
