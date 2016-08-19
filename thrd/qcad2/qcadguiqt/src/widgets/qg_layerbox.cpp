/****************************************************************************
** $Id: qg_layerbox.cpp 6880 2007-11-05 16:51:18Z andrew $
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

#include "qg_layerbox.h"


/**
 * Default Constructor. You must call init manually before using 
 * this class.
 */
QG_LayerBox::QG_LayerBox(QWidget* parent)
        : QComboBox(parent) {

    showByBlock = false;
    showUnchanged = false;
    unchanged = false;
    layerList = NULL;
    currentLayer = NULL;
}



/**
 * Destructor
 */
QG_LayerBox::~QG_LayerBox() {}



/**
 * Initialisation (called manually only once).
 *
 * @param layerList Layer list which provides the layer names that are 
 *                  available.
 * @param showByBlock true: Show attribute ByBlock.
 */
void QG_LayerBox::init(RS_LayerList& layerList, 
        bool showByBlock, bool showUnchanged) {
    this->showByBlock = showByBlock;
    this->showUnchanged = showUnchanged;
    this->layerList = &layerList;

    if (showUnchanged) {
        addItem(tr("- Unchanged -"));
    }
            
    QStringList layers;
    for (int i=0; i<layerList.count(); ++i) {
        RS_Layer* lay = layerList.at(i);
        if (lay!=NULL && (lay->getName()!="ByBlock" || showByBlock)) {
            layers << lay->getName();
        }
    }

    layers.sort();
    
    for (int i=0; i<layers.count(); ++i) {
        addItem(layers.at(i));
    }

    connect(this, SIGNAL(activated(int)),
            this, SLOT(slotLayerChanged(int)));

    setCurrentIndex(0);

    slotLayerChanged(currentIndex());
}



/**
 * Sets the layer shown in the combobox to the given layer.
 */
void QG_LayerBox::setLayer(RS_Layer& layer) {
    currentLayer = &layer;

    setCurrentIndex(findText(layer.getName()));
    slotLayerChanged(currentIndex());
}



/**
 * Sets the layer shown in the combobox to the given layer.
 */
void QG_LayerBox::setLayer(RS_String& layer) {

    setCurrentIndex(findText(layer));
    slotLayerChanged(currentIndex());
}



/**
 * Called when the color has changed. This method 
 * sets the current color to the value chosen or even
 * offers a dialog to the user that allows him/ her to
 * choose an individual color.
 */
void QG_LayerBox::slotLayerChanged(int index) {

    if (index==0 && showUnchanged) {
        unchanged = true;
    }
    else {
        unchanged = false;
    }

    currentLayer = layerList->find(itemText(index));

    //printf("Current color is (%d): %s\n",
    //       index, currentLayer.name().latin1());

    emit layerChanged(currentLayer);
}


