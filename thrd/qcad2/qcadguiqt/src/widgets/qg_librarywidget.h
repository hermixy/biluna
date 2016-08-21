/****************************************************************************
** $Id: qg_librarywidget.h 2612 2005-08-14 22:20:55Z andrew $
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

#ifndef QG_LIBRARYWIDGET_H
#define QG_LIBRARYWIDGET_H

#include <QWidget>
#include "cad_global.h"

#include "ui_qg_librarywidget.h"

//#include "rs_actionhandler.h"

class QTreeWidgetItem;


/**
 * This widget displays a browser for inserting parts and symbols
 * from a library of drawings.
 */
class CAD_EXPORT QG_LibraryWidget: public QWidget, private Ui::QG_LibraryWidget {
    Q_OBJECT

public:
    QG_LibraryWidget(/*RS_ActionHandler* ah,*/ QWidget* parent);
    ~QG_LibraryWidget();

    QString getItemDir(QTreeWidgetItem* item);
    QString getItemPath(QListWidgetItem* item);
    QPixmap getPixmap(const QString& dir, const QString& dxfFile, 
        const QString& dxfPath);
    QString getPathToPixmap(const QString& dir, const QString& dxfFile, 
        const QString& dxfPath);

signals:
    void escape();

public slots:
    void insert();
    void appendTree(QTreeWidgetItem* item, const QString& directory);
    void updatePreview(QTreeWidgetItem* item, QTreeWidgetItem*);
    void setInsertEnabled(bool on);
    void itemCollapsed(QTreeWidgetItem* item);
    void itemExpanded(QTreeWidgetItem* item);

protected:
    virtual void keyPressEvent(QKeyEvent* e);

private:
    //RS_ActionHandler* actionHandler;
};

#endif
