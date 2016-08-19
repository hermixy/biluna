/****************************************************************************
** $Id: qg_dialogfactory.cpp 10351 2008-04-06 19:23:52Z andrew $
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

#include "qg_dialogfactory.h"

#include <typeinfo>

#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QStringList>
#include <QMdiArea>

#include "rs_actioninterface.h"
#include "rs_document.h"
#include "rs_hatch.h"
#include "rs_patternlist.h"
#include "rs_settings.h"
#include "rs_spline.h"
#include "rs_system.h"
#include "rs_text.h"

#include "rs_actionblocksinsert.h"
#include "rs_actiondimaligned.h"
#include "rs_actiondimangular.h"
#include "rs_actiondimdiametric.h"
#include "rs_actiondimlinear.h"
#include "rs_actiondimlinear.h"
#include "rs_actiondimordinate.h"
#include "rs_actiondimradial.h"
#include "rs_actiondrawarc.h"
#include "rs_actiondrawarctangential.h"
#include "rs_actiondrawcirclecr.h"
#include "rs_actiondrawimage.h"
#include "rs_actiondrawlineangle.h"
#include "rs_actiondrawlinebisector.h"
#include "rs_actiondrawline.h"
#include "rs_actiondrawlineparallel.h"
#include "rs_actiondrawlineparallelthrough.h"
#include "rs_actiondrawlinepolygon2.h"
#include "rs_actiondrawlinepolygon.h"
#include "rs_actiondrawlinerelangle.h"
#include "rs_actiondrawspline.h"
#include "rs_actiondrawtext.h"
#include "rs_actioneditpaste.h"
#include "rs_actionlibraryinsert.h"
#include "rs_actionmodifybevel.h"
#include "rs_actionmodifydivide2.h"
#include "rs_actionmodifyisoproject.h"
#include "rs_actionmodifymoverotate.h"
#include "rs_actionmodifyround.h"
#include "rs_actionmodifytrimamount.h"

#ifdef RS_PROF
#include "qg_commandwidget.h"
#include "qg_polylineoptions.h"
#include "qg_polylineequidistantoptions.h"
#endif

#ifdef RS_PROPERTYEDITOR
#include "qg_propertyeditor.h"
#endif

#ifdef RS_CAM
#include "rs_camdialog.h"
#include "rs_simulationcontrols.h"
#endif

#include "qg_arcoptions.h"
#include "qg_arctangentialoptions.h"
#include "qg_beveloptions.h"
#include "qg_blockdialog.h"
#include "qg_blockwidget.h"
#include "qg_cadtoolbar.h"
#include "qg_circleoptions.h"
#include "qg_dimlinearoptions.h"
#include "qg_dimoptions.h"
#include "qg_divide2options.h"
#include "qg_dlgarc.h"
#include "qg_dlgattributes.h"
#include "qg_dlgcircle.h"
#include "qg_dlgdimension.h"
#include "qg_dlgdimlinear.h"
#include "qg_dlgellipse.h"
#include "qg_dlghatch.h"
#include "qg_dlginsert.h"
#include "qg_dlgline.h"
#include "qg_dlgmirror.h"
#include "qg_dlgmove.h"
#include "qg_dlgmoverotate.h"
#include "qg_dlgonetime.h"
#include "qg_dlgoptionsdrawing.h"
#include "qg_dlgoptionsgeneral.h"
#include "qg_dlgpoint.h"
#include "qg_dlgrotate2.h"
#include "qg_dlgrotate.h"
#include "qg_dlgscale.h"
#include "qg_dlgspline.h"
#include "qg_dlgtext.h"
#include "qg_editpasteoptions.h"
#include "qg_imageoptions.h"
#include "qg_insertoptions.h"
#include "qg_isoprojectoptions.h"
#include "qg_layerdialog.h"
#include "qg_layerwidget.h"
#include "qg_libraryinsertoptions.h"
#include "qg_librarywidget.h"
#include "qg_lineangleoptions.h"
#include "qg_linebisectoroptions.h"
#include "qg_lineoptions.h"
#include "qg_lineparalleloptions.h"
#include "qg_lineparallelthroughoptions.h"
#include "qg_linepolygon2options.h"
#include "qg_linepolygonoptions.h"
#include "qg_linerelangleoptions.h"
#include "qg_mainwindow.h"
#include "qg_mdiwindow.h"
#include "qg_mousewidget.h"
#include "qg_moverotateoptions.h"
#include "qg_pentoolbar.h"
#include "qg_printpreviewoptions.h"
#include "qg_roundoptions.h"
#include "qg_selectionwidget.h"
#include "qg_snapcoordinateoptions.h"
#include "qg_snapcoordinatepolaroptions.h"
#include "qg_snapdistoptions.h"
#include "qg_splineoptions.h"
#include "qg_textoptions.h"
#include "qg_trimamountoptions.h"

#ifdef RS_ARCH
#include "rs_archutils.h"
#endif


/**
 * Constructor.
 *
 * @param parent Pointer to parent widget which can host dialogs.
 */
QG_DialogFactory::QG_DialogFactory(QWidget* parent)
        : RS_DialogFactoryInterface() {
    RS_DEBUG->print("QG_DialogFactory::QG_DialogFactory");

    this->parent = parent;

    optionWidget = NULL;
    coordinateWidget = NULL;
    mouseWidget = NULL;
    selectionWidget = NULL;
    cadToolBar = NULL;
    //mainWindow = NULL;
    qgMainWindow = NULL;
    
    layerWidget = NULL;
    blockWidget = NULL;
    libraryWidget = NULL;
#ifdef RS_PROF
    commandWidget = NULL;
#endif
#ifdef RS_PROPERTYEDITOR
    propertyEditor = NULL;
#endif
#ifdef RS_CAM
    simulationControls = NULL;
#endif
    penToolBar = NULL;

    RS_DEBUG->print("QG_DialogFactory::QG_DialogFactory: OK");
}



/**
 * Destructor
 */
QG_DialogFactory::~QG_DialogFactory() {
    RS_DEBUG->print("QG_DialogFactory::~QG_DialogFactory");
    RS_DEBUG->print("QG_DialogFactory::~QG_DialogFactory: OK");
}
    
    
    
/**
 * Links factory to a widget that can host tool options.
 */
QToolBar* QG_DialogFactory::createOptionWidget(QWidget* parent) {
    optionWidget = new QToolBar(parent);
    optionWidget->setObjectName("toolbar_tooloptions");
    // Qt4 hack to give the toolbar a fixed size:
    QWidget* w = new QWidget(optionWidget);
    w->setFixedSize(QSize(1,28));
    optionWidget->addWidget(w);
    //optionWidget->setFixedHeight(26);
    optionWidget->setWindowTitle(tr("Tool Options"));
    return optionWidget;
}
    
    
/*
// maybe at some point...
void QG_DialogFactory::addWidgetToOptionWidget(RS_Widget* w) {
    if (optionWidget!=NULL) {
        optionWidget->addWidget(w);
    }
}
*/


/**
 * Creates the coordinate widget.
 *
 * @param parent Parent of the widget.
 * @return Pointer to the new widget.
 */
QG_CoordinateWidget* QG_DialogFactory::createCoordinateWidget(QWidget* parent) {
    coordinateWidget = new QG_CoordinateWidget(parent);
    return coordinateWidget;
}

/**
 * Links this dialog factory to a mouse widget.
 */
QG_MouseWidget* QG_DialogFactory::createMouseWidget(QWidget* parent) {
    mouseWidget = new QG_MouseWidget(parent);
    return mouseWidget;
}

/**
 * Links this dialog factory to a selection widget.
 */
QG_SelectionWidget* QG_DialogFactory::createSelectionWidget(QWidget* parent) {
    selectionWidget = new QG_SelectionWidget(parent);
    return selectionWidget;
}

/**
 * Links this dialog factory to the cad tool bar.
 */
QG_CadToolBar* QG_DialogFactory::createCadToolBar(/*RS_ActionHandler* ah,*/ QWidget* parent) {
    cadToolBar = new QG_CadToolBar(parent);
    cadToolBar->createSubToolBars(/*ah*/);
    connect(cadToolBar, SIGNAL(signalBack()),
            qgMainWindow, SLOT(slotBack()));
    connect(qgMainWindow, SIGNAL(windowsChanged(bool)),
            cadToolBar, SLOT(setEnabled(bool)));
    return cadToolBar;
}

/**
 * Links this dialog factory to a command widget.
 */
#ifdef RS_PROF
QG_CommandWidget* QG_DialogFactory::createCommandWidget(
    /*RS_ActionHandler* ah,*/ QWidget* parent) {

    commandWidget = new QG_CommandWidget(/*ah,*/ parent);
    connect(commandWidget, SIGNAL(escape()),
            qgMainWindow, SLOT(slotFocus()));
    return commandWidget;
}
#endif

/**
 * Links this dialog factory to a layer widget.
 */
QG_LayerWidget* QG_DialogFactory::createLayerWidget(
    /*RS_ActionHandler* ah,*/ QWidget* parent) {
    
    layerWidget = new QG_LayerWidget(/*ah,*/ parent);
    //layerWidget->setFocusPolicy(Qt::NoFocus);
    connect(layerWidget, SIGNAL(escape()),
            qgMainWindow, SLOT(slotFocus()));
    connect(qgMainWindow, SIGNAL(windowsChanged(bool)),
            layerWidget, SLOT(setEnabled(bool)));
    return layerWidget;
}

/**
 * Links this dialog factory to a block widget.
 */
QG_BlockWidget* QG_DialogFactory::createBlockWidget(
    /*RS_ActionHandler* ah,*/ QWidget* parent) {

    blockWidget = new QG_BlockWidget(/*ah,*/ parent);
    blockWidget->setFocusPolicy(Qt::NoFocus);
    connect(blockWidget, SIGNAL(escape()),
            qgMainWindow, SLOT(slotFocus()));
    connect(qgMainWindow, SIGNAL(windowsChanged(bool)),
            blockWidget, SLOT(setEnabled(bool)));
    return blockWidget;
}

/**
 * Links this dialog factory to a library widget.
 */
QG_LibraryWidget* QG_DialogFactory::createLibraryWidget(/*RS_ActionHandler* ah,*/ QWidget* parent) {
    libraryWidget = new QG_LibraryWidget(/*ah,*/ parent);
    libraryWidget->setFocusPolicy(Qt::NoFocus);
    connect(libraryWidget, SIGNAL(escape()),
            qgMainWindow, SLOT(slotFocus()));
    connect(qgMainWindow, SIGNAL(windowsChanged(bool)),
            libraryWidget, SLOT(setInsertEnabled(bool)));
    return libraryWidget;
}



/**
 * Links this dialog factory to a property editor.
 */
#ifdef RS_PROPERTYEDITOR
QG_PropertyEditor* QG_DialogFactory::createPropertyEditor(
    /*RS_ActionHandler* ah,*/ QWidget* parent) {
    
    propertyEditor = new QG_PropertyEditor(/*ah,*/ parent);
    //propertyEditor->setFocusPolicy(Qt::NoFocus);
    connect(propertyEditor, SIGNAL(escape()),
            qgMainWindow, SLOT(slotFocus()));
    connect(qgMainWindow, SIGNAL(windowsChanged(bool)),
            propertyEditor, SLOT(setEnabled(bool)));
    return propertyEditor;
}
#endif

    

/**
 * Enables / disables (gray out) the property editor widget.
 **/
#ifdef RS_PROPERTYEDITOR
void QG_DialogFactory::enablePropertyEditor(bool on) {
    if (propertyEditor!=NULL) {
        propertyEditor->setEnabled(on);
    }
}
#endif



/**
 * Updates the property editor (if available) to reflect the given entity.
 */
#ifdef RS_PROPERTYEDITOR
void QG_DialogFactory::updatePropertyEditor(RS_PropertyOwner* e, bool includeGeometry) {
    RS_DEBUG->print("QG_DialogFactory::updatePropertyEditor");
    if (propertyEditor!=NULL) {
        propertyEditor->updateFor(e, includeGeometry);
        //propertyEditor->updateGui();
    }
    else {
        RS_DEBUG->print("QG_DialogFactory::updatePropertyEditor: propertyEditor==NULL");
    }
}
#endif
    
#ifdef RS_PROPERTYEDITOR
void QG_DialogFactory::updatePropertyEditor(RS_PropertyOwnerContainerInterface* doc, bool includeGeometry) {
    RS_DEBUG->print("QG_DialogFactory::updatePropertyEditor");
    if (propertyEditor!=NULL) {
        propertyEditor->update(doc, includeGeometry);
    }
    else {
        RS_DEBUG->print("QG_DialogFactory::updatePropertyEditor: propertyEditor==NULL");
    }
}
#endif


#ifdef RS_PROPERTYEDITOR
void QG_DialogFactory::updatePropertyEditorGui() {
    RS_DEBUG->print("QG_DialogFactory::updatePropertyEditorGui");
    if (propertyEditor!=NULL) {
        propertyEditor->updateGui();
    }
    else {
        RS_DEBUG->print("QG_DialogFactory::updatePropertyEditorGui: propertyEditor==NULL");
    }
}
#endif
    
    
#ifdef RS_CAM
RS_SimulationControls* QG_DialogFactory::createSimulationControls(QWidget* parent) {
    simulationControls = new RS_SimulationControls(parent);
    simulationControls->setFocusPolicy(Qt::NoFocus);
    connect(simulationControls, SIGNAL(escape()),
            qgMainWindow, SLOT(slotFocus()));
    connect(qgMainWindow, SIGNAL(windowsChanged(bool)),
            simulationControls, SLOT(setEnabled(bool)));
    return simulationControls;
}
#endif


/**
 * Links this dialog factory to a pen tool bar.
 */
QG_PenToolBar* QG_DialogFactory::createPenToolBar(QWidget* parent) {
    RS_DEBUG->print("QG_DialogFactory::createPenToolBar");
    penToolBar = new QG_PenToolBar(parent);
    penToolBar->setObjectName("toolbar_pen");
    penToolBar->setWindowTitle(tr("Pen"));
    penToolBar->hide();

    if (qgMainWindow==NULL) {
        RS_DEBUG->print("QG_DialogFactory::createPenToolBar: qgMainWindow==NULL");
    }
    
    connect(penToolBar, SIGNAL(penChanged(RS_Pen)),
            qgMainWindow, SLOT(slotPenChanged(RS_Pen)));

    RS_DEBUG->print("QG_DialogFactory::createPenToolBar: OK");
    return penToolBar;
}


/**
 * Shows a message dialog.
 */
void QG_DialogFactory::requestWarningDialog(const RS_String& warning) {
    QMessageBox::information(parent, QMessageBox::tr("Warning"),
                             warning,
                             QMessageBox::Ok);
}



/**
 * Requests a new document from the main window.
 */
RS_GraphicView* QG_DialogFactory::requestNewDocument(const RS_String& fileName, RS_Document* doc) {
    if (qgMainWindow!=NULL) {
        qgMainWindow->createNewDocument(fileName, doc);
        return qgMainWindow->getGraphicView();
    }
    return NULL;
}


/**
 * Requests the simulation controls.
 */
#ifdef RS_CAM
void QG_DialogFactory::requestSimulationControls() {
    if (qgMainWindow!=NULL) {
        qgMainWindow->showSimulationControls();
    }
}
#endif


/**
 * Requests main window resize.
 */
void QG_DialogFactory::requestWindowResize(int width, int height) {
    RS_DEBUG->print("QG_DialogFactory::requestWindowResize");
    if (qgMainWindow!=NULL) {
        qgMainWindow->resizeWindow(width, height);
    }
}


/**
 * Shows a dialog for adding a layer. Doesn't add the layer.
 * This is up to the caller.
 *
 * @return a pointer to the newly created layer that
 * should be added.
 */
RS_Layer* QG_DialogFactory::requestNewLayerDialog(RS_LayerList* layerList) {

    RS_Layer* layer = NULL;

    QString layer_name = "noname";
    int i = 2;

    if (layerList!=NULL) {
        while (layerList->find(layer_name) > 0) {
            layer_name.sprintf("noname%d", i++);
        }
    }

    // Layer for parameter livery
    layer = new RS_Layer(layer_name);

    QG_LayerDialog dlg(parent);
    dlg.setLayer(layer);
    dlg.setLayerList(layerList);
    if (dlg.exec()) {
        dlg.updateLayer();
    } else {
        delete layer;
        layer = NULL;
    }
    return layer;
}



/**
 * Shows a dialog that asks the user if the selected layer
 * can be removed. Doesn't remove the layer. This is up to the caller.
 *
 * @return a pointer to the layer that should be removed.
 */
RS_Layer* QG_DialogFactory::requestLayerRemovalDialog(RS_LayerList* layerList) {

    RS_Layer* layer = NULL;
    if (layerList==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_DialogFactory::requestLayerRemovalDialog(): "
            "layerList is NULL");
        return NULL;
    }

    // Layer for parameter livery
    layer = layerList->getActive();

    if (layer!=NULL) {
        if (layer->getName()!="0") {
            int remove =
                QMessageBox::information(
                    parent,
                    QMessageBox::tr("Remove Layer"),
                    QMessageBox::tr("Layer \"%1\" and all "
                                    "entities on it will be removed.")
                    .arg(layer->getName()),
                    QMessageBox::Ok,
                    QMessageBox::Cancel);
            if (remove==QMessageBox::Ok) {}
            else {
                layer = NULL;
            }
        } else {
            QMessageBox::information(parent,
                                     QMessageBox::tr("Remove Layer"),
                                     QMessageBox::tr("Layer \"%1\" can never "
                                                     "be removed.")
                                     .arg(layer->getName()),
                                     QMessageBox::Ok);
        }
    }

    return layer;
}



/**
 * Shows a dialog for editing a layer. A new layer is created and
 * returned. Modifying the layer is up to the caller.
 *
 * @return A pointer to a new layer with the changed attributes
 *         or NULL if the dialog was cancelled.
 */
RS_Layer* QG_DialogFactory::requestEditLayerDialog(RS_LayerList* layerList) {

    RS_DEBUG->print("QG_DialogFactory::requestEditLayerDialog");

    RS_Layer* layer = NULL;
    /*
       if (layerList==NULL) {
           if (container!=NULL && container->rtti()==RS2::EntityGraphic) {
               layerList = (RS_LayerList*)container;
           } else {
               return NULL;
           }
       }
    */

    if (layerList==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_DialogFactory::requestEditLayerDialog(): "
            "layerList is NULL");
        return NULL;
    }

    // Layer for parameter livery
    if (layerList->getActive()!=NULL) {
        layer = new RS_Layer(*layerList->getActive());

        QG_LayerDialog dlg(parent);
        dlg.setLayer(layer);
        dlg.setLayerList(layerList);
        dlg.setEditLayer(TRUE);
        if (dlg.exec()) {
            dlg.updateLayer();
        } else {
            delete layer;
            layer = NULL;
        }
    }

    return layer;
}



/**
 * Shows a dialog for adding a block. Doesn't add the block.
 * This is up to the caller.
 *
 * @return a pointer to the newly created block that 
 * should be added.
 */
RS_BlockData QG_DialogFactory::requestNewBlockDialog(RS_BlockList* blockList) {
    //RS_Block* block = NULL;
    RS_BlockData ret;
    ret = RS_BlockData("", RS_Vector(false), false);

    if (blockList==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_DialogFactory::requestNewBlockDialog(): "
            "blockList is NULL");
        return ret;
    }

    // Block for parameter livery
    //block = new RS_Block(container, "noname", RS_Vector(0.0,0.0));

    QG_BlockDialog dlg(parent);
    dlg.setBlockList(blockList);
    if (dlg.exec()) {
        ret = dlg.getBlockData();
    }

    return ret;
}



/**
 * Shows a dialog for renaming the currently active block. 
 *
 * @return a pointer to the modified block or NULL on cancellation.
 */
RS_BlockData QG_DialogFactory::requestBlockAttributesDialog(RS_BlockList* blockList) {
    //RS_Block* block = NULL;
    RS_BlockData ret;
    ret = RS_BlockData("", RS_Vector(false), false);

    if (blockList==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_DialogFactory::requestBlockAttributesDialog(): "
            "blockList is NULL");
        return ret;
    }

    QG_BlockDialog dlg(parent);
    dlg.setBlockList(blockList);
    if (dlg.exec()) {
        ret = dlg.getBlockData();
    }

    return ret;
}


/**
 * Shows a dialog that asks the user if the selected block
 * can be removed. Doesn't remove the block. This is up to the caller.
 *
 * @return a pointer to the block that should be removed.
 */
RS_Block* QG_DialogFactory::requestBlockRemovalDialog(RS_BlockList* blockList) {
    RS_Block* block = NULL;

    if (blockList==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_DialogFactory::requestBlockRemovalDialog(): "
            "blockList is NULL");
        return NULL;
    }

    // Block for parameter livery
    block = blockList->getActive();

    if (block!=NULL) {
        int remove =
            QMessageBox::information(parent,
                                     QMessageBox::tr("Remove Block"),
                                     QMessageBox::tr("Block \"%1\" and all "
                                                     "its entities will be removed.")
                                     .arg(block->getName()),
                                     QMessageBox::Ok,
                                     QMessageBox::Cancel);
        
        RS_DEBUG->print("QG_DialogFactory::requestBlockRemovalDialog: remove is: %d", remove);
        if (remove==QMessageBox::Ok) {}
        else {
            block = NULL;
        }
    }

    return block;
}


/**
 * Shows a dialog for choosing a file name. Opening the file is up to
 * the caller.
 *
 * @return File name with path and extension to determine the file type
 *         or an empty string if the dialog was cancelled.
 */
RS_String QG_DialogFactory::requestImageOpenDialog() {
    RS_String fn = "";

    // read default settings:
    RS_SETTINGS->beginGroup("/Paths");
    RS_String defDir = RS_SETTINGS->readEntry("/OpenImage",
                       RS_SYSTEM->getHomeDir());
    RS_String defFilter = RS_SETTINGS->readEntry("/ImageFilter",
                          "Portable Network Graphic (*.png)");
    RS_SETTINGS->endGroup();

    bool cancel = false;

    QFileDialog fileDlg(NULL, Qt::Dialog);

    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    QStringList filters;
    QString all = "";

    for (QList<QByteArray>::Iterator it = formats.begin(); it!=formats.end(); ++it) {
        QString ext = (*it);

        RS_DEBUG->print("QG_DialogFactory::requestImageOpenDialog: "
            "supported image format: %s", 
            (const char*)ext.toLatin1());

        QString st;
        if (ext=="JPEG") {
            st = QString("%1 (*.%2 *.jpg)").arg(extToFormat(ext))
                 .arg(ext.toLower());
        } else {
            st = QString("%1 (*.%2)").arg(extToFormat(ext))
                 .arg(ext.toLower());
        }
        filters.append(st);

        if (!all.isEmpty()) {
            all += " ";
        }

        if (ext=="JPEG") {
            all += QString("*.%1 *.jpg").arg(ext.toLower());
        } else {
            all += QString("*.%1").arg(ext.toLower());
        }
    }
    filters.append(QObject::tr("All Image Files (%1)").arg(all));
    filters.append(QObject::tr("All Files (*.*)"));

    //filters.append("Drawing Exchange (*.)");
    //filters.append("Font (*.cxf)");

    fileDlg.setFilters(filters);
    fileDlg.setFileMode(QFileDialog::ExistingFile);
    fileDlg.setWindowTitle(tr("Open Image"));
    fileDlg.setDirectory(defDir);
    fileDlg.selectFilter(defFilter);

    if (fileDlg.exec()==QDialog::Accepted) {
        QStringList fns = fileDlg.selectedFiles();
        if (fns.count()>=1) {
            fn = fns.at(0);
            cancel = false;
        }
    } else {
        cancel = true;
    }

    // store new default settings:
    if (!cancel) {
        RS_SETTINGS->beginGroup("/Paths");
        RS_SETTINGS->writeEntry("/OpenImage", QFileInfo(fn).absolutePath());
        RS_SETTINGS->writeEntry("/ImageFilter", fileDlg.selectedFilter());
        RS_SETTINGS->endGroup();
    }

    return fn;
}



void QG_DialogFactory::requestOptions(RS_ActionInterface* action,
                                      bool on, bool update) {
    RS_DEBUG->print("QG_DialogFactory::requestOptions");

    if (action==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_DialogFactory::requestOptions: action is NULL");
        return;
    }
    
    /*
    RS_DEBUG->print("QG_DialogFactory::requestOptions: typeid: %s", typeid(action).name());
    RS_DEBUG->print("QG_DialogFactory::requestOptions: name: %s", (const char*)action->getName().toLatin1());
    RS_DEBUG->print("QG_DialogFactory::requestOptions: NULL: %d", (int)(dynamic_cast<RS_ActionDrawLineAngle*>(action)==NULL));
    if (typeid(action)==typeid(RS_ActionDrawLineAngle)) {
        RS_DEBUG->print("QG_DialogFactory::requestOptions: RS_ActionDrawLineAngle");
        requestActionOptions<QG_LineAngleOptions>(action, on, update);
    }
    */
    if (dynamic_cast<RS_ActionDrawLineAngle*>(action)!=NULL) {
        requestActionOptions<QG_LineAngleOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionFilePrintPreview*>(action)!=NULL) {
        requestActionOptions<QG_PrintPreviewOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawLine*>(action)!=NULL) {
        requestActionOptions<QG_LineOptions>(action, on, update);
        return;
    }
        
    else if (dynamic_cast<RS_ActionDrawPolyline*>(action)!=NULL) {
        requestActionOptions<QG_PolylineOptions>(action, on, update);
        return;
    }
    
    else if (dynamic_cast<RS_ActionPolylineEquidistant*>(action)!=NULL) {
        requestActionOptions<QG_PolylineEquidistantOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawLineAngle*>(action)!=NULL) {
        requestActionOptions<QG_LineAngleOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawLineParallel*>(action)!=NULL) {
        requestActionOptions<QG_LineParallelOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawLineParallelThrough*>(action)!=NULL) {
        requestActionOptions<QG_LineParallelThroughOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawLineBisector*>(action)!=NULL) {
        requestActionOptions<QG_LineBisectorOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawLineRelAngle*>(action)!=NULL) {
        requestActionOptions<QG_LineRelAngleOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawLinePolygon*>(action)!=NULL) {
        requestActionOptions<QG_LinePolygonOptions>(action, on, update);
        return;
    }
    
    else if (dynamic_cast<RS_ActionDrawLinePolygon2*>(action)!=NULL) {
        requestActionOptions<QG_LinePolygon2Options>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawArc*>(action)!=NULL) {
        requestActionOptions<QG_ArcOptions>(action, on, update);
        return;
    }
    
    else if (dynamic_cast<RS_ActionDrawArcTangential*>(action)!=NULL) {
        requestActionOptions<QG_ArcTangentialOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawCircleCR*>(action)!=NULL) {
        requestActionOptions<QG_CircleOptions>(action, on, update);
        return;
    }
        
    else if (dynamic_cast<RS_ActionDrawSpline*>(action)!=NULL) {
        requestActionOptions<QG_SplineOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawText*>(action)!=NULL) {
        requestActionOptions<QG_TextOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionBlocksInsert*>(action)!=NULL) {
        requestActionOptions<QG_InsertOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDrawImage*>(action)!=NULL) {
        requestActionOptions<QG_ImageOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDimAligned*>(action)!=NULL) {
        requestActionOptions<QG_DimOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDimLinear*>(action)!=NULL) {
        requestActionOptions<QG_DimOptions>(action, on, update);
        if (((RS_ActionDimLinear*)action)->hasFixedAngle()==false) {
            requestActionOptions<QG_DimLinearOptions>(action, on, update);
        }
    }

    else if (dynamic_cast<RS_ActionDimRadial*>(action)!=NULL) {
        requestActionOptions<QG_DimOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDimDiametric*>(action)!=NULL) {
        requestActionOptions<QG_DimOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionDimAngular*>(action)!=NULL) {
        requestActionOptions<QG_DimOptions>(action, on, update);
        return;
    }
    
    else if (dynamic_cast<RS_ActionDimOrdinate*>(action)!=NULL) {
        requestActionOptions<QG_DimOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionModifyTrimAmount*>(action)!=NULL) {
        requestActionOptions<QG_TrimAmountOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionModifyMoveRotate*>(action)!=NULL) {
        requestActionOptions<QG_MoveRotateOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionModifyBevel*>(action)!=NULL) {
        requestActionOptions<QG_BevelOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionModifyRound*>(action)!=NULL) {
        requestActionOptions<QG_RoundOptions>(action, on, update);
        return;
    }
    
    else if (dynamic_cast<RS_ActionModifyDivide2*>(action)!=NULL) {
        requestActionOptions<QG_Divide2Options>(action, on, update);
        return;
    }
    
    else if (dynamic_cast<RS_ActionModifyIsoProject*>(action)!=NULL) {
        requestActionOptions<QG_IsoProjectOptions>(action, on, update);
        return;
    }

    else if (dynamic_cast<RS_ActionLibraryInsert*>(action)!=NULL) {
        requestActionOptions<QG_LibraryInsertOptions>(action, on, update);
        return;
    }
    
    else if (dynamic_cast<RS_ActionEditPaste*>(action)!=NULL) {
#ifndef RS_ARCH
        requestActionOptions<QG_EditPasteOptions>(action, on, update);
#endif
        return;
    }

    RS_DEBUG->print("QG_DialogFactory::requestOptions: OK");
}


void QG_DialogFactory::requestSnapOptions(RS_Snapper* snapper, bool on) {
    RS_DEBUG->print("QG_DialogFactory::requestSnapOptions");

    if (snapper==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_DialogFactory::requestSnapOptions: snapper is NULL");
        return;
    }

    switch (snapper->getSnapMode()) {
    case RS2::ActionSnapDist:
        requestSnapOptions2<QG_SnapDistOptions>(snapper, on);
        break;
    
    case RS2::ActionSnapCoordinate:
        RS_DEBUG->print("QG_DialogFactory::requestSnapOptions: requestSnapOptions2");
        requestSnapOptions2<QG_SnapCoordinateOptions>(snapper, on);
        RS_DEBUG->print("QG_DialogFactory::requestSnapOptions: requestSnapOptions2: OK");
        break;
    
    case RS2::ActionSnapCoordinatePolar:
        requestSnapOptions2<QG_SnapCoordinatePolarOptions>(snapper, on);
        break;

    default:
        break;
    }
    RS_DEBUG->print("QG_DialogFactory::requestSnapOptions: OK");
}



/**
 * Shows a widget for options for an action.
 */
template <class T>
void QG_DialogFactory::requestActionOptions(RS_ActionInterface* action,
        bool on, bool update) {
    static T* toolWidget = NULL;
    
    RS_DEBUG->print("QG_DialogFactory::requestActionOptions");

    if (optionWidget!=NULL) {
        if (toolWidget!=NULL) {
            toolWidget->hide();
            delete toolWidget;
            toolWidget = NULL;
        }
        if (on==true && toolWidget==NULL) {
            toolWidget = new T(optionWidget);
            toolWidget->setAction(action, update);
            optionWidget->addWidget(toolWidget);
        }
    }
    RS_DEBUG->print("QG_DialogFactory::requestActionOptions: OK");
}



/**
 * Shows a widget for options for a snap function.
 */
template <class T>
void QG_DialogFactory::requestSnapOptions2(RS_Snapper* snapper, bool on) {
    static T* toolWidget = NULL;
        
    RS_DEBUG->print("QG_DialogFactory::requestSnapOptions2");
    
    if (optionWidget!=NULL) {
        if (toolWidget!=NULL) {
            toolWidget->hide();
            // crashes os x
            //toolWidget->setParent(NULL);
            delete toolWidget;
            toolWidget = NULL;
        }
        if (on==true && toolWidget==NULL) {
            toolWidget = new T(optionWidget);
            toolWidget->setSnapper(snapper);
            optionWidget->addWidget(toolWidget);
        }
    }
    
    RS_DEBUG->print("QG_DialogFactory::requestSnapOptions2: OK");
}



/**
 * Shows the given toolbar.
 */
void QG_DialogFactory::requestToolBar(RS2::ToolBarId id) {
    if (cadToolBar!=NULL) {
        cadToolBar->showToolBar(id);
    }
}



/**
 * Shows the select toolbar with the given action to launch.
 */
void QG_DialogFactory::requestToolBarSelect(RS_ActionInterface* (*f)()) {
    if (cadToolBar!=NULL) {
        cadToolBar->showToolBarSelect(f);
    }
}



/**
 * Shows attributes options dialog presenting the given data.
 */
bool QG_DialogFactory::requestAttributesDialog(RS_AttributesData& data,
        RS_LayerList& layerList) {

    QG_DlgAttributes dlg(parent);
    dlg.setData(&data, layerList);
    if (dlg.exec()) {
        dlg.updateData();
        return true;
    }
    return false;
}



/**
 * Shows move options dialog presenting the given data.
 */
bool QG_DialogFactory::requestMoveDialog(RS_MoveData& data) {
    QG_DlgMove dlg(parent);
    dlg.setData(&data);
    if (dlg.exec()) {
        dlg.updateData();
        return true;
    }
    return false;
}



/**
 * Shows rotate options dialog presenting the given data.
 */
bool QG_DialogFactory::requestRotateDialog(RS_RotateData& data) {
    QG_DlgRotate dlg(parent);
    dlg.setData(&data);
    if (dlg.exec()) {
        dlg.updateData();
        return true;
    }
    return false;
}



/**
 * Shows scale options dialog presenting the given data.
 */
bool QG_DialogFactory::requestScaleDialog(RS_ScaleData& data) {
    QG_DlgScale dlg(parent);
    dlg.setData(&data);
    if (dlg.exec()) {
        dlg.updateData();
        return true;
    }
    return false;
}



/**
 * Shows mirror options dialog presenting the given data.
 */
bool QG_DialogFactory::requestMirrorDialog(RS_MirrorData& data) {
    QG_DlgMirror dlg(parent);
    dlg.setData(&data);
    if (dlg.exec()) {
        dlg.updateData();
        return true;
    }
    return false;
}



/**
 * Shows move/rotate options dialog presenting the given data.
 */
bool QG_DialogFactory::requestMoveRotateDialog(RS_MoveRotateData& data) {
    QG_DlgMoveRotate dlg(parent);
    dlg.setData(&data);
    if (dlg.exec()) {
        dlg.updateData();
        return true;
    }
    return false;
}



/**
 * Shows rotate around two centers options dialog presenting the given data.
 */
bool QG_DialogFactory::requestRotate2Dialog(RS_Rotate2Data& data) {
    QG_DlgRotate2 dlg(parent);
    dlg.setData(&data);
    if (dlg.exec()) {
        dlg.updateData();
        return true;
    }
    return false;
}



/**
 * Shows a dialog to edit the given entity.
 */
bool QG_DialogFactory::requestModifyEntityDialog(RS_Entity* entity) {
    if (entity==NULL) {
        return false;
    }

    bool ret = false;

    switch (entity->rtti()) {
    case RS2::EntityPoint: {
            QG_DlgPoint dlg(parent);
            dlg.setPoint(*(dynamic_cast<RS_Point*>(entity)));
            if (dlg.exec()) {
                dlg.updatePoint();
                ret = true;
            }
        }
        break;

    case RS2::EntityLine: {
            QG_DlgLine dlg(parent);
            dlg.setLine(*(dynamic_cast<RS_Line*>(entity)));
            if (dlg.exec()) {
                dlg.updateLine();
                ret = true;
            }
        }
        break;

    case RS2::EntityArc: {
            QG_DlgArc dlg(parent);
            dlg.setArc(*(dynamic_cast<RS_Arc*>(entity)));
            if (dlg.exec()) {
                dlg.updateArc();
                ret = true;
            }
        }
        break;

    case RS2::EntityCircle: {
            QG_DlgCircle dlg(parent);
            dlg.setCircle(*(dynamic_cast<RS_Circle*>(entity)));
            if (dlg.exec()) {
                dlg.updateCircle();
                ret = true;
            }
        }
        break;

    case RS2::EntityEllipse: {
            QG_DlgEllipse dlg(parent);
            dlg.setEllipse(*(dynamic_cast<RS_Ellipse*>(entity)));
            if (dlg.exec()) {
                dlg.updateEllipse();
                ret = true;
            }
        }
        break;
        
    case RS2::EntitySpline: {
            QG_DlgSpline dlg(parent);
            dlg.setSpline(*(dynamic_cast<RS_Spline*>(entity)));
            if (dlg.exec()) {
                dlg.updateSpline();
                ret = true;
            }
        }
        break;

    case RS2::EntityInsert: {
            QG_DlgInsert dlg(parent);
            dlg.setInsert(*(dynamic_cast<RS_Insert*>(entity)));
            if (dlg.exec()) {
                dlg.updateInsert();
                ret = true;
                entity->update();
            }
        }
        break;

    case RS2::EntityDimAligned:
    case RS2::EntityDimAngular:
    case RS2::EntityDimDiametric:
    case RS2::EntityDimRadial: {
            QG_DlgDimension dlg(parent);
            dlg.setDim(*(dynamic_cast<RS_Dimension*>(entity)));
            if (dlg.exec()) {
                dlg.updateDim();
                ret = true;
                (dynamic_cast<RS_Dimension*>(entity))->update(false);
            }
        }
        break;
      
    case RS2::EntityDimLinear: {
            QG_DlgDimLinear dlg(parent);
            dlg.setDim(*(dynamic_cast<RS_DimLinear*>(entity)));
            if (dlg.exec()) {
                dlg.updateDim();
                ret = true;
                (dynamic_cast<RS_DimLinear*>(entity))->update(false);
            }
        }
        break;
        
    case RS2::EntityText: {
            QG_DlgText dlg(parent);
            dlg.setText(*dynamic_cast<RS_Text*>(entity), false);
            if (dlg.exec()) {
                dlg.updateText();
                ret = true;
                entity->update();
            }
        }
        break;

    case RS2::EntityHatch: {
            QG_DlgHatch dlg(parent);
            dlg.setHatch(*dynamic_cast<RS_Hatch*>(entity), false);
            if (dlg.exec()) {
                dlg.updateHatch();
                ret = true;
                entity->update();
            }
        }
        break;

    default:
        break;
    }

    return ret;
}



/**
 * Shows a dialog to edit the attributes of the given text entity.
 */
bool QG_DialogFactory::requestTextDialog(RS_Text* text) {
    if (text==NULL) {
        return false;
    }

    QG_DlgText dlg(parent);
    dlg.setText(*text, true);
    if (dlg.exec()) {
        dlg.updateText();
        return true;
    }

    return false;
}


/**
 * Shows a dialog to edit pattern / hatch attributes of the given entity.
 */
bool QG_DialogFactory::requestHatchDialog(RS_Hatch* hatch) {
    if (hatch==NULL) {
        return false;
    }

    RS_PATTERNLIST->init();

    QG_DlgHatch dlg(parent);
    dlg.setHatch(*hatch, true);
    if (dlg.exec()) {
        dlg.updateHatch();
        return true;
    }

    return false;
}



/**
 * Shows a dialog for CAM options.
 */
#ifdef RS_CAM
bool QG_DialogFactory::requestCamOptionsDialog(RS_Graphic& graphic) {
    RS_DEBUG->print("QG_DialogFactory::requestCamOptionsDialog");
    RS_CamDialog dlg(graphic, parent);
    RS_DEBUG->print("QG_DialogFactory::requestCamOptionsDialog: exec");
    if (dlg.exec()) {
        RS_DEBUG->print("QG_DialogFactory::requestCamOptionsDialog: OK");
        return true;
    }
    RS_DEBUG->print("QG_DialogFactory::requestCamOptionsDialog: Cancel");
    return false;
}
#endif


/**
 * Shows dialog for general application options.
 */
void QG_DialogFactory::requestOptionsGeneralDialog() {
    QG_DlgOptionsGeneral dlg(parent);
    dlg.exec();
}



/**
 * Shows dialog for drawing options.
 */
void QG_DialogFactory::requestOptionsDrawingDialog(RS_Graphic& graphic) {
    QG_DlgOptionsDrawing dlg(parent);
    dlg.setGraphic(&graphic);
    dlg.exec();
}


/**
 * Back to last menu in cad toolbar.
 */
void QG_DialogFactory::requestPreviousMenu() {
    if (cadToolBar!=NULL) {
        cadToolBar->showToolBarMain();
    }
}


/**
 * Shows a dialog with the given text warning and a tick box the user can
 * tick to never show this message again.
 */
bool QG_DialogFactory::requestOneTimeDialog(const RS_String& text, const RS_String& id) {
    RS_SETTINGS->beginGroup("/ShownDialogs");
    int shown = RS_SETTINGS->readNumEntry(RS_String("/") + id, 0);
    RS_SETTINGS->endGroup();

    bool ret = false;
    
    if (shown==0) {
        QG_DlgOneTime dlg(parent);
        dlg.setText(text);
        ret = dlg.exec();

        int dontShowAgain = (int)dlg.getDontShowAgain();

        RS_SETTINGS->beginGroup("/ShownDialogs");
        RS_SETTINGS->writeEntry(RS_String("/") + id, dontShowAgain);
        RS_SETTINGS->endGroup();
    }
    else {
        ret = true;
    }

    return ret;
}



/**
 * Provides a new window for editing the active block.
 */
void QG_DialogFactory::requestEditBlockWindow(RS_BlockList* blockList) {
    RS_DEBUG->print("QG_DialogFactory::requestEditBlockWindow()");

    //QG_MainWindow* appWindow = QG_MainWindow::getAppWindow();
    QG_MdiWindow* parent = qgMainWindow->getMdiWindow();
    if (parent!=NULL) {
        if (blockList!=NULL) {
            RS_Block* blk = blockList->getActive();
            if (blk!=NULL) {
                QG_MdiWindow* w = qgMainWindow->slotFileNew(blk);
                // the parent needs a pointer to the block window and
                //   vice versa
                parent->addChildWindow(w);
                w->getGraphicView()->zoomAuto(false);
            }
        }
    }
}
    
    
void QG_DialogFactory::requestEditBlockWindow(const RS_String& blockName) {
    QG_MdiWindow* parent = qgMainWindow->getMdiWindow();
    if (parent==NULL) {
        return;
    }
    
    RS_Graphic* graphic = parent->getGraphic();
    if (graphic==NULL) {
        return;
    }
    
    RS_BlockList* blockList = graphic->getBlockList();
    if (blockList==NULL) {
        return;
    }

    RS_Block* blk = blockList->find(blockName);
    if (blk==NULL) {
        return;
    }

    QG_MdiWindow* w = qgMainWindow->slotFileNew(blk);
    // the parent needs a pointer to the block window and
    //   vice versa
    parent->addChildWindow(w);
    w->getGraphicView()->zoomAuto(false);
}



/**
 * Closes all windows that are editing the given block.
 */
void QG_DialogFactory::closeEditBlockWindow(RS_Block* block) {
    RS_DEBUG->print("QG_DialogFactory::closeEditBlockWindow");
    
    //QG_MainWindow* appWindow = QG_MainWindow::getAppWindow();
    QMdiArea* workspace = qgMainWindow->getWorkspace();
    
    RS_PtrList<QG_MdiWindow> closeList;
    closeList.setAutoDelete(true);

    if (workspace!=NULL) {
        RS_DEBUG->print("QG_DialogFactory::closeEditBlockWindow: "
            "workspace found");
        
        QWidgetList windows = workspace->windowList();
        for (int i = 0; i < int(windows.count()); ++i) {
            RS_DEBUG->print("QG_DialogFactory::closeEditBlockWindow: "
                "window: %d", i);
            QG_MdiWindow* m = (QG_MdiWindow*)windows.at(i);
            if (m!=NULL) {
                RS_DEBUG->print(
                    "QG_DialogFactory::closeEditBlockWindow: got mdi");
                if (m->getDocument()==block) {
                    m->detach();
                    closeList.append(m);
                    RS_DEBUG->print("QG_DialogFactory::closeEditBlockWindow: "
                        "MDI found that is editing block");
                }
            }
        }
    }

    RS_DEBUG->print("QG_DialogFactory::closeEditBlockWindow: "
        "closing MDIs");
    closeList.clear();
        //workspace->setActiveWindow(closeList.at(i));
        //workspace->closeActiveWindow();
    
    // 20080124: NULL will not update block list:
    qgMainWindow->slotWindowActivated(NULL);
    //qgMainWindow->slotWindowActivated(qgMainWindow->getMdiWindow());
    
    RS_DEBUG->print("QG_DialogFactory::closeEditBlockWindow: OK");
}




/**
 * Called whenever the mouse position changed.
 */
void QG_DialogFactory::updateCoordinateWidget(const RS_Vector& abs,
        const RS_Vector& rel, bool updateFormat) {
    if (coordinateWidget!=NULL) {
        coordinateWidget->setCoordinates(abs, rel, updateFormat);
    }
}



/**
 * Called when an action has a mouse hint.
 */
void QG_DialogFactory::updateMouseWidget(const RS_String& left,
        const RS_String& right) {

    if (mouseWidget!=NULL) {
        mouseWidget->setHelp(left, right);
    }
#ifdef RS_PROF
    if (commandWidget!=NULL) {
        commandWidget->setCommand(left);
    }
#endif
}



/**
 * Called whenever the selection changed.
 */
void QG_DialogFactory::updateSelectionWidget(int num) {
    if (selectionWidget!=NULL) {
        selectionWidget->setNumber(num);
    }

    // update property editor to reflect selection
    updatePropertyEditor((RS_PropertyOwnerContainerInterface*)qgMainWindow->getDocument());
/*
#ifdef RS_PROPERTYEDITOR
    if (propertyEditor!=NULL && qgMainWindow!=NULL) {
        propertyEditor->update(qgMainWindow->getDocument());
    }
#endif
*/
}


/**
 * Called when an action needs to communicate 'message' to the user.
 */
void QG_DialogFactory::commandMessage(const RS_String& message) {
    RS_DEBUG->print("QG_DialogFactory::commandMessage");
#ifdef RS_PROF
    if (commandWidget!=NULL) {
        commandWidget->appendHistory(message);
    }
#endif
    RS_DEBUG->print("QG_DialogFactory::commandMessage: OK");
}



/**
 * Converts an extension to a format description.
 * e.g. "PNG" to "Portable Network Graphic"
 *
 * @param Extension
 * @return Format description
 */
QString QG_DialogFactory::extToFormat(const QString& ext) {
    QString e = ext.toLower();

    if (e=="bmp") {
        return QObject::tr("Windows Bitmap");
    } else if (e=="jpeg" || e=="jpg") {
        return QObject::tr("Joint Photographic Experts Group");
    } else if (e=="gif") {
        return QObject::tr("Graphics Interchange Format");
    } else if (e=="mng") {
        return QObject::tr("Multiple-image Network Graphics");
    } else if (e=="pbm") {
        return QObject::tr("Portable Bit Map");
    } else if (e=="pgm") {
        return QObject::tr("Portable Grey Map");
    } else if (e=="png") {
        return QObject::tr("Portable Network Graphic");
    } else if (e=="ppm") {
        return QObject::tr("Portable Pixel Map");
    } else if (e=="xbm") {
        return QObject::tr("X Bitmap Format");
    } else if (e=="xpm") {
        return QObject::tr("X Pixel Map");
    }
    else {
        return ext.toUpper();
    }
}


    
/*
void QG_DialogFactory::setCurrentDocument(RS_Document* doc) {
    if (doc==NULL) {
        RS_DEBUG->print("QG_DialogFactory::setCurrentDocument: document is NULL");
        setCurrentLayerList(NULL);
        return;
    }

    setCurrentLayerList(doc->getLayerList());
}


void QG_DialogFactory::setCurrentLayerList(RS_LayerList* l) {
    if (getPenToolBar()!=NULL) {
        getPenToolBar()->setLayerList(l);
    }
    if (getLayerWidget()!=NULL) {
        getLayerWidget()->setLayerList(l);
    }
}
*/
    
    
void QG_DialogFactory::updateUiConnections() {
    RS_DEBUG->print("QG_DialogFactory::updateUiConnections");

    RS_Document* document = QG_DIALOGFACTORY->getDocument();
    if (document==NULL) {
        RS_DEBUG->print("QG_DialogFactory::updateUiConnections: "
            "document is NULL");
        return;
    }
    
    if (getLayerWidget()!=NULL) {
        getLayerWidget()->setLayerList(document->getLayerList());
    }
    
    disconnect(document->getLayerList(), 0, 0, 0);

    if (QG_DIALOGFACTORY->getGraphicView()!=NULL) {
        connect(document->getLayerList(), SIGNAL(signalLayerEdited(RS_Layer*)), 
            (QG_GraphicView*)QG_DIALOGFACTORY->getGraphicView(), SLOT(slotRedraw()));
        connect(document->getLayerList(), SIGNAL(signalLayerToggled(RS_Layer*)), 
            (QG_GraphicView*)QG_DIALOGFACTORY->getGraphicView(), SLOT(slotRedraw()));
    }

    if (QG_DIALOGFACTORY->getLayerWidget()!=NULL) {
        connect(document->getLayerList(), SIGNAL(signalUpdate()), 
            QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotUpdated()));
        connect(document->getLayerList(), SIGNAL(signalLayerActivated(RS_Layer*)), 
            QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerActivated(RS_Layer*)));
        connect(document->getLayerList(), SIGNAL(signalLayerAdded(RS_Layer*)), 
            QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerAdded(RS_Layer*)));
        connect(document->getLayerList(), SIGNAL(signalLayerEdited(RS_Layer*)), 
            QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerEdited(RS_Layer*)));
        connect(document->getLayerList(), SIGNAL(signalLayerRemoved(RS_Layer*)), 
            QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerRemoved(RS_Layer*)));
        connect(document->getLayerList(), SIGNAL(signalLayerToggled(RS_Layer*)), 
            QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerToggled(RS_Layer*)));
    }

#ifdef RS_PROPERTYEDITOR
    if (QG_DIALOGFACTORY->getPropertyEditor()!=NULL) {
        disconnect(QG_DIALOGFACTORY->getPropertyEditor(), 0, 0, 0);
        connect(QG_DIALOGFACTORY->getPropertyEditor(), 
            SIGNAL(signalPropertyChanged(RS_PropertyOwner*, 
                const QString&, const QVariant&)),
            this, SLOT(slotPropertyChanged(RS_PropertyOwner*, 
                const QString&, const QVariant&)));
        connect(QG_DIALOGFACTORY->getPropertyEditor(), 
            SIGNAL(signalPropertyChanged(RS_PropertyOwnerContainerInterface*, 
                const QString&, const QVariant&)),
            this, SLOT(slotPropertyChanged(RS_PropertyOwnerContainerInterface*,
                const QString&, const QVariant&)));
    }
#endif
    
    RS_DEBUG->print("QG_DialogFactory::updateUiConnections: OK");
}


    
void QG_DialogFactory::slotPropertyChanged(RS_PropertyOwner* /*propertyOwner*/, 
    const QString& /*propertyName*/, const QVariant& /*propertyValue*/) {

    RS_DEBUG->print("QG_DialogFactory::slotPropertyChanged");
        
    RS_DEBUG->print("QG_DialogFactory::slotPropertyChanged: OK");
}


void QG_DialogFactory::slotPropertyChanged(
    RS_PropertyOwnerContainerInterface* propertyOwnerContainer, 
    const QString& propertyName, const QVariant& propertyValue) {
#ifdef RS_PROPERTYEDITOR

    RS_DEBUG->print("QG_DialogFactory::slotPropertyChanged (for container)");
        
    RS_PropertyAttributes propertyAttributes =
        propertyOwnerContainer->getPropertyAttributesOfChildren(propertyName);

    // the change of this property requires a redraw of the graphic view:
    if (QG_DIALOGFACTORY->getGraphicView()!=NULL) {
        //if (!propertyAttributes.contains("noRedrawRequired")) {
            QG_DIALOGFACTORY->getGraphicView()->redraw();
        //}
    }

    // if the change of this property affects other properties, the property
    //   editor needs to be updated:
    if (QG_DIALOGFACTORY->getPropertyEditor()!=NULL) {
        if (propertyAttributes.affectsOtherProperties) {
            QG_DIALOGFACTORY->getPropertyEditor()->update(propertyOwnerContainer);
        }
    }
            
    // make layer / floor visible as a reaction of moving
    //   the current selection on the layer / floor.
    if (propertyName==RS_ENTITY_LAYER 
#ifdef RS_ARCH
        || propertyName==RS_ENTITY_FLOOR
#endif
        ) {
        RS_Document* document = QG_DIALOGFACTORY->getDocument();
        if (document!=NULL) {
            RS_LayerList* layerList = document->getLayerList();
            if (layerList!=NULL) {
                if (propertyName==RS_ENTITY_LAYER) {
                    layerList->freeze(propertyValue.toString(), false);
                }
#ifdef RS_ARCH
                else if (propertyName==RS_ENTITY_FLOOR) {
                    layerList->activate(RS_ArchUtils::floorToLayerName(propertyValue.toString()));
                }
#endif
            }
        }
    }
#endif

    RS_DEBUG->print("QG_DialogFactory::slotPropertyChanged: OK");
}
