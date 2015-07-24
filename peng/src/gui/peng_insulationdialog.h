/*****************************************************************
 * $Id: peng_insulationdialog.h 2165 2014-09-24 19:22:36Z rutger $
 * Created: Aug 18, 2005 - rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_INSULATIONDIALOG_H
#define PENG_INSULATIONDIALOG_H

#include "rb_dialog.h"
#include "ui_peng_insulationdialog.h"

class PENG_InsulationDialog : public RB_Dialog, private Ui::PENG_InsulationDialog {

    Q_OBJECT

public:
    explicit PENG_InsulationDialog(QWidget *parent = 0);
    ~PENG_InsulationDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "PENG Insulation"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    virtual void changeEvent(QEvent *e);

private:
    void updateModels();
    void addDefaultRows(RB_MmProxy* model);
    void setDefaultRow(RB_MmProxy* model, double lineSize);
    void setVisibleColumns(RB_MmProxy* model, RB_TableView* tv);
    void setColdTemperatureHeader(RB_MmProxy* model);
    void setWarmTemperatureHeader(RB_MmProxy* model);

    //! Insulation model for cold conservation
    RB_MmProxy* mModelCC;
    //! Insulation model for heat conservation indoor
    RB_MmProxy* mModelHCI;
    //! Insulation model for heat conservation outdoor
    RB_MmProxy* mModelHCO;
    //! Insulation model for personnel protection
    RB_MmProxy* mModelPP;
};

#endif // PENG_INSULATIONDIALOG_H
