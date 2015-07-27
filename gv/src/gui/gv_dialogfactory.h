/*****************************************************************
 * $Id: gv_dialogfactory.h 1479 2011-11-17 15:51:46Z rutger $
 * Created: Sep 29, 2008 4:07:28 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_DIALOGFACTORY_H
#define GV_DIALOGFACTORY_H

#include <QObject>
#include "rb_dialogfactory.h"

#define GV_DIALOGFACTORY GV_DialogFactory::getInstance()

class GV_Drawing;
class GV_GraphicsView;
class RB_ModelFactory;
class GV_Snapper;

/**
 * Dialog and widget factory for GV project
 */
class GV_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~GV_DialogFactory();
    static GV_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();

    /**
     * DB Dialog types, GV = 24
     * example = 113XXX:
     * - 11 = ACC,
     * - 3 = dialog,
     * - XXX = sequence number
     * 113100 is starting number leaving 113000 to 113099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        DialogNone = 243100,            /**< Invalid dialog */
        DialogLineAttributes,           /**< Line attributes dialog */
        DialogPipeLine,                 /**< Pipe line (LDT) dialog */
        DialogProject,                  /**< Project edit and selection dialog */
        DialogDefault                   /**< Default- or all dialogs */
    };

    /**
     * DB Widget/dialog manager types, GV = 24
     * example = 112XXX:
     * - 11 = DB,
     * - 2 = widget,
     * - XXX = sequence number
     * 112100 is starting number leaving 112000 to 112099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        WidgetNone = 242100,            /**< Invalid model */
        WidgetDrawing,                  /**< Drawing widget */
        WidgetDrawingList,              /**< Drawing manager dockwidget */
        WidgetGraphicsViewQt,           /**< Graphics view, neutral viewer in Qt */
        WidgetGraphicsViewQV,           /**< Graphics view, QGraphicsView and QGraphicsScene */
        WidgetLegend,                   /**< Legend dockwidget */
        WidgetPreview,                  /**< Drawing preview dockwidget */
        WidgetProperties,               /**< Properties dockwidget */
        WidgetDefault                   /**< Default- or all models */
    };

    virtual GV_Drawing* getDrawing();
    virtual GV_GraphicsView* getGraphicsView();
    virtual void updateMouseWidget(const RB_String&, const RB_String) { }

private:
    //! Private constructor
    GV_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static GV_DialogFactory* mActiveFactory;

};

#endif /*GV_DIALOGFACTORY_H*/
