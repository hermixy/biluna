#include <QList>
#include <QVectorIterator>
#include <QFileInfo>
#include <QDir>

#include "RDebug.h"
//#include "RDeleteFunctor.h"
#include "RGuiAction.h"
#include "RScriptHandler.h"
//#include "RStringTokenizer.h"



QMap<QString, RGuiAction*> RGuiAction::actionsByCommand;
QMap<QString, RGuiAction*> RGuiAction::actionsByPrimaryCommand;
QMap<QString, RGuiAction*> RGuiAction::actionsByScriptFile;
QMultiMap<QString, RGuiAction*> RGuiAction::actionsByGroup;
QList<RGuiAction*> RGuiAction::actions;




RGuiAction::RGuiAction()
  : factory(NULL),
    groupDefault(false),
    requiresDocument(true), 
    requiresSelection(false),
    requiresUndoableTransaction(false),
    requiresRedoableTransaction(false),
    override(false)
    {
    
    actions.push_back(this);
}



RGuiAction::~RGuiAction() {
	QList<QMap<QString, RGuiAction*>*> maps;
	maps << &actionsByCommand;
	maps << &actionsByPrimaryCommand;
	maps << &actionsByScriptFile;
	maps << &actionsByGroup;

    QListIterator<QMap<QString, RGuiAction*>*> vi(maps);
	while (vi.hasNext()) {
		QStringList keys;
		QMap<QString, RGuiAction*>* map = vi.next();
		QMapIterator<QString, RGuiAction*> i(*map);
		while (i.hasNext()) {
			i.next();
			if (i.value() == this) {
				keys << i.key();
			}
		}
		keys.removeDuplicates();
		QStringListIterator ki(keys);
		while (ki.hasNext()) {
			QString s = ki.next();
			map->remove(s);
		}
	}

	actions.removeAll(this);
}


void RGuiAction::updateTransactionListener(const RDocument* document) {
	if (document == NULL) {
		return;
	}

	if (requiresUndoableTransaction) {
		setEnabled(document->isUndoAvailable());
	}
	if (requiresRedoableTransaction) {
		setEnabled(document->isRedoAvailable());
	}
    if (requiresSelection) {
        setEnabled(document->hasSelection());
    }
}

void RGuiAction::updateSelectionListener(const RDocument* document) {
    if (document == NULL) {
        return;
    }

    if (requiresSelection) {
        setEnabled(document->hasSelection());
    }
}

void RGuiAction::updateFocus(RDocumentInterface* documentInterface) {
    if (documentInterface==NULL) {
        return;
    }
    if (group.isEmpty()) {
        return;
    }

    // re-activate active action in action group:
    if (isChecked()) {
        doAction();
    }
    /*
    QList<RGuiAction*> actions = actionsByGroup.values(group);
    for (int i = 0; i < actions.size(); ++i) {
        RGuiAction* action = actions.at(i);
        if (action->isChecked()) {
            action->doAction();
        }
    }
    */
}


/**
 * Sets the command(s) that can trigger this action from a command line.
 * 
 * \param cmd Comma separated list of command(s) to set for this action.  
 *      e.g. "mycommand", "mycommand,mycom,mc".
 */
void RGuiAction::setCommand(const QString& cmd) {
    commandList = cmd.toLower().split(',');
    //RStringTokenizer::split(cmd, ",");

    // update internal hash of action pointers:
    if (commandList.size()>0 && commandList.begin()->size()!=0) {
        QStringList::iterator it;
        for (it=commandList.begin(); it!=commandList.end(); ++it) {
            actionsByCommand[*it] = this;

            // first command or any command with length>2 will be included in tab-search:
            if (it==commandList.begin() || (*it).size()>2) {
                actionsByPrimaryCommand[*it] = this;
            }

            // upper case command is the main command for that action:
            if ((*it).size()>0 && (*it).at(0).isUpper()) {
                mainCommand = (*it);
            }
        }

        if (mainCommand.size()==0) {
            mainCommand = (*commandList.begin());
        }
    }

    initTexts();
}



void RGuiAction::setScriptFile(const QString& sf) {
	QDir dir(".");
	QString relSf = dir.relativeFilePath(sf);
	scriptFile = relSf;
	actionsByScriptFile[relSf] = this;
}



void RGuiAction::setFactory(FactoryFunction f) {
    factory = f;
}

void RGuiAction::setChecked(bool /*on*/) {
}

void RGuiAction::triggerGroupDefault(const QString& group) {
    if (!group.isEmpty()) {
        // if nothing else in this group is checked,
        // check default action of this group:
        bool found = false;
        RGuiAction* defaultAction = NULL;
        QList<RGuiAction*> actions = actionsByGroup.values(group);
        for (int i = 0; i < actions.size(); ++i) {
            RGuiAction* action = actions.at(i);
            /*
            if (action->isChecked() && action!=this) {
                found = true;
                break;
            }
            */
            if (action->isGroupDefault()) {
                defaultAction = action;
            }
        }
        if (!found && defaultAction!=NULL && !defaultAction->isChecked()) {
            defaultAction->doAction();
        }
    }
}

void RGuiAction::setGroup(const QString& title) {
    group = title;
    actionsByGroup.insert(group, this);
}

QString RGuiAction::getGroup() {
    return group;
}

void RGuiAction::setGroupDefault(bool on) {
    groupDefault = on;
}

bool RGuiAction::isGroupDefault() {
    return groupDefault;
}

/**
 * Triggers the first action in the list of actions that is registered
 * to use the given command.
 *
 * \param command Command that was launched (e.g. "line" or "ln").
 */
bool RGuiAction::triggerByCommand(const QString& cmd) {
	QString cmdLower = cmd.toLower();
	if (actionsByCommand.count(cmdLower) != 0 && actionsByCommand[cmdLower]
			!= NULL) {
		actionsByCommand[cmdLower]->triggerAction();
		return true;
	} else {
		return false;
	}
}



/**
 * Triggers the first action in the list of actions that is based
 * on the given \c scriptFile.
 */
bool RGuiAction::triggerByScriptFile(const QString& scriptFile) {
    RGuiAction* guiAction = getByScriptFile(scriptFile);
    if (guiAction!=NULL) {
        guiAction->triggerAction();
        return true;
    }
    else {
        return false;
    }
}



/**
 * \return The first action in the list of actions that is based
 * on the given \c scriptFile.
 */
RGuiAction* RGuiAction::getByScriptFile(const QString& scriptFile) {
	QDir dir(".");
	QString relFilePath = dir.relativeFilePath(scriptFile);

	if (actionsByScriptFile.count(relFilePath) != 0
			&& actionsByScriptFile[relFilePath] != NULL) {
		return actionsByScriptFile[relFilePath];
	} else {
		RDebug::warning("RGuiAction::getByScriptFile: no action found:");
		RDebug::warning(QString("RGuiAction::getByScriptFile: %1").arg(scriptFile));
		return NULL;
	}
}


/**
 * \return A list of available top level commands. These are commands
 *      that can be used to trigger actions.
 *
 * \param primaryOnly True: only include primary commands (e.g. 'point')
 *                    False: also include secondary commands (e.g. 'po')
 */
QStringList RGuiAction::getAvailableCommands(bool primaryOnly) {
    QStringList ret;

    if (primaryOnly) {
        QMap<QString, RGuiAction*>::iterator it;
        for (it=actionsByPrimaryCommand.begin(); it!=actionsByPrimaryCommand.end(); ++it) {
            ret.push_back(it.key());
        }
    }
    else {
        QMap<QString, RGuiAction*>::iterator it;
        for (it=actionsByCommand.begin(); it!=actionsByCommand.end(); ++it) {
            ret.push_back(it.key());
        }
    }
        
    return ret;
}



/**
 * \return List of available commands for triggering actions that start with \c start.
 *
 * This is typically used when the user presses the Tab key in a command line to
 * complete a started command. For example entering "li<tab>" will result in the
 * command to be completed to "line".
 *
 * \param primaryOnly True: only include primary commands (e.g. 'point')
 *                    False: also include secondary commands (e.g. 'po')
 */
QStringList RGuiAction::getAvailableCommands(const QString& start, bool primaryOnly) {
    QStringList ret;

    QStringList all = getAvailableCommands(primaryOnly);
    QStringList::iterator it;
    for (it=all.begin(); it<all.end(); ++it) {
        if (start.size()==0 || (*it).indexOf(start)==0) {
            ret.push_back(*it);
        }
    }

    return ret;
}



/**
 * This should be called when the action is triggered, i.e. a GUI element is 
 * activated (button pressed, menu selected, etc).
 */
bool RGuiAction::doAction() {
	RMainWindow* mainWindow = RMainWindow::getMainWindow();
	if (mainWindow == NULL) {
		RDebug::warning(QString("RGuiAction::doAction: mainWindow is NULL"));
        return false;
	} else if (!getMainCommand().isEmpty()) {
        // display main command e.g. in command line:
		mainWindow->handleUserCommand(getMainCommand());
	}

    // uncheck all other actions in this group and check this action:
    if (!group.isEmpty()) {
        setChecked(true);
        QList<RGuiAction*> actions = actionsByGroup.values(group);
        for (int i = 0; i < actions.size(); ++i) {
            RGuiAction* action = actions.at(i);
            if (action!=this) {
                action->setChecked(false);
            }
        }
    }

	if (scriptFile.size() > 0) {
		// call action factory of script handler:
		if (requiresDocument) {
			RDocumentInterface* di = RMainWindow::getDocumentInterfaceStatic();
			if (di == NULL) {
				mainWindow->handleUserWarning(QObject::tr("This action requires a document to be open."));
                return true;
			}

			QString extension = QFileInfo(scriptFile).suffix();
			RScriptHandler* h = di->getScriptHandler(extension);
			if (h == NULL) {
				RDebug::warning("RGuiAction::doAction: "
					"no script handler found for scriptFile: %s",
						(const char*) scriptFile.toUtf8());
                return false;
			}

			h->createActionDocumentLevel(scriptFile, this);
		} else {
			RScriptHandler::triggerActionApplicationLevel(scriptFile, this);
		}
        return true;
	} else if (factory != NULL) {
        // call C++ action factory:
        factory();
        return true;
	}

    //RDebug::warning("RGuiAction::doAction: factory is NULL");
    // A QAction based action might choose to call QAction::trigger if false is
    // returned:
    return false;
}



/** 
 * Cleans up all GUI action objects.
 */
void RGuiAction::clear() {
    //std::for_each(actions.begin(), actions.end(), RDeleteFunctor<RGuiAction*>());
    while (!actions.isEmpty()) {
        delete actions.takeFirst();
    }
    //actions.clear();
}

void RGuiAction::addArgument(const QString& arg) {
    arguments.append(arg);
}

QStringList RGuiAction::getArguments() {
    return arguments;
}
