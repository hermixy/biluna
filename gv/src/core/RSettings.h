#ifndef RSETTINGS_H
#define RSETTINGS_H

#include <QColor>
#include <QSettings>
#include <QFont>

#include "RColor.h"

// workaround for src2srcml bug:
#define invColor RColor()
#define invVariant QVariant()

/**
 * \ingroup core
 * \scriptable
 */
class RSettings {
public:
    // GraphicsView Appearance Fonts
    static void setRulerFont(const QFont& font);
    static QFont getRulerFont();

    static bool getAutoScalePatterns();
    static QString getQtVersion();
    static QString getReleaseDate();
    static QString getVersion();
    static int getSnapRange();
    static bool getShowCrosshair();
    static void setShowCrosshair(bool on);
    static bool getConcurrentDrawing();
    static void setConcurrentDrawing(bool on);

    static void addRecentFile(QString fileName);
    static void removeRecentFile(QString fileName);
    static QStringList getRecentFiles();
    static void clearRecentFiles();

    static RColor getColor(const QString& key, const RColor& defaultValue=invColor);

    static QVariant getValue(const QString& key, const QVariant& defaultValue=invVariant);
    static void setValue(const QString& key, const QVariant& value);

public:
    // GraphicsView Appearance Colors
    static const QString BackgroundColor;
    static const QString GridColor;
    static const QString ZoomBoxColor;
    static const QString SelectionColor;
    static const QString SelectionBoxColor;
    static const QString SelectionBoxBackgroundColor;
    static const QString SelectionBoxBackgroundCrossColor;
    static const QString ReferencePointColor;
    static const QString CrosshairColor;
    static const QString AuxShapeColor;
    static const QString MetaGridColor;
    static const QString RelativeZeroColor;
    static const QString OriginColor;

    static const QString Locale;
    static const QString CadToolBarButtonSize;
    static const QString GraphicsViewMargin;

private:
    static bool isInitialized();
    static void shortenRecentFiles();

private:
    static QMap<QString, QVariant> cache;
    static QFont rulerFont;
    static int snapRange;
    static int showCrosshair;
    static int concurrentDrawing;
    static QStringList recentFiles;
};

Q_DECLARE_METATYPE(RSettings*)

#endif
