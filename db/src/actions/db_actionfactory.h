/*****************************************************************
 * $Id: db_actionfactory.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFACTORY_H
#define DB_ACTIONFACTORY_H

#include <map>
#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"

#define DB_ACTIONFACTORY DB_ActionFactory::getInstance()

class QPrinter;
class RB_ActionPlugin;

/**
 * Main action factory for Biluna applications
 */
class DB_EXPORT DB_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~DB_ActionFactory();
    static DB_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getFileNewMenu(QMenu* menu, QToolBar* tb);
    virtual void getFileMenu(QMenu* menu, QToolBar* tb);
    virtual void getEditMenu(QMenu* menu, QToolBar* tb);
    virtual void getFormatMenu(QMenu* menu, QToolBar* tb);
    virtual void getPerspectiveMenu(QMenu* menu, QToolBar* tb = NULL);
    virtual QAction* getPerspectiveAction();
    virtual void getToolsMenu(QMenu* menu, QToolBar* tb = NULL);
    virtual void getSystemMenu(QMenu* menu, QToolBar* tb = NULL);
    virtual void getHelpMenu(QMenu* menu, QToolBar* tb = NULL);

    virtual void enableEditMenu(int widgetType);
    virtual void enableFormatMenu(int widgetType);

    virtual void setFormatAlignLeft(bool check);
    virtual void setFormatAlignRight(bool check);
    virtual void setFormatAlignCenter(bool check);
    virtual void setFormatAlignJustify(bool check);
    virtual void setFormatBoldChecked(bool check);
    virtual void setFormatItalicChecked(bool check);
    virtual void setFormatUnderlineChecked(bool check);
    virtual void setFormatStrikethroughChecked(bool check);
    virtual void setFormatNumberedListChecked(bool check);
    virtual void setFormatBulletedListChecked(bool check);

    virtual void setRecentFile(const RB_String& fn);
    virtual void updateRecentFileActions();

    virtual std::vector<RB_ActionFactory*> getFactoryList();
    virtual void registerFactory(RB_ActionFactory* factory);
    virtual void unregisterFactory(RB_ActionFactory* factory);
//    virtual void closeAllFactories();

    virtual void removeAllPlugins();
    virtual void closePlugin(const RB_String& pluginId);

    virtual QPrinter* getPrinter();

signals:
    void formatAlignLeft(bool check);
    void formatAlignRight(bool check);
    void formatAlignCenter(bool check);
    void formatAlignJustify(bool check);
    void formatBoldChecked(bool check);
    void formatItalicChecked(bool check);
    void formatUnderlineChecked(bool check);
    void formatStrikethroughChecked(bool check);
    void formatNumberedListChecked(bool check);
    void formatBulletedListChecked(bool check);


protected slots:
    void slotOpenRecentFile();
    void slotPluginClicked();


private:
    DB_ActionFactory(RB_MainWindow* mw);
    void setPluginsDir(QDir& pluginsDir);
    bool loadPlugin(const RB_String& pluginId, const RB_String& fileName);
    void removePlugin(const RB_String& pluginId);

    //! Unique factory instance
    static DB_ActionFactory* mActiveFactory;
    //! List of all currently created action factories
    std::vector<RB_ActionFactory*> mFactoryList;
    //! Generic edit menu actions, can be enabled by other widget types
    std::vector<RB_GuiAction*> mEditMenuActionList;
    //! Generic text format menu actions, can be enabled by other widget types
    std::vector<RB_GuiAction*> mFormatMenuActionList;
    //! Map of active plugins
    std::map<RB_String, RB_ActionPlugin*> mPluginList;

    //! To enable the insertion of perspective menus before perspective menu
    QAction* mPerspectiveAction;
    QMenu* mPerspectiveMenu;

    //! Recent files
    enum { MaxRecentFiles = 9 };
    //! Recent files action
    QAction* mRecentFileActs[MaxRecentFiles];
    //! Recent files action separator
    QAction* mSeparatorAct;

    //! System printer object
    QPrinter* mPrinter;
};

#endif // DB_ACTIONFACTORY_H
