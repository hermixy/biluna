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


#include "rs_guiaction.h"

#include "rs_debug.h"
#include "rs_settings.h"
    
RS_Hash<RS_String, RS_GuiAction*> RS_GuiAction::actionsByKeycode;
RS_Hash<RS_String, RS_GuiAction*> RS_GuiAction::actionsByCommand;
RS_Hash<RS_String, RS_GuiAction*> RS_GuiAction::actionsByPrimaryCommand;
RS_List<RS_GuiAction*> RS_GuiAction::actions;



/**
 * @param id: Id of action used in config files.
 * @param text: Menu text, tooltip text.
 */
RS_GuiAction::RS_GuiAction(const RS_String& id, const RS_String& text) :
    QAction(text, NULL), identifier(id) {
    
    init(text);
}



/**
 * @param id: Id of action used in config files.
 * @param iconId: E.g. 'linesnormal' for icon ':/qcad/images/linesnormal.png'.
 * @param text: Menu text, tooltip text.
 */
RS_GuiAction::RS_GuiAction(const RS_String& id, const RS_String& iconId, const RS_String& text) :
    QAction(QIcon(":/qcad/images/" + iconId + ".png"), text, NULL), identifier(id) {
    
    this->iconId = iconId;
    init(text);
}



/**
 * @param id: Id of action used in config files.
 * @param icon: Action icon shown on buttons and in menu.
 * @param text: Menu text, tooltip text.
 */
RS_GuiAction::RS_GuiAction(const RS_String& id, const QIcon& icon, const RS_String& text) :
    QAction(icon, text, NULL), identifier(id) {
    
    init(text);
}



RS_GuiAction::~RS_GuiAction() {
    // todo: remove this action from hashes and lists
}
    


RS_String RS_GuiAction::getIdentifier() {
    return identifier;
}
    

void RS_GuiAction::init(const RS_String& text) {
    factory = NULL;
    
    setText(text);

    initTexts();

    actions.append(this);
}


/**
 * Initializes the GUI action texts (for menus, tooltips, ...).
 */
void RS_GuiAction::initTexts() {
    RS_String textAndKeycode = text();
    QKeySequence sc = shortcut();

    // Remove previously added keycode:
#ifdef Q_OS_MACX
    if (textAndKeycode.indexOf(QLatin1Char(']'))!=-1) {
        textAndKeycode = textAndKeycode.mid(textAndKeycode.indexOf(']')+2);
    }
#else
    if (textAndKeycode.indexOf(QLatin1Char('\t'))!=-1) {
        textAndKeycode = textAndKeycode.left(textAndKeycode.indexOf('\t'));
    }
#endif

    RS_String textOnly = textAndKeycode;
    textOnly.replace('&', "");

    // Use tab to separate menu text from keycodes.
    //   The menu text with keycode looks like this: "Line\tli".
    //   This does not work under OS X. Text after tab (\t) is
    //   ignored.
    RS_String kcs = getKeycodeString();
    if (!kcs.isEmpty()) {
#ifdef Q_OS_MACX
        textAndKeycode = RS_String("[%1] %2").arg(kcs).arg(textAndKeycode);
#else
        textAndKeycode += QLatin1Char('\t');
        textAndKeycode += kcs;
#endif
    }

    setText(textAndKeycode);
    if (!kcs.isEmpty()) {
        QAction::setToolTip(textOnly + RS_String(" [%1]").arg(kcs));
    }
    else {
        QAction::setToolTip(textOnly);
    }
}
    
    
/**
 * @return String with keycode and shortcut, e.g. "uu,Ctrl-Z" for
 * Undo. The QCad.ini settings /Appearance/ShowKeycodes and
 * /Appearance/ShowShortcuts define whether keycodes and shortcuts
 * are displayed.
 */
RS_String RS_GuiAction::getKeycodeString() {
    RS_String ret;

    RS_SETTINGS->beginGroup("/Appearance");
    bool showKeycodes = (bool)RS_SETTINGS->readNumEntry("/ShowKeycodes", 
#ifdef Q_OS_MACX
    0
#else
    1
#endif
    );
    bool showShortcuts = (bool)RS_SETTINGS->readNumEntry("/ShowShortcuts", 0);
    RS_SETTINGS->endGroup();
    
    if (!showKeycodes && !showShortcuts) {
        return ret;
    }
    
    QKeySequence sc = shortcut();
    
    // keycode (usually two letter code):
    if (keycodeList.count()>0 && keycodeList.at(0).length()!=0 && showKeycodes) {
        ret += keycodeList.at(0);
    }
    
    // add shortcut (e.g. Ctrl+X):
    if (sc.toString().length()!=0 && showShortcuts) {
        if (keycodeList.count()>0 && keycodeList.at(0).length()!=0 && showKeycodes) {
            ret += QString(", ");
        }
        QList<QKeySequence> scs = shortcuts();
        for (int i=0; i<scs.count(); ++i) {
            ret += scs.at(i).toString();
            if (i!=scs.count()-1) {
                ret += QString(", ");
            }
        }
    }

    return ret;
}


/**
 * Sets the command(s) that can trigger this action from a command line.
 */
void RS_GuiAction::setCommand(const RS_String& cmd) {
    RS_SETTINGS->beginGroup("/Commands");
    commandList = RS_SETTINGS->readEntry(getIdentifier(), cmd).split(',', QString::SkipEmptyParts);
    RS_SETTINGS->endGroup();

    // update internal hash of action pointers:
    if (commandList.count()>0 && commandList.at(0).length()!=0) {
        for (int i=0; i<commandList.count(); ++i) {
            actionsByCommand[commandList.at(i)] = this;
            // first command or any command with length>2 will be included in tab-search:
            if (i==0 || commandList.at(i).length()>2) {
                actionsByPrimaryCommand[commandList.at(i)] = this;
            }
            // upper case command is the main command for that action:
            if (commandList.at(i).length()>0 && commandList.at(i).at(0).isUpper()) {
                //commandsByType[typeOld] = commandList.at(i);
                mainCommand = commandList.at(i);
            }
        }

        if (mainCommand.isEmpty() && commandList.count()>0) {
            mainCommand = commandList.at(0);
        }
    }

    initTexts();
    RS_DEBUG->print("RS_GuiAction::setCommand: %s", (const char*)cmd.toLatin1());
}


void RS_GuiAction::setKeycode(const RS_String& kc) {
    RS_SETTINGS->beginGroup("/Keycodes");
    keycodeList = RS_SETTINGS->readEntry(getIdentifier(), kc).split(',', QString::SkipEmptyParts);
    RS_SETTINGS->endGroup();

    // update internal hash of action pointers:
    if (keycodeList.count()>0 && keycodeList.at(0).length()!=0) {
        for (int i=0; i<keycodeList.count(); ++i) {
            actionsByKeycode[keycodeList.at(i)] = this;
        }
    }

    initTexts();
    RS_DEBUG->print("RS_GuiAction::setKeycode: %s", (const char*)kc.toLatin1());
}


void RS_GuiAction::setShortcut(const RS_String& sc) {

    RS_SETTINGS->beginGroup("/Shortcuts");
    shortcutList = RS_SETTINGS->readEntry(getIdentifier(), sc).split(',', QString::SkipEmptyParts);
    RS_SETTINGS->endGroup();

    QList<QKeySequence> shortcutKeys;
    for (int i=0; i<shortcutList.count(); ++i) {
        shortcutKeys.append(QKeySequence(shortcutList.at(i)));
    }
    QAction::setShortcuts(shortcutKeys);

    initTexts();
    
    RS_DEBUG->print("RS_GuiAction::setShortcut: %s", (const char*)sc.toLatin1());
}

void RS_GuiAction::setShortcut(const QKeySequence& shortcut) {
    setShortcut(shortcut.toString());
}



void RS_GuiAction::setToolTip(const RS_String& tip) {
    QAction::setToolTip(tip);
    initTexts();
}

void RS_GuiAction::setStatusTip(const RS_String& tip) {
    QAction::setStatusTip(tip);
    initTexts();
}

bool RS_GuiAction::triggerByKeycode(const RS_String& kc) {
    RS_DEBUG->print("RS_GuiAction::triggerByKeycode: %s", (const char*)kc.toLatin1());

    if (actionsByKeycode.contains(kc) && actionsByKeycode[kc]!=NULL) {
        actionsByKeycode[kc]->slotTriggered();
        return true;
    }
    else {
        RS_DEBUG->print("RS_GuiAction::triggerByKeycode: no action found");
        return false;
    }
}


bool RS_GuiAction::triggerByCommand(const RS_String& cmd) {
    RS_DEBUG->print("RS_GuiAction::triggerByCommand: %s", (const char*)cmd.toLatin1());

    if (actionsByCommand.contains(cmd) && actionsByCommand[cmd]!=NULL) {
        actionsByCommand[cmd]->slotTriggered();
        return true;
    }
    else {
        RS_DEBUG->print("RS_GuiAction::triggerByCommand: no action found");
        return false;
    }
}


void RS_GuiAction::setFactory(RS_ActionInterface* (*f)()) {
    factory = f;
    connect(this, SIGNAL(triggered()),
            this, SLOT(slotTriggered()));
}
    
    

/**
 * @return A list of available top level commands. These are commands
 * that can be used to trigger new actions.
 *
 * @param primaryOnly True: only include primary commands (e.g. 'point')
 *                    False: also include secondary commands (e.g. 'po')
 */
RS_StringList RS_GuiAction::getAvailableCommands(bool primaryOnly) {
    if (primaryOnly) {
        return actionsByPrimaryCommand.keys();
    }
    else {
        return actionsByCommand.keys();
    }
}


RS_StringList RS_GuiAction::getAvailableCommands(const RS_String& start, bool primaryOnly) {
    RS_StringList ret;
    RS_StringList all = getAvailableCommands(primaryOnly);

    for (int i=0; i<all.count(); ++i) {
        if (start.isEmpty() || all.at(i).startsWith(start)) {
            ret.append(all.at(i));
        }
    }

    return ret;
}



RS_ActionInterface* RS_GuiAction::slotTriggered() {
    if (factory!=NULL) {
        if (!getMainCommand().isEmpty()) {
            RS_DIALOGFACTORY->commandMessage(getMainCommand());
        }
        return factory();
    }
    else {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
            "RS_GuiAction::slotTriggered: factory is NULL");
        return NULL;
    }
}
