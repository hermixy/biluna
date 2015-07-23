/*****************************************************************
 * $Id: db_actionsystemstyledark.cpp 2197 2014-11-15 18:59:07Z rutger $
 * Created: Oct 29, 2014 1:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemstyledark.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_mainwindow.h"


DB_ActionSystemStyleDark::DB_ActionSystemStyleDark() : RB_Action() {

}


RB_GuiAction* DB_ActionSystemStyleDark::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionSystemStyleDark::getName(),
                                  // "fileclose",
                                  tr("&Dark Style"));
        ga->setStatusTip(tr("Set application to dark style"));
//        ga->setShortcut("Ctrl+W");
        ga->setFactory(DB_ActionSystemStyleDark::factory);
        ga->setCheckable(true);
        ga->setChecked(false);

        connect(DB_DIALOGFACTORY, SIGNAL(alternativeApplicationStyle(bool)),
                ga, SLOT(setChecked(bool)));
    }

    return ga;
}

RB_Action* DB_ActionSystemStyleDark::factory() {
    RB_Action* a = new DB_ActionSystemStyleDark();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 * TODO: bring this
 */
void DB_ActionSystemStyleDark::trigger() {
    RB_String originalStyle = DB_DIALOGFACTORY->getOriginalStyle();
    QPalette originalPalette = DB_DIALOGFACTORY->getOriginalPalette();

    if (originalStyle != QApplication::style()->objectName()) {
        // QStyleFactory::keys() returns a list of valid keys, typically
        // including "windows", "motif", "cde", "motifplus", "platinum",
        // "sgi" and "compact". This was in Qt 3.
        // Depending on the platform, "Fusion", "Windows", "WindowsXP",
        // "WindowsVista", "aqua" or "macintosh" may be available.
        // QStringList keys = QStyleFactory::keys();

        QApplication::setStyle(QStyleFactory::create(originalStyle));
        QApplication::setPalette(originalPalette);
    } else {
        // set stylesheet, not good: legibility and missing parts
    //    QFile f(":/styles/dark_orange.qss");
    //    if(f.open(QIODevice::ReadOnly))        {
    //       QTextStream textStream(&f);
    //       QString styleSheet = textStream.readAll();
    //       f.close();
    //       app.setStyleSheet(styleSheet);
    //    }

        // set stylesheet Fusion, this is the only build in
    //    app.setStyle(QStyleFactory::create("fusion"));

        // set stylesheet Fusion, this is the only build in but now in dark colors
        // Looks best
        // Roles: QPalette::Disabled, QPalette::Active (normal), QPalette::Inactive
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        QPalette palette;

        // Active
        palette.setColor(QPalette::Active, QPalette::Window, QColor(64,64,64));
        palette.setColor(QPalette::Active, QPalette::WindowText, QColor(255,255,255));
        palette.setColor(QPalette::Active, QPalette::Base, QColor(0,0,0));
        palette.setColor(QPalette::Active, QPalette::AlternateBase, QColor(32,32,32));
        palette.setColor(QPalette::Active, QPalette::ToolTipBase, QColor(204,204,64));
        palette.setColor(QPalette::Active, QPalette::ToolTipText, QColor(0,0,0));
        palette.setColor(QPalette::Active, QPalette::Text, QColor(255,255,255)); // text in QLineEdits default text
        palette.setColor(QPalette::Active, QPalette::Button, QColor(64,64,64));
        palette.setColor(QPalette::Active, QPalette::ButtonText, QColor(255,255,255));
        palette.setColor(QPalette::Active, QPalette::BrightText, QColor(255,255,255));

        palette.setColor(QPalette::Active, QPalette::Light, QColor(96,96,96));
        palette.setColor(QPalette::Active, QPalette::Midlight, QColor(80,80,80));
        palette.setColor(QPalette::Active, QPalette::Mid, QColor(42,42,42));
        palette.setColor(QPalette::Active, QPalette::Dark, QColor(32,32,32));
        palette.setColor(QPalette::Active, QPalette::Shadow, QColor(0,0,0));
        
        palette.setColor(QPalette::Active, QPalette::Highlight, QColor(204,102,0)); // 250,170,0 orange
        palette.setColor(QPalette::Active, QPalette::HighlightedText, QColor(0,0,0));
        
        // Inactive
        palette.setColor(QPalette::Inactive, QPalette::Window, QColor(64,64,64));
        palette.setColor(QPalette::Inactive, QPalette::WindowText, QColor(255,255,255));
        palette.setColor(QPalette::Inactive, QPalette::Base, QColor(0,0,0));
        palette.setColor(QPalette::Inactive, QPalette::AlternateBase, QColor(32,32,32));
        palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, QColor(102,102,32));
        palette.setColor(QPalette::Inactive, QPalette::ToolTipText, QColor(255,255,255));
        palette.setColor(QPalette::Inactive, QPalette::Text, QColor(255,255,255)); // text in QLineEdits default text
        palette.setColor(QPalette::Inactive, QPalette::Button, QColor(64,64,64));
        palette.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(255,255,255));
        palette.setColor(QPalette::Inactive, QPalette::BrightText, QColor(255,255,255));

        palette.setColor(QPalette::Inactive, QPalette::Light, QColor(96,96,96));
        palette.setColor(QPalette::Inactive, QPalette::Midlight, QColor(80,80,80));
        palette.setColor(QPalette::Inactive, QPalette::Mid, QColor(42,42,42));
        palette.setColor(QPalette::Inactive, QPalette::Dark, QColor(32,32,32));
        palette.setColor(QPalette::Inactive, QPalette::Shadow, QColor(0,0,0));

        palette.setColor(QPalette::Inactive, QPalette::Highlight, QColor(64,32,0)); // 250,170,0 orange
        palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(255,255,255));

        // Disabled
        palette.setColor(QPalette::Disabled, QPalette::Window, QColor(64,64,64));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(32,32,32));
        palette.setColor(QPalette::Disabled, QPalette::Base, QColor(64,64,64));
        palette.setColor(QPalette::Disabled, QPalette::AlternateBase, QColor(64,64,64));
        palette.setColor(QPalette::Disabled, QPalette::ToolTipBase, QColor(204,204,64));
        palette.setColor(QPalette::Disabled, QPalette::ToolTipText, QColor(0,0,0));
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(0,0,0)); // text in QLineEdits default text
        palette.setColor(QPalette::Disabled, QPalette::Button, QColor(64,64,64));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(32,32,32));
        palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255,255,255));

        palette.setColor(QPalette::Disabled, QPalette::Light, QColor(96,96,96));
        palette.setColor(QPalette::Disabled, QPalette::Midlight, QColor(80,80,80));
        palette.setColor(QPalette::Disabled, QPalette::Mid, QColor(42,42,42));
        palette.setColor(QPalette::Disabled, QPalette::Dark, QColor(32,32,32));
        palette.setColor(QPalette::Disabled, QPalette::Shadow, QColor(0,0,0));

        palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(204,102,0)); // 250,170,0 orange
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(0,0,0));

        QApplication::setPalette(palette);

        // set stylesheet based on a global color, can also be a button with all colors set
        // Easiest
//        QApplication::setStyle(QStyleFactory::create("Fusion"));
//        QPalette palette(QColor(53,53,53));
//        QApplication::setPalette(palette);
        // end set stylesheet

    }
}

