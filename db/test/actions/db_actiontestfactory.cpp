/*****************************************************************
 * $Id: db_actiontestfactory.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestfactory.h"
#include "db_objectfactory.h"
#include "db_testdialogfactory.h"
#include "db_testobjectfactory.h"
#include "rb_mdiwindow.h"
#include "rb_objectcontainer.h"


DB_ActionTestFactory::DB_ActionTestFactory(DB_TestTextWidget* tw)
        : RB_Action() {
    mTextWidget = tw;
}


RB_GuiAction* DB_ActionTestFactory::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga == NULL) {
        ga = new RB_GuiAction(DB_ActionTestFactory::getName(),
                                  "fileclose",
                                  tr("Object &Factory"));
        ga->setStatusTip(tr("Test object factory"));
        ga->setCommand("factory, obf");
        ga->setKeycode("of");
        ga->setStatusTip(tr("Test object factory"));
        ga->setShortcut("Ctrl+F");
        ga->setToolTip("Test object factory");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestFactory::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestFactory::factory() {
    RB_MdiWindow* mdiWin = DB_TESTDIALOGFACTORY->getMdiWindow(
            DB_TestDialogFactory::WidgetTestText);
    DB_TestTextWidget* tw = dynamic_cast<DB_TestTextWidget*>(mdiWin->getWidget());

    if (mdiWin && tw) {
        RB_Action* a = new DB_ActionTestFactory(tw);
        // no graphicsView with eventhandler which deletes the action
        a->trigger();
        delete a;
        a = NULL;
        // show window
        mdiWin->show();
    }
    return NULL;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestFactory::trigger() {
    mTextWidget->slotClearText();
    bool writeMembers = false;
    RB_ObjectBase* root = NULL;
    RB_String str = "";

//    root = DB_OBJECTFACTORY->newObject("", NULL, "");
//
//    RB_ObjectContainer* objC = dynamic_cast<RB_ObjectContainer*>(root);
//    objC = objC->getContainer(0); // DB_MapList
//    RB_ObjectBase* obj = DB_OBJECTFACTORY->newObject("", objC, "");
//
//    objC = dynamic_cast<RB_ObjectContainer*>(obj);
//    objC = objC->getContainer(0); // DB_MappingList
//    obj = DB_OBJECTFACTORY->newObject("", objC, "");
//
//    str = "MainWindow::testFactory() (1):";
//    writeModel(str, root, writeMembers);
//    mTextWidget->appendText(str);
//    delete root;
//    root = NULL;

    str = "MainWindow::testFactory() (2):";
    root = DB_OBJECTFACTORY->getModelStructure("DB_Project");
    writeModel(str, root, writeMembers);
    mTextWidget->appendText(str);

    str = "MainWindow::testFactory() (3):";
    RB_ObjectBase* clone = root->copy(); // same as resolve none
    writeModel(str, clone, writeMembers);
    mTextWidget->appendText(str);
    delete clone;
    clone = NULL;

    str = "MainWindow::testFactory() (4):";
    clone = root->clone(RB2::ResolveOne);
    writeModel(str, clone, writeMembers);
    mTextWidget->appendText(str);
    delete clone;
    clone = NULL;

    str = "MainWindow::testFactory() (5):";
    clone = root->copy(RB2::ResolveTwo);
    writeModel(str, clone, writeMembers);
    mTextWidget->appendText(str);
    delete clone;
    clone = NULL;

    str = "MainWindow::testFactory() (6) container:";
    RB_ObjectBase* objC = root->getContainer(0); // DB_MapList
    clone = objC->copy(RB2::ResolveTwo);
    writeModel(str, clone, writeMembers);
    mTextWidget->appendText(str);
    delete clone;
    clone = NULL;

    delete DB_OBJECTFACTORY;

    str = "MainWindow::testFactory() (7) DB_TESTOBJECTFACTORY:";
    root = DB_TESTOBJECTFACTORY->getModelStructure("DB_TestProject");
    writeModel(str, root, writeMembers);
    mTextWidget->appendText(str);

    delete DB_TESTOBJECTFACTORY;
}

/**
 * Writes the model to the string
 * @param str string
 * @param obj object
 * @param writeMembers write members if true
 */
void DB_ActionTestFactory::writeModel(RB_String& str, RB_ObjectBase* obj, bool writeMembers) {
    RB_DEBUG->print("DB_ActionTestObject::writeModel()" + obj->getName());

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
