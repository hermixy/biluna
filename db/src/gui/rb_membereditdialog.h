#ifndef RB_MEMBEREDITDIALOG_H
#define RB_MEMBEREDITDIALOG_H

#include "rb_dialog.h"

namespace Ui {
class RB_MemberEditDialog;
}

class RB_MemberEditDialog : public RB_Dialog {

    Q_OBJECT

public:
    explicit RB_MemberEditDialog(QWidget *parent = 0);
    virtual ~RB_MemberEditDialog();
    virtual showObjectMembers(RB_ObjectBase* obj, const QStringList &memberList);

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();

private:
    Ui::RB_MemberEditDialog* ui;
    RB_ObjectBase* mObject;
    QStringList mMemberList;
};

#endif // RB_MEMBEREDITDIALOG_H
