/*****************************************************************
 * $Id: db_actionsystemtoolsdevelop.cpp 2143 2014-04-24 20:21:43Z rutger $
 * Created: Dec 17, 2011 1:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemtoolsdevelop.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_mainwindow.h"

#include "db_version.h"
#include "db_objectfactory.h"
#include "rb_aesencryption.h"

#define A(c)            (c) - 0x17
#define UNHIDE_STR(str) do { char *p = str;  while (*p) *p++ += 0x17; } while (0)
#define HIDE_STR(str)   do { char *p = str;  while (*p) *p++ -= 0x17; } while (0)


DB_ActionSystemToolsDevelop::DB_ActionSystemToolsDevelop() : RB_Action() {

}


RB_GuiAction* DB_ActionSystemToolsDevelop::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionSystemToolsDevelop::getName(),
                                  // "fileclose",
                                  tr("&Develop"));
        ga->setStatusTip(tr("Develop, debug and test only"));
//        ga->setShortcut("Ctrl+W");
        ga->setFactory(DB_ActionSystemToolsDevelop::factory);
    }

    return ga;
}

RB_Action* DB_ActionSystemToolsDevelop::factory() {
    RB_Action* a = new DB_ActionSystemToolsDevelop();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemToolsDevelop::trigger() {
    testDbReadWhere();
    // DB_DIALOGFACTORY->requestWarningDialog(tr("This function is disabled."));
}

void DB_ActionSystemToolsDevelop::testEncryption() {
    /*
    (From: http://etutorials.org/Programming/secure+programming/Chapter+12.+Anti-Tampering/12.11+Hiding+Strings/)

    Strings can also be stored encrypted in the binary and in memory. You can
    achieve this by generating separate object files with the encrypted strings
    in them, by encrypting the strings in the binary after compilation, or by
    initializing the strings with encrypted characters. The following code
    demonstrates the last technique, using the A macro to subtract a constant
    value from each character in the string. Note that this is not a strong
    encryption method, but rather a quick and simple obfuscation of the value of each character.

    #define A(c)            (c) - 0x19
    #define UNHIDE_STR(str) do { char *p = str;  while (*p) *p++ += 0x19; } while (0)
    #define HIDE_STR(str)   do { char *p = str;  while (*p) *p++ -= 0x19; } while (0)

    Each character of the string must be initialized, which makes this method
    somewhat cumbersome, but it allows the obfuscation to take place at
    compile time:

    #include

    int main(int argc, char *argv[  ]) {
      char str[  ] = {
        A('/'), A('e'), A('t'), A('c'), A('/'),
        A('p'), A('a'), A('s'), A('s'), A('w'), A('d'), 0
      };

      UNHIDE_STR(str);
      printf("%s\n", str);
      HIDE_STR(str);

      return 0;
    }
    */
    char cstr[  ] = {
      A('/'), A('e'), A('t'), A('c'), A('/'),
      A('p'), A('a'), A('s'), A('s'), A('w'), A('d'), 0
    };

    printf("%s\n", cstr);
    UNHIDE_STR(cstr);
    printf("%s\n", cstr);
    HIDE_STR(cstr);
    printf("%s\n", cstr);
    return;

    RB_AesEncryption aes;
    // aes.test("rutger");
    aes.init("password");
    QByteArray baTest3 = aes.encrypt("This is a much longer story, \nlet's see "
                                      "whether this works also. Here are some "
                                      "numbers 0123456789. Not to shocking "
                                      "+_)(*&^%$#@!~ OK.");
    RB_DEBUG->print(QLatin1String(baTest3));
    RB_String strTest4 = aes.decrypt(baTest3);
    RB_DEBUG->print(strTest4);
}

void DB_ActionSystemToolsDevelop::testIdxSplit() {
    // String starts counting from 0, SQL starts with 1
    RB_String strTest = "{00000000-0000-0000-0000-000000ID5100}And some text";
    RB_String strTest1 = strTest;
    RB_String strTest2 = strTest;
    strTest1 = strTest1.remove(38, strTest1.length());
    strTest2 = strTest2.remove(0, 38);
    RB_DEBUG->print(strTest);
    RB_DEBUG->print(strTest1);
    RB_DEBUG->print(strTest2);
}

void DB_ActionSystemToolsDevelop::updateDatabaseIdxFields() {
    // Update database
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QStringList list;
    list << "ACC_BankTrans;chartmaster_idx"
    << "ACC_CogsGlPostings;glcode_idx"
    << "ACC_CreditorTrans;chartmaster_idx"
    << "ACC_CreditorTrans;taxhighchartmaster_idx"
    << "ACC_CreditorTrans;taxlowchartmaster_idx"
    << "ACC_CreditorTrans;taxotherchartmaster_idx"
    << "ACC_DebtorTrans;chartmaster_idx"
    << "ACC_DebtorTrans;taxhighchartmaster_idx"
    << "ACC_DebtorTrans;taxlowchartmaster_idx"
    << "ACC_DebtorTrans;taxotherchartmaster_idx"
    << "ACC_GlTrans;chartmaster_idx"
    << "ACC_Project;debtorsact_idx"
    << "ACC_Project;creditorsact_idx"
    << "ACC_Project;payrollact_idx"
    << "ACC_Project;grnact_idx"
    << "ACC_Project;retainedearnings_idx"
    << "ACC_Project;freightact_idx"
    << "ACC_Project;exchangediffact_idx"
    << "ACC_Project;purchasesexchangediffact_idx"
    << "ACC_Project;pytdiscountact_idx"
    << "ACC_SalesGlPostings;discountglact_idx"
    << "ACC_SalesGlPostings;salesglact_idx"
    << "ACC_StockCategory;stockact_idx"
    << "ACC_StockCategory;adjglact_idx"
    << "ACC_StockCategory;purchpricevaract_idx"
    << "ACC_StockCategory;materialuseagevarac_idx"
    << "ACC_StockCategory;wipact_idx"
    << "ACC_TaxAuthorities;salestaxglacct_idx"
    << "ACC_TaxAuthorities;purchtaxglacct_idx"
    << "ACC_TaxAuthRates;taxacct_idx";

    QSqlDatabase db = DB_MODELFACTORY->getDatabase();

    RB_ObjectContainer* objList = new RB_ObjectContainer("", NULL, "ACC_ChartMasterList");
    objList->setValue("parent", "ID0001");
    bool success = objList->dbReadList(db, RB2::ResolveOne);

    if (!success) {
        QApplication::restoreOverrideCursor();
        delete objList;
        return;
    }

    QSqlQuery qRead(db);
    QSqlQuery qWrite(db);
    QSqlRecord rec;
    int count = 0;
    RB_String strValue;

    QString strObjName; // = list.at(1).split(";").at(0);
    QString strMemName; // = list.at(1).split(";").at(1);
    QString str;

    // Step through list
    for (int j = 0; j < list.count(); ++j) {
        strObjName = list.at(j).split(";").at(0);
        strMemName = list.at(j).split(";").at(1);

        // Select all from table
        QString strSql = "SELECT * FROM " + strObjName.toLower();
        success = qRead.exec(strSql);
        if (!success) {
            QApplication::restoreOverrideCursor();
            RB_DEBUG->error("DB_ActionSystemToolsDevelop::trigger()");
            RB_DEBUG->print("  " + strObjName + " read ERROR");
            delete objList;
            return;
        }

        // Step through records
        while (qRead.next())  {
            // Step through record fields
            rec = qRead.record();
            count = rec.count();
            strValue = "";
            str = "";

            // Get value of field/member
            for (int i = 0; i < count; ++i) {
                if (rec.field(i).name() == strMemName) {
                    strValue = rec.field(i).value().toString();
                }
            }

            RB_ObjectIterator* iter = objList->createIterator();
            for (iter->first(); !iter->isDone(); iter->next()) {
                RB_ObjectBase* obj = iter->currentObject();

                if (strValue == obj->getId()) {
                    str = "{00000000-0000-0000-0000-000000"
                            + obj->getId() + "}"
                            + obj->getValue("accountcode").toString() + " - "
                            + obj->getValue("accountname").toString();
                }


            }
            delete iter;

            // Set new value of field
            if (!str.isEmpty() && !strValue.isEmpty()) {
                strSql = "UPDATE " + strObjName.toLower() + " SET "
                        + strMemName + "='" + str + "' WHERE id='"
                        + rec.field(0).value().toString() + "';";
                if (!qWrite.exec(strSql)) {
                    RB_DEBUG->error("DB_ActionSystemToolsDevelop::trigger()");
                    RB_DEBUG->print("  " + strObjName + " write ERROR");
                } // if
            } // if
        } // while(qRead.next)
    } // for (j)

    delete objList;

    QApplication::restoreOverrideCursor();
}

void DB_ActionSystemToolsDevelop::testDbReadWhere() {
/*
 select * from db_version where
 (id = '{78df8f92-21ab-4742-b53f-1210e50b8451}' AND perspectivecode = 'DB') OR
 (id='{18fb4cfd-d4ce-494b-8bc6-90b0e7af020a}' AND perspectivecode = 'DB') OR
 (id='{daa41183-8a36-473d-a596-fca5ad4ad51e}' AND minor = 14)
*/
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        return;
    }

    RB_ObjectContainer* versionList =
            new RB_ObjectContainer("", nullptr, "DB_VersionList", DB_OBJECTFACTORY);
    versionList->dbReadWhere(DB_MODELFACTORY->getDatabase(),
                             "(id = '{78df8f92-21ab-4742-b53f-1210e50b8451}' AND perspectivecode = 'DB') OR "
                             "(id='{18fb4cfd-d4ce-494b-8bc6-90b0e7af020a}' AND perspectivecode = 'DB') OR "
                             "(id='{daa41183-8a36-473d-a596-fca5ad4ad51e}' AND minor = 14)");
    RB_DEBUG->printObject(versionList, RB2::ResolveAll);
    delete versionList;
}


