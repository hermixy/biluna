/*****************************************************************
 * $Id: acc_stockcategory.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Jan 16, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_stockcategory.h"

ACC_StockCategory::ACC_StockCategory (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_StockCategory::ACC_StockCategory(ACC_StockCategory* uom) : RB_ObjectContainer(uom) {
    createMembers();
    *this = *uom;
}


ACC_StockCategory::~ACC_StockCategory() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Members are:
 * - categorycode, an abbreviation for easy reference
 * - categorydescription, the description of the category of stock items
 * - stocktype, type of stock such as raw material, finished product, purchased of non-stock (manhours)
 * - stockact_idx, stock gl (general ledger) account
 * - adjglact_idx, stock adjustment gl account
 * - purchpricevaract_idx, purchase price variance gl account
 * - materialuseagevarac_idx, material usage variance gl account
 * - wipact_idx, work in progress gl account
 */
void ACC_StockCategory::createMembers() {
    addMember(this, "categorycode", "-", "<NEW>", RB2::MemberChar20);    // 3
    addMember(this, "categorydescription", "-", "", RB2::MemberChar125);
    addMember(this, "stocktype", "-", 0, RB2::MemberInteger);    // int_id
    addMember(this, "taxcat_id", "-", "0", RB2::MemberChar40);
    addMember(this, "stockact_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "adjglact_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "purchpricevaract_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "materialuseagevarac_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "wipact_idx", "-", "0", RB2::MemberChar165);
}
