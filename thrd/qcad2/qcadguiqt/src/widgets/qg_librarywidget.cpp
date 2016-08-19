/****************************************************************************
** $Id: qg_librarywidget.cpp 2612 2005-08-14 22:20:55Z andrew $
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

#include "qg_librarywidget.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDateTime>
#include <QImage>
#include <QPixmap>

#include "rs_system.h"
#include "rs_actionlibraryinsert.h"
#include "rs_staticgraphicview.h"
#include "rs_painterqt.h"

/**
 * Constructor.
 */
QG_LibraryWidget::QG_LibraryWidget(/*RS_ActionHandler* ah,*/ QWidget* parent)
        : QWidget(parent) {
    setupUi(this);

    twDirectory->setHeaderLabels(QStringList(tr("Directories")));
    
    connect(twDirectory, 
        SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
        this, SLOT(updatePreview(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(twDirectory, SIGNAL(itemExpanded(QTreeWidgetItem*)),
        this, SLOT(itemExpanded(QTreeWidgetItem*)));
    connect(twDirectory, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
        this, SLOT(itemCollapsed(QTreeWidgetItem*)));
    connect(bInsert, SIGNAL(clicked()),
        this, SLOT(insert()));

    //actionHandler = ah;

    QStringList directoryList = RS_SYSTEM->getDirectoryList("library");
    for (QStringList::Iterator it = directoryList.begin(); it!=directoryList.end(); ++it) {
        appendTree(NULL, (*it));
    }
}



/**
 * Destructor
 */
QG_LibraryWidget::~QG_LibraryWidget() {
}


/**
 * Escape releases focus.
 */
void QG_LibraryWidget::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {

    case Qt::Key_Escape:
        emit escape();
        break;

    default:
        QWidget::keyPressEvent(e);
        break;
    }
}


/**
 * Insert.
 */
void QG_LibraryWidget::insert() {
    QListWidgetItem* item = lwPreview->currentItem();
    QString dxfPath = getItemPath(item);
    RS_DEBUG->print("QG_LibraryWidget::insert:"
        "dxfPath: %s", (const char*)dxfPath.toLatin1());

    if (QFileInfo(dxfPath).isReadable()) {
        //if (actionHandler!=NULL) {
            RS_GuiAction* ga = RS_ActionLibraryInsert::createGuiAction();
            RS_ActionInterface* a = ga->slotTriggered();
            //RS_GuiAction::triggerByType(RS2::ActionLibraryInsert);
            //RS_ActionInterface* a =
            //    actionHandler->setCurrentAction(RS2::ActionLibraryInsert);
            if (a!=NULL) {
                RS_ActionLibraryInsert* action = (RS_ActionLibraryInsert*)a;
                action->setFile(dxfPath);
            } else {
                RS_DEBUG->print(RS_Debug::D_ERROR,
                                "QG_LibraryWidget::insert:"
                                "Cannot create action RS_ActionLibraryInsert");
            }
        //}
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR,
            "QG_LibraryWidget::insert: Can't read file: '%s'", 
            (const char*)dxfPath.toLatin1());
    }
}



/**
 * Appends the given directory to the given list view item. Called recursively until all
 * library directories are appended.
 */
void QG_LibraryWidget::appendTree(QTreeWidgetItem* item, const QString& directory) {
    RS_DEBUG->print("QG_LibraryWidget::appendTree: %s", 
        (const char*)directory.toLatin1());
    
    QStringList::Iterator it;
    QDir dir(directory);

    // read subdirectories of this directory:
    if (dir.exists()) {
        bool subItemLevel = false;
        if (item!=NULL) {
            subItemLevel = true;
        }
        QStringList lDirectoryList = dir.entryList(QDir::Dirs, QDir::Name);

        QTreeWidgetItem* newItem = NULL;
        QTreeWidgetItem* searchItem = NULL;
        for (it=lDirectoryList.begin(); it!=lDirectoryList.end(); ++it) {
            if ((*it)!="." && (*it)!="..") {
    
                RS_DEBUG->print("QG_LibraryWidget::appendTree: dir: %s", 
                    (const char*)(*it).toLatin1());

                newItem=NULL;

                // Look for an item already existing and take this
                //   instead of making a new one:
                int i=0;
                do {
                    if (subItemLevel) {
                        searchItem = item->child(i);
                    }
                    else {
                        searchItem = twDirectory->topLevelItem(i);
                    }
                    if (searchItem!=NULL) {
                        RS_DEBUG->print("QG_LibraryWidget::appendTree: searching item: %s", 
                            (const char*)searchItem->text(0).toLatin1());
                    }
                    if (searchItem!=NULL && searchItem->text(0)==(*it)) {
                        newItem = searchItem;
                        break;
                    }
                    i++;
                } while (searchItem!=NULL);
                
                
                // Create new item if no existing was found:
                if (newItem==NULL) {
                    if (subItemLevel) {
                        RS_DEBUG->print("QG_LibraryWidget::appendTree: creating new child item");
                        newItem = new QTreeWidgetItem(item);
                    } else {
                        RS_DEBUG->print("QG_LibraryWidget::appendTree: creating new top level item");
                        newItem = new QTreeWidgetItem(twDirectory);
                    }
                    newItem->setIcon(0, QIcon(":/qcad/images/folderclosed.png"));
                    newItem->setText(0, *it);
                }

                appendTree(newItem, directory + QDir::separator() + (*it));
            }
        }
    }
}


/**
 * Updates the icon preview.
 */
void QG_LibraryWidget::updatePreview(QTreeWidgetItem* item, QTreeWidgetItem*) {
    if (item==NULL) {
        return;
    }
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    // dir from the point of view of the library browser (e.g. /mechanical/screws)
    QString directory = getItemDir(item);
    lwPreview->clear();

    // List of all directories that contain part libraries:
    QStringList directoryList = RS_SYSTEM->getDirectoryList("library");
    QStringList::Iterator it;
    QDir itemDir;
    QStringList itemPathList;

    // look in all possible system directories for DXF files in the current library path:
    for (it=directoryList.begin(); it!=directoryList.end(); ++it) {
        itemDir.setPath((*it)+directory);

        if (itemDir.exists()) {
            QStringList itemNameList =
                itemDir.entryList(QStringList("*.dxf"), QDir::Files, QDir::Name);
            QStringList::Iterator it2;
            for (it2=itemNameList.begin(); it2!=itemNameList.end(); ++it2) {
                itemPathList += itemDir.path() + QDir::separator() + (*it2);
            }
        }
    }

    // Sort entries:
    itemPathList.sort();

    // Fill items into icon view:
    QListWidgetItem* newItem;
    for (it=itemPathList.begin(); it!=itemPathList.end(); ++it) {
        QString label = QFileInfo(*it).completeBaseName();
        newItem = new QListWidgetItem(lwPreview);
        newItem->setToolTip(label);
        newItem->setData(Qt::UserRole, label);
        if (label.length()<8) {
            newItem->setText(label);
        }
        else {
            newItem->setText(label.left(6) + "..");
        }
        QPixmap pixmap = getPixmap(directory, QFileInfo(*it).fileName(), (*it));
        newItem->setIcon(pixmap);
    }
    QApplication::restoreOverrideCursor();
}


/**
 * @return Directory (in terms of the List view) to the given item (e.g. /mechanical/screws)
 */
QString QG_LibraryWidget::getItemDir(QTreeWidgetItem* item) {
    QString ret = "";

    if (item==NULL) {
        return ret;
    }

    QTreeWidgetItem* parent = item->parent();
    return getItemDir(parent) + QString("/%1").arg(item->text(0));
}



/**
 * @return Path of the DXF file that is represented by the given item.
 */
QString QG_LibraryWidget::getItemPath(QListWidgetItem* item) {
    QString dir = getItemDir(twDirectory->currentItem());
    if (item!=NULL) {
        // List of all directories that contain part libraries:
        QStringList directoryList = RS_SYSTEM->getDirectoryList("library");
        QStringList::Iterator it;
        QDir itemDir;

        // look in all possible system directories for DXF files in the current library path:
        for (it=directoryList.begin(); it!=directoryList.end(); ++it) {
            itemDir.setPath((*it)+dir);
            if (itemDir.exists()) {
                QString name = item->data(Qt::UserRole).toString();
                QString f = (*it) + dir + QDir::separator() + name + ".dxf";
                if (QFileInfo(f).isReadable()) {
                    return f;
                }
            }
        }

        return "";
    } else {
        return "";
    }
}



/**
 * @return Pixmap that serves as icon for the given DXF File.
 * The existing PNG file is returned or created and returned..
 *
 * @param dir Library directory (e.g. "/mechanical/screws")
 * @param dxfFile File name (e.g. "screw1.dxf")
 * @param dxfPath Full path to the existing DXF file on disk 
 *                          (e.g. /home/tux/.qcad/library/mechanical/screws/screw1.dxf)
 */
QPixmap QG_LibraryWidget::getPixmap(const QString& dir, const QString& dxfFile,
                                    const QString& dxfPath) {
    QString pngFile = getPathToPixmap(dir, dxfFile, dxfPath);
    QFileInfo fiPng(pngFile);

    // found or created thumbnail:
    if (fiPng.isFile()) {
        return QPixmap(pngFile);
    }
    // default empty thumbnail:
    else {
        QPixmap p = QPixmap(64,64);
        p.fill();
        return p;
    }
}



/**
 * @return Path to the thumbnail of the given DXF file. If no thumbnail exists, one is
 * created in the user's home. If no thumbnail can be created, an empty string is returned.
 */
QString QG_LibraryWidget::getPathToPixmap(const QString& dir,
        const QString& dxfFile,
        const QString& dxfPath) {

    RS_DEBUG->print("QG_LibraryWidget::getPathToPixmap: "
                    "dir: '%s' dxfFile: '%s' dxfPath: '%s'",
                    (const char*)dir.toLatin1(), 
                    (const char*)dxfFile.toLatin1(), 
                    (const char*)dxfPath.toLatin1());

    // List of all directories that contain part libraries:
    QStringList directoryList = RS_SYSTEM->getDirectoryList("library");
    directoryList.prepend(RS_SYSTEM->getHomeDir() + 
        QDir::separator() + ".qcad" + QDir::separator() + "library");
    QStringList::Iterator it;

    QFileInfo fiDxf(dxfPath);
    QString itemDir;
    QString pngPath;

    // look in all possible system directories for PNG files
    //  in the current library path:
    for (it=directoryList.begin(); it!=directoryList.end(); ++it) {
        itemDir = (*it)+dir;
        pngPath = itemDir + QDir::separator() + fiDxf.completeBaseName() + ".png";
        RS_DEBUG->print("QG_LibraryWidget::getPathToPixmap: checking: '%s'",
                        (const char*)pngPath.toLatin1());
        QFileInfo fiPng(pngPath);

        // the thumbnail exists:
        if (fiPng.isFile()) {
            RS_DEBUG->print("QG_LibraryWidget::getPathToPixmap: dxf date: %s, png date: %s",
                            (const char*)fiDxf.lastModified().toString().toLatin1(), 
                            (const char*)fiPng.lastModified().toString().toLatin1());
            if (fiPng.lastModified() > fiDxf.lastModified()) {
                RS_DEBUG->print("QG_LibraryWidget::getPathToPixmap: thumbnail found: '%s'",
                                (const char*)pngPath.toLatin1());
                return pngPath;
            } else {
                RS_DEBUG->print("QG_LibraryWidget::getPathToPixmap: thumbnail needs to be updated: '%s'",
                                (const char*)pngPath.toLatin1());
            }
        }
    }

    // the thumbnail must be created in the user's home.

    // create all directories needed:
    RS_SYSTEM->createHomePath("/.qcad/library" + dir);

    QString d = RS_SYSTEM->getHomeDir() + "/.qcad/library" + dir;

    pngPath = d + QDir::separator() + fiDxf.completeBaseName() + ".png";

    QPixmap* buffer = new QPixmap(132,132);
    RS_PainterQt* painter = new RS_PainterQt(buffer);
    painter->setBackground(RS_Color(255,255,255));
    painter->eraseRect(0,0, 132,132);

    RS_StaticGraphicView gv(132,132, painter);
    RS_Graphic graphic;
    if (graphic.open(dxfPath, RS2::FormatUnknown)) {
        RS_Color black(0,0,0);
        for (RS_Entity* e=graphic.firstEntity(RS2::ResolveAll);
                e!=NULL; e=graphic.nextEntity(RS2::ResolveAll)) {
            RS_Pen pen = e->getPen();
            pen.setColor(RS_Color(Qt::black));
            e->setPen(pen);
        }

        gv.setContainer(&graphic);
        gv.zoomAuto(false);
        gv.drawEntity(&graphic, true);

        QImage img;
        img = buffer->toImage();
        img = img.scaled(66,66, 
            Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        img = img.copy(1,1,64,64);
        if (!img.save(pngPath, "PNG")) {
            RS_DEBUG->print(RS_Debug::D_ERROR,
                            "QG_LibraryWidget::getPathToPixmap: Cannot write thumbnail: '%s'",
                            (const char*)pngPath.toLatin1());
            pngPath = "";
        }
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR,
                        "QG_LibraryWidget::getPathToPixmap: Cannot open file: '%s'",
                        (const char*)dxfPath.toLatin1());
    }

    // GraphicView deletes painter
    painter->end();
    delete buffer;

    return pngPath;
}
    
    
void QG_LibraryWidget::setInsertEnabled(bool on) {
    bInsert->setEnabled(on);
}

    
void QG_LibraryWidget::itemExpanded(QTreeWidgetItem* item) {
    if (item!=NULL) {
        item->setIcon(0, QIcon(":/qcad/images/folderopen.png"));
    }
}

void QG_LibraryWidget::itemCollapsed(QTreeWidgetItem* item) {
    if (item!=NULL) {
        item->setIcon(0, QIcon(":/qcad/images/folderclosed.png"));
    }
}
