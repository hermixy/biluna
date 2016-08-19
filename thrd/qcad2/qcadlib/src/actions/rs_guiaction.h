/****************************************************************************
** Copyright (C) 2006 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef RS_GUIACTION_H
#define RS_GUIACTION_H

#include <qaction.h>

#include "rs_dialogfactory.h"



/**
 * GUI actions are abstract user interface actions that can be used to
 * define the function of a button, menu, ...
 *
 * Every GUI action object has a unique identifier string which is only used
 * to store user defined related information for the action (for example
 * user defined keycodes and commands that trigger the action).
 */
class RS_GuiAction : public QAction {
    Q_OBJECT

public:
    RS_GuiAction(const RS_String& id, const RS_String& text);
    RS_GuiAction(const RS_String& id, const QIcon& icon, const RS_String& text);
    RS_GuiAction(const RS_String& id, const RS_String& iconId, const RS_String& text);
    ~RS_GuiAction(); 

    RS_String getIdentifier();

    void init(const RS_String& text);
    void initTexts();

    RS_String getKeycodeString();

    void setToolTip(const RS_String& tip);
    void setStatusTip(const RS_String& tip);
    void setShortcut(const RS_String& shortcut);
    void setShortcut(const QKeySequence& shortcut);
    void setKeycode(const RS_String& kc);
    void setCommand(const RS_String& cmd);
    RS_String getMainCommand() {
        return mainCommand;
    }
    RS_String getIconId() {
        return iconId;
    }
    RS_StringList getKeycodeList() {
        return keycodeList;
    }
    RS_StringList getCommandList() {
        return commandList;
    }
    RS_StringList getShortcutList() {
        return shortcutList;
    }

    void setFactory(RS_ActionInterface* (*f)());

    static bool triggerByKeycode(const RS_String& kc);
    static bool triggerByCommand(const RS_String& cmd);
    static RS_StringList getAvailableCommands(bool primaryOnly=false);
    static RS_StringList getAvailableCommands(const RS_String& start, bool primaryOnly=false);

    static RS_List<RS_GuiAction*> getActions() {
        return actions;
    }

public slots:
    RS_ActionInterface* slotTriggered();

private:
    static RS_Hash<RS_String, RS_GuiAction*> actionsByKeycode;
    static RS_Hash<RS_String, RS_GuiAction*> actionsByCommand;
    static RS_Hash<RS_String, RS_GuiAction*> actionsByPrimaryCommand;
    static RS_List<RS_GuiAction*> actions;

    RS_String identifier;
    RS_String iconId;
    RS_String mainCommand;
    RS_StringList commandList;
    RS_StringList keycodeList;
    RS_StringList shortcutList;
    RS_ActionInterface* (*factory)();
};

#endif
