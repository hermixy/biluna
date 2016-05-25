/*****************************************************************
 * $Id: db_actiontestobject.cpp 2011 2013-10-16 16:09:55Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestobject.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_mdiwindow.h"
#include "rb_objectcontainer.h"


DB_ActionTestObject::DB_ActionTestObject() : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionTestObject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionTestObject::getName(),
                                  "fileclose",
                                  tr("&Object"));
        ga->setCommand("data, do");
        ga->setKeycode("do");
        ga->setStatusTip(tr("Test data object"));
        ga->setShortcut("Ctrl+O");
        ga->setToolTip("Test data object");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestObject::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestObject::factory() {
    RB_Action* a = new DB_ActionTestObject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestObject::trigger() {
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetTestText);
    DB_TestTextWidget* tw = dynamic_cast<DB_TestTextWidget*>(mdiWin->getWidget());

    tw->slotClearText();
    bool writeMembers = true;

    RB_ObjectBase* origObj = new RB_ObjectContainer("origObjId", NULL, "objName", NULL);
    origObj->addMember("mem1Name", "N/mm2", 0.01, RB2::MemberDouble);
    origObj->addMember("mem2Name", "-", "content", RB2::MemberChar125);
    QString dtStr = QDateTime::currentDateTime().toString(Qt::ISODate);
    origObj->setValue("created", dtStr);
    origObj->setValue("changed", dtStr);

    RB_String str = "";
    writeModel(str, origObj, writeMembers);
    tw->appendText("\nOriginal object");
    tw->appendText(str);

    RB_ObjectBase* copyObj = new RB_ObjectContainer("copyObjId", NULL, "objName", NULL);
    *copyObj = *origObj;

    writeModel(str, copyObj, writeMembers);
    tw->appendText("\nCopy object");
    tw->appendText(str);

    delete copyObj;
    delete origObj;

    writeModelFunctionTest(tw);
    // show window
    mdiWin->show();
}

/**
 * Writes the model to the string
 * @param str string
 * @param obj object
 * @param writeMembers write members if true
 */
void DB_ActionTestObject::writeModel(RB_String& str, RB_ObjectBase* obj, bool writeMembers) {
    RB_DEBUG->print("DB_ActionTestObject::writeModel() "
                    + obj->getName());

    str += "\n" + obj->getId();
    str += " " + RB_DEBUG->pointerToString(obj);
    str += " " + RB_DEBUG->pointerToString(obj->getParent());
    str += " " + obj->getName();

    // write members with their attributes
    if (writeMembers) {
        for (int i = 0; i < obj->memberCount(); i++) {
            str += "\n    " + obj->getMember(i)->getName() + "="
                        + obj->getMember(i)->getValue().toString();
        }
    }

    // write children if children are atomic or container
    if (obj->isContainer()) {
        RB_ObjectIterator* iter = obj->createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            writeModel(str, iter->currentObject(), writeMembers);
        }
        delete iter;
    }
}

void DB_ActionTestObject::writeModelFunctionTest(DB_TestTextWidget* tw) {
    RB_MmProxy* m = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTest);
    m->setRoot("");
    RB_String str = "\n\nTODO: For ID = 111 value2 = " + m->getValue("111", "value2").toString();
    tw->appendText(str);
}
