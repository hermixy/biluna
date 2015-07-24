/*****************************************************************
 * $Id: acc_transdoc.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Sep 5, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_transdoc.h"


ACC_TransDoc::ACC_TransDoc (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_TransDoc::ACC_TransDoc(ACC_TransDoc* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_TransDoc::~ACC_TransDoc() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - doctype is the document type(transaction type) as per webERP: salesdefault 10?
 * - docname is the document name, corresponds with a GL account name
 * - salestype is the sales (order) type or pricelist as per webERP (tpe) or same as doctype for example sales orders
 * - transdate is the transaction date
 * - transno is the transaction order number not the same as invoice number
 * - debtor_idx is the debtor ID if applicable
 * - creditor_idx is the creditor ID if applicable
 * - asset_id is the relevant asset ID if applicable
 * - contract_id is the relevant contract ID if applicable
 * - refno is the external reference number such as supplier invoice number
 * - description is the short description of the document content
 * - totalamountrec is the total amount (to be) received on this document, also default amount, negative means credit
 * - totalamountpay is the total amount (to be) paid on this document
 * - totaltax is the total tax amount
 * - duedate is the due date of the order
 * - rate is the exchange rate of the order value to the company value
 * - diffonexch is the (expected) difference on exchange
 * - alloc is the amount allocated (paid or received) by bank or cash
 * - settled is the order settled, the total amount (to pay or to receive) is allocated
 * - hold is the status of this order on hold
 * - edisent is the edi data sent
 * - consignment is the TODO
 * - and link to sales orders members:
 * - salesorder_id is the id of the sales order
 */
void ACC_TransDoc::createMembers() {
    addMember(this, "doctype", "-", 0, RB2::MemberInteger);
    addMember(this, "docname", "-", "", RB2::MemberChar125);
    addMember(this, "salestype_id", "-", "0", RB2::MemberChar40);  // rel_id
    addMember(this, "transdate", "-", "2000-01-01T00:00:00", RB2::MemberDateTime);
    addMember(this, "transno", "-", 0, RB2::MemberInteger);
    addMember(this, "debtor_idx", "-", "0", RB2::MemberChar165);        // rel_id,
    addMember(this, "creditor_idx", "-", "0", RB2::MemberChar165);      // rel_id,
    addMember(this, "refno", "-", "", RB2::MemberChar40);
    addMember(this, "description", "-", "", RB2::MemberChar255);
    addMember(this, "totalamountrec", "-", 0.0, RB2::MemberDouble);
    addMember(this, "totalamountpay", "-", 0.0, RB2::MemberDouble);
    addMember(this, "totaltax", "-", 0.0, RB2::MemberDouble);
    addMember(this, "duedate", "-", "2000-01-01T00:00:00", RB2::MemberDateTime);
    addMember(this, "rate", "-", 1.0, RB2::MemberDouble);
    addMember(this, "diffonexch", "-", 0.0, RB2::MemberDouble);
    addMember(this, "alloc", "-", 0.0, RB2::MemberDouble);
    addMember(this, "settled", "-", 0, RB2::MemberInteger);
    addMember(this, "hold", "-", 0, RB2::MemberInteger);
    addMember(this, "edisent", "-", 0, RB2::MemberInteger);
    addMember(this, "consignment", "-", "", RB2::MemberChar20);
    // Link with sales/purchase order
    addMember(this, "order_id", "-", "0", RB2::MemberChar40);    // rel_id,
    addMember(this, "invoicecomment", "-", "", RB2::MemberChar255);
}
