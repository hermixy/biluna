#ifndef RB_DATABASETYPEDIALOG_H
#define RB_DATABASETYPEDIALOG_H

#include <QDialog>

namespace Ui {
class RB_DatabaseTypeDialog;
}

class RB_DatabaseTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RB_DatabaseTypeDialog(QWidget *parent = 0);
    ~RB_DatabaseTypeDialog();

private:
    Ui::RB_DatabaseTypeDialog *ui;
};

#endif // RB_DATABASETYPEDIALOG_H
