/*****************************************************************
 * $Id: gv_pipelinedialog.cpp 904 2009-01-13 20:40:56Z rutger $
 * Created: Jan 7, 2009 7:11:49 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_pipelinedialog.h"

#include "ldt_line.h"
#include "ldt_project.h"
#include "rb_debug.h"

/**
 * Constructor
 */
GV_PipeLineDialog::GV_PipeLineDialog(QWidget* parent, const RB_ObjectBase& proj, 
		RB_ObjectBase& line) : QDialog(parent) {
	RB_DEBUG->print("GV_PipeLineDialog::GV_PipeLineDialog()");
	setupUi(this);
	
	QGridLayout* lo = new QGridLayout(this);
	QLabel* lbl = new QLabel(tr("Do you want to add a new pipe line?"), this);
	lbl->setSizeIncrement(1,0);
	lo->addWidget(lbl,0,0);
	mWdgt = new LDT_LineMainWidget(this);
	lo->addWidget(mWdgt,1,0);
	// refer for the name layoutWidget in the XML of the .ui file
	lo->addWidget(layoutWidget,2,0);  
	// there was no layout yet
	setLayout(lo);
	
	mLine = &line;
	mWdgt->setDialogData(proj, line);
}

/**
 * Destructor
 */
GV_PipeLineDialog::~GV_PipeLineDialog() {
	RB_DEBUG->print("GV_PipeLineDialog::~GV_PipeLineDialog()");
	delete mWdgt;
	mWdgt = NULL;
	RB_DEBUG->print("GV_PipeLineDialog::~GV_PipeLineDialog() OK");
}

/**
 * Button OK clicked.
 */
void GV_PipeLineDialog::on_pbOk_clicked() {
	RB_DEBUG->print("GV_PipeLineDialog::on_pbOk_clicked()");
	
	mWdgt->getDialogData(*mLine);
	accept();
}
