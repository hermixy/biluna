/*****************************************************************
 * $Id: rb_dialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 14, 2009 3:21:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_DIALOG_H
#define RB_DIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QPrinter>
#include <QTableView>
#include <QTreeView>
#include "db_global.h"
#include "rb_datawidgetmapper.h"
#include "rb_debug.h"
#include "rb_mmproxy.h"

class QComboBox;
class RB_TableView;
class RB_TreeView;

/**
 * Abstract class for dialogs
 */
class DB_EXPORT RB_Dialog : public QDialog {

    Q_OBJECT

public:
    RB_Dialog(QWidget* parent);
    virtual ~RB_Dialog();

    virtual RB_String getSaveAsFileName();

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual void filePdf(QPrinter* pr);
    virtual void filePrint(QPrinter* pr);

    virtual RB_ObjectBase* getCurrentObject() { return NULL; }
    virtual RB_ObjectBase* getCurrentChild1Object() { return NULL; }
    virtual RB_ObjectBase* getCurrentChild2Object() { return NULL; }

    virtual RB_MmProxy* getModel() { return NULL; }
    virtual RB_MmProxy* getChild1Model() { return NULL; }
    virtual RB_MmProxy* getChild2Model() { return NULL; }

    virtual void setPrinterProperties(QPrinter* pr);

    virtual RB_String getCurrentFileName();
    virtual void setCurrentFileName(const RB_String& fileName);

public slots:
    virtual void reject();

    virtual void slotPrintPreview(QPrinter* pr);
    virtual void slotDataIsChanged(const QModelIndex&, const QModelIndex&);
    virtual void slotDataIsSaved();
    virtual void slotItemViewHeaderClicked(int);

protected:
    virtual void addComboBoxMapping(RB_MmProxy* model,
                                    const RB_String& field,
                                    const RB_String& relationTable,
                                    const RB_String& relationId,
                                    const RB_String& relationField,
                                    QComboBox* comboBox,
                                    RB_DataWidgetMapper* mapper);
    virtual void setRelationalModelData(RB_MmProxy* pm, const RB_String& field,
                                        RB_ObjectBase* selObj);
    virtual void setCbToLeStyle(QComboBox* cb);

    virtual void setFormatTableView(QTableView* tv, RB_MmProxy* m); // TODO: deprecated
    virtual void formatTableView(RB_TableView* tv, RB_MmProxy* m);
    virtual void setFormatTreeView(QTreeView* trv, RB_MmProxy* m); // TODO: deprecated
    virtual void formatTreeView(RB_TreeView *trv, RB_MmProxy* m);

    virtual void readSettings();
    virtual void writeSettings();

    virtual bool maybeSave();
    virtual void beforeFileSave();
    virtual void closeEvent(QCloseEvent* e);

    virtual void findInTable(const QString& text, QTableView* tv);
    virtual void findInTree(const QString& text, QTreeView* trv);
    virtual void filterTable(QTableView* tv);
//    virtual void filterTree(QTreeView* trv);
//    virtual void fetchAll(RB_MmProxy* m, const QModelIndex& parent = QModelIndex());
    virtual void filterSourceModel(RB_MmProxy* m, const RB_String& tableName,
                                   const RB_String& fieldName,
                                   const RB_String& fStr);
    virtual void removeFilterSourceModel(RB_MmProxy* m);
    virtual void promote(QTreeView* trv);
    virtual void demote(QTreeView* trv);

    virtual void keyPressEvent(QKeyEvent* e);

    //! Full path and file name of current file
    RB_String mFullFileName;

private:
    void filter(RB_MmProxy* m, QTableView *tv);
    void expandParent(QTreeView* trv, const QModelIndex& idx);
    void readChildrenSettings(QWidget* wObj);
    void writeChildrenSettings(QWidget* wObj);

    //! Flag indicated search started from the beginning of the document
    bool mRestartedAtBegin;

};

#endif // RB_DIALOG_H
