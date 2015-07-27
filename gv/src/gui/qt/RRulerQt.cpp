#include "RDocumentInterface.h"
#include "RGraphicsView.h"
#include "RGrid.h"
#include "RRulerQt.h"
#include "RSettings.h"

#include <QPainter>

/**
 * Constructor
 */
RRulerQt::RRulerQt(QWidget* parent) :
	QFrame(parent), RRuler(), painter(NULL), lastSize(0,0), viewportChanged(false) {
    
    //setFrameStyle(QFrame::Panel|QFrame::Raised);
    //setLineWidth(1);

    /*
    RS_SETTINGS->beginGroup("/Appearance");
    fsize = RS_SETTINGS->readNumEntry("/StatusBarFontSize", fsize);
    RS_SETTINGS->endGroup();
    */

    QPalette p = palette();
    QColor bg = p.color(QPalette::Window);
    p.setColor(QPalette::Window, bg.darker(120));
    setPalette(p);
    
    setAutoFillBackground(true);

    cursorArrow.moveTo(0,0);
    cursorArrow.lineTo(-3,-3);
    cursorArrow.lineTo(3,-3);
}


/**
 * Destructor
 */
RRulerQt::~RRulerQt() {}


QSize RRulerQt::sizeHint() const {
    // approximation:
    double pixelSize = getFont().pointSize()/72.0*96.0;
    int offset = pixelSize>8.0 ? 12 : 10;
    QSize size(100, pixelSize + offset);
    if (orientation == Qt::Vertical) {
            size.transpose();
    }
    return size;
}

void RRulerQt::setOrientation(Qt::Orientation orientation){
    RRuler::setOrientation(orientation);
    if (orientation == Qt::Horizontal) {
        setFixedHeight(sizeHint().height());
    }
    else {
        setFixedWidth(sizeHint().width());
    }
}

QFont RRulerQt::getFont() const {
	return RSettings::getRulerFont();
}

void RRulerQt::paintTick(int pos, bool major, const QString& label) {
    if (painter == NULL) {
        return;
    }
    int lineLength = 3;
    if (major) {
        lineLength = 7;
    }
    if (orientation == Qt::Horizontal) {
        painter->setPen(Qt::white);
        painter->drawLine(pos + 1, height() - lineLength, pos + 1, height());
        painter->setPen(Qt::black);
        painter->drawLine(pos, height() - lineLength, pos, height());
        if (!label.isEmpty()) {
            //QFontMetrics fm(getFont());
            QRect rect(pos - 250, 0, 500, 500);
            painter->drawText(rect, Qt::AlignTop | Qt::AlignHCenter, label);
        }
    } else {
        painter->setPen(Qt::white);
        painter->drawLine(width() - lineLength, pos + 1, width(), pos + 1);
        painter->setPen(Qt::black);
        painter->drawLine(width() - lineLength, pos, width(), pos);
        if (!label.isEmpty()) {
            painter->save();
            painter->rotate(-90);
            painter->translate(-pos, 0);
            QRect rect(- 250, 0, 500, 500);
            painter->drawText(rect, Qt::AlignTop | Qt::AlignHCenter, label);
            painter->restore();
        }
    }
}
    
void RRulerQt::paintEvent(QPaintEvent* e) {
    if (orientation == Qt::Horizontal) {
        if (sizeHint().height() != lastSize.height()) {
            lastSize.setHeight(sizeHint().height());
            updateViewport();
            return;
        }
    } else {
        if (sizeHint().width() != lastSize.width()) {
            lastSize.setWidth(sizeHint().width());
            updateViewport();
            return;
        }
    }

    QFrame::paintEvent(e);
    if (view==NULL) {
        return;
    }

    RDocumentInterface* di = view->getDocumentInterface();
    if (di==NULL) {
        return;
    }

    QSize newSize = size();
    if (lastSize != newSize) {
        buffer = QImage(newSize, QImage::Format_ARGB32);
        lastSize = newSize;
        viewportChanged = true;
    }

    if (viewportChanged) {
        painter = new QPainter(&buffer);
        painter->setCompositionMode(QPainter::CompositionMode_Clear);
        painter->eraseRect(QRect(QPoint(0, 0), size()));
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter->setPen(Qt::black);
        painter->setFont(getFont());

        RGrid* grid = view->getGrid();
        if (grid == NULL) {
            return;
        }

        grid->paintRuler(*view, *this);

        delete painter;
        painter = NULL;
    }

    QPainter wPainter(this);
    wPainter.drawImage(0,0,buffer);
    //wPainter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
    //wPainter.setPen(Qt::white);
    RVector p = view->mapToView(cursorPosition);
    if (orientation==Qt::Horizontal) {
        wPainter.translate(p.x, height()-4);
        wPainter.fillPath(cursorArrow, Qt::black);
    } else {
        wPainter.rotate(-90);
        wPainter.translate(-p.y+1, width()-4);
        wPainter.fillPath(cursorArrow, Qt::black);
        //wPainter.drawLine(0, cursorPosition.y, width(), cursorPosition.y);
    }
    wPainter.end();

    viewportChanged = false;
}

void RRulerQt::updateViewport() {
    viewportChanged = true;
    if (orientation==Qt::Horizontal) {
        resize(width(), sizeHint().height());
        setMinimumHeight(sizeHint().height());
        setMaximumHeight(sizeHint().height());
    }
    else {
        resize(sizeHint().width(), height());
        setMinimumWidth(sizeHint().width());
        setMaximumWidth(sizeHint().width());
    }
    update();
}

void RRulerQt::setCoordinate(const RVector& wcsPosition, const RVector& ucsPosition) {
    cursorPosition = wcsPosition;
}
