#ifndef RGUIACTIONQT_H
#define RGUIACTIONQT_H

#include <QAction>
#include <QEvent>

#include "RGuiAction.h"

class QMenu;
class QToolBar;
class RDocument;
class RDocumentInterface;




/**
 * \brief GUI action implementation for Qt specific GUI actions.
 *
 * \ingroup gui_qt
 *
 * \scriptable
 *
 */
class RGuiActionQt : public QAction, public RGuiAction {

    Q_OBJECT

public:
    RGuiActionQt(const QString& text, QObject* parent);
    virtual ~RGuiActionQt(); 

    virtual void initTexts();

    static QString getToolTip(const QString& text, const QString& shortcut);

    static RGuiActionQt* getByScriptFile(const QString& scriptFile);

    virtual void triggerAction();

    virtual void setIcon(const QString& iconFile);
    virtual void setToolTip(const QString& tip);
    virtual void setStatusTip(const QString& tip);
    //virtual void setShortcut(const QString& shortcut);
    void setShortcut(const QKeySequence& shortcut);
    virtual void setEnabled(bool on);
    virtual void setChecked(bool on);
    virtual bool isChecked();
    void setShortcutText(const QString& text);
    void setSortOrder(int sortOrder);
    int getSortOrder();

    void addToMenu(QMenu* menu);
    void addToToolBar(QToolBar* tb);
    void addToWidget(QWidget* w);

    void setScriptFile(const QString& sf);

public slots:
    void slotTrigger() {
        doAction();
    }

private:
    QString shortcutText;
    int sortOrder;
};

Q_DECLARE_METATYPE(RGuiActionQt*)

#endif
