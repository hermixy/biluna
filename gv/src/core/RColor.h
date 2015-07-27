#ifndef RCOLOR_H_
#define RCOLOR_H_

#include <QCoreApplication>
#include <QObject>
#include <QColor>
#include <QMetaType>
#include <QMap>
#include <QIcon>
#include <QDebug>

/**
 * Color. Fixed RGBA or ByLayer or ByBlock.
 *
 * \ingroup core
 * \scriptable
 * \copyable
 */
class RColor: public QColor {

    Q_DECLARE_TR_FUNCTIONS(RColor);


public:
    friend QDataStream& operator<<(QDataStream& stream, const RColor& color);

    friend QDataStream& operator>>(QDataStream& stream, RColor& color);

    enum Mode {
        ByLayer, ByBlock, Fixed, BlackWhite
    };

public:
    RColor();
    RColor(int r, int g, int b, int a = 255, RColor::Mode mode = RColor::Fixed);
    RColor(Qt::GlobalColor color, RColor::Mode mode = RColor::Fixed);
    RColor(RColor::Mode mode);
    RColor(const QString & name, RColor::Mode mode = RColor::Fixed);
    /**
     * \nonscriptable
     */
    RColor(const QColor& color, RColor::Mode mode = RColor::Fixed);


    static RColor getColor(const QString& name);
    static QList<QPair<QString, RColor> > getList(bool onlyFixed = false,
                    bool combineBw = true);
    static QIcon getIcon(const RColor& color);

    unsigned long long getHash() const;

    bool isValid() const;
    QString getName() const;
    bool isByLayer() const;
    bool isByBlock() const;
    bool isFixed() const;
    bool isBlackWhite() const;
    void setBlackWhite();
    static RColor getHighlighted(const RColor& color);

    bool equalsCorrected(const RColor & color) const;

    bool operator==(const RColor & color) const;
    bool operator!=(const RColor & color) const;
    bool operator<(const RColor & color) const;

private:
    static void init();
    static void init(const QString& cn, const RColor& c);

private:
    static bool isInitialized;
    static QList<QPair<QString, RColor> > list;
    static QMap<RColor, QIcon> iconMap;
    Mode mode;
};

/**
 *\nonscriptable
 */
QDataStream &operator<<(QDataStream &stream, const RColor &color);

/**
 *\nonscriptable
 */
QDataStream &operator>>(QDataStream &stream, RColor &color);

QDebug operator<<(QDebug dbg, const RColor& c);

Q_DECLARE_METATYPE(RColor)
Q_DECLARE_METATYPE(RColor*)
typedef QPair<QString, RColor> _RPairStringRColor;
Q_DECLARE_METATYPE(_RPairStringRColor)
Q_DECLARE_METATYPE(QList< _RPairStringRColor >)
Q_DECLARE_METATYPE(RColor::Mode)

#endif /* RCOLOR_H_ */
