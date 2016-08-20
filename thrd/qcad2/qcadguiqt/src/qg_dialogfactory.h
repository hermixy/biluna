/****************************************************************************
** $Id: qg_dialogfactory.h 10351 2008-04-06 19:23:52Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
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

#ifndef QG_DIALOGFACTORY_H
#define QG_DIALOGFACTORY_H

#include <QObject>
#include <QWidget>
#include <QToolBar>

#include "rs_dialogfactory.h"
#include "rs_dialogfactoryinterface.h"
#include "rs_vector.h"
#include "rs_debug.h"
//#include "rs_layerlistlistener.h"

#include "qg_cadtoolbar.h"
#include "qg_coordinatewidget.h"
#include "qg_mainwindow.h"
#include "qg_mousewidget.h"
#include "qg_selectionwidget.h"

#include "qg_commandwidget.h"

#include "qg_propertyeditor.h"

class RS_Document;
#ifdef RS_CAM
class RS_SimulationControls;
#endif

class QG_LayerWidget;
class QG_BlockWidget;
class QG_LibraryWidget;
class QG_PenToolBar;
class QG_PropertyEditor;


#define QG_DIALOGFACTORY (RS_DialogFactory::instance()->getFactoryObject()->isAdapter()==false ? ((QG_DialogFactory*)RS_DialogFactory::instance()->getFactoryObject()) : NULL)

/**
 * This is the Qt implementation of a widget which can create and
 * show dialogs and tool option widgets.
 */
class QG_DialogFactory: public QObject, public RS_DialogFactoryInterface {
    Q_OBJECT

protected:
    QG_DialogFactory(QWidget* parent);

public:
    static void init(QWidget* parent) {
        RS_DialogFactory::instance()->setFactoryObject(
            new QG_DialogFactory(parent));
    }

    virtual ~QG_DialogFactory();

public:
    /*
    virtual RS_LayerListListener* getLayerWidget(int i) {
        if (i==0) {
            return (RS_LayerListListener*)penToolBar;
        }
        else if (i==1) {
            return (RS_LayerListListener*)layerWidget;
        }
        else {
            return NULL;
        }
    }
    */

    /*
    virtual RS_PtrList<RS_LayerListListener> getLayerWidgets() {
        RS_PtrList<RS_LayerListListener> ret;

        if (penToolBar!=NULL) {
            RS_DEBUG->print("QG_DialogFactory::getLayerWidgets: "
                "penToolBar address: %d", (int)penToolBar);
            ret.append((RS_LayerListListener*)penToolBar);
        }
        if (layerWidget!=NULL) {
            RS_DEBUG->print("QG_DialogFactory::getLayerWidgets: "
                "layerWidget address: %d", (int)layerWidget);
            ret.append((RS_LayerListListener*)layerWidget);
        }

        return ret;
    }
    */
    
    virtual QToolBar* createOptionWidget(QWidget* parent);

    virtual QToolBar* getOptionWidget() {
        return optionWidget;
    }
    
    //virtual void addWidgetToOptionWidget(RS_Widget* w);

    virtual QG_CoordinateWidget* createCoordinateWidget(QWidget* parent);

    virtual QG_CoordinateWidget* getCoordinateWidget() {
        return coordinateWidget;
    }

    virtual QG_MouseWidget* createMouseWidget(QWidget* parent);

    virtual QG_MouseWidget* getMouseWidget() {
        return mouseWidget;
    }
    
    virtual QG_SelectionWidget* createSelectionWidget(QWidget* parent);

    virtual QG_SelectionWidget* getSelectionWidget() {
        return selectionWidget;
    }

    virtual QG_CadToolBar* createCadToolBar(/*RS_ActionHandler* ah, */
        QWidget* parent);

    virtual QG_CadToolBar* getCadToolBar() {
        return cadToolBar;
    }

    virtual QG_LayerWidget* createLayerWidget(/*RS_ActionHandler* ah, */
        QWidget* parent);

    virtual QG_LayerWidget* getLayerWidget() {
        return layerWidget;
    }
    
    virtual QG_BlockWidget* createBlockWidget(/*RS_ActionHandler* ah, */
        QWidget* parent);

    virtual QG_BlockWidget* getBlockWidget() {
        return blockWidget;
    }
    
    virtual QG_LibraryWidget* createLibraryWidget(/*RS_ActionHandler* ah, */
        QWidget* parent);

    virtual QG_LibraryWidget* getLibraryWidget() {
        return libraryWidget;
    }

    virtual QG_CommandWidget* createCommandWidget(/*RS_ActionHandler* ah, */
        QWidget* parent);

    /**
     * @return Command widget or NULL.
     */
    QG_CommandWidget* getCommandWidget() {
        return commandWidget;
    }

    virtual QG_PropertyEditor* createPropertyEditor(/*RS_ActionHandler* ah, */
        QWidget* parent);

    /**
     * @return Property editor or NULL.
     */
    virtual QG_PropertyEditor* getPropertyEditor() {
        return propertyEditor;
    }
        
    virtual void enablePropertyEditor(bool on);
    virtual void updatePropertyEditor(RS_PropertyOwner* e, bool includeGeometry=true);
    virtual void updatePropertyEditor(RS_PropertyOwnerContainerInterface* doc, bool includeGeometry=true);
    virtual void updatePropertyEditorGui();
    
#ifdef RS_CAM
    virtual RS_SimulationControls* createSimulationControls(QWidget* parent);

    virtual RS_SimulationControls* getSimulationControls() {
        return simulationControls;
    }
#endif
    
    virtual QG_PenToolBar* createPenToolBar(QWidget* parent);

    virtual QG_PenToolBar* getPenToolBar() {
        return penToolBar;
    }
    
    virtual void requestWarningDialog(const RS_String& warning);
    
    virtual RS_GraphicView* requestNewDocument(const RS_String& fileName = RS_String::null, 
            RS_Document* doc=NULL);
#ifdef RS_CAM
    virtual void requestSimulationControls();
#endif
    virtual void requestWindowResize(int width, int height);

    virtual RS_Layer* requestNewLayerDialog(
        RS_LayerList* layerList = NULL);
    virtual RS_Layer* requestLayerRemovalDialog(
        RS_LayerList* layerList = NULL);
    virtual RS_Layer* requestEditLayerDialog(
        RS_LayerList* layerList = NULL);

    virtual RS_BlockData requestNewBlockDialog(RS_BlockList* blockList);
    virtual RS_Block* requestBlockRemovalDialog(
        RS_BlockList* blockList);
    virtual RS_BlockData requestBlockAttributesDialog(
        RS_BlockList* blockList);
    //virtual RS_String requestFileSaveAsDialog();
    //virtual RS_String requestFileOpenDialog();
    
    virtual RS_String requestImageOpenDialog();


    virtual void requestOptions(RS_ActionInterface* action,
                                bool on, bool update = false);
    virtual void requestSnapOptions(RS_Snapper* snapper, 
                                bool on);

protected:
    template<class T> void requestActionOptions(RS_ActionInterface* action,
                                    bool on, bool update);
    template<class T> void requestSnapOptions2(RS_Snapper* snapper,
                                    bool on);

public:
    virtual void requestToolBar(RS2::ToolBarId id);
    virtual void requestToolBarSelect(RS_ActionInterface* (*f)());

    virtual bool requestAttributesDialog(RS_AttributesData& data, 
                RS_LayerList& layerList);
    virtual bool requestMoveDialog(RS_MoveData& data);
    virtual bool requestRotateDialog(RS_RotateData& data);
    virtual bool requestScaleDialog(RS_ScaleData& data);
    virtual bool requestMirrorDialog(RS_MirrorData& data);
    virtual bool requestMoveRotateDialog(RS_MoveRotateData& data);
    virtual bool requestRotate2Dialog(RS_Rotate2Data& data);
    
    virtual bool requestModifyEntityDialog(RS_Entity* entity);
    virtual bool requestTextDialog(RS_Text* text);
    virtual bool requestHatchDialog(RS_Hatch* hatch);
    
#ifdef RS_CAM
    virtual bool requestCamOptionsDialog(RS_Graphic& graphic);
#endif

    virtual void requestOptionsGeneralDialog();
    virtual void requestOptionsDrawingDialog(RS_Graphic& graphic);

    virtual void requestPreviousMenu();
    
    virtual bool requestOneTimeDialog(const RS_String& text, const RS_String& id);
    
    virtual void requestEditBlockWindow(RS_BlockList* blockList = NULL);
    virtual void requestEditBlockWindow(const RS_String& blockName);
    virtual void closeEditBlockWindow(RS_Block* block = NULL);

    virtual void updateCoordinateWidget(const RS_Vector& abs,
                                        const RS_Vector& rel,
                                        bool updateFormat=false);
    virtual void updateMouseWidget(const RS_String& left,
                                   const RS_String& right);
    virtual void updateSelectionWidget(int num);
    virtual void commandMessage(const RS_String& message);
    virtual bool isAdapter() { return false; }

    static QString extToFormat(const QString& ext);
    
    virtual void setMainWindow(QG_MainWindow* mw) {
        RS_DEBUG->print("QG_MainWindow::setMainWindow");
        RS_DialogFactoryInterface::setMainWindow(mw);
        qgMainWindow = mw;
        RS_DEBUG->print("QG_MainWindow::setMainWindow: OK");
    }

    /*
    virtual void setCurrentDocument(RS_Document* doc);
    virtual void setCurrentLayerList(RS_LayerList* l);
    */
    virtual void updateUiConnections();
            
public slots:
    void slotPropertyChanged(RS_PropertyOwner* propertyOwner, 
        const QString& propertyName, const QVariant& propertyValue);
    void slotPropertyChanged(RS_PropertyOwnerContainerInterface* propertyOwnerContainer, 
        const QString& propertyName, const QVariant& propertyValue);

protected:
    //! Pointer to the widget which can host dialogs
    QWidget* parent;
    //! Pointer to the widget which can host individual tool options
    QToolBar* optionWidget;
    //! Pointer to the coordinate widget.
    QG_CoordinateWidget* coordinateWidget;
    //! Pointer to the mouse widget.
    QG_MouseWidget* mouseWidget;
    //! Pointer to the selection widget.
    QG_SelectionWidget* selectionWidget;
    //! Pointer to the CAD tool bar
    QG_CadToolBar* cadToolBar;
    //! Layer list widget
    QG_LayerWidget* layerWidget;
    //! Block list widget
    QG_BlockWidget* blockWidget;
    //! Library browser widget
    QG_LibraryWidget* libraryWidget;
    //! Pointer to the command line widget
    QG_CommandWidget* commandWidget;
    //! Property editor widget
    QG_PropertyEditor* propertyEditor;
#ifdef RS_CAM
    //! CAM Simulation panel
    RS_SimulationControls* simulationControls;
#endif
    //! Pen tool bar
    QG_PenToolBar* penToolBar;
    //! main window
    QG_MainWindow* qgMainWindow;
};

#endif
