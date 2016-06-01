/*****************************************************************
 * $Id: peng_dialogfactory.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_dialogfactory.h"

#include "db_dialogfactory.h"
#include "peng_conestrikeoutwidget.h"
//#include "peng_equipmentmasterwidget.h"
#include "peng_fluiddialog.h"
#include "peng_fluidstabilitydialog.h"
#include "peng_hazardgroupdialog.h"
#include "peng_insulationdialog.h"
#include "peng_linedetailwidget.h"
#include "peng_navigationwidget.h"
#include "peng_paintdialog.h"
#include "peng_pedeqsummarywidget.h"
#include "peng_pedwidget.h"
#include "peng_pipeclassdialog.h"
#include "peng_projectdialog.h"
#include "peng_projecteditwidget.h"
#include "peng_settingsdialog.h"
#include "peng_typeeditdialog.h"
#include "peng_typeselectdialog.h"
#include "peng_weldoletweldvolumewidget.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"


PENG_DialogFactory* PENG_DialogFactory::mActiveFactory = 0;

/**
 * Constructor
 */
PENG_DialogFactory::PENG_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    this->setPerspective(RB2::PerspectivePENG);
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PENG_DialogFactory::~PENG_DialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("PENG_DialogFactory::~PENG_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
PENG_DialogFactory* PENG_DialogFactory::getInstance(RB_MainWindow* mw) {
    RB_DEBUG->print("PENG_DialogFactory::getInstance()");

    if (!mActiveFactory) {
        mActiveFactory = new PENG_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* PENG_DialogFactory::getModelFactory() {
    return PENG_MODELFACTORY;
}

/**
 * Create widget for dock- or MDI window, could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* PENG_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = findWidget(type);
    if (wgt) return wgt; // widget already found

    switch (type) {
    case WidgetConeStrikeOut: {
        wgt = new PENG_ConeStrikeOutWidget(parent);
        break;
    }
    case WidgetEquipmentDetail: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(PENG_DialogFactory::WidgetNavigation,
                                        Qt::LeftDockWidgetArea);
        PENG_NavigationWidget* wdgt = dynamic_cast<PENG_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(PENG_NavigationWidget::TabEquipment);
        dw->show();

        // TODO: create requested widget
//        wgt = new PENG_EquipmentWidget(parent);
        break;
    }
    case WidgetLineDetail: {
        RB_DockWidget* dw = getDockWidget(PENG_DialogFactory::WidgetNavigation,
                                        Qt::LeftDockWidgetArea);
        PENG_NavigationWidget* wdgt = dynamic_cast<PENG_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(PENG_NavigationWidget::TabPipeLine);
        dw->show();

        wgt = new PENG_LineDetailWidget(parent);
        break;
    }
    case WidgetNavigation: {
        // Main dockwidget P&ID, Equipment, Lines, etc.
        if (!wgt) {
            wgt = new PENG_NavigationWidget(parent);
        }
        break;
    }
    case WidgetPedClassification: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(PENG_DialogFactory::WidgetNavigation,
                                        Qt::LeftDockWidgetArea);
        PENG_NavigationWidget* wdgt = dynamic_cast<PENG_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(PENG_NavigationWidget::TabEquipment);
        dw->show();

        // create requested widget
        if (!wgt) {
            wgt = new PENG_PedWidget(parent);
        }
        break;
    }
    case WidgetPedEqSummary: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(PENG_DialogFactory::WidgetNavigation,
                                        Qt::LeftDockWidgetArea);
        PENG_NavigationWidget* wdgt = dynamic_cast<PENG_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(PENG_NavigationWidget::TabEquipment);
        dw->show();

        // create requested widget
        if (!wgt) {
            wgt = new PENG_PedEqSummaryWidget(parent);
        }
        break;
    }
    case WidgetProjectEdit: {
        wgt = new PENG_ProjectEditWidget(parent);
        break;
    }
    case WidgetWeldoletWeldVolume: {
        wgt = new PENG_WeldoletWeldVolumeWidget(parent);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PENG_DialogFactory::getWidget() "
                        "non-existing widget requested ERROR");
        break;
    }

    if (wgt) {
        wgt->setWidgetType(type);
        wgt->setDialogFactory(this);
        // Set widget as unique widget, set 'inactive' via removeWidget()
        mWidgetList[type] = wgt;
        // not required here wgt->init();
    }

    return wgt;
}

/**
 * Request a dialog
 * @param type type of dialog
 * @return Dialog
 */
RB_Dialog* PENG_DialogFactory::getDialog(int type) {
    RB_DEBUG->print("PENG_TestDialogFactory::getDialog()");

    RB_Dialog* dlg = NULL;

    switch (type) {
    case DialogEqTypeEdit: {
        dlg = new PENG_TypeEditDialog(mMainWindow);
        break;
    }
    case DialogEqTypeSelect: {
        dlg = new PENG_TypeSelectDialog(mMainWindow);
        break;
    }
    case DialogFluid: {
        dlg = new PENG_FluidDialog(mMainWindow);
        break;
    }
    case DialogFluidStability: {
        dlg = new PENG_FluidStabilityDialog(mMainWindow);
        break;
    }
    case DialogHazardGroup: {
        dlg = new PENG_HazardGroupDialog(mMainWindow);
        break;
    }
    case DialogInsulation: {
        dlg = new PENG_InsulationDialog(mMainWindow);
        break;
    }
    case DialogPaint: {
        dlg = new PENG_PaintDialog(mMainWindow);
        break;
    }
    case DialogPipeClass: {
        dlg = new PENG_PipeClassDialog(mMainWindow);
        break;
    }
    case DialogProject: {
        dlg = new PENG_ProjectDialog(mMainWindow);
        break;
    }
    case DialogSettings: {
        dlg = new PENG_SettingsDialog(mMainWindow);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PENG_DialogFactory::getDialog(type)"
                        "non-existing dialog requested ERROR");
        requestWarningDialog("PENG_DialogFactory::getDialog() ERROR\n"
                             "Non-existing dialog requested.");
        break;
    }

    if (dlg) {
        dlg->init();
    }
    return dlg;
}

/**
 * Update signal slot user interface connections
 */
void PENG_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("PENG_DialogFactory::updateUiConnections()");

    // For dock windows

}

/**
 * Called when an action needs to communicate 'message' to the user.
 */
void PENG_DialogFactory::commandMessage(const RB_String& message) {
    DB_DIALOGFACTORY->commandMessage(message);
}

