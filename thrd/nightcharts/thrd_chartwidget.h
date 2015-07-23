#ifndef THRD_CHARTWIDGET_H
#define THRD_CHARTWIDGET_H

#include "rb_widget.h"
#include "ui_thrd_chartwidget.h"

class DB_EXPORT THRD_ChartWidget : public RB_Widget,
                private Ui::THRD_ChartWidget {

    Q_OBJECT

public:
    explicit THRD_ChartWidget(QWidget* parent = 0);
    virtual ~THRD_ChartWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "DB Simple Chart Review"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }


protected slots:
    void on_pbAddItem_clicked();
    void on_pbDeleteItem_clicked();
    void on_pbAddStackName_clicked();
    void on_pbDeleteStackName_clicked();
    void on_pbFont_clicked();
    void on_pbRefresh_clicked();

protected:
    void init();

private:
    void initChartItems();
    void addChartRow(const QString& name, const QString& color, double value);
    void addStackRow(const QString& name);

    QFont mFont;
};

#endif // THRD_CHARTWIDGET_H
