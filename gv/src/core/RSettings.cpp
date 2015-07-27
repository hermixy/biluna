#include <QtDebug>
#include <QApplication>
#include <QFontMetrics>

#include "RSettings.h"
#include "RVersion.h"
#include "RDebug.h"

QMap<QString, QVariant> RSettings::cache;

const QString RSettings::BackgroundColor = "GraphicsViewColors/BackgroundColor";
const QString RSettings::GridColor = "GraphicsViewColors/GridColor";
const QString RSettings::ZoomBoxColor = "GraphicsViewColors/ZoomBoxColor";
const QString RSettings::SelectionColor = "GraphicsViewColors/SelectionColor";
const QString RSettings::SelectionBoxColor = "GraphicsViewColors/SelectionBoxColor";
const QString RSettings::SelectionBoxBackgroundColor = "GraphicsViewColors/SelectionBoxBackgroundColor";
const QString RSettings::SelectionBoxBackgroundCrossColor = "GraphicsViewColors/SelectionBoxBackgroundCrossColor";
const QString RSettings::ReferencePointColor = "GraphicsViewColors/ReferencePointColor";
const QString RSettings::CrosshairColor = "GraphicsViewColors/CrosshairColor";
const QString RSettings::AuxShapeColor = "GraphicsViewColors/AuxShapeColor";
const QString RSettings::MetaGridColor = "GraphicsViewColors/MetaGridColor";
const QString RSettings::RelativeZeroColor = "GraphicsViewColors/RelativeZeroColor";
const QString RSettings::OriginColor = "GraphicsViewColors/OriginColor";

const QString RSettings::Locale = "Language/UiLanguage";
const QString RSettings::CadToolBarButtonSize = "CadToolBar/IconSize";
const QString RSettings::GraphicsViewMargin = "GraphicsView/Margin";

QFont RSettings::rulerFont;
int RSettings::snapRange = -1;
int RSettings::showCrosshair = -1;
int RSettings::concurrentDrawing = -1;
QStringList RSettings::recentFiles;

void RSettings::addRecentFile(QString fileName) {
	if (recentFiles.contains(fileName)) {
		return;
	}
	recentFiles += fileName;
	shortenRecentFiles();
}

QStringList RSettings::getRecentFiles() {
	if (recentFiles.isEmpty()) {
		recentFiles = getValue("RecentFiles/Files", QStringList()).value<
				QStringList> ();
		return recentFiles;
	}
	shortenRecentFiles();
	return recentFiles;
}

void RSettings::shortenRecentFiles(){
	int historySize = getValue("RecentFiles/HistorySize", 5).toInt();
	while (recentFiles.length() > historySize) {
		recentFiles.removeFirst();
	}
	setValue("RecentFiles/Files", recentFiles);
}

void RSettings::removeRecentFile(QString fileName){
	recentFiles.removeAll(fileName);
	setValue("RecentFiles/Files", recentFiles);
}

void RSettings::clearRecentFiles(){
	recentFiles.clear();
	setValue("RecentFiles/Files", recentFiles);
}

void RSettings::setRulerFont(const QFont& font){
	setValue("GraphicsView/RulerFont", font);
	rulerFont = font;
}

QFont RSettings::getRulerFont() {
	static bool first = true;
	if (first) {
		// get application's default font (pixel size is -1, point size e.g. 10pt):
		QFont font;
        font.setPointSize(font.pointSize()/3.0*2.0);
		rulerFont = getValue("GraphicsView/RulerFont", font).value<QFont> ();
        qDebug() << "rulerFont: " << rulerFont.pointSize();
		first = false;
	}
	return rulerFont;
}

bool RSettings::getAutoScalePatterns() {
	return getValue("GraphicsView/AutoScalePatterns", true).toBool();
}


QString RSettings::getQtVersion() {
	return qVersion();
}

QString RSettings::getVersion() {
	return RLIBRARIES_VERSION;
}

QString RSettings::getReleaseDate() {
	return __DATE__;
}

int RSettings::getSnapRange() {
    if (snapRange==-1) {
        snapRange = getValue("GraphicsView/SnapRange", QVariant(10)).toInt();
    }
    return snapRange;
}

bool RSettings::getShowCrosshair() {
    if (showCrosshair==-1) {
        showCrosshair = getValue("GraphicsView/ShowCrosshair", 
            QVariant(true)).toBool();
    }
    return (bool)showCrosshair;
}

void RSettings::setShowCrosshair(bool on) {
    setValue("GraphicsView/ShowCrosshair", on);
    showCrosshair = on;
}

bool RSettings::getConcurrentDrawing() {
    if (concurrentDrawing==-1) {
        concurrentDrawing = getValue("GraphicsView/ConcurrentDrawing", 
            QVariant(false)).toBool();
    }
    return (bool)concurrentDrawing;
}

void RSettings::setConcurrentDrawing(bool on) {
    setValue("GraphicsView/ConcurrentDrawing", on);
    concurrentDrawing = on;
}

RColor RSettings::getColor(const QString& key, const RColor& defaultValue) {
    return getValue(key, defaultValue).value<RColor>();
}

QVariant RSettings::getValue(const QString& key, const QVariant& defaultValue) {
	if (!isInitialized()) {
		return defaultValue;
	}
	if (cache.contains(key)) {
		return cache[key];
	}

	// slow operation:
	QVariant ret = QSettings().value(key, defaultValue);
	if (defaultValue.isValid()) {
        //ret.convert(defaultValue.type());
	}
	cache[key] = ret;
	return ret;
}

void RSettings::setValue(const QString& key, const QVariant& value) {
	if (!isInitialized()) {
		return;
	}
	cache[key] = value;
	QSettings().setValue(key, value);
}

bool RSettings::isInitialized() {
	return !qApp->organizationName().isEmpty();
}
