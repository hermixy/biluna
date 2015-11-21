/*****************************************************************
 * $Id: rb_widget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 4, 2009 12:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_WIDGET_H
#define RB_WIDGET_H

#include <QtPrintSupport/QPrinter>
#include <QtWidgets>
#include "db_global.h"
#include "db_seltreesiblingdialog.h"
#include "rb.h"
#include "rb_datawidgetmapper.h"
#include "rb_debug.h"
#include "rb_mmproxy.h"
#include "rb_string.h"

class RB_DialogFactory;
class RB_TableView;
class RB_TreeView;


/**
 * Abstract class for central widgets on dock- and MDI windows
 */
class DB_EXPORT RB_Widget : public QWidget {

    Q_OBJECT

public:
    RB_Widget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~RB_Widget();

    virtual RB_String getId() const;
    virtual void setId(const RB_String& widgetId);
    //! Get name, a unique MDI widget returns non-empty string
    virtual RB_String getName() const = 0;
    virtual void setName(const RB_String& docName);
    virtual RB_String getSaveAsFileName();

    virtual int getWidgetType() const;
    virtual void setWidgetType(int type);
    virtual int getParentWidgetType() const;
    virtual void setParentWidgetType(int type);
    virtual void setDialogFactory(RB_DialogFactory* df);
    virtual RB_DialogFactory* getDialogFactory();

    virtual QWidget* getMainWidget() { return NULL; }

    //! Get contextType such as DB and LDT
    virtual RB2::PerspectiveType getPerspectiveType() const = 0;
    virtual void init();
    virtual void initFocus();

    virtual void fileNew();
    virtual void fileOpen();
    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();
    virtual void filePdf(QPrinter* pr);
    virtual void filePrint(QPrinter* pr);
    virtual void filePrintPreview(QPrinter* pr);

    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editUndo();
    virtual void editRedo();
    virtual void editSelectAll();
    virtual void editZoomIn(double factor = 1.4142135623730950488); // 2^0.5
    virtual void editZoomOut(double factor = 0.7071067811865475244); // 1/(2^0.5)
    virtual void editInsertImage(const RB_String& fn);
    virtual void editCreateLink();

    virtual void formatBold();
    virtual void formatItalic();
    virtual void formatUnderline();
    virtual void formatStrikethrough(); //
    virtual void formatAlignLeft();
    virtual void formatAlignCenter();
    virtual void formatAlignRight();
    virtual void formatAlignJustify(); //
    virtual void formatIncreaseIndent(); //
    virtual void formatDecreaseIndent(); //
    virtual void formatNumberedList(); //
    virtual void formatBulletedList(); //
    virtual void formatFontAll(); // ? same as formatFontName()
    virtual void formatFontName();
    virtual void formatFontSize();
    virtual void formatTextColor();
    virtual void formatBackgroundColor(); //
    virtual void formatRemove();

    virtual RB_String getHelpSubject() const;

    virtual bool loadFile(const RB_String& fn);
    virtual bool saveFile(const RB_String &fn);

    virtual void setPrinterProperties(QPrinter* pr);

    virtual bool closeWidget();

    virtual QModelIndex getCurrentModelIndex(int modelType) const;

    virtual QSize sizeHint() const;
    virtual bool isWindowModified() const;

    virtual RB_String getCurrentFileName();
    virtual void setCurrentFileName(const RB_String& fileName);

    virtual void setIsNewWidget(bool isNewWidget);
    virtual bool isNewWidget();

public slots:
    virtual void slotPrintPreview(QPrinter* pr);
    virtual void slotDataIsChanged(const QModelIndex&, const QModelIndex&);
    virtual void slotDataIsSaved();
    virtual void setWindowModified(bool modified);
    virtual void slotItemViewHeaderClicked(int);
    virtual void slotResetWidgets();


signals:
    //! Signal for MDI window to change modification state in window title
    void modificationChanged(bool changed);
    //! Close button in widget clicked
    void widgetClose();

protected:
    virtual void addComboBoxMapping(RB_MmProxy* model,
                                    const RB_String& field,
                                    const RB_String& relationTable,
                                    const RB_String& relationId,
                                    const RB_String& relationField,
                                    QComboBox* comboBox,
                                    RB_DataWidgetMapper* mapper);
    virtual void setRelationalModelData(RB_MmProxy* pm, const RB_String& field,
                                        const QModelIndex& selIndex,
                                        QLineEdit* le = NULL);
    virtual void setRelationalModelData(RB_MmProxy* pm, const RB_String& field,
                                        RB_ObjectBase* selObj);
    virtual void setCbToLeStyle(QComboBox* cb); // not used any more

    virtual void setFormatTableView(QTableView* tv, RB_MmProxy* m); // TODO: deprecated
    virtual void formatTableView(RB_TableView* tv, RB_MmProxy* m);
    virtual void setFormatTreeView(QTreeView* trv, RB_MmProxy* m); // TODO: deprecated
    virtual void formatTreeView(RB_TreeView *trv, RB_MmProxy* m);

    virtual void readSettings();
    virtual void writeSettings();

    virtual bool maybeSave();
    virtual void beforeFileSave();

    virtual bool isDuplicateIdxFound(RB_MmProxy* model,
                                     const RB_String& fieldName,
                                     const RB_String& id, int excludeRow);
    virtual bool isDuplicateEntryFound(RB_MmProxy* model,
                                       const RB_String& fieldName,
                                       const RB_String& entry, int excludeRow);

    virtual void findInTable(const QString& text, QTableView* tv);
    virtual void findInTree(const QString& text, QTreeView* trv);
    virtual void filterTable(QTableView* tv);
//    virtual void filterTree(QTreeView* trv);
    virtual void filterSourceModel(RB_MmProxy* m, const RB_String& tableName,
                                   const RB_String& fieldName,
                                   const RB_String& fStr);
    virtual void removeFilterSourceModel(RB_MmProxy* m);
    virtual void promote(QTreeView* trv);
    virtual void demote(QTreeView* trv);

    virtual void keyPressEvent(QKeyEvent* e);
    virtual void changeEvent(QEvent* e);

    virtual void setPaletteColors();


    //! Size hint for widget
    QSize mSizeHint;
    //! Full path and file name of current file
    RB_String mFullFileName;
    //! Widget ID, 'UntitledX' or file name or ID from database or widget name
    RB_String mWidgetId;

    QColor mAlternateBaseColor;
    QColor mBaseColor;
    QColor mTextColor;

private:
    void printHelper(QPrinter* pr);
    void filter(RB_MmProxy* m, QTableView *header);
    void expandParent(QTreeView* trv, const QModelIndex& idx);
    void readChildrenSettings(QWidget* wObj);
    void writeChildrenSettings(QWidget* wObj);

    //! Widget type
    int mWidgetType;
    //! Parent widget type, used by MDI windows to find corresponding dockwindows
    int mParentWidgetType;
    //! Dialog factory instead of DIALOGFACTORY to be able to test if exists
    RB_DialogFactory* mDialogFactory;
    //! Flag indicated search started from the beginning of the document
    bool mRestartedAtBegin;
    //! Flag indicating that this is a new widget, not from file or database
    bool mIsNewWidget;

};

#endif // RB_WIDGET_H
