#include "thrd_chartwidget.h"

#include <QtWidgets>
#include "thrd_chart.h"


/**
 * Constructor
 */
THRD_ChartWidget::THRD_ChartWidget(QWidget* parent) :
                        RB_Widget(parent) {

    setupUi(this);

    // init(); in dialog factory
}

/**
 * Destructor
 */
THRD_ChartWidget::~THRD_ChartWidget() {
    // nothing
}

/**
 * Initialize widget
 */
void THRD_ChartWidget::init() {

    QStringList items;
    items << tr("Bar Chart")  << tr("Line") << tr("Pie 2D")
          << tr("Pie 3D") << tr("Stacked Bar Chart");
    cbChartType->addItems(items);
    cbChartType->setCurrentIndex(1); // Line

    items.clear();
    items << tr("Axes Horizontal") << tr("Axes Angled")
          << tr("Horizontal") << tr("Vertical") << tr("Round");
    cbLegendType->addItems(items);
    cbLegendType->setCurrentIndex(0); // Vertical for bar and lines

    items.clear();
    items << tr("Yes") << tr("No");
    cbShadow->addItems(items);
    cbShadow->setCurrentIndex(0);
    cbInsideValues->addItems(items);
    cbInsideValues->setCurrentIndex(0);

    leGraphX->setText("50");
    leGraphY->setText("10");
    leGraphHeight->setText("300");
    leGraphWidth->setText("300");
    leLegendX->setText("250");
    leLegendY->setText("0");

    leImageWidth->setText("500");
    leImageHeight->setText("500");

    mFont.setFamily("MS Shell Dlg 2");
    mFont.setPointSize(8);
    leFont->setText(mFont.family());
    leFont->setEnabled(false);
    leStartAngle->setText("0");
    leLabelBefore->setText("");
    leLabelAfter->setText("%");
    leLengthYaxis->setText("100");

    initChartItems();

    readSettings();
}

/**
 * Initialize chart items in tableWidget
 */
void THRD_ChartWidget::initChartItems() {
    // Only for shorthand
    QTableWidget* tw = twItems;

    tw->clear();
    tw->setEditTriggers(QAbstractItemView::DoubleClicked
                        | QAbstractItemView::EditKeyPressed);
    tw->setSelectionMode(QAbstractItemView::SingleSelection);
    tw->setColumnCount(3);
    tw->verticalHeader()->setDefaultSectionSize(20); // row height

    QTableWidgetItem* headerItem = new QTableWidgetItem(tr("Name"));
    tw->setHorizontalHeaderItem(0, headerItem);
    headerItem = new QTableWidgetItem(tr("Color"));
    tw->setHorizontalHeaderItem(1, headerItem);
    headerItem = new QTableWidgetItem(tr("Value"));
    tw->setHorizontalHeaderItem(2, headerItem);

    addChartRow("Item1", "#cc0a31", 34.0);
    addChartRow("Item2", "green", 27.0);
    addChartRow("Item3", "cyan", 14.0);
    addChartRow("Item4", "yellow", 9.0);
    addChartRow("Item5", "blue", 8.0);
    addChartRow("Item6", "orangered", 7.0);
    addChartRow("Item7", "pink", 5.0);
    addChartRow("Item8", "#66ffee", 3.0);
    addChartRow("Item9", "#ffee66", 2.0);

    tw = twStacks;

    tw->clear();
    tw->setEditTriggers(QAbstractItemView::DoubleClicked
                        | QAbstractItemView::EditKeyPressed);
    tw->setSelectionMode(QAbstractItemView::SingleSelection);
    tw->setColumnCount(1);
    tw->verticalHeader()->setDefaultSectionSize(20); // row height

    headerItem = new QTableWidgetItem(tr("Name"));
    tw->setHorizontalHeaderItem(0, headerItem);

    addStackRow("Stack1");
    addStackRow("Stack2");
    addStackRow("Stack3");
}

/**
 * Add a chart row
 * @param name name of the chart item
 * @param color color of the chart item in RRGGBB format
 * @param value the value of the chart item
 */
void THRD_ChartWidget::addChartRow(const QString &name,
                                        const QString &color,
                                        double value) {
    // Only for shorthand
    QTableWidget* tw = twItems;
    int row = tw->rowCount();
    int rowCount = row + 1;
    tw->setRowCount(rowCount);

    QTableWidgetItem* twItem = NULL;
    twItem = new QTableWidgetItem(name);
    tw->setItem(row, 0, twItem);

    twItem = new QTableWidgetItem(color);
    tw->setItem(row, 1, twItem);

    twItem = new QTableWidgetItem(QString::number(value));
    twItem->setTextAlignment(Qt::AlignRight);
    // twItem->setFlags(!Qt::ItemIsEnabled);
    tw->setItem(row, 2, twItem);
}

/**
 * Add a stack name
 * @param name name of the stack
 */
void THRD_ChartWidget::addStackRow(const QString& name) {
    // Only for shorthand
    QTableWidget* tw = twStacks;
    int row = tw->rowCount();
    int rowCount = row + 1;
    tw->setRowCount(rowCount);

    QTableWidgetItem* twItem = NULL;
    twItem = new QTableWidgetItem(name);
    tw->setItem(row, 0, twItem);
}

/**
 * Push button Add item clicked
 */
void THRD_ChartWidget::on_pbAddItem_clicked() {
    addChartRow(tr("<New>"), "#999999", 0.0);
}

/**
 * Push button Delete item clicked
 */
void THRD_ChartWidget::on_pbDeleteItem_clicked() {
    int row = twItems->currentRow();

    if (row >=0 && row < twItems->rowCount()) {
        twItems->removeRow(row);
    }
}

/**
 * Push button Add stack name clicked
 */
void THRD_ChartWidget::on_pbAddStackName_clicked() {
    addStackRow(tr("<New>"));
}

/**
 * Push button Delete stack name clicked
 */
void THRD_ChartWidget::on_pbDeleteStackName_clicked() {
    int row = twStacks->currentRow();

    if (row >=0 && row < twStacks->rowCount()) {
        twStacks->removeRow(row);
    }
}

/**
 * Pushbutton Font clicked
 */
void THRD_ChartWidget::on_pbFont_clicked() {
    QFontDialog dlg;
    dlg.setCurrentFont(mFont);

    if (QDialog::Accepted == dlg.exec()) {
        mFont = dlg.selectedFont();
        leFont->setText(mFont.family());
    }
}

/**
 * Pushbutton Refresh NightCharts clicked
 */
void THRD_ChartWidget::on_pbRefresh_clicked() {
    QTextEdit* te = textEdit;

    te->setHtml("<p>This is testing of 'NightCharts' chart:</p>"
                "<p align=\"center\"></p>");

    THRD_Chart chart;

    switch (cbChartType->currentIndex()) {
    case 0 :
        chart.setChartType(THRD_Chart::Bar);
        break;
    case 1 :
        chart.setChartType(THRD_Chart::Line);
        break;
    case 2 :
        chart.setChartType(THRD_Chart::Pie2D);
        break;
    case 3 :
        chart.setChartType(THRD_Chart::Pie3D);
        break;
    case 4 :
        chart.setChartType(THRD_Chart::StackedBar);
        break;
    default :
        chart.setChartType(THRD_Chart::Bar);
        break;
    }

    switch (cbLegendType->currentIndex()) {
    case 0 :
        chart.setLegendType(THRD_Chart::AxesHoriz);
        break;
    case 1 :
        chart.setLegendType(THRD_Chart::AxesAngled);
        break;
    case 2 :
        chart.setLegendType(THRD_Chart::Horizontal);
        break;
    case 3 :
        chart.setLegendType(THRD_Chart::Vertical);
        break;
    case 4 :
        chart.setLegendType(THRD_Chart::Round);
        break;
    default :
        chart.setLegendType(THRD_Chart::Vertical);
        break;
    }

    switch (cbShadow->currentIndex()) {
    case 0 :
        chart.setShadow(true);
        break;
    case 1 :
        chart.setShadow(false);
        break;
    default :
        chart.setShadow(true);
        break;
    }

    switch (cbInsideValues->currentIndex()) {
    case 0 :
        chart.setValueInChart(true);
        break;
    case 1 :
        chart.setValueInChart(false);
        break;
    default :
        chart.setValueInChart(true);
        break;
    }

    chart.setChartCoords(leGraphX->text().toDouble(),
                    leGraphY->text().toDouble(),
                    leGraphWidth->text().toDouble(),
                    leGraphHeight->text().toDouble());
    chart.setLegendCoords(leLegendX->text().toDouble(),
                          leLegendY->text().toDouble());
    chart.setPieStartAngle(leStartAngle->text().toDouble());
    chart.setLabelBefore(leLabelBefore->text());
    chart.setLabelAfter(leLabelAfter->text());
    chart.setLengthYaxis(leLengthYaxis->text().toDouble());
    chart.setFont(mFont);

    QString name;
    QColor color;
    double value;

    for (int row = 0; row < twItems->rowCount(); ++row) {
        QTableWidgetItem* twi = NULL;
        twi = twItems->item(row, 0);
        name = twi->text();
        twi = twItems->item(row, 1);
        color.setNamedColor(twi->text());
        twi = twItems->item(row, 2);
        value = twi->text().toDouble();

        chart.addChartItem(name, color, value);
    }

    for (int row = 0; row < twStacks->rowCount(); ++row) {
        QTableWidgetItem* twi = NULL;
        twi = twStacks->item(row, 0);
        name = twi->text();

        chart.addStackName(name);
    }

    QImage img(QSize(leImageWidth->text().toInt(),
                     leImageHeight->text().toInt()),
               QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);

    chart.draw(&painter);
    chart.drawLegend(&painter);

    // Insert image
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertImage(img);

}
