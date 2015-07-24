/*****************************************************************
 * $Id: peng_settingsdialog.h 2155 2014-08-04 16:50:28Z rutger $
 * Created: Aug 4, 2014 12:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_SETTINGDIALOG_H
#define PENG_SETTINGDIALOG_H

#include "rb_dialog.h"
#include "ui_peng_settingsdialog.h"

/**
 * Edit dialog for the main settings of PENG,
 * such as labels of custom line fields
 */
class PENG_SettingsDialog : public RB_Dialog,
        private Ui::PENG_SettingsDialog {

    Q_OBJECT

public:
    explicit PENG_SettingsDialog(QWidget *parent = 0);
    ~PENG_SettingsDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "PENG Settings"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void slotIndexChanged(int index);


private:
    //! Data model
    RB_MmProxy* mModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // PENG_SETTINGDIALOG_H
