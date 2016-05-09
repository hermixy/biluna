#ifndef PCALC_CLASSGENERATORWIDGET_H
#define PCALC_CLASSGENERATORWIDGET_H

#include <QWidget>

namespace Ui {
class PCALC_ClassGeneratorWidget;
}

class PCALC_ClassGeneratorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PCALC_ClassGeneratorWidget(QWidget *parent = 0);
    ~PCALC_ClassGeneratorWidget();

private:
    Ui::PCALC_ClassGeneratorWidget *ui;
};

#endif // PCALC_CLASSGENERATORWIDGET_H
