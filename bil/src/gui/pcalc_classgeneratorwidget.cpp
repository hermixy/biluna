#include "pcalc_classgeneratorwidget.h"
#include "ui_pcalc_classgeneratorwidget.h"

PCALC_ClassGeneratorWidget::PCALC_ClassGeneratorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PCALC_ClassGeneratorWidget)
{
    ui->setupUi(this);
}

PCALC_ClassGeneratorWidget::~PCALC_ClassGeneratorWidget()
{
    delete ui;
}
