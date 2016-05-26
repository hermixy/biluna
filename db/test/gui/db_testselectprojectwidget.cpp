#include "db_testselectprojectwidget.h"

#include "db_modelfactory.h"

DB_TestSelectProjectWidget::DB_TestSelectProjectWidget(QWidget *parent) :
    RB_TableWidget(parent) {
}

DB_TestSelectProjectWidget::~DB_TestSelectProjectWidget() {
    delete mModel;
}

void DB_TestSelectProjectWidget::init() {
    // set model
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTestProject);
    mModel->setRoot(DB_MODELFACTORY->getRootId());
    mModel->select();

    setWindowTitle(tr("Select test project"));
    lblRoot->setText(tr("Root"));
    lblRoot->hide();
    leRoot->hide();
    gbTable->setTitle(tr("GroupBox Blah Blah"));

    this->initEditSort();
//    this->initEditUpDown();
//    initSelectionOnly();
//    this->initTest();

    this->setHelpSubject("DB_TestSelectProjectDialog");
    readSettings();
}
