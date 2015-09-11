#ifndef RB_DATABASETABLEWIDGET_H
#define RB_DATABASETABLEWIDGET_H

#include <QWidget>

namespace Ui {
class RB_DatabaseTableWidget;
}

class RB_DatabaseTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RB_DatabaseTableWidget(QWidget *parent = 0);
    ~RB_DatabaseTableWidget();

private:
    Ui::RB_DatabaseTableWidget *ui;
};

#endif // RB_DATABASETABLEWIDGET_H
