/*****************************************************************
 * $Id: db_actionfactory.cpp 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfactory.h"

#include <QtPrintSupport>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#include "db_actioneditcopy.h"
#include "db_actioneditcreatelink.h"
#include "db_actioneditcut.h"
#include "db_actioneditfind.h"
#include "db_actioneditfindreplace.h"
#include "db_actioneditinserthtml.h"
#include "db_actioneditinsertimage.h"
#include "db_actioneditpaste.h"
#include "db_actioneditredo.h"
#include "db_actioneditselectall.h"
#include "db_actioneditundo.h"
#include "db_actioneditzoomin.h"
#include "db_actioneditzoomout.h"
#include "db_actionfilenew.h"
#include "db_actionfilenewplaintexteditor.h"
#include "db_actionfilenewhtmleditor.h"
#include "db_actionfilenewhtmlview.h"
#include "db_actionfileopen.h"
#include "db_actionfilepdf.h"
#include "db_actionfileprint.h"
#include "db_actionfileprintpreview.h"
#include "db_actionfilequit.h"
#include "db_actionfilesaveas.h"
#include "db_actionfilesave.h"
#include "db_actionformataligncenter.h"
#include "db_actionformatalignjustify.h"
#include "db_actionformatalignleft.h"
#include "db_actionformatalignright.h"
#include "db_actionformatbackgroundcolor.h"
#include "db_actionformatbold.h"
#include "db_actionformatbulletedlist.h"
#include "db_actionformatdecreaseindent.h"
#include "db_actionformatfontname.h"
#include "db_actionformatfontsize.h"
#include "db_actionformatincreaseindent.h"
#include "db_actionformatitalic.h"
#include "db_actionformatnumberedlist.h"
#include "db_actionformatremove.h"
#include "db_actionformatstrikethrough.h"
#include "db_actionformattextcolor.h"
#include "db_actionformatunderline.h"
#include "db_actionhelpabout.h"
#include "db_actionhelpcredits.h"
#include "db_actionhelpindex.h"
#include "db_actionhelpsubject.h"
#include "db_actionpermissionreport.h"
#include "db_actionstyleaddress.h"
#include "db_actionstyleheading1.h"
#include "db_actionstyleheading2.h"
#include "db_actionstyleheading3.h"
#include "db_actionstyleheading4.h"
#include "db_actionstyleheading5.h"
#include "db_actionstyleheading6.h"
#include "db_actionstyleparagraph.h"
#include "db_actionstylepreformatted.h"
#include "db_actionsystemcalendar.h"
#include "db_actionsystemclearsettings.h"
#include "db_actionsystemdatabasebrowser.h"
#include "db_actionsystemdatabaseconnect.h"
#include "db_actionsystemdatabasedisconnect.h"
#include "db_actionsystemdatabasemapping.h"
#include "db_actionsystemeditproject.h"
#include "db_actionsystemgroup.h"
#include "db_actionsystempermission.h"
#include "db_actionsystemselectproject.h"
#include "db_actionsystemstylecolorpicker.h"
#include "db_actionsystemstyledark.h"
#include "db_actionsystemtoolsdevelop.h"
#include "db_actionsystemtoolsexportmodel.h"
#include "db_actionsystemtoolsimportmodel.h"
#include "db_actionsystemtoolsplugins.h"
#include "db_actionsystemtoolsregexp.h"
#include "db_actionsystemtoolssignalspy.h"
#include "db_actionsystemtoolsviewmodel.h"
#include "db_actionsystemversion.h"
#include "db_actiontoolscalculator.h"
#include "db_actiontoolsscreenshot.h"
#include "db_actiontoolssimplechart.h"
#include "db_actiontoolssimplereport.h"
#include "db_actiontoolstextfilelist.h"
#include "db_actiontoolsxmlview.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_actionplugin.h"
#include "rb_settings.h"
#include "rb_textedit.h"

DB_ActionFactory* DB_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 * \todo read initial printer settings from RB_Settings
 */
DB_ActionFactory::DB_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    RB_DEBUG->print("DB_ActionFactory::DB_ActionFactory()");

    mPerspectiveAction = NULL;
    mPerspectiveMenu = NULL;

    // Settings
    // TODO: read from settings
    mPrinter = new QPrinter(QPrinter::HighResolution); // new QPrinter();
    mPrinter->setPaperSize(QPrinter::A4);
    mPrinter->setOrientation(QPrinter::Portrait);
    mPrinter->setPageMargins(20, 15, 15, 15, QPrinter::Millimeter);

    if (QPrinterInfo::defaultPrinter().isNull()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No default printer set, will\n"
                                                  "result in undefined behavior"));
    }
}

/**
 * Destructor
 */
DB_ActionFactory::~DB_ActionFactory() {
    delete mPrinter;
    // closeAllFactories();
    removeAllPlugins();
    mActiveFactory = NULL;
    RB_DEBUG->print("DB_ActionFactory::~DB_ActionFactory() OK");
}

/**
 * @return unique instance of this action factory
 */
DB_ActionFactory* DB_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new DB_ActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get the generic 'File New' actions to the submenu
 * - New text document
 * @param menu
 * @param tb toolbar, not used
 */
void DB_ActionFactory::getFileNewMenu(QMenu* menu, QToolBar* tb) {
    RB_GuiAction* ga = NULL;
    // QMenu* subMenu = NULL;

    ga = DB_ActionFileNew::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionFileNewPlainTextEditor::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionFileNewHtmlEditor::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    // Internet browser TODO: rename
    ga = DB_ActionFileNewHtmlView::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
}

/**
 * Get the generic 'File' actions to the menu and toolbar
 * - Open
 * - Save
 * - Save as
 * - PDF
 * - Close
 * - Print
 * - Print Preview
 * - Quit
 * @param menu
 * @param tb toolbar
 */
void DB_ActionFactory::getFileMenu(QMenu* menu, QToolBar* tb) {
    RB_GuiAction* ga = NULL;
    // QMenu* subMenu = NULL;

    ga = DB_ActionFileOpen::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionFileSave::createGuiAction();
    ga->setEnabled(false);
    menu->addAction(ga);
    tb->addAction(ga);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowActive(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionFileSaveAs::createGuiAction();
    ga->setEnabled(false);
    menu->addAction(ga);
    tb->addAction(ga);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowActive(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionFilePdf::createGuiAction();
    ga->setEnabled(false);
    menu->addAction(ga);
    tb->addAction(ga);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowActive(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = DB_ActionFilePrint::createGuiAction();
    ga->setEnabled(false);
    menu->addAction(ga);
    tb->addAction(ga);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowActive(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionFilePrintPreview::createGuiAction();
    ga->setEnabled(false);
    menu->addAction(ga);
    tb->addAction(ga);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowActive(int)),
            ga, SLOT(slotSetEnabled(int)));
//    connect(this, SIGNAL(printPreviewChanged(bool)), ga, SLOT(setChecked(bool)));

    mSeparatorAct = menu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i) {
        mRecentFileActs[i] = new QAction(this);
        mRecentFileActs[i]->setVisible(false);
        menu->addAction(mRecentFileActs[i]);
        connect(mRecentFileActs[i], SIGNAL(triggered()),
                this, SLOT(slotOpenRecentFile()));
    }
    updateRecentFileActions();

    menu->addSeparator();

    ga = DB_ActionFileQuit::createGuiAction();
    menu->addAction(ga);
    connect(ga, SIGNAL(triggered()),
            DB_DIALOGFACTORY->getMainWindow(), SLOT(slotFileQuit()));
}

/**
 * Get Edit menu items 'on subwindow changed', for:
 * - Cut
 * - Copy
 * - Paste
 * - Undo
 * - Redo
 * - Select all
 * - HTML sub menu with insert image and hyperlink
 * @param menu
 * @param tb toolbar
 */
void DB_ActionFactory::getEditMenu(QMenu* menu, QToolBar* tb) {
    RB_GuiAction* ga = NULL;

    ga = DB_ActionEditCut::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false); // start with false
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    ga = DB_ActionEditCopy::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    ga = DB_ActionEditPaste::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    menu->addSeparator();
    tb->addSeparator();

    ga = DB_ActionEditUndo::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    ga = DB_ActionEditRedo::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    menu->addSeparator();
    tb->addSeparator();

    ga = DB_ActionEditSelectAll::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    ga = DB_ActionEditFind::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    ga = DB_ActionEditFindReplace::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    menu->addSeparator();
    tb->addSeparator();

    ga = DB_ActionEditZoomIn::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    ga = DB_ActionEditZoomOut::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mEditMenuActionList.push_back(ga);

    // HTML only
    //
    QMenu* subMenu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&HTML"));
    menu->addMenu(subMenu);
    QToolBar* subTb = new QToolBar(DB_DIALOGFACTORY->getMainWindow());
    subTb->setFixedHeight(30); // (32) ?
    subTb->setIconSize(QSize(16,16));
    subTb->setObjectName("toolbar_edit_html");
    subTb->setWindowTitle(tr("HTML"));
    DB_DIALOGFACTORY->getMainWindow()->addToolBar(Qt::TopToolBarArea, subTb);
    subTb->hide();

    ga = DB_ActionEditInsertImage::createGuiAction();
    ga->setEnabled(false);
    subMenu->addAction(ga);
    subTb->addAction(ga);
    ga->setEnabled(false); // start with false
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetTextDocument);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionEditInsertHtml::createGuiAction();
    ga->setEnabled(false);
    subMenu->addAction(ga);
    subTb->addAction(ga);
    ga->setEnabled(false); // start with false
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetTextDocument);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionEditCreateLink::createGuiAction();
    ga->setEnabled(false);
    subMenu->addAction(ga);
    subTb->addAction(ga);
    ga->setEnabled(false); // start with false
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
}

/**
 * Get Format menu items 'on subwindow changed', for:
 * - style, Paragraph, Heading 1,2,3,4,5,6, Preformatted, Address
 * - Align left, center, right, justify
 * - Bold
 * - Italic
 * - Underline
 * - Strikethrough
 * - Increase or decrease indent
 * - Numbered or bullet list
 * - Font name and size
 * - Text color and background color
 * @param menu
 * @param tb toolbar
 */
void DB_ActionFactory::getFormatMenu(QMenu* menu, QToolBar* tb) {
    RB_GuiAction* ga = NULL;
    QMenu* subMenu = NULL;

    // HTML only
    //
    subMenu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("St&yle"));
    menu->addMenu(subMenu);

    ga = DB_ActionStyleParagraph::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionStyleHeading1::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionStyleHeading2::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionStyleHeading3::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionStyleHeading4::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionStyleHeading5::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionStyleHeading6::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionStylePreformatted::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionStyleAddress::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    // General text edit
    //
    subMenu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Align"));
    menu->addMenu(subMenu);

    ga = DB_ActionFormatAlignLeft::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatAlignLeft(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatAlignRight::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatAlignRight(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatAlignCenter::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatAlignCenter(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatAlignJustify::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatAlignJustify(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    menu->addSeparator();
    tb->addSeparator();

    ga = DB_ActionFormatBold::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatBoldChecked(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatItalic::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatItalicChecked(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatUnderline::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatUnderlineChecked(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatStrikethrough::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatStrikethroughChecked(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    menu->addSeparator();
    tb->addSeparator();

    ga = DB_ActionFormatIncreaseIndent::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatDecreaseIndent::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mFormatMenuActionList.push_back(ga);

    menu->addSeparator();
    tb->addSeparator();

    ga = DB_ActionFormatNumberedList::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatNumberedListChecked(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatBulletedList::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    connect(this, SIGNAL(formatBulletedListChecked(bool)),
            ga, SLOT(setChecked(bool)));
    mFormatMenuActionList.push_back(ga);

    menu->addSeparator();
    tb->addSeparator();

    ga = DB_ActionFormatFontName::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatFontSize::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mFormatMenuActionList.push_back(ga);

    menu->addSeparator();
    tb->addSeparator();

    ga = DB_ActionFormatTextColor::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mFormatMenuActionList.push_back(ga);

    ga = DB_ActionFormatBackgroundColor::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_DIALOGFACTORY->WidgetHtmlEditor);
    connect(DB_DIALOGFACTORY->getMainWindow(), SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
    mFormatMenuActionList.push_back(ga);
}

/**
 * Get perspective menu items, such as ACC for accounting
 * @param menu
 * @param tb toolbar, not used here
 */
void DB_ActionFactory::getPerspectiveMenu(QMenu* menu, QToolBar* /* tb */) {
    mPerspectiveMenu = menu;
    mPerspectiveAction = menu->menuAction();

    QDir pluginsDir;
    setPluginsDir(pluginsDir);
    if (pluginsDir.entryList(QDir::Files).count() < 1) {
        RB_DEBUG->warning("DB_ActionFactory::getPerspectiveMenu() "
                          "no plugins found WARNING");
        return;
    }

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        RB_StringList partList = fileName.split("_");
        RB_String perspectiveName = partList.at(0).toUpper();
#if defined(Q_OS_MAC)
        // remove LIB from LIBACC
        perspectiveName.remove(0, 3);
#endif

        RB_GuiAction* ga = new RB_GuiAction(tr("Load perspective"),
                                            QIcon(":/images/icons/eye.png"),
                                            perspectiveName); // ACC, CAD, CRM, PENG
        ga->setCheckable(true);
        ga->setChecked(false);
        menu->addAction(ga);
        connect(ga, SIGNAL(triggered()), this, SLOT(slotPluginClicked()));
    }
}

/**
 * @returns the perspective main action to insert plugin menus before
 * this perspective action
 */
QAction* DB_ActionFactory::getPerspectiveAction() {
    return mPerspectiveAction;
}

/**
 * Get Tools menu items, for:
 * - Simple database report
 * - Calculator
 * - File text in text format
 * - Simple chart development
 * - etc.
 * @param menu
 * @param tb toolbar, not used here
 */
void DB_ActionFactory::getToolsMenu(QMenu* menu, QToolBar* /* tb */) {
    RB_GuiAction* ga = NULL;
    // QMenu* subMenu = NULL;

    ga = DB_ActionSystemCalendar::createGuiAction();
    menu->addAction(ga);

    ga = DB_ActionToolsCalculator::createGuiAction();
    menu->addAction(ga);

    ga = DB_ActionToolsSimpleReport::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));


    menu->addSeparator();


    ga = DB_ActionSystemStyleDark::createGuiAction();
    menu->addAction(ga);

    ga = DB_ActionSystemStyleColorPicker::createGuiAction();
    menu->addAction(ga);


    menu->addSeparator();


    ga = DB_ActionToolsScreenshot::createGuiAction();
    menu->addAction(ga);

    ga = DB_ActionToolsTextFileList::createGuiAction();
    menu->addAction(ga);

    ga = DB_ActionSystemToolsRegExp::createGuiAction();
    menu->addAction(ga);

    ga = DB_ActionToolsSimpleChart::createGuiAction();
    menu->addAction(ga);

#ifdef BILUNA_DEBUG
    // TODO: implement
    ga = DB_ActionToolsXmlView::createGuiAction();
    menu->addAction(ga);
#endif
}

/**
 * Get Settings menu items, for:
 * - Database connection
 * - Administrator, with submenu items
 * - - Browse database
 * - - Map data to database
 * @param menu
 * @param tb toolbar, not used here
 */
void DB_ActionFactory::getSystemMenu(QMenu* menu, QToolBar* /* tb */) {
    RB_GuiAction* ga = NULL;
    QMenu* subMenu = NULL;

    ga = DB_ActionSystemDatabaseConnect::createGuiAction();
    menu->addAction(ga);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsNotSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemDatabaseDisconnect::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemEditProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    // create and add submenu - Administrator
    subMenu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Administrator"));
    menu->addMenu(subMenu);

    ga = DB_ActionSystemGroup::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemPermission::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionPermissionReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subMenu->addSeparator();

    ga = DB_ActionSystemDatabaseBrowser::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemDatabaseMapping::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemVersion::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // create and add submenu - development
    subMenu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("De&velopment"));
    menu->addMenu(subMenu);

    ga = DB_ActionSystemToolsViewModel::createGuiAction();
    subMenu->addAction(ga);

    ga = DB_ActionSystemToolsExportModel::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemToolsImportModel::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
// #ifdef BILUNA_DEBUG
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemToolsPlugins::createGuiAction();
    subMenu->addAction(ga);
// #endif

    ga = DB_ActionSystemToolsSignalSpy::createGuiAction();
    subMenu->addAction(ga);
#ifndef BILUNA_DEBUG
    ga->setEnabled(false);
#endif

    ga = DB_ActionSystemToolsDevelop::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
#ifdef BILUNA_DEBUG
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));
#endif

    menu->addSeparator();

    ga = DB_ActionSystemClearSettings::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(true);
}

/**
 * Enable generic edit menu actions for widget type, to be called after
 * the creation of the edit menu
 * @param widgetType widget type for which menu should be activated
 */
void DB_ActionFactory::enableEditMenu(int widgetType) {
    for (uint i = 0; i < mEditMenuActionList.size(); i++) {
        RB_GuiAction* ga = mEditMenuActionList.at(i);
        ga->addEnabledType(widgetType);
    }
}

/**
 * Enable generic format menu actions for widget type, to be called after
 * the creation of the format menu
 * @param widgetType widget type for which menu should be activated
 */
void DB_ActionFactory::enableFormatMenu(int widgetType) {
    for (uint i = 0; i < mFormatMenuActionList.size(); i++) {
        RB_GuiAction* ga = mFormatMenuActionList.at(i);
        ga->addEnabledType(widgetType);
    }
}

/**
 * Get Help menu items, for:
 * - About
 * - Index
 * - Subject
 * - Credits
 * @param menu
 * @param tb toolbar, not used here
 */
void DB_ActionFactory::getHelpMenu(QMenu* menu, QToolBar* /* tb */) {
    RB_GuiAction* ga = NULL;
    // QMenu* subMenu = NULL;

    ga = DB_ActionHelpAbout::createGuiAction();
    menu->addAction(ga);
    connect(ga, SIGNAL(triggered()),
            DB_DIALOGFACTORY->getMainWindow(), SLOT(slotHelpAbout()));

    ga = DB_ActionHelpIndex::createGuiAction();
    menu->addAction(ga);
    connect(ga, SIGNAL(triggered()),
            DB_DIALOGFACTORY->getMainWindow(), SLOT(slotHelpIndex()));

    ga = DB_ActionHelpSubject::createGuiAction();
    menu->addAction(ga);
    // signal/slot connect not required because factory() directly calls
    // the slotHelpSubject() in RB_MainWindow with subject

    ga = DB_ActionHelpCredits::createGuiAction();
    menu->addAction(ga);
    connect(ga, SIGNAL(triggered()),
            DB_DIALOGFACTORY->getMainWindow(), SLOT(slotHelpCredits()));
}

/**
 * Emits signal to update format align left action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatAlignLeft(bool check) {
    emit this->formatAlignLeft(check);
}

/**
 * Emits signal to update format align right action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatAlignRight(bool check) {
    emit this->formatAlignRight(check);
}

/**
 * Emits signal to update format align center action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatAlignCenter(bool check) {
    emit this->formatAlignCenter(check);
}

/**
 * Emits signal to update format align justify action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatAlignJustify(bool check) {
    emit this->formatAlignJustify(check);
}

/**
 * Emits signal to update format bold action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatBoldChecked(bool check) {
    emit this->formatBoldChecked(check);
}

/**
 * Emits signal to update format italic action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatItalicChecked(bool check) {
    emit this->formatItalicChecked(check);
}

/**
 * Emits signal to update format underline action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatUnderlineChecked(bool check) {
    emit this->formatUnderlineChecked(check);
}

/**
 * Emits signal to update format bold action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatStrikethroughChecked(bool check) {
    emit this->formatStrikethroughChecked(check);
}

/**
 * Emits signal to update the numbered list action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatNumberedListChecked(bool check) {
    emit this->formatNumberedListChecked(check);
}

/**
 * Emits signal to update bulleted list action
 * @param check true if action is checked
 */
void DB_ActionFactory::setFormatBulletedListChecked(bool check) {
    emit this->formatBulletedListChecked(check);
}

/**
 * Set recent file in the file menu.
 * @param fn file name
 */
void DB_ActionFactory::setRecentFile(const RB_String &fn) {
    RB_SETTINGS->beginGroup("recentFileList");
    RB_StringList files = RB_SETTINGS->value("recentFiles").toStringList();
    RB_SETTINGS->endGroup();

    files.removeAll(fn);
    files.prepend(fn);

    while (files.size() > MaxRecentFiles) {
        files.removeLast();
    }


    RB_SETTINGS->beginGroup("recentFileList");
    RB_SETTINGS->setValue("recentFiles", files);
    RB_SETTINGS->endGroup();

    updateRecentFileActions();
}

/**
 * Update recent file actions, usually after openFile or saveFileAs action
 */
void DB_ActionFactory::updateRecentFileActions() {
    RB_SETTINGS->beginGroup("recentFileList");
    RB_StringList files = RB_SETTINGS->value("recentFiles").toStringList();
    RB_SETTINGS->endGroup();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        RB_String text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
        mRecentFileActs[i]->setText(text);
        mRecentFileActs[i]->setData(files[i]);
        mRecentFileActs[i]->setVisible(true);
    }

    for (int j = numRecentFiles; j < MaxRecentFiles; ++j) {
        mRecentFileActs[j]->setVisible(false);

    }

    mSeparatorAct->setVisible(numRecentFiles > 0);
}

/**
 * User clicked recent file on the main file menu
 */
void DB_ActionFactory::slotOpenRecentFile() {
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        DB_ActionFileOpen* ga = new DB_ActionFileOpen();
        ga->setOpenFilename(action->data().toString());
        ga->trigger();
        DB_DIALOGFACTORY->commandMessage("Open recent file: " + action->data().toString());
        delete ga;
    }
}

/**
 * User clicked perspective to be loaded or removed
 */
void DB_ActionFactory::slotPluginClicked() {
    RB_GuiAction* ga = qobject_cast<RB_GuiAction*>(sender());
    if (!ga) return;

    // Check if Biluna plugin has already been loaded
    RB_String pluginId = ga->text();
    RB_ActionPlugin* iPlugin = NULL;

    std::map<RB_String, RB_ActionPlugin*>::iterator iter;
    iter = mPluginList.find(pluginId);

    if (iter != mPluginList.end()) {
        iPlugin = (*iter).second;
    }

    // NOTE: by clicking the unchecked action the action is checked when arriving here
    if (iPlugin) {
        // remove perspective plugin
        int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Close perspective"),
                                        tr("Do you want to close the perspective?"));
        if (res != QMessageBox::Yes) {
            ga->setChecked(true); // only to make sure
            return;
        }

        removePlugin(pluginId);
        ga->setChecked(false); // only to make sure
    } else {
        // load plugin
        QApplication::setOverrideCursor(Qt::WaitCursor);
        DB_DIALOGFACTORY->statusBarMessage(tr("Loading perspective ..."), 4000);

        QDir pluginsDir;
        setPluginsDir(pluginsDir);

        if (pluginsDir.entryList(QDir::Files).count() < 1) {
            RB_DEBUG->error("DB_ActionFactory::slotPluginClicked() "
                            "no plugins found ERROR");
            return;
        }

        RB_String pluginId = ga->text();

        foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
            RB_StringList partList = fileName.split("_");
            RB_String perspectiveName = partList.at(0).toUpper();
#if defined(Q_OS_MAC)
            // remove LIB from LIBACC
            perspectiveName.remove(0, 3);
#endif

            if (pluginId == perspectiveName) {
                ga->setChecked(loadPlugin(pluginId, fileName));
            }
        }

        DB_DIALOGFACTORY->statusBarMessage(tr("Done"), 4000);
        QApplication::restoreOverrideCursor();
    }
}

/**
 * Set plugins directory
 * @param pluginsDir directory to be set
 */
void DB_ActionFactory::setPluginsDir(QDir& pluginsDir) {
    pluginsDir.setPath(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
//    if (pluginsDir.dirName().toLower() == "debug"
//            || pluginsDir.dirName().toLower() == "release") {
//        pluginsDir.cdUp();
//    }
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
}

/**
 * Load plugin
 * @param filePath plugin filepath
 */
bool DB_ActionFactory::loadPlugin(const RB_String& pluginId,
                                  const RB_String& fileName) {
    QDir pluginsDir;
    setPluginsDir(pluginsDir);

    QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
    QObject* plugin = pluginLoader.instance();

    if (plugin) {
        // Biluna action plugin interface
        RB_ActionPlugin* iPlugin = qobject_cast<RB_ActionPlugin* >(plugin);

        if (iPlugin) {
            iPlugin->getGuiActions(mMainWindow);
            mPluginList[pluginId] = iPlugin;
            return true;
        } else {
            RB_DEBUG->error("DB_ActionFactory::loadPlugin() " + fileName
                            + " could not cast plugin ERROR");
            DB_DIALOGFACTORY->commandMessage("DB_ActionFactory::loadPlugin() "
                                             + fileName
                                             + " could not cast plugin ERROR");
        }
    } else {
        RB_DEBUG->error("DB_ActionFactory::loadPlugin() " + fileName
                        + " could not create instance ERROR");
        DB_DIALOGFACTORY->commandMessage("DB_ActionFactory::loadPlugin() "
                                         + fileName
                                         + " could not create instance ERROR");
    }

    return false;
}

/**
 * Remove plugin
 * @param pluginId ID of plugin in perspective menu, such as ACC or CRM
 */
void DB_ActionFactory::removePlugin(const RB_String& pluginId) {
    // key: first, value: second
    std::map<RB_String, RB_ActionPlugin*>::iterator iter;
    iter = mPluginList.begin();
    while (iter != mPluginList.end()) {
        if ((*iter).first == pluginId) {
            delete (*iter).second;
            (*iter).second = NULL;
            iter = mPluginList.end();
        } else {
            ++iter;
        }
    }
}

/**
 * @return factory list
 */
std::vector<RB_ActionFactory*> DB_ActionFactory::getFactoryList() {
    return mFactoryList;
}

/**
 * Register factory
 * @param f Objectfactory
 */
void DB_ActionFactory::registerFactory(RB_ActionFactory* f) {
    bool exists = false;

    std::vector<RB_ActionFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end() && !exists) {
        if(f == *iter) {
            exists = true;
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_ActionFactory::registerFactory() "
                            "objectfactory already registered ERROR");
        }
        ++iter;
    }

    if (!exists) {
        mFactoryList.push_back(f);
    }
}

/**
 * Unregister factory
 * @param f Objectfactory
 */
void DB_ActionFactory::unregisterFactory(RB_ActionFactory* f) {
    bool exists = false;

    std::vector<RB_ActionFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (!exists && iter != mFactoryList.end()) {
        if (f == *iter) {
            mFactoryList.erase(iter);
            exists = true;
        } else {
            ++iter; // iterator not incrementable after erase last item
        }
    }

    if (!exists) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_ActionFactory::unregisterFactory() "
                        "objectfactory was not registered ERROR");
    }
}

/**
 * Close and delete all factories except this (DB) factory
 */
//void DB_ActionFactory::closeAllFactories() {
//    while (!mFactoryList.empty()) {
//        RB_ActionFactory* f = mFactoryList.back();
//        delete f; // unregisters factory and removes, therefor no pop_back()
//    }
//}

/**
 * Remove all plugins and the action(s) from the main menu/toolbars
 */
void DB_ActionFactory::removeAllPlugins() {
    std::map<RB_String, RB_ActionPlugin*>::iterator iter;
    iter = mPluginList.begin();
    while (iter != mPluginList.end()) {
        delete (*iter).second;
        (*iter).second = NULL;
        ++iter;
    }
    mPluginList.clear();
}

/**
 * Get (address/pointer) to system printer object
 */
QPrinter* DB_ActionFactory::getPrinter() {
    // HACK: because QTextDocument/QTextEdit do not work in Qt 5.2.0
    if (mPrinter) {
        delete mPrinter;
    }

#if !defined(QT_NO_EXCEPTIONS)
    try // For MSVC2013 added only 2015-06-17 Rutger
#endif
    {
        mPrinter = new QPrinter(QPrinter::HighResolution); // new QPrinter();
#if !defined(QT_NO_EXCEPTIONS)
    } catch(std::exception& e) {
        DB_DIALOGFACTORY->requestWarningDialog(e.what());
    } catch(...) {
        DB_DIALOGFACTORY->requestWarningDialog("Exception caught "
                                               "when creating printer");
#endif
    }

    mPrinter->setPaperSize(QPrinter::A4);
    mPrinter->setOrientation(QPrinter::Portrait);
    mPrinter->setPageMargins(20, 15, 15, 15, QPrinter::Millimeter);
    return mPrinter;
}

/**
 * @brief DB_ActionFactory::slotClosePerspective
 * @param code abbreviation of plugin for example ACC for Accounting
 */
void DB_ActionFactory::closePlugin(const RB_String& pluginId) {
    removePlugin(pluginId);

    if (mPerspectiveMenu) {
        QList<QAction*> actionList = mPerspectiveMenu->actions();

        int count = actionList.count();

        for (int i = 0; i < count; ++i) {
            QAction* a = actionList.at(i);

            if (a->text() == pluginId) {
                a->setChecked(false);
                i = count;
            }
        }
    }
}

