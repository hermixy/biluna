#ifndef RB_DATABASETABLEDIALOG_H
#define RB_DATABASETABLEDIALOG_H

#include "rb_dialog.h"

namespace Ui {
class RB_DatabaseTableDialog;
}

class RB_DatabaseTableDialog : public RB_Dialog {

    Q_OBJECT

public:
    RB_DatabaseTableDialog(QWidget* parent = 0);
    virtual ~RB_DatabaseTableDialog();

private:
    Ui::RB_DatabaseTableDialog* ui;
};

#endif // RB_DATABASETABLEDIALOG_H
