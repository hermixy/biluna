#include <QPainter>
#include <QPixmap>
#include <QBrush>

#include "RColor.h"
#include "RDebug.h"

QList<QPair<QString, RColor> > RColor::list;
QMap<RColor, QIcon> RColor::iconMap;
bool RColor::isInitialized = false;

/**
 * Constructs an invalid color with the fixed RGB value (0, 0, 0).
 */
RColor::RColor() :
    QColor(), mode(RColor::Fixed) {
}

RColor::RColor(const QColor& color, RColor::Mode mode) :
    QColor(color), mode(mode) {
}

RColor::RColor(int r, int g, int b, int a, RColor::Mode mode) :
    QColor(r, g, b, a), mode(mode) {
}

RColor::RColor(Qt::GlobalColor color, RColor::Mode mode) :
    QColor(color), mode(mode) {
}

RColor::RColor(RColor::Mode mode) :
    QColor(), mode(mode) {
}

RColor::RColor(const QString & name, RColor::Mode mode) :
    QColor(name), mode(mode) {
}

bool RColor::equalsCorrected(const RColor & color) const {
    if (isBlackWhite() && color.isBlackWhite()) {
        return true;
    }
    if (isFixed() && color.isBlackWhite()) {
        return QColor::operator==(Qt::black) || QColor::operator==(Qt::white);
    }
    if (isBlackWhite() && color.isFixed()) {
        return color.QColor::operator==(Qt::black)
                || color.QColor::operator==(Qt::white);
    }
    return operator==(color);
}

bool RColor::operator==(const RColor & color) const {
    if (mode == RColor::ByLayer) {
        return color.mode == RColor::ByLayer;
    }
    if (mode == RColor::ByBlock) {
        return color.mode == RColor::ByBlock;
    }
    return mode == color.mode && QColor::operator==(color);
}

bool RColor::operator!=(const RColor & color) const {
	return !operator==(color);
}

bool RColor::operator<(const RColor & color) const {
	return getHash() < color.getHash();
}

unsigned long long RColor::getHash() const {
	return ((unsigned long long)rgba())
			+ (((unsigned long long)mode) << (4 * 8))
			+ (((unsigned long long)isValid()) << (4 * 8 + 4));
}

/**
 * \return Highlighted color for the given color.
 */
RColor RColor::getHighlighted(const RColor& clr) {
	if (!clr.isValid()) {
		return Qt::gray;
	}

	if (clr.value() < 192) {
		// entity color is dark
		if (clr == Qt::black) {
			return Qt::gray;
		}
		return clr.lighter(200);
	}

	if (clr == Qt::white) {
		return Qt::gray;
	}

	// entity color is bright
	return clr.darker(200);
}

bool RColor::isValid() const {
	if (mode == RColor::ByBlock || mode == RColor::ByLayer) {
		return true;
	}
	return QColor::isValid();
}

bool RColor::isByLayer() const {
	return mode == RColor::ByLayer;
}

bool RColor::isByBlock() const {
	return mode == RColor::ByBlock;
}

bool RColor::isFixed() const {
	return mode == RColor::Fixed;
}

bool RColor::isBlackWhite() const {
	return mode == RColor::BlackWhite;
}

void RColor::setBlackWhite() {
    mode = RColor::BlackWhite;
}

/**
 * \return Name of the color if known, otherwise "#RRGGBB".
 */
QString RColor::getName() const {

	init();

	QListIterator<QPair<QString, RColor> > it(list);
	while (it.hasNext()) {
		QPair<QString, RColor> p = it.next();
		if (p.second == *this) {
			return p.first;
		}
	}

	return QColor::name();
}

/**
 * \return List of known colors.
 *
 * \param onlyFixed Only fixed colors, not ByLayer, ByBlock, ...
 * \param combineBw Combine Black and white into one color.
 */
QList<QPair<QString, RColor> > RColor::getList(bool onlyFixed, bool combineBw) {
	init();
	QList<QPair<QString, RColor> > l = list;
	if (combineBw) {
		QString name = RColor(Qt::black).getName();
		l.removeAll(QPair<QString, RColor> (name, Qt::black));
		name = RColor(Qt::white).getName();
		l.removeAll(QPair<QString, RColor> (name, Qt::white));
	} else {
		QString name = RColor(Qt::black, RColor::BlackWhite).getName();
		l.removeAll(QPair<QString, RColor> (name, RColor(Qt::black,
				RColor::BlackWhite)));
	}
	if (!onlyFixed) {
		return l;
	}

	// remove "By Layer"
	QString name = RColor(RColor::ByLayer).getName();
	l.removeAll(QPair<QString, RColor> (name, RColor(RColor::ByLayer)));

	// remove "By Block"
	name = RColor(RColor::ByBlock).getName();
	l.removeAll(QPair<QString, RColor> (name, RColor(RColor::ByBlock)));

	return l;
}

RColor RColor::getColor(const QString& name) {
	init();

	QListIterator<QPair<QString, RColor> > it(list);
	while (it.hasNext()) {
		QPair<QString, RColor> p = it.next();
		if (p.first == name) {
			return p.second;
		}
	}

	return RColor(name);
}

void RColor::init() {
    if (!isInitialized) {
        isInitialized = true;
        init(tr("By Layer"), RColor(RColor::ByLayer));
        init(tr("By Block"), RColor(RColor::ByBlock));
        init(tr("Red"), RColor(Qt::red));
        init(tr("Yellow"), RColor(Qt::yellow));
        init(tr("Green"), RColor(Qt::green));
        init(tr("Cyan"), RColor(Qt::cyan));
        init(tr("Blue"), RColor(Qt::blue));
        init(tr("Magenta"), RColor(Qt::magenta));
        init(tr("Black / White"), RColor(Qt::black, RColor::BlackWhite));
        init(tr("Black"), RColor(Qt::black));
        init(tr("White"), RColor(Qt::white));
        init(tr("Gray"), RColor(Qt::gray));
        init(tr("Light Gray"), RColor(Qt::lightGray));
        init(tr("Others..."), RColor());
    }
}

void RColor::init(const QString& cn, const RColor& c) {
    list.append(QPair<QString, RColor> (cn, c));
    iconMap.insert(c, getIcon(c));
}


QIcon RColor::getIcon(const RColor& color) {
    init();

    if (iconMap.contains(color)) {
            return iconMap[color];
    }

    RColor col = color;
    if (color.isByLayer() || color.isByBlock() || color == RColor(Qt::black,
                    RColor::BlackWhite) || !color.isValid()) {
            col = Qt::white;
    }
    QImage img(32, 10, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);
    QPainter painter(&img);
    int w = img.width();
    int h = img.height();
    //	painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(0, 0, w, h, col);
    if (!color.isValid()) {
        // icon for "Other colors..."
        QLinearGradient grad(0, 0, w, 0);
        grad.setColorAt(0, Qt::red);
        grad.setColorAt(0.33, Qt::yellow);
        grad.setColorAt(0.66, Qt::blue);
        grad.setColorAt(1, Qt::green);
        painter.fillRect(QRect(0, 0, w, h), grad);
    } else if (color == RColor(Qt::black, RColor::BlackWhite)
               || color == RColor(Qt::white, RColor::BlackWhite)) {
        // icon for black / white
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.moveTo(0, 0);
        path.lineTo(w, 0);
        path.lineTo(w, h);
        painter.fillPath(path, Qt::black);
        painter.setRenderHint(QPainter::Antialiasing, false);
    } else if (col.alpha() != 255) {
        // indicate alpha by an inset
        QBrush opaqueBrush = col;
        col.setAlpha(255);
        opaqueBrush.setColor(col);
        painter.fillRect(w / 4, h / 4, w / 2, h / 2, opaqueBrush);
    }
    painter.setPen(Qt::black);
    painter.drawRect(0, 0, w - 1, h - 1);
    painter.end();
    return QIcon(QPixmap::fromImage(img));
}

/**
 * Stream operator for QDebug
 */
QDebug operator<<(QDebug dbg, const RColor& c) {
    if (c.isValid()) {
        if (c.isByLayer()) {
            dbg.nospace() << "RColor(byLayer)";
        } else if (c.isByBlock()) {
            dbg.nospace() << "RColor(byBlock)";
        } else {
            dbg.nospace() << "RColor(" << c.alpha() << "," << c.red() << ", "
                            << c.green() << ", " << c.blue() << ")";
        }
    } else {
        dbg.nospace() << "RColor(invalid)";
    }
    return dbg.space();
}

QDataStream& operator<<(QDataStream& stream, const RColor& color) {
    stream << (const QColor&) color;
    stream << (quint16) color.mode;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, RColor& color) {
    stream >> (QColor&) color;
    quint16 mode;
    stream >> mode;
    color.mode = (RColor::Mode) mode;
    return stream;
}
