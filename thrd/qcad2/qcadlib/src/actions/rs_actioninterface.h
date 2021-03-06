/****************************************************************************
** $Id: rs_actioninterface.h 9301 2008-03-03 14:14:57Z andrew $
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


#ifndef RS_ACTIONINTERFACE_H
#define RS_ACTIONINTERFACE_H

#include <qobject.h>

#include "cad_global.h"
#include "rs_entitycontainer.h"
#include "rs_commandevent.h"
#include "rs_event.h"
#include "rs_graphic.h"
#include "rs_graphicview.h"
#include "rs_mainwindowinterface.h"
#include "rs_mouseevent.h"
#include "rs_snapper.h"
#include "rs_string.h"
#include "rs_preview.h"
#include "rs_dialogfactory.h"
#include "rs_stringlist.h"
#include "rs_guiaction.h"

class RS_Preview;
class RS_Widget;

/**
 * This is the interface that must be implemented for all 
 * action classes. Action classes handle actions such
 * as drawing lines, moving entities or zooming in.
 *
 * Inherited from QObject for Qt translation features.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionInterface : public QObject, public RS_Snapper, public RS_PropertyOwner {
    Q_OBJECT

public:
    RS_ActionInterface(RS_EntityContainer& container,
                       RS_GraphicView& graphicView);
    virtual ~RS_ActionInterface();
    
    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
    
    virtual RS_String name() {
        return "Action Interface";
    }
    
    /**
     * Reimplement to make the action exclusive (i.e. terminate
     * all other actions when this one is triggered).
     */
    virtual bool isExclusive() {
        return false;
    }
    
    /**
     * Reimplement to make the action persistent (i.e. never terminate
     * it unless the user terminates it).
     */
    virtual bool isPersistent() {
        return false;
    }

    virtual void init();
    virtual void init(int status);
    virtual void mouseMoveEvent(RS_MouseEvent*);
    virtual void mousePressEvent(RS_MouseEvent*);

    virtual void mouseReleaseEvent(RS_MouseEvent*);
    virtual void keyPressEvent(RS_KeyEvent* e);
    virtual void keyReleaseEvent(RS_KeyEvent* e);
    virtual void dragEnterEvent(QDragEnterEvent* e);
    virtual void dragMoveEvent(QDragMoveEvent* e);
    virtual void dropEvent(QDropEvent* e);
    virtual void coordinateEvent(RS_CoordinateEvent*);
    virtual void commandEvent(RS_CommandEvent*);
    virtual RS_StringList getAvailableCommands(const RS_String& start);
    virtual RS_StringList getAvailableCommands();
    virtual void setStatus(int status);
    virtual int getStatus();
    virtual void trigger();
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();
    virtual bool isFinished();
    virtual void setFinished();
    virtual void finish();
    virtual void setPredecessor(RS_ActionInterface* pre);
    virtual void suspend();
    virtual void resume();
    virtual void hideOptions();
    virtual void showOptions();
    virtual bool optionsFromAction();
    virtual RS_String command(const RS_String& cmd) {
        return cmd;
    }
    virtual RS_String msgAvailableCommands() {
        return QObject::tr("Available commands:");
    }
    virtual bool checkCommand(const RS_String& cmd, const RS_String& str) {
        return cmd.toLower()==str.toLower(); 
    }
    
    virtual RS_Vector snapPoint(RS_MouseEvent* e);
    virtual RS_Vector snapPoint(int x, int y);
    
    virtual void clearPreview();
    virtual void drawPreview();

    virtual void deletePreview();

private:
    /**
     * Current status of the action. After an action has
     * been created the action status is set to 0. Actions
     * that are terminated have a status of -1. Other status
     * numbers can be used to describe the stage this action
     * is in. E.g. a window zoom consists of selecting the
     * first corner (status 0), and selecting the second
     * corner (status 1).
     */
    int status;

protected:
    /**
     * This flag is set when the action has terminated and 
     * can be deleted.
     */
    bool finished;

    /**
     * Pointer to the graphic if this container is a graphic. 
     * NULL otherwise
     */
    RS_Graphic* graphic;

    /**
     * Pointer to the document (graphic or block) or NULL.
     */
    RS_Document* document;
    
    /**
     * Pointer to the main window or NULL.
     */
    RS_MainWindowInterface* mainWindow;

    /**
     * Predecessor of this action or NULL.
     */
    RS_ActionInterface* predecessor;
    
    /**
     * Preview that holds the entities to be previewed.
     */
    RS_Preview* preview;
    /**
     * Current offset of the preview.
     */
    RS_Vector offset;
};

#endif
