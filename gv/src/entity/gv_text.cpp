/****************************************************************************
**
** Copyright (C) 2001-2007 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you. Updated for Biluna 2016-08-11 Updated for Biluna 2016-08-11
**
**********************************************************************/


#include "gv_text.h"

#include "gv_fontlist.h"
#include "gv_insert.h"


const char* GV_Text::propertyInsertionPointX = QT_TRANSLATE_NOOP("QObject", "Position|X");
const char* GV_Text::propertyInsertionPointY = QT_TRANSLATE_NOOP("QObject", "Position|Y");
const char* GV_Text::propertyHeight = QT_TRANSLATE_NOOP("QObject", "Height");
const char* GV_Text::propertyVAlign = QT_TRANSLATE_NOOP("QObject", "Alignment|Vertical");
const char* GV_Text::propertyHAlign = QT_TRANSLATE_NOOP("QObject", "Alignment|Horizontal");
const char* GV_Text::propertyLineSpacingFactor = QT_TRANSLATE_NOOP("QObject", "Line Spacing Factor");
const char* GV_Text::propertyText = QT_TRANSLATE_NOOP("QObject", "Text");
const char* GV_Text::propertyStyle = QT_TRANSLATE_NOOP("QObject", "Style");
const char* GV_Text::propertyAngle = QT_TRANSLATE_NOOP("QObject", "Angle");


/**
 * Constructor.
 */
GV_Text::GV_Text(GV_EntityContainer* parentContainer,
                 const GV_TextData& d)
        : GV_Entity(parentContainer), GV_EntityContainer(parentContainer), data(d) {

    usedTextHeight = 0.0;
    usedTextWidth = 0.0;
    setText(data.text);
}



/**
 * Sets a new text. The entities representing the 
 * text are updated.
 */
void GV_Text::setText(const QString& t) {
    data.text = t;

    // handle some special flags embedded in the text:
    if (data.text.left(4)=="\\A0;") {
        data.text = data.text.mid(4);
        data.valign = GV2::VAlignBottom;
    } else if (data.text.left(4)=="\\A1;") {
        data.text = data.text.mid(4);
        data.valign = GV2::VAlignMiddle;
    } else if (data.text.left(4)=="\\A2;") {
        data.text = data.text.mid(4);
        data.valign = GV2::VAlignTop;
    }

    if (data.updateMode==GV2::Update) {
        update();
    }
}



/**
 * Gets the alignment as an int.
 *
 * @return  1: top left ... 9: bottom right
 */
int GV_Text::getAlignment() {
    if (data.valign==GV2::VAlignTop) {
        if (data.halign==GV2::HAlignLeft) {
            return 1;
        } else if (data.halign==GV2::HAlignCenter) {
            return 2;
        } else if (data.halign==GV2::HAlignRight) {
            return 3;
        }
    } else if (data.valign==GV2::VAlignMiddle) {
        if (data.halign==GV2::HAlignLeft) {
            return 4;
        } else if (data.halign==GV2::HAlignCenter) {
            return 5;
        } else if (data.halign==GV2::HAlignRight) {
            return 6;
        }
    } else if (data.valign==GV2::VAlignBottom) {
        if (data.halign==GV2::HAlignLeft) {
            return 7;
        } else if (data.halign==GV2::HAlignCenter) {
            return 8;
        } else if (data.halign==GV2::HAlignRight) {
            return 9;
        }
    }

    return 1;
}



/**
 * Sets the alignment from an int.
 *
 * @param a 1: top left ... 9: bottom right
 */
void GV_Text::setAlignment(int a) {
    switch (a%3) {
    default:
    case 1:
        data.halign = GV2::HAlignLeft;
        break;
    case 2:
        data.halign = GV2::HAlignCenter;
        break;
    case 0:
        data.halign = GV2::HAlignRight;
        break;
    }

    switch ((int)ceil(a/3.0)) {
    default:
    case 1:
        data.valign = GV2::VAlignTop;
        break;
    case 2:
        data.valign = GV2::VAlignMiddle;
        break;
    case 3:
        data.valign = GV2::VAlignBottom;
        break;
    }

}



/**
 * @return Number of lines in this text entity. 
 */
int GV_Text::getNumberOfLines() {
    int c=1;

    for (int i=0; i<(int)data.text.length(); ++i) {
        if (data.text.at(i).unicode()==0x0A) {
            c++;
        }
    }

    return c;
}



GV_Vector GV_Text::getNearestSelectedRef(const GV_Vector& coord,
        double* dist,
        GV2::RefMode refMode) {

    return GV_Entity::getNearestSelectedRef(coord, dist, refMode);
}



void GV_Text::calculateBorders(bool /*visibleOnly*/) {
    GV_EntityContainer::calculateBorders();

    if (minV.distanceTo(maxV)<1.0e-4) {
        minV = maxV = data.insertionPoint;
    }
}



/**
 * Updates the Inserts (letters) of this text. Called when the 
 * text or it's data, position, alignment, .. changes.
 * This method also updates the usedTextWidth / usedTextHeight property.
 */
void GV_Text::update() {

    RB_DEBUG->print("GV_Text::update");

    clear();

    if (isUndone()) {
        RB_DEBUG->print("GV_Text::update: text is in undo buffer. aborting");
        return;
    }
    
    lineInsertionPoints.clear();

//    RB_DEBUG->print("GV_Text::update: %d lines", getNumberOfLines());

    usedTextWidth = 0.0;
    usedTextHeight = 0.0;

    GV_Font* font = GV_FONTLIST->requestFont(data.style);

    if (font==NULL) {
//        RB_DEBUG->print("GV_Text::update: font '%s' not found. aborting",
//            (const char*)data.style.toLatin1());
        return;
    }

    GV_Vector letterPos = GV_Vector(0.0, -9.0);
    GV_Vector letterSpace = GV_Vector(font->getLetterSpacing(), 0.0);
    GV_Vector space = GV_Vector(font->getWordSpacing(), 0.0);
    int lineCounter = 0;

    // Every single text line gets stored in this entity container
    //  so we can move the whole line around easely:
    GV_EntityContainer* oneLine = new GV_EntityContainer(this);
    /*
    if (isSelected()) {
        oneLine->setSelected(isSelected());
    }
    */

    // First every text line is created with
    //   alignement: top left
    //   angle: 0
    //   height: 9.0
    // Rotation, scaling and centering is done later

    // For every letter:
    for (int i=0; i<(int)data.text.length(); ++i) {
        switch (data.text.at(i).unicode()) {
        case 0x0A:
            // line feed:
            updateAddLine(oneLine, lineCounter++);
            oneLine = new GV_EntityContainer(this);
            letterPos = GV_Vector(0.0, -9.0);
            break;

        case 0x20:
            // Space:
            letterPos+=space;
            break;

        case 0x5C: {
                // code (e.g. \S, \P, ..)
                i++;
                int ch = data.text.at(i).unicode();
                switch (ch) {
                case 'P':
                    updateAddLine(oneLine, lineCounter++);
                    oneLine = new GV_EntityContainer(this);
                    letterPos = GV_Vector(0.0, -9.0);
                    break;

                case 'S': {
                        QString up;
                        QString dw;

                        // get upper string:
                        i++;
                        while (data.text.at(i).unicode()!='^' &&
                               //data.text.at(i).unicode()!='/' &&
                               data.text.at(i).unicode()!='\\' &&
                               //data.text.at(i).unicode()!='#' &&
                                i<(int)data.text.length()) {
                            up += data.text.at(i);
                            i++;
                        }
                        
                        i++;
                        
                        if (data.text.at(i-1).unicode()=='^' &&
                             data.text.at(i).unicode()==' ') {
                            i++;
                        }
                        
                        // get lower string:
                        while (data.text.at(i).unicode()!=';' &&
                                i<(int)data.text.length()) {
                            dw += data.text.at(i);
                            i++;
                        }

                        // add texts:
                        GV_Text* upper =
                            new GV_Text(
                                oneLine,
                                GV_TextData(letterPos + GV_Vector(0.0,9.0),
                                            4.0, 100.0, GV2::VAlignTop, 
                                            GV2::HAlignLeft,
                                            GV2::LeftToRight, GV2::Exact,
                                            1.0, up, data.style,
                                            0.0, GV2::Update));
                        upper->setLayer(NULL);
                        upper->setPen(GV_Pen(GV2::FlagInvalid));
                        oneLine->addEntity(upper);

                        GV_Text* lower =
                            new GV_Text(
                                oneLine,
                                GV_TextData(letterPos+GV_Vector(0.0,4.0),
                                            4.0, 100.0, GV2::VAlignTop, 
                                            GV2::HAlignLeft,
                                            GV2::LeftToRight, GV2::Exact,
                                            1.0, dw, data.style,
                                            0.0, GV2::Update));
                        lower->setLayer(NULL);
                        lower->setPen(GV_Pen(GV2::FlagInvalid));
                        oneLine->addEntity(lower);

                        // move cursor:
                        upper->calculateBorders();
                        lower->calculateBorders();

                        double w1 = upper->getSize().x;
                        double w2 = lower->getSize().x;

                        if (w1>w2) {
                            letterPos += GV_Vector(w1, 0.0);
                        } else {
                            letterPos += GV_Vector(w2, 0.0);
                        }
                        letterPos += letterSpace;
                    }
                    break;

                default:
                    break;
                }
            }
            break;

        default: {
                RB_DEBUG->print("GV_Text::update: insert letter");
                QChar chr = data.text.at(i);
                GV_Block* letter = font->findLetter(QString(chr));

                // since 2.1.2.5: show ? for unknown letters:
                if (letter==NULL) {
                    chr = '?';
                    letter = font->findLetter(QString(chr));
                }

                // One Letter:
                if (letter!=NULL) {
                
//                    RB_DEBUG->print("GV_Text::update: insert "
//                      "letter '%s' at pos: %f/%f",
//                      (const char*)QString(chr).toLatin1(),
//                      letterPos.x, letterPos.y);

                    GV_InsertData d;
					d = GV_InsertData(QString(chr), 
								    letterPos,
                                    GV_Vector(1.0, 1.0),
                                    0.0,
                                    1,1, GV_Vector(0.0,0.0),
                                    font->getLetterList(), GV2::NoUpdate);
                    
                    GV_Insert* letter = new GV_Insert(this, d);
                    GV_Vector letterWidth;
                    letter->setPen(GV_Pen(GV2::FlagInvalid));
                    letter->setLayer(NULL);
                    letter->update();
                    letter->calculateBorders();
                    
                    // until 2.0.4.5:
                    //letterWidth = GV_Vector(letter->getSize().x, 0.0);
                    // from 2.0.4.6:
                    letterWidth = GV_Vector(letter->getMax().x-letterPos.x, 0.0);
                    
                    oneLine->addEntity(letter);

                    // next letter position:
                    letterPos += letterWidth;
                    letterPos += letterSpace;
                }
            }
            break;
        }
    }

    updateAddLine(oneLine, lineCounter);
    usedTextHeight -= data.height*data.lineSpacingFactor*1.6
                      - data.height;
    calculateBorders();
    
    RB_DEBUG->print("GV_Text::update: OK");
}



/**
 * Used internally by update() to add a text line created with 
 * default values and alignment to this text container.
 *
 * @param textLine The text line.
 * @param lineCounter Line number.
 */
void GV_Text::updateAddLine(GV_EntityContainer* textLine, int lineCounter) {
//    RB_DEBUG->print("GV_Text::updateAddLine: width: %f", textLine->getSize().x);

    textLine->calculateBorders();
    GV_Vector textSize = textLine->getSize();
    
//    RB_DEBUG->print("GV_Text::updateAddLine: width 2: %f", textSize.x);

    // Horizontal Align:
    switch (data.halign) {
    case GV2::HAlignCenter:
//        RB_DEBUG->print("GV_Text::updateAddLine: move by: %f", -textSize.x/2.0);
        textLine->move(GV_Vector(-textSize.x/2.0, 0.0));
        break;

    case GV2::HAlignRight:
        textLine->move(GV_Vector(-textSize.x, 0.0));
        break;

    default:
        break;
    }
    
    // temporary point that will in the end indicate the insertion point 
    //  of this line:
    GV_Point* ref = new GV_Point(textLine, GV_PointData(GV_Vector(0.0,0.0)));
    textLine->addEntity(ref);

    // Vertical Align:
    double vSize = getNumberOfLines()*9.0*data.lineSpacingFactor*1.6
                   - (9.0*data.lineSpacingFactor*1.6 - 9.0);

    switch (data.valign) {
    case GV2::VAlignMiddle:
        textLine->move(GV_Vector(0.0, vSize/2.0));
        break;

    case GV2::VAlignBottom:
        textLine->move(GV_Vector(0.0, vSize));
        break;

    default:
        break;
    }
    
    // Move to correct line position:
    textLine->move(GV_Vector(0.0, -9.0 * lineCounter
                             * data.lineSpacingFactor * 1.6));

    // Scale:
    textLine->scale(GV_Vector(0.0,0.0),
                    GV_Vector(data.height/9.0, data.height/9.0));

    textLine->calculateBorders();

    // Update actual text size (before rotating, after scaling!):
    if (textLine->getSize().x>usedTextWidth) {
        usedTextWidth = textLine->getSize().x;
    }

    usedTextHeight += data.height*data.lineSpacingFactor*1.6;

    // Rotate:
    textLine->rotate(GV_Vector(0.0,0.0), data.angle);

    // Move:
    textLine->move(data.insertionPoint);
    textLine->setPen(GV_Pen(GV2::FlagInvalid));
    textLine->setLayer(NULL);
    textLine->calculateBorders();
    
    GV_Vector refPos = ref->getPos();
    textLine->removeEntity(ref);

    lineInsertionPoints.set(lineCounter, refPos);
    
//    RB_DEBUG->print("GV_Text::updateAddLine: line at: %f/%f ", refPos.x, refPos.y);

    addEntity(textLine);
}



GV_VectorList GV_Text::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret(data.insertionPoint);
    return ret;
}

    
    
GV_Vector GV_Text::getNearestRef(const GV_Vector& coord,
                                     double* dist,
                                     GV2::RefMode refMode) {

    return GV_Entity::getNearestRef(coord, dist, refMode);
}


GV_VectorList GV_Text::getLineInsertionPoints() {
    return lineInsertionPoints;
}


void GV_Text::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    if (ref.distanceTo(data.insertionPoint)<1.0e-4) {
        move(offset);
    }
}


void GV_Text::move(const GV_Vector& offset) {
    data.insertionPoint.move(offset);
    GV_EntityContainer::move(offset);
    lineInsertionPoints.move(offset);
}



void GV_Text::rotate(const GV_Vector& center, double angle) {
    data.insertionPoint.rotate(center, angle);
    data.angle = GV_Math::correctAngle(data.angle+angle);
    update();
}



void GV_Text::scale(const GV_Vector& center, const GV_Vector& factor) {
    data.insertionPoint.scale(center, factor);
    data.width*=factor.x;
    data.height*=factor.x;
    update();
}



void GV_Text::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    data.insertionPoint.mirror(axisPoint1, axisPoint2);
    bool readable = GV_Math::isAngleReadable(data.angle);

    GV_Vector vec;
    vec.setPolar(1.0, data.angle);
    vec.mirror(GV_Vector(0.0,0.0), axisPoint2-axisPoint1);
    data.angle = vec.angle();

    bool corr;
    data.angle = GV_Math::makeAngleReadable(data.angle, readable, &corr);

    if (corr) {
        if (data.halign==GV2::HAlignLeft) {
            data.halign=GV2::HAlignRight;
        } else if (data.halign==GV2::HAlignRight) {
            data.halign=GV2::HAlignLeft;
        }
    } else {
        if (data.valign==GV2::VAlignTop) {
            data.valign=GV2::VAlignBottom;
        } else if (data.valign==GV2::VAlignBottom) {
            data.valign=GV2::VAlignTop;
        }
    }
    update();
}



bool GV_Text::hasEndpointsWithinWindow(const GV_Vector& /*v1*/, const GV_Vector& /*v2*/) {
    return false;
}



/**
 * Implementations must stretch the given range of the entity 
 * by the given offset.
 */
void GV_Text::stretch(const GV_Vector& firstCorner,
                      const GV_Vector& secondCorner,
                      const GV_Vector& offset) {

    if (getMin().isInWindow(firstCorner, secondCorner) &&
            getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    }
}



QVariant GV_Text::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_TEXT_INSERTIONPOINT_X) {
        return getInsertionPoint().x;
    } else if (name==GV_TEXT_INSERTIONPOINT_Y) {
        return getInsertionPoint().y;
    } else if (name==GV_TEXT_HEIGHT) {
        return getHeight();
    } else if (name==GV_TEXT_VALIGN) {
        return vAlignToText(getVAlign());
    } else if (name==GV_TEXT_HALIGN) {
        return hAlignToText(getHAlign());
    } else if (name==GV_TEXT_LINESPACINGFACTOR) {
        return getLineSpacingFactor();
    } else if (name==GV_TEXT_TEXT) {
        return getText();
    } else if (name==GV_TEXT_STYLE) {
        return getStyle();
    } else if (name==GV_TEXT_ANGLE) {
        return GV_Math::rad2deg(getAngle());
    } else {
        return GV_EntityContainer::getProperty(name, def);
    }
}



QStringList GV_Text::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_EntityContainer::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_TEXT_INSERTIONPOINT_X;
        ret << GV_TEXT_INSERTIONPOINT_Y;
        ret << GV_TEXT_HEIGHT;
        ret << GV_TEXT_VALIGN;
        ret << GV_TEXT_HALIGN;
        ret << GV_TEXT_LINESPACINGFACTOR;
        if (!data.text.contains('\n')) {
            ret << GV_TEXT_TEXT;
        }
        ret << GV_TEXT_STYLE;
        ret << GV_TEXT_ANGLE;
    }
    return ret;
}


void GV_Text::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_TEXT_INSERTIONPOINT_X) {
        data.insertionPoint.x = GV_Math::eval(value.toString());
    } else if (name==GV_TEXT_INSERTIONPOINT_Y) {
        data.insertionPoint.y = GV_Math::eval(value.toString());
    } else if (name==GV_TEXT_HEIGHT) {
        setHeight(GV_Math::eval(value.toString()));
    } else if (name==GV_TEXT_VALIGN) {
        setVAlign(textToVAlign(value.toString()));
    } else if (name==GV_TEXT_HALIGN) {
        setHAlign(textToHAlign(value.toString()));
    } else if (name==GV_TEXT_LINESPACINGFACTOR) {
        setLineSpacingFactor(GV_Math::eval(value.toString()));
    } else if (name==GV_TEXT_TEXT) {
        setText(value.toString());
    } else if (name==GV_TEXT_STYLE) {
        setStyle(value.toString());
    } else if (name==GV_TEXT_ANGLE) {
        setAngle(GV_Math::deg2rad(GV_Math::eval(value.toString())));
    } else {
        GV_EntityContainer::setProperty(name, value);
    }
    update();
}


/**
 * Limits the choices for the text style property.
 */
GV_PropertyAttributes GV_Text::getPropertyAttributes(const QString& propertyName) {
    GV_PropertyAttributes ret = 
        GV_EntityContainer::getPropertyAttributes(propertyName);

    if (propertyName==GV_TEXT_STYLE) {
        for (GV_Font* f=GV_FONTLIST->firstFont();
            f!=NULL;
            f=GV_FONTLIST->nextFont()) {
            
            ret.choices << f->getFileName();
        }
    }
    else if (propertyName==GV_TEXT_HALIGN) {
        ret.choices
            << hAlignToText(GV2::HAlignLeft)
            << hAlignToText(GV2::HAlignCenter)
            << hAlignToText(GV2::HAlignRight);
    }
    else if (propertyName==GV_TEXT_VALIGN) {
        ret.choices
            << vAlignToText(GV2::VAlignTop)
            << vAlignToText(GV2::VAlignMiddle)
            << vAlignToText(GV2::VAlignBottom);
    }

    return ret;
}
