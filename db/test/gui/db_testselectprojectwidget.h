#ifndef DB_TESTSELECTPROJECTWIDGET_H
#define DB_TESTSELECTPROJECTWIDGET_H

#include "db_tablewidget.h"

class DB_TestSelectProjectWidget : public DB_TableWidget {

    Q_OBJECT

public:
    explicit DB_TestSelectProjectWidget(QWidget *parent = 0);
    ~DB_TestSelectProjectWidget();

    RB_String getName() const { return "DB_TestSelectProjectWidget"; }
    RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }
    void init();

private:

};

#endif // DB_TESTSELECTPROJECTWIDGET_H
