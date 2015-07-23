/*****************************************************************
 * $Id: rb_guiaction.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 2, 2009 5:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_GUIACTION_H
#define RB_GUIACTION_H

#include <vector>
#include <QAction>
#include "db_global.h"
#include "rb_hash.h"
#include "rb_list.h"
#include "rb_string.h"
#include "rb_stringlist.h"

class RB_Action;
class RB_DialogFactoryInterface;

/**
 * GUI actions are abstract user interface actions that can be used to
 * define the function of a button, menu, ...
 *
 * Every GUI action object has a unique identifier string which is only used
 * to store user defined related information for the action (for example
 * user defined keycodes and commands that trigger the action).
 */
class DB_EXPORT RB_GuiAction : public QAction {

    Q_OBJECT

public:
    RB_GuiAction(const RB_String& id, const RB_String& text);
    RB_GuiAction(const RB_String& id, const QIcon& icon, const RB_String& text);
    RB_GuiAction(const RB_String& id, const RB_String& iconId, const RB_String& text);
    ~RB_GuiAction();

    RB_String getIdentifier();

    void init(const RB_String& text);
    void initTexts();

    RB_String getKeycodeString();

    void setToolTip(const RB_String& tip);
    void setStatusTip(const RB_String& tip);
    void setShortcut(const RB_String& shortcut);
    void setShortcut(const QKeySequence& shortcut);
    void setKeycode(const RB_String& kc);
    void setCommand(const RB_String& cmd);
    RB_String getMainCommand() {
        return mainCommand;
    }
    RB_String getIconId() {
        return iconId;
    }
    RB_StringList getKeycodeList() {
        return keycodeList;
    }
    RB_StringList getCommandList() {
        return commandList;
    }
    RB_StringList getShortcutList() {
        return shortcutList;
    }

    void setFactory(RB_Action* (*f)());

    // TODO: move static functions and containers to DB_ActionFactory?
    static bool triggerByKeycode(const RB_String& kc);
    static bool triggerByCommand(const RB_String& cmd);
    static QStringList getAvailableCommands(bool primaryOnly=false);
    static RB_StringList getAvailableCommands(const RB_String& start, bool primaryOnly=false);

    static RB_List<RB_GuiAction*> getActions() {
        return actions;
    }

    void addEnabledType(int type);

public slots:
    virtual RB_Action* slotTriggered();
    virtual void slotSetEnabled(int type);

private:
    static RB_Hash<RB_String, RB_GuiAction*> actionsByKeycode;
    static RB_Hash<RB_String, RB_GuiAction*> actionsByCommand;
    static RB_Hash<RB_String, RB_GuiAction*> actionsByPrimaryCommand;
    static RB_List<RB_GuiAction*> actions;

    RB_String identifier;
    RB_String iconId;
    RB_String mainCommand;
    RB_StringList commandList;
    RB_StringList keycodeList;
    RB_StringList shortcutList;
    RB_Action* (*factory)();

    /*! Relevant enabled type (such as widget type) list,
    for which this action should be active */
    std::vector<int> mEnabledList;

};

#endif
