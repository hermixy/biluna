/*****************************************************************
 * $Id: acc.h 2212 2015-01-28 16:45:01Z rutger $
 * Created: Sep 15, 2010 5:50:56 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC2_H
#define ACC2_H

/**
 * Class namespace for various enums along with some simple
 * wrapper methods for converting the enums to the Qt counterparts.
 */
class ACC2 {

public:

    /**
     * Allocation type, to customer- or supplier invoice
     */
    enum AllocnType {
        AllocnCustomer,
        AllocnSupplier
    };

    /**
     * Account control type for:
     * - document or book type (bank/cash, debtor, creditor, journal)
     * - sales and purchase accounts related to sales tax
     * - (sales) tax account type
     */
    enum ControlType {
        ControlNone = 0, // 'None', 0 (this last number (0) or other than 0 indicates the corresponding tax account)
        ControlDefault = 1, // 'standaard', 0 (required, used by the system if no other account is available)
        ControlYearOpening = 2, // 'jaar opening', 0
        ControlSalesAllTax = 9, // 'verkopen/omzet alle btw tarieven', 23
        ControlSalesNoTax = 10, // 'verkopen/omzet onbelast (rubriek 1e)', 0
        ControlSalesLowTax = 11, // 'verkopen/omzet laag (excl btw: rubriek 1b)', 21
        ControlSalesHighTax = 12, // 'verkopen/omzet hoog (excl btw: rubriek 1a)', 22
        ControlSalesOtherTax = 13, // 'verkopen/omzet overig (excl btw: rubriek 1c)', 23
        ControlSalesRevTax = 14, // 'verkopen/omzet verlegd (excl btw: rubriek 1e)', 24
        ControlPrivateUseNoTax = 16, // 'prive gebruik', 0
        ControlPrivateUseTax = 19, // 'prive gebruik (excl btw: rubriek 1d einde jaar)', 29
        ControlTaxPayLow = 21, // 'btw af te dragen laag (verkopen)', 0
        ControlTaxPayHigh = 22, // 'btw af te dragen hoog (verkopen)', 0
        ControlTaxPayOther = 23, // 'btw af te dragen overig (verkopen)', 0
        ControlTaxSalesPayRev = 24, // 'btw af te dragen verlegd (verkopen)', 0
        ControlTaxSalesRecRev = 25, // 'btw te vorderen verlegd (verkopen)', 0
        ControlTaxPayGlobLow = 26, // 'btw af te dragen globalisatie laag', 0
        ControlTaxPayGlobHigh = 27, // 'btw af te dragen globalisatie hoog', 0
        ControlTaxPayGlobOther = 28, // 'btw af te dragen globalisatie overig', 0
        ControlTaxPrivateUse = 29, // 'btw af te dragen prive gebruik', 0
        ControlPurchaseAllTax = 30, // 'inkopen/kosten alle btw tarieven (rubriek 5b)', 43
        ControlPurchaseLowTax = 31, // 'inkopen/kosten laag (excl btw: rubriek 5b)', 41
        ControlPurchaseHighTax = 32, // 'inkopen/kosten hoog (excl btw: rubriek 5b)', 42
        ControlPurchaseOtherTax = 33, // 'inkopen/kosten overig (excl btw: rubriek 5b)', 43
        ControlPurchaseRevTax = 34, // 'inkopen/kosten verlegd (excl btw: rubriek 2a)', 44
        ControlTaxRecLow = 41, // 'btw te vorderen laag (inkopen)', 0
        ControlTaxRecHigh = 42, // 'btw te vorderen hoog (inkopen)', 0
        ControlTaxRecOther = 43, // 'btw te vorderen overig (inkopen)', 0
        ControlTaxPurchPayRev = 44, // 'btw af te dragen verlegd (inkopen)', 0
        ControlTaxPurchRecRev = 45, // 'btw te vorderen verlegd (inkopen)', 0
        ControlPurchInEuLow = 61, // 'inkopen import binnen EU laag (rubriek 4b)', 66
        ControlPurchInEuHigh = 62, // 'inkopen import binnen EU hoog (rubriek 4b)', 66
        ControlPurchInEuOther = 63, // 'inkopen import binnen EU overig (rubriek 4b)', 66
        ControlTaxPurchInEu = 66, // 'btw te vorderen inkopen import binnen EU (rubriek 4b)', 0
        ControlPurchOutEuLow = 71, // 'inkopen import buiten EU laag (rubriek 4a)', 76
        ControlPurchOutEuHigh = 72, // 'inkopen import buiten EU hoog (rubriek 4a)', 76
        ControlPurchOutEuOther = 73, // 'inkopen import buiten EU overig (rubriek 4a)', 76
        ControlTaxPurchOutEu = 76, // 'btw te vorderen inkopen import buiten EU (rubriek 4a)', 0
        ControlSalesInEu = 80, // 'verkopen export binnen EU (rubriek 3b)', 0
        ControlSalesOutEu = 81, // 'verkopen export buiten EU (rubriek 3a)', 0
        ControlBadDebt = 82, // 'oninbare vorderingen', 0
        ControlCorrection = 83, // 'herrekening/correcties', 0
        ControlInstallInEu = 84, // 'installatie/televerkoop binnen EU', 0
        ControlSalesBpm = 85, // 'BPM verkoop', 0
        ControlPurchBpm = 86, // 'BPM inkoop', 0
        ControlStockBpm = 87, // 'BPM voorraad', 0
        ControlServiceInEu = 88, // 'dienstverlening binnen EU (rubriek 3b)', 0
        ControlSalesGlobLow = 91, // 'verkoop globalisatie laag (rubriek 1b)', 0
        ControlSalesGlobHigh = 92, // 'verkoop globalisatie hoog (rubriek 1a)', 0
        ControlSalesGlobOther = 93, // 'verkoop globalisatie overig (rubriek 1c)', 0
        ControlPurchGlobLow = 94, // 'inkoop globalisatie laag (rubriek 1b)', 0
        ControlPurchGlobHigh = 95, // 'inkoop globalisatie hoog (rubriek 1a)', 0
        ControlPurchGlobOther = 96, // 'inkoop globalisatie overig (rubriek 1c)', 0
        ControlCreditLimitSales = 100, // 'kredietbeperking (verkopen)', 0
        ControlCreditLimitPurch = 101, // 'kredietbeperking (inkopen)', 0
        ControlPayDiscount = 102, // 'betalingskorting', 0
        ControlSuspenseAccount = 110, // 'tussenrekening betalingen', 0
        ControlUnkownPayment = 111, // 'onbekende betalingen', 0
        ControlCashPayment = 120, // 'betaalwijze contant', 0
        ControlElectronicPayment = 121, // 'betaalwijze electronisch', 0
        ControlWorkCost = 185, // 'werkkosten regeling', 0
        ControlDebtorAccrued = 901, // 'Openstaande debiteuren (verkopen) van vorig boekjaar', 0
        ControlCreditorAccrued = 902, // 'Openstaande crediteuren (inkopen)van vorig boekjaar', 0
        ControlDebtor = 1001, // 'Debiteuren (verkopen)', 0
        ControlCreditor = 1002, // 'Crediteuren (inkopen)', 0
        ControlCash = 2000, // 'Kas', 0
        ControlBank = 2100, // 'Bank', 0
        ControlMemoBook = 3000, // 'Memoriaal', 0
        ControlUserDefined = 65536, // 2^16 'gebruiker gedefinieerd', 0
        ControlUserDefined1 = 65541, // 'gebruiker gedefinieerd 1', 0
        ControlUserDefined2 = 65542, // 'gebruiker gedefinieerd 2', 0
        ControlUserDefined3 = 65543, // 'gebruiker gedefinieerd 3', 0
        ControlUserDefined4 = 65544, // 'gebruiker gedefinieerd 4', 0
        ControlUserDefined5 = 65545, // 'gebruiker gedefinieerd 5', 0
        ControlUserDefined6 = 65546, // 'gebruiker gedefinieerd 6', 0
        ControlUserDefined7 = 65547, // 'gebruiker gedefinieerd 7', 0
        ControlUserDefined8 = 65548, // 'gebruiker gedefinieerd 8', 0
        ControlUserDefined9 = 65549 // 'gebruiker gedefinieerd 9', 0
    };

    /**
     * Period interval, used in ACC_ValueAssetHelper
     */
    enum IntervalType {
        IntervalNone = 0,
        IntervalMonth = 1,
        IntervalYear = 2
    };

    /**
     * Stock document serial number, order- or transaction document type.
     * Some are also used to record the sequence number of the document type.
     * Identifier for ACC_TransDoc.doctype e.g. 20, 40 are creditor documents
     * and 10, 30 are debtor documents
     * Identifier for ACC_SysType, member 'systypecode'.
     */
    enum TransType {
        TransNone = 0,
        TransBankCash = 13,         /**< Bank or cash transaction type */
        TransCreditor = 20,         /**< Creditor, purchase invoice or supplier transaction type */
        TransDebtor = 10,           /**< Debtor, sales invoice or customer transaction type */
        TransMemo = 5,              /**< Memorandum transaction or journal type */
        TransPurchOrder = 40,       /**< Purchase order (Note: not purchase invoice) */
        TransSalesOrder = 30,       /**< Sales order (Note: not sales invoice) */
        TransStockSerialNumber = 105/**< Stock serial number */
        /*
        10 sales invoice
        11 sales credit note
        12 sales receipt

        //

        0,Journal - GL,2
        1,Payment - GL,2
        2,Receipt - GL,0
        3,Standing Journal,0
        10,Sales Invoice,10800049
        11,Credit Note,3
        12,Receipt,3
        13,Bank Cash,0
        15,Journal - Debtors,0
        16,Location Transfer,22
        17,Stock Adjustment,17
        18,Purchase Order,2
        20,Purchase Invoice,17
        21,Debit Note,3
        22,Creditors Payment,4
        23,Creditors Journal,0
        25,Purchase Order Delivery,31
        26,Work Order Receipt,7
        28,Work Order Issue,7
        29,Work Order Variance,1
        30,Sales Order,10700002
        31,Shipment Close,26
        35,Cost Update,19
        36,Exchange Difference,1
        40,Work Order,14
        50,Opening Balance,0
        500,Auto Debtor Number,0
        */
    };


    /**
     * Unit of measurement type, used in for example
     * property (asset) valuation, to make distinction
     * between currency and other units
     */
    enum UnitType {
        UnitCurrency,
        UnitKilogram,
        UnitMeter,
        UnitMeterSquare
    };

    /**
     * Round double money value, 2.3450 and up is 2.35, below 2.3450 is 2.34
     * -2.3450 and down is -2.35, above -2.3450 is 2.34
     * @returns rounded money value in correct amount of cents
     */
    static double roundMoney(double amount) {
        int amtMille = (int)(amount * 1000);
        int remain = amtMille % 10;

        if (amtMille >= 0.0) {
            amtMille = remain >= 5 ? amtMille+(10-remain) : amtMille-remain;
        } else {
            amtMille = remain <= -5 ? amtMille-(10+remain) : amtMille-remain;
        }
        return ((double)amtMille) / 1000;
    }

};

#endif // ACC2_H
