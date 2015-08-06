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
    addMember("stockcode", "-", "<NEW>", RB2::MemberChar20);      // 3 or stockcode
    addMember("description", "-", "", RB2::MemberChar125);
    addMember("longdescription", "-", "", RB2::MemberChar2500);
    addMember("unitofmeasurement", "-", "", RB2::MemberChar40);
    addMember("mbflag", "-", "", RB2::MemberChar10);
    addMember("lastcurcostdate", "-", "", RB2::MemberDateTime);
    addMember("actualcost", "-", 0.0, RB2::MemberDouble);
    addMember("lastcost", "-", 0.0, RB2::MemberDouble);
    addMember("materialcost", "-", 0.0, RB2::MemberDouble);
    addMember("labourcost", "-", 0.0, RB2::MemberDouble);
    addMember("overheadcost", "-", 0.0, RB2::MemberDouble);
    addMember("lowestlevel", "-", 0, RB2::MemberInteger);
    addMember("discontinued", "-", 0, RB2::MemberInteger);
    addMember("controlled", "-", 0, RB2::MemberInteger);
    addMember("eoq", "-", 0.0, RB2::MemberDouble); // economic order quantity
    addMember("volume", "-", 0.0, RB2::MemberDouble);
    addMember("kgs", "-", 0.0, RB2::MemberDouble);
    addMember("barcode", "-", "", RB2::MemberChar125);
    addMember("discountcategory", "-", "", RB2::MemberChar10);
//    addMember("taxcat_id", "-", "0", RB2::MemberChar40);
    addMember("serialised", "-", 0, RB2::MemberInteger);
    addMember("stockimage", "-", "", RB2::MemberByteArray);
    addMember("perishable", "-", 0, RB2::MemberInteger);
    addMember("decimalplaces", "-", 0, RB2::MemberInteger);
    addMember("serialno", "-", "", RB2::MemberChar40);
    addMember("pansize", "-", 0.0, RB2::MemberDouble);
    addMember("shrinkfactor", "-", 0.0, RB2::MemberDouble);
    addMember("netweight", "-", 0.0, RB2::MemberDouble);
    addMember("costcenter_idx", "-", "0", RB2::MemberChar165);
}
