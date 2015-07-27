/*****************************************************************
 * $Id: gv_pipelinedialog.h 904 2009-01-13 20:40:56Z rutger $
 * Created: Jan 7, 2009 6:49:37 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_PIPELINEDIALOG_H
#define GV_PIPELINEDIALOG_H

#include <QtGui>
#include "ldt_linemainwidget.h"
#include "rb_objectbase.h"
#include "ui_gv_pipelinedialog.h"


/**
 * Dialog for editing main pipe line data
 */
class GV_PipeLineDialog : public QDialog, private Ui::GV_PipeLineDialog {
	
	Q_OBJECT
	
public:
	GV_PipeLineDialog(QWidget* parent, const RB_ObjectBase& proj, 
						RB_ObjectBase& line);
	virtual ~GV_PipeLineDialog();
	
private slots:
	// Format for autoconnection slots:
	//   void on_<widget name>_<signal name>(<signal parameters>);
	void on_pbOk_clicked();

	
private:
	LDT_LineMainWidget* mWdgt;
	RB_ObjectBase* mLine;
};

#endif /*GV_PIPELINEDIALOG_H*/
