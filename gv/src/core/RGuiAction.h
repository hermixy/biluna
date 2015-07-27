#ifndef RGUIACTION_H
#define RGUIACTION_H

#include <QMap>
#include <QMetaType>
#include <QString>
#include <QStringList>
#include <QList>

#include "RTransactionListener.h"
#include "RFocusListener.h"
#include "RSelectionListener.h"

class RDocument;
class RDocumentInterface;


/**
 * \brief Abstract base class for GUI action classes. 
 *
 * Such classes represent a GUI action that can be used to start a tool.
 * One GUI action class may be assigned to multiple GUI elements, for 
 * example a menu, a toolbutton and a context menu.
 *
 * Each GUI action can have multiple commands assigned to it. These commands
 * can be used to trigger the action from a command line.
 *
 * Each GUI action can have multiple shortcuts assigned to it. Shortcuts
 * are 'traditional' key combinations that can be used to trigger the action.
 * e.g. Ctrl + Z, Ctrl + N, ...
 * 
 * \ingroup core
 * \scriptable
 */
class RGuiAction:
    public RFocusListener,
    public RTransactionListener,
    public RSelectionListener {
public:
    typedef void (*FactoryFunction)();

public:
    RGuiAction();
    virtual ~RGuiAction();
    
    /**
     * Initializes the GUI action texts (for menus, tooltips, ...).
     * This function is called whenever the text of the action changes.
     */
    virtual void initTexts() = 0;

    /**
     * Sets the action icon to the given icon file.
     */
    virtual void setIcon(const QString& iconFile) = 0;

    /**
     * Sets the shortcut(s) for this action.
     *
     * \param shortcut Comma separated list of shortcuts (e.g. "Ctrl-Shift-Z,Ctrl-Y").
     */
    //virtual void setShortcut(const QString& shortcut) = 0;
    
    /**
     * \return Complete list of shortcuts  that may be used to trigger this action.
     */
    /*
    QStringList getShortcutList() {
        return shortcutList;
    }
    */

    /**
     * Sets the tooltip for this action. Tooltips are shown when the mouse cursor
     * hovers over a GUI element for some time.
     */
    virtual void setToolTip(const QString& tip) = 0;

    /**
     * Sets the status tip of this action. The status tip is shown in the status bar
     * of the application when the mouse cursor hovers of a menu entry.
     */
    virtual void setStatusTip(const QString& tip) = 0;
    
    /**
     * Enables or disables the action.
     */
    virtual void setEnabled(bool on) = 0;
    
    /**
     * Checks or unchecks this action.
     */
    virtual void setChecked(bool on);

    static void triggerGroupDefault(const QString& group);

    /**
     * \return True if this action is checked.
     */
    virtual bool isChecked() = 0;
    
    /**
     * If \c on is true, this action requires a document to be open. The GUI element(s)
     * can for example be grayed out if no document is open.
     */
    void setRequiresDocument(bool on) {
        requiresDocument = on;
    }

    /**
     * \retval true This action requires an open document to be started.
     * \retval false This action does not require an open document to be started.
     */
    bool getRequiresDocument() {
        return requiresDocument;
    }

    /**
     * If \c on is true, this action requires a selection to operate on. The GUI element(s)
     * can for example be grayed out if no selection is present.
     */
    void setRequiresSelection(bool on) {
        requiresSelection = on;
    }

    /**
     * \retval true This action requires an selection to be started.
     * \retval false This action does not require a selection to be started.
     */
    bool getRequiresSelection() {
        return requiresSelection;
    }

    void setRequiresUndoableTransaction(bool on) {
        requiresUndoableTransaction = on;
    }

    bool getRequiresUndoableTransaction() {
        return requiresUndoableTransaction;
    }

    void setRequiresRedoableTransaction(bool on) {
        requiresRedoableTransaction = on;
    }

    bool getRequiresRedoableTransaction() {
        return requiresRedoableTransaction;
    }

    void setOverride() {
        override = true;
    }

    bool isOverride() {
        return override;
    }

    void setUniqueGroup(const QString& ug) {
        uniqueGroup = ug;
    }

    QString getUniqueGroup() {
        return uniqueGroup;
    }

    /**
     * \return The main command used to trigger this action. This is typically the
     *      full command (e.g. "line") rather than one of the abbreviated commands
     *      such as "ln".
     */
    QString getMainCommand() {
        return mainCommand;
    }

    /**
     * \return Complete list of commands that may be used to trigger this action.
     */
    QStringList getCommandList() {
        return commandList;
    }
    
    void setCommand(const QString& cmd);
    
    /**
     * \nonscriptable
     */
    void setFactory(FactoryFunction f);
    
    /**
     * Sets the script file to be used for this action. This is only used for
     * script based actions.
     */
    void setScriptFile(const QString& sf);

    /**
     * \return Script file on which this action is based on.
     */
    QString getScriptFile() {
        return scriptFile;
    }

    /**
     * Called when the action is triggered (e.g. a button is pressed or a menu chosen).
     */
    virtual bool doAction();

    virtual void triggerAction() = 0;

    virtual void setGroup(const QString& title);
    QString getGroup();

    void setGroupDefault(bool on);
    bool isGroupDefault();

    /**
     * \return List of all instantiated actions.
     */
    static QList<RGuiAction*> getActions() {
        return actions;
    }

    static bool triggerByCommand(const QString& cmd);
    static RGuiAction* getByScriptFile(const QString& scriptFile);
    static bool triggerByScriptFile(const QString& scriptFile);
    
    static QStringList getAvailableCommands(bool primaryOnly=false);
    static QStringList getAvailableCommands(const QString& start, bool primaryOnly=false);

    static void clear();

    virtual void updateTransactionListener(const RDocument* document);
    virtual void updateSelectionListener(const RDocument* document);
    virtual void updateFocus(RDocumentInterface* documentInterface);

    void addArgument(const QString& arg);
    QStringList getArguments();

protected:
    static QMap<QString, RGuiAction*> actionsByCommand;
    static QMap<QString, RGuiAction*> actionsByPrimaryCommand;
    static QMap<QString, RGuiAction*> actionsByScriptFile;
    static QMultiMap<QString, RGuiAction*> actionsByGroup;
    static QList<RGuiAction*> actions;

    FactoryFunction factory;
    QString scriptFile;
    QString mainCommand;
    QStringList commandList;
    //QStringList shortcutList;
    QString group;
    bool groupDefault;
    
    bool requiresDocument;
    bool requiresSelection;
    bool requiresUndoableTransaction;
    bool requiresRedoableTransaction;
    bool override;
    QString uniqueGroup;
    QStringList arguments;
};

Q_DECLARE_METATYPE(RGuiAction*)
Q_DECLARE_METATYPE(QList<RGuiAction*>)

#endif
