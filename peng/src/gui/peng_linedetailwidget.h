/*****************************************************************
 * $Id: peng_linedetailwidget.h 2155 2014-08-04 16:50:28Z rutger $
 * Created: 2004 - rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_LINEDETAILWIDGET_H
#define PENG_LINEDETAILWIDGET_H

#include "rb_widget.h"
#include "ui_peng_linedetailwidget.h"


/**
 * Detail widget for (pipe) line from line designation table
 */
class PENG_LineDetailWidget : public RB_Widget, private Ui::PENG_LineDetailWidget {

    Q_OBJECT

public:
    PENG_LineDetailWidget(QWidget *parent = 0);
    virtual ~PENG_LineDetailWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "PENG Line Details"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool closeWidget();

public slots:
    virtual void slotFileSave();
    virtual void slotFileRevert();
    virtual void on_ileFluidCode_clicked();
    virtual void on_ileFluidCode_clear();
    virtual void on_ilePipeClass_clicked();
    virtual void on_ilePipeClass_clear();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void slotParentRowChanged(const QModelIndex& curr,
                              const QModelIndex& prev);
    void slotHandleParentRowChanged();
    void slotUpdateSettings(int dialogWidgetType);

private:
    RB_MmProxy* mModelLine;
    RB_DataWidgetMapper* mMapperLine;
    RB_MmProxy* mModelLineClassif;
    RB_DataWidgetMapper* mMapperLineClassif;
    RB_MmProxy* mModelLineCustom;
    RB_DataWidgetMapper* mMapperLineCustom;
    RB_MmProxy* mModelLineEnginDes;
    RB_DataWidgetMapper* mMapperLineEnginDes;
    RB_MmProxy* mModelLineProcess;
    RB_DataWidgetMapper* mMapperLineProcess;

    //! Last selected line ID
    RB_String mCurrentId;
};
#endif // PENG_LINEDETAILWIDGET_H

/*
//#include <QAssistantClient>
#include <QMainWindow>
#include <QObject>
#include <QTableWidgetItem>
#include "finddialog.h"
//#include "jobmainwindow.h"
//#include "ldt_dmline.h"
#include "ldt_mmline.h"
#include "ldt_objectfactory.h"
#include "rb_mmobjectinterface.h"
#include "rb_signalspydialog.h"	// todo: debug only
//#include "textedit.h"
#include "ui_mainwindow.h"

class FindDialog;
class FluidReport;
class InspectionDataReport;
class InspectionLoopReport;
class InsulationReport;
class PaintReport;
class PipeClassReport;
class TieInReport;

class QAction;
class QMenu;
class QToolBar;

class MainWindow : public QMainWindow, private Ui::MainWindow {
	
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
    
    RB_MmObjectInterface* getModelLineManager() { return modelLine; }
    RB_MmObjectInterface* getModelInspectionManager() { return modelInspectionItem; }
    int getCurrentRow() { return tableView->currentIndex().row(); }
    QItemSelectionModel* getSelectionModel() { return selectionModel; }
	
	// todo: debug only
	Q4puGenericSignalSpy* spy;
	RB_SignalSpyDialog* ssd;
	

public slots:
    virtual void fileNew();
    virtual void fileProjectEdit();
    virtual void fileOpen();
    virtual bool fileSave();
    virtual bool fileSaveAs();
    // virtual void filePrint();
    virtual void fileImportLdt();
    virtual void fileImportPcc();
    virtual void fileExportLdt();
    // virtual void fileExit(); replaced by closeEvent
    virtual void closeEvent(QCloseEvent* event);
    virtual void editUndo();
    virtual void editRedo();
    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editFind();
	virtual void editFilterOn();
	virtual void editFilterOff();
	virtual void editCommitChanges();
	virtual void viewProcess();
	virtual void viewUtility();
	virtual void viewChanges();
	virtual void lineNew();
    virtual void lineDelete();
    virtual void lineExecute();
    virtual void lineClassification();
    virtual void lineTieIn();
    virtual void specificationFluid();
    virtual void specificationInsulation();
    virtual void specificationPainting();
    virtual void specificationPipeClass();
    virtual void specificationInspectionLoop();
    virtual void specificationTieIn();
//    virtual void jobWork();
//    virtual void jobResource();
    virtual void jobTask();
//    virtual void jobResourceUse();
    virtual void databaseConnect();
    virtual void databaseSelectProject();
    virtual void databaseCreateTables();
    virtual void databaseSettings();
    virtual void databaseBrowse();
    virtual void databasePermission();
    virtual void databaseImportCsv();
    virtual void databaseImportXml();
    virtual void databaseExportXml();
    virtual void databaseDisconnect();
    virtual void reportTable();
    virtual void reportFluid();
	virtual void reportInspectionData();
	virtual void reportInspectionLoop();
	virtual void reportInsulation();
	virtual void reportPaint();
	virtual void reportPipeClass();
	virtual void reportTieIn();
    virtual void helpIndex();
    virtual void helpContents();
    virtual void helpAbout();
    virtual void helpDebugSpy();		// TODO: for debug only, to be removed
    virtual void helpDebugModel();		// TODO: for debug only, to be removed
    virtual void messageWarning(const QString& str);	// TODO: not used
    virtual void messageCritical(const QString& str);	// TODO: not used
    
    virtual void modelDataChanged(const QModelIndex&, const QModelIndex&);
    
    virtual void selectTableRow(int row) {
    	tableView->selectRow(row);
    }
	
	virtual void setColumnTableView(int column, int oldSize, int newSize);
	virtual void setColumnTvInspectionItem(int column, int oldSize, int newSize);

protected:


protected slots:
    virtual void languageChange();

private:
	void createFileActions();
	void createEditActions();
	void createViewActions();
	void createLineActions();
	void createSpecificationActions();
	void createJobActions();
	void createDatabaseActions();
	void createReportActions();
	void createHelpActions();
	
	void createModel();
	void createConnections();
	void removeConnections();
	void formatTableView();
	void formatTvInspectionItem();
	void initStatusBar();
//	void resizeEvent(QResizeEvent* event);
	
    void updateMenus();
    void resetModel();
	void formatLineFields();

    void clearProjectFields();
    void clearLineFields();

	void readSettings();
	void writeSettings();
	void setCurrentFileName(const QString& fn);
	void loadFile(const QString &fileName, const QString &fileType);
	bool saveFile(const QString &fileName);
    bool maybeSave();
    QString strippedFileName(const QString &fullFileName);
    	
	QToolBar* toolBar;

    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileProjectEditAction;
    QAction* fileSaveAction;
    QAction* fileSaveAsAction;
    QAction* fileImportLdtAction;
    QAction* fileImportPccAction;
    QAction* fileExportLdtAction;
    QAction* editUndoAction;
    QAction* editRedoAction;
    QAction* editCutAction;
    QAction* editCopyAction;
    QAction* editPasteAction;
    QAction* editFindAction;
    QAction* editFilterOnAction;
    QAction* editFilterOffAction;
    QAction* editCommitChangesAction;
    QAction* viewProcessAction;
    QAction* viewUtilityAction;
    QAction* viewChangesAction;
    QAction* lineNewAction;
    QAction* lineDeleteAction;
    QAction* lineFindAction;
    QAction* lineExecuteAction;
    
//	QAction* jobWorkAction;
//	QAction* jobResourceAction;
    QAction* jobTaskAction;
//	QAction* jobResourceUseAction;
    
    QAction* dbConnectAction;
    QAction* dbSelectProjectAction;
    QAction* dbCreateAction;
    QAction* dbSettingsAction;
    QAction* dbBrowseAction;
    QAction* dbPermissionAction;
    QAction* dbImportCsvAction;
    QAction* dbImportXmlAction;
    QAction* dbExportXmlAction;
    QAction* dbDisconnectAction;
    // QAction* reportTableAction;
    
    QString currentFileName;
//    QString openPath;
//    QString savePath;
    
    RB_MmObjectInterface* modelLine;			/ **< current GUI model * /
    QItemSelectionModel* selectionModel;
    RB_MmObjectInterface* modelInspectionItem;	/ **< sub model inspection item * /
    QItemSelectionModel* selectionInspectionModel;
    RB_MmObjectInterface* modelLineImport;		/ **< temporary model for XML import * /
    RB_MmObjectInterface* modelLineExport;		/ **< temporary model for XML export * /
    // int currentRow;

	FindDialog* findDialog;
	FluidReport* fluidReport;
	InspectionDataReport* inspectionDataReport;
	InspectionLoopReport* inspectionLoopReport;
	InsulationReport* insulationReport;
	PaintReport* paintReport;
	PipeClassReport* pipeClassReport;
//	JobMainWindow* jobMainWindow;
	TieInReport* tieInReport;

	// help assistant
//	QAssistantClient* assistantClient;
	
	// keep track of changed textEdits
	bool textEditRemProcessChanged;
	bool textEditRemEnginDesChanged;
	bool textEditRemClassificationChanged;
	bool textEditRemOperMaintChanged;
	bool textEditRemCustomChanged;
	void saveTextEditRemProcess(int row);
	void saveTextEditRemEnginDes(int row);
	void saveTextEditRemClassification(int row);
	void saveTextEditRemOperMaint(int row);
	void saveTextEditRemCustom(int row);

	// color
	QPalette revPalette;
	
    
private slots:

    void clipboardDataChanged();
    
	// signal from selectionModel, emitted when selection in tableView changes
	void changeCurrent(const QModelIndex& current, const QModelIndex& previous);
			
	void setProjectFields(const RB_ObjectContainer& activeProject);
	void setLineFields();
	void setLineFieldDecoration();

	// Format for autoconnection slots:
	//   void on_<widget name>_<signal name>(<signal parameters>);
	void on_pushButtonNewInspItem_clicked();
	void on_pushButtonDeleteInspItem_clicked();
	void on_pushButtonUpInspItem_clicked();
	void on_pushButtonDownInspItem_clicked();	
	void on_pushButtonEditInspItem_clicked();
	
	void on_lineEditLineNo_editingFinished();
	void on_lineEditLineRev_editingFinished();
	void on_lineEditSize_editingFinished();
	void on_comboBoxProcessUtility_activated(const QString& str);
	void on_lineEditFrom_editingFinished();
	void on_comboBoxFromType_activated(const QString& str);
	void on_lineEditFromNozzle_editingFinished();
	void on_lineEditTo_editingFinished();
	void on_comboBoxToType_activated(const QString& str);
	void on_lineEditToNozzle_editingFinished();
	void on_lineEditFluidID_editingFinished();
	void on_lineEditPipeClass_editingFinished();
	void on_lineEditTd_editingFinished();
	void on_lineEditPd_editingFinished();
	
	// detail data
	void on_lineEditProcessFluidDescr_editingFinished();
	void on_lineEditProcessOperPress_editingFinished();
	void on_lineEditProcessOperTemp_editingFinished();
	void on_lineEditProcessOperTempLow_editingFinished();
	void on_lineEditProcessOperTempHigh_editingFinished();
	void on_lineEditProcessTestMedium_editingFinished();
	void on_lineEditProcessTestPress_editingFinished();
	void on_lineEditProcessPressDrop_editingFinished();
	void on_lineEditProcessMassFlowRate_editingFinished();
	void on_lineEditProcessMassFlowRateMax_editingFinished();
	void on_lineEditProcessVelocity_editingFinished();
	void on_comboBoxProcessPhase_activated(const QString& str);
	void on_lineEditProcessBoilingPoint_editingFinished();
	void on_lineEditProcessVapourPress_editingFinished();
	void on_lineEditProcessMolWeight_editingFinished();
	void on_lineEditProcessDensityLiquid_editingFinished();
	void on_lineEditProcessDensityVapour_editingFinished();
	void on_lineEditProcessViscosityLiquid_editingFinished();
	void on_lineEditProcessViscosityVapour_editingFinished();
	void on_lineEditProcessStandardVolumeFlow_editingFinished();
	void on_lineEditProcessStandardVolumeFlowMax_editingFinished();
	void on_lineEditProcessActualFlow_editingFinished();
	void on_lineEditProcessActualFlowMax_editingFinished();
	void on_lineEditProcessSimulation_editingFinished();
	void on_lineEditDesPnidFrom_editingFinished();
	void on_lineEditDesPnidTo_editingFinished();
	void on_lineEditDesGaFrom_editingFinished();
	void on_lineEditDesGaTo_editingFinished();
	void on_lineEditDesIsoFrom_editingFinished();
	void on_lineEditDesIsoTo_editingFinished();
	void on_comboBoxDesLineFunction_activated(const QString& str);
	void on_comboBoxDesExternalPressure_activated(const QString& str);
	void on_comboBoxDesTracingType_activated(const QString& str);
	void on_lineEditDesTracingNumberOf_editingFinished();
	void on_comboBoxDesInsulationType_activated(const QString& str);
	void on_lineEditDesInsulationThickness_editingFinished();
	void on_checkBoxDesPaintingAsPerSpec_released();
	void on_lineEditDesPaintingSystem_editingFinished();
	void on_lineEditClassNomOperPress_editingFinished();
	void on_lineEditClassNomOperTemp_editingFinished();
	void on_lineEditClassMaxDesPress_editingFinished();
	void on_lineEditClassMaxDesTemp_editingFinished();
	void on_lineEditClassMinDesTemp_editingFinished();
	void on_lineEditClassAuthorRegul_editingFinished();
	void on_comboBoxClassFluidDangerClassGroup_activated(const QString& str);
	void on_comboBoxClassGasStability_activated(const QString& str);
	void on_lineEditClassRegulClassGroup_editingFinished();
	void on_lineEditClassRegulSubClassGroup_editingFinished();
			
	void on_dateEditOperInstallation_dateChanged(const QDate& date);
	void on_checkBoxOperLineOutOfService_released();
	void on_checkBoxOperPeriodicInspectionRequired_released();
	void on_pushButtonLoopNumber_clicked();
	void on_pushButtonClearNumber_clicked();
	void on_lineEditOperInspectionFrequency_editingFinished();
	
	// on_textEditRemProcess_textChanged() triggers the modelModified 
	//   when loading a file, separate slots necessary with QtextEdit
	//   otherwise problems with invalid index and other than first
	//   textEdit does not work properly, something wrong with the sequence
	//   of firing events.
	void on_textEditRemProcess_cursorPositionChanged();
	void on_textEditRemEnginDes_cursorPositionChanged();
	void on_textEditRemClassification_cursorPositionChanged();
	void on_textEditRemOperMaint_cursorPositionChanged();
	void on_textEditRemCustom_cursorPositionChanged();

	void on_lineEditCustValue1_editingFinished();
	void on_lineEditCustValue2_editingFinished();
	void on_lineEditCustValue3_editingFinished();
	void on_lineEditCustValue4_editingFinished();
	void on_lineEditCustValue5_editingFinished();
	void on_lineEditCustText6_editingFinished();
	void on_lineEditCustText7_editingFinished();
	void on_lineEditCustText8_editingFinished();
	void on_lineEditCustText9_editingFinished();
	void on_lineEditCustText10_editingFinished();
	
};
*/

