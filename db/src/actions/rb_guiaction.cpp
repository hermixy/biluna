/*****************************************************************
 * $Id: rb_guiaction.cpp 2135 2014-04-06 10:40:16Z rutger $
 * Created: Nov 2, 2009 5:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_guiaction.h"

#include "rb_debug.h"
#include "db_dialogfactory.h"
#include "rb_settings.h"
    
RB_Hash<RB_String, RB_GuiAction*> RB_GuiAction::actionsByKeycode;
RB_Hash<RB_String, RB_GuiAction*> RB_GuiAction::actionsByCommand;
RB_Hash<RB_String, RB_GuiAction*> RB_GuiAction::actionsByPrimaryCommand;
RB_List<RB_GuiAction*> RB_GuiAction::actions;



/**
 * @param id: Id of action used in config files.
 * @param text: Menu text, tooltip text.
 */
RB_GuiAction::RB_GuiAction(const RB_String& id, const RB_String& text) :
    QAction(text, NULL), identifier(id) {
    RB_DEBUG->info("RB_GuiAction::RB_GuiAction(id, text) " + id);
    init(text);
}



/**
 * @param id: Id of action used in config files.
 * @param iconId: E.g. 'linesnormal' for icon ':/qcad/images/linesnormal.png'.
 * @param text: Menu text, tooltip text.
 */
RB_GuiAction::RB_GuiAction(const RB_String& id, const RB_String& iconId, const RB_String& text) :
    QAction(QIcon(":/images/" + iconId + ".png"), text, NULL), identifier(id) {
    RB_DEBUG->info("RB_GuiAction::RB_GuiAction(iconId) " + id);
    this->iconId = iconId;
    init(text);
}



/**
 * @param id: Id of action used in config files.
 * @param icon: Action icon shown on buttons and in menu.
 * @param text: Menu text, tooltip text.
 */
RB_GuiAction::RB_GuiAction(const RB_String& id, const QIcon& icon, const RB_String& text) :
    QAction(icon, text, NULL), identifier(id) {
    RB_DEBUG->info("RB_GuiAction::RB_GuiAction(id, icon, text) " + id);
    init(text);
}



RB_GuiAction::~RB_GuiAction() {
    // TODO: remove this action from hashes and lists
    RB_DEBUG->info("RB_GuiAction::~RB_GuiAction() " + getIdentifier());
}
    


RB_String RB_GuiAction::getIdentifier() {
    return identifier;
}
    

void RB_GuiAction::init(const RB_String& text) {
    factory = NULL;
    setText(text);
    initTexts();
    actions.append(this);
}


/**
 * Initializes the GUI action texts (for menus, tooltips, ...).
 */
void RB_GuiAction::initTexts() {
    RB_String textAndKeycode = text();
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

    RB_String textOnly = textAndKeycode;
    textOnly.replace('&', "");

    // Use tab to separate menu text from keycodes.
    //   The menu text with keycode looks like this: "Line\tli".
    //   This does not work under OS X. Text after tab (\t) is
    //   ignored.
    RB_String kcs = getKeycodeString();
    if (!kcs.isEmpty()) {
#ifdef Q_OS_MACX
        textAndKeycode = RB_String("[%1] %2").arg(kcs).arg(textAndKeycode);
#else
        textAndKeycode += QLatin1Char('\t');
        textAndKeycode += kcs;
#endif
    }

    setText(textAndKeycode);
    if (!kcs.isEmpty()) {
        QAction::setToolTip(textOnly + RB_String(" [%1]").arg(kcs));
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
RB_String RB_GuiAction::getKeycodeString() {
    RB_String ret;

    RB_SETTINGS->beginGroup("/Appearance");
    bool showKeycodes = (bool)RB_SETTINGS->readNumEntry("/ShowKeycodes",
#ifdef Q_OS_MACX
    0
#else
    1
#endif
    );
    bool showShortcuts = (bool)RB_SETTINGS->readNumEntry("/ShowShortcuts", 0);
    RB_SETTINGS->endGroup();
    
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
void RB_GuiAction::setCommand(const RB_String& cmd) {
    RB_SETTINGS->beginGroup("/Commands");
    commandList = RB_SETTINGS->readEntry(getIdentifier(), cmd).split(',', QString::SkipEmptyParts);
    RB_SETTINGS->endGroup();

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
    RB_DEBUG->print("RB_GuiAction::setCommand: " + cmd);
}


void RB_GuiAction::setKeycode(const RB_String& kc) {
    RB_SETTINGS->beginGroup("/Keycodes");
    keycodeList = RB_SETTINGS->readEntry(getIdentifier(), kc).split(',', QString::SkipEmptyParts);
    RB_SETTINGS->endGroup();

    // update internal hash of action pointers:
    if (keycodeList.count()>0 && keycodeList.at(0).length()!=0) {
        for (int i=0; i<keycodeList.count(); ++i) {
            actionsByKeycode[keycodeList.at(i)] = this;
        }
    }

    initTexts();
    RB_DEBUG->print("RB_GuiAction::setKeycode: " + kc);
}


void RB_GuiAction::setShortcut(const RB_String& sc) {

    RB_SETTINGS->beginGroup("/Shortcuts");
    shortcutList = RB_SETTINGS->readEntry(getIdentifier(), sc).split(',', QString::SkipEmptyParts);
    RB_SETTINGS->endGroup();

    QList<QKeySequence> shortcutKeys;
    for (int i=0; i<shortcutList.count(); ++i) {
        shortcutKeys.append(QKeySequence(shortcutList.at(i)));
    }
    QAction::setShortcuts(shortcutKeys);
    initTexts();
    RB_DEBUG->print("RB_GuiAction::setShortcut: " + sc);
}

void RB_GuiAction::setShortcut(const QKeySequence& shortcut) {
    setShortcut(shortcut.toString());
}



void RB_GuiAction::setToolTip(const RB_String& tip) {
    QAction::setToolTip(tip);
    initTexts();
}

void RB_GuiAction::setStatusTip(const RB_String& tip) {
    QAction::setStatusTip(tip);
    initTexts();
}

bool RB_GuiAction::triggerByKeycode(const RB_String& kc) {
    RB_DEBUG->print("RB_GuiAction::triggerByKeycode: " + kc);

    if (actionsByKeycode.contains(kc) && actionsByKeycode[kc]!=NULL) {
        actionsByKeycode[kc]->slotTriggered();
        return true;
    } else {
        RB_DEBUG->print("RB_GuiAction::triggerByKeycode: no action found");
        return false;
    }
}


bool RB_GuiAction::triggerByCommand(const RB_String& cmd) {
    RB_DEBUG->print("RB_GuiAction::triggerByCommand: " + cmd);

    if (actionsByCommand.contains(cmd) && actionsByCommand[cmd]!=NULL) {
        actionsByCommand[cmd]->slotTriggered();
        return true;
    } else {
        RB_DEBUG->print("RB_GuiAction::triggerByCommand: no action found");
        return false;
    }
}


void RB_GuiAction::setFactory(RB_Action* (*f)()) {
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
QStringList RB_GuiAction::getAvailableCommands(bool primaryOnly) {
    if (primaryOnly) {
        return actionsByPrimaryCommand.keys();
    } else {
        return actionsByCommand.keys();
    }
}


RB_StringList RB_GuiAction::getAvailableCommands(const RB_String& start, bool primaryOnly) {
    RB_StringList ret;
    RB_StringList all = getAvailableCommands(primaryOnly);

    for (int i=0; i<all.count(); ++i) {
        if (start.isEmpty() || all.at(i).startsWith(start)) {
            ret.append(all.at(i));
        }
    }

    return ret;
}

/**
 * Add enabled type such as a widget type for which this action is relevant
 * @param type widget type
 */
void RB_GuiAction::addEnabledType(int type) {
    bool enable = false;
    std::vector<int>::iterator iter;
    iter = mEnabledList.begin();
    while (iter != mEnabledList.end() && !enable) {
        if (*iter == type) {
            enable = true;
        }
        ++iter;
    }

    // not yet part of the enable widget list
    if (!enable) {
        mEnabledList.push_back(type);
    }
}

RB_Action* RB_GuiAction::slotTriggered() {
    if (factory!=NULL) {
        if (!getMainCommand().isEmpty()) {
            DB_DIALOGFACTORY->commandMessage(getMainCommand());
        }
        return factory();
    } else {
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "RB_GuiAction::slotTriggered: factory is NULL");
        return NULL;
    }
}

/**
 * Enable action based on widget type. The widget types are set in the
 * action factories
 * @param type current active MDI widget type or RB2::ValidTrue, RB2::ValidFalse
 */
void RB_GuiAction::slotSetEnabled(int type) {
    bool enable = false;
    if (type == RB2::ValidTrue) enable = true;

    std::vector<int>::iterator iter;
    iter = mEnabledList.begin();
    while (iter != mEnabledList.end() && !enable) {
        if (*iter == type) {
            enable = true;
        }
        ++iter;
    }

    if (type == RB2::ValidFalse) enable = false;
    setEnabled(enable);
}

