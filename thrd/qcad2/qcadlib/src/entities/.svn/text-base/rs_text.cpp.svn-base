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
** not clear to you.
**
**********************************************************************/


#include "rs_text.h"

#include "rs_fontlist.h"
#include "rs_insert.h"


const char* RS_Text::propertyInsertionPointX = QT_TRANSLATE_NOOP("QObject", "Position|X");
const char* RS_Text::propertyInsertionPointY = QT_TRANSLATE_NOOP("QObject", "Position|Y");
const char* RS_Text::propertyHeight = QT_TRANSLATE_NOOP("QObject", "Height");
const char* RS_Text::propertyVAlign = QT_TRANSLATE_NOOP("QObject", "Alignment|Vertical");
const char* RS_Text::propertyHAlign = QT_TRANSLATE_NOOP("QObject", "Alignment|Horizontal");
const char* RS_Text::propertyLineSpacingFactor = QT_TRANSLATE_NOOP("QObject", "Line Spacing Factor");
const char* RS_Text::propertyText = QT_TRANSLATE_NOOP("QObject", "Text");
const char* RS_Text::propertyStyle = QT_TRANSLATE_NOOP("QObject", "Style");
const char* RS_Text::propertyAngle = QT_TRANSLATE_NOOP("QObject", "Angle");


/**
 * Constructor.
 */
RS_Text::RS_Text(RS_EntityContainer* parentContainer,
                 const RS_TextData& d)
        : RS_Entity(parentContainer), RS_EntityContainer(parentContainer), data(d) {

    usedTextHeight = 0.0;
    usedTextWidth = 0.0;
    setText(data.text);
}



/**
 * Sets a new text. The entities representing the 
 * text are updated.
 */
void RS_Text::setText(const RS_String& t) {
    data.text = t;

    // handle some special flags embedded in the text:
    if (data.text.left(4)=="\\A0;") {
        data.text = data.text.mid(4);
        data.valign = RS2::VAlignBottom;
    } else if (data.text.left(4)=="\\A1;") {
        data.text = data.text.mid(4);
        data.valign = RS2::VAlignMiddle;
    } else if (data.text.left(4)=="\\A2;") {
        data.text = data.text.mid(4);
        data.valign = RS2::VAlignTop;
    }

    if (data.updateMode==RS2::Update) {
        update();
    }
}



/**
 * Gets the alignment as an int.
 *
 * @return  1: top left ... 9: bottom right
 */
int RS_Text::getAlignment() {
    if (data.valign==RS2::VAlignTop) {
        if (data.halign==RS2::HAlignLeft) {
            return 1;
        } else if (data.halign==RS2::HAlignCenter) {
            return 2;
        } else if (data.halign==RS2::HAlignRight) {
            return 3;
        }
    } else if (data.valign==RS2::VAlignMiddle) {
        if (data.halign==RS2::HAlignLeft) {
            return 4;
        } else if (data.halign==RS2::HAlignCenter) {
            return 5;
        } else if (data.halign==RS2::HAlignRight) {
            return 6;
        }
    } else if (data.valign==RS2::VAlignBottom) {
        if (data.halign==RS2::HAlignLeft) {
            return 7;
        } else if (data.halign==RS2::HAlignCenter) {
            return 8;
        } else if (data.halign==RS2::HAlignRight) {
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
void RS_Text::setAlignment(int a) {
    switch (a%3) {
    default:
    case 1:
        data.halign = RS2::HAlignLeft;
        break;
    case 2:
        data.halign = RS2::HAlignCenter;
        break;
    case 0:
        data.halign = RS2::HAlignRight;
        break;
    }

    switch ((int)ceil(a/3.0)) {
    default:
    case 1:
        data.valign = RS2::VAlignTop;
        break;
    case 2:
        data.valign = RS2::VAlignMiddle;
        break;
    case 3:
        data.valign = RS2::VAlignBottom;
        break;
    }

}



/**
 * @return Number of lines in this text entity. 
 */
int RS_Text::getNumberOfLines() {
    int c=1;

    for (int i=0; i<(int)data.text.length(); ++i) {
        if (data.text.at(i).unicode()==0x0A) {
            c++;
        }
    }

    return c;
}



RS_Vector RS_Text::getNearestSelectedRef(const RS_Vector& coord,
        double* dist,
        RS2::RefMode refMode) {

    return RS_Entity::getNearestSelectedRef(coord, dist, refMode);
}



void RS_Text::calculateBorders(bool /*visibleOnly*/) {
    RS_EntityContainer::calculateBorders();

    if (minV.distanceTo(maxV)<1.0e-4) {
        minV = maxV = data.insertionPoint;
    }
}



/**
 * Updates the Inserts (letters) of this text. Called when the 
 * text or it's data, position, alignment, .. changes.
 * This method also updates the usedTextWidth / usedTextHeight property.
 */
void RS_Text::update() {

    RS_DEBUG->print("RS_Text::update");

    clear();

    if (isUndone()) {
        RS_DEBUG->print("RS_Text::update: text is in undo buffer. aborting");
        return;
    }
    
    lineInsertionPoints.clear();

    RS_DEBUG->print("RS_Text::update: %d lines", getNumberOfLines());

    usedTextWidth = 0.0;
    usedTextHeight = 0.0;

    RS_Font* font = RS_FONTLIST->requestFont(data.style);

    if (font==NULL) {
        RS_DEBUG->print("RS_Text::update: font '%s' not found. aborting", 
            (const char*)data.style.toLatin1());
        return;
    }

    RS_Vector letterPos = RS_Vector(0.0, -9.0);
    RS_Vector letterSpace = RS_Vector(font->getLetterSpacing(), 0.0);
    RS_Vector space = RS_Vector(font->getWordSpacing(), 0.0);
    int lineCounter = 0;

    // Every single text line gets stored in this entity container
    //  so we can move the whole line around easely:
    RS_EntityContainer* oneLine = new RS_EntityContainer(this);
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
            oneLine = new RS_EntityContainer(this);
            letterPos = RS_Vector(0.0, -9.0);
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
                    oneLine = new RS_EntityContainer(this);
                    letterPos = RS_Vector(0.0, -9.0);
                    break;

                case 'S': {
                        RS_String up;
                        RS_String dw;

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
                        RS_Text* upper =
                            new RS_Text(
                                oneLine,
                                RS_TextData(letterPos + RS_Vector(0.0,9.0),
                                            4.0, 100.0, RS2::VAlignTop, 
                                            RS2::HAlignLeft,
                                            RS2::LeftToRight, RS2::Exact,
                                            1.0, up, data.style,
                                            0.0, RS2::Update));
                        upper->setLayer(NULL);
                        upper->setPen(RS_Pen(RS2::FlagInvalid));
                        oneLine->addEntity(upper);

                        RS_Text* lower =
                            new RS_Text(
                                oneLine,
                                RS_TextData(letterPos+RS_Vector(0.0,4.0),
                                            4.0, 100.0, RS2::VAlignTop, 
                                            RS2::HAlignLeft,
                                            RS2::LeftToRight, RS2::Exact,
                                            1.0, dw, data.style,
                                            0.0, RS2::Update));
                        lower->setLayer(NULL);
                        lower->setPen(RS_Pen(RS2::FlagInvalid));
                        oneLine->addEntity(lower);

                        // move cursor:
                        upper->calculateBorders();
                        lower->calculateBorders();

                        double w1 = upper->getSize().x;
                        double w2 = lower->getSize().x;

                        if (w1>w2) {
                            letterPos += RS_Vector(w1, 0.0);
                        } else {
                            letterPos += RS_Vector(w2, 0.0);
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
                RS_DEBUG->print("RS_Text::update: insert letter");
                RS_Char chr = data.text.at(i);
                RS_Block* letter = font->findLetter(QString(chr));

                // since 2.1.2.5: show ? for unknown letters:
                if (letter==NULL) {
                    chr = '?';
                    letter = font->findLetter(QString(chr));
                }

                // One Letter:
                if (letter!=NULL) {
                
                    RS_DEBUG->print("RS_Text::update: insert "
                      "letter '%s' at pos: %f/%f", 
                      (const char*)QString(chr).toLatin1(), 
                      letterPos.x, letterPos.y);

                    RS_InsertData d;
					d = RS_InsertData(RS_String(chr), 
								    letterPos,
                                    RS_Vector(1.0, 1.0),
                                    0.0,
                                    1,1, RS_Vector(0.0,0.0),
                                    font->getLetterList(), RS2::NoUpdate);
                    
                    RS_Insert* letter = new RS_Insert(this, d);
                    RS_Vector letterWidth;
                    letter->setPen(RS_Pen(RS2::FlagInvalid));
                    letter->setLayer(NULL);
                    letter->update();
                    letter->calculateBorders();
                    
                    // until 2.0.4.5:
                    //letterWidth = RS_Vector(letter->getSize().x, 0.0);
                    // from 2.0.4.6:
                    letterWidth = RS_Vector(letter->getMax().x-letterPos.x, 0.0);
                    
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
    
    RS_DEBUG->print("RS_Text::update: OK");
}



/**
 * Used internally by update() to add a text line created with 
 * default values and alignment to this text container.
 *
 * @param textLine The text line.
 * @param lineCounter Line number.
 */
void RS_Text::updateAddLine(RS_EntityContainer* textLine, int lineCounter) {
    RS_DEBUG->print("RS_Text::updateAddLine: width: %f", textLine->getSize().x);

    textLine->calculateBorders();
    RS_Vector textSize = textLine->getSize();
    
    RS_DEBUG->print("RS_Text::updateAddLine: width 2: %f", textSize.x);

    // Horizontal Align:
    switch (data.halign) {
    case RS2::HAlignCenter:
        RS_DEBUG->print("RS_Text::updateAddLine: move by: %f", -textSize.x/2.0);
        textLine->move(RS_Vector(-textSize.x/2.0, 0.0));
        break;

    case RS2::HAlignRight:
        textLine->move(RS_Vector(-textSize.x, 0.0));
        break;

    default:
        break;
    }
    
    // temporary point that will in the end indicate the insertion point 
    //  of this line:
    RS_Point* ref = new RS_Point(textLine, RS_PointData(RS_Vector(0.0,0.0)));
    textLine->addEntity(ref);

    // Vertical Align:
    double vSize = getNumberOfLines()*9.0*data.lineSpacingFactor*1.6
                   - (9.0*data.lineSpacingFactor*1.6 - 9.0);

    switch (data.valign) {
    case RS2::VAlignMiddle:
        textLine->move(RS_Vector(0.0, vSize/2.0));
        break;

    case RS2::VAlignBottom:
        textLine->move(RS_Vector(0.0, vSize));
        break;

    default:
        break;
    }
    
    // Move to correct line position:
    textLine->move(RS_Vector(0.0, -9.0 * lineCounter
                             * data.lineSpacingFactor * 1.6));

    // Scale:
    textLine->scale(RS_Vector(0.0,0.0),
                    RS_Vector(data.height/9.0, data.height/9.0));

    textLine->calculateBorders();

    // Update actual text size (before rotating, after scaling!):
    if (textLine->getSize().x>usedTextWidth) {
        usedTextWidth = textLine->getSize().x;
    }

    usedTextHeight += data.height*data.lineSpacingFactor*1.6;

    // Rotate:
    textLine->rotate(RS_Vector(0.0,0.0), data.angle);

    // Move:
    textLine->move(data.insertionPoint);
    textLine->setPen(RS_Pen(RS2::FlagInvalid));
    textLine->setLayer(NULL);
    textLine->calculateBorders();
    
    RS_Vector refPos = ref->getPos();
    textLine->removeEntity(ref);

    lineInsertionPoints.set(lineCounter, refPos);
    
    RS_DEBUG->print("RS_Text::updateAddLine: line at: %f/%f ", refPos.x, refPos.y);

    addEntity(textLine);
}



RS_VectorList RS_Text::getRefPoints(RS2::RefMode /*refMode*/) {
    RS_VectorList ret(data.insertionPoint);
    return ret;
}

    
    
RS_Vector RS_Text::getNearestRef(const RS_Vector& coord,
                                     double* dist,
                                     RS2::RefMode refMode) {

    return RS_Entity::getNearestRef(coord, dist, refMode);
}


RS_VectorList RS_Text::getLineInsertionPoints() {
    return lineInsertionPoints;
}


void RS_Text::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    if (ref.distanceTo(data.insertionPoint)<1.0e-4) {
        move(offset);
    }
}


void RS_Text::move(const RS_Vector& offset) {
    data.insertionPoint.move(offset);
    RS_EntityContainer::move(offset);
    lineInsertionPoints.move(offset);
}



void RS_Text::rotate(const RS_Vector& center, double angle) {
    data.insertionPoint.rotate(center, angle);
    data.angle = RS_Math::correctAngle(data.angle+angle);
    update();
}



void RS_Text::scale(const RS_Vector& center, const RS_Vector& factor) {
    data.insertionPoint.scale(center, factor);
    data.width*=factor.x;
    data.height*=factor.x;
    update();
}



void RS_Text::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    data.insertionPoint.mirror(axisPoint1, axisPoint2);
    bool readable = RS_Math::isAngleReadable(data.angle);

    RS_Vector vec;
    vec.setPolar(1.0, data.angle);
    vec.mirror(RS_Vector(0.0,0.0), axisPoint2-axisPoint1);
    data.angle = vec.angle();

    bool corr;
    data.angle = RS_Math::makeAngleReadable(data.angle, readable, &corr);

    if (corr) {
        if (data.halign==RS2::HAlignLeft) {
            data.halign=RS2::HAlignRight;
        } else if (data.halign==RS2::HAlignRight) {
            data.halign=RS2::HAlignLeft;
        }
    } else {
        if (data.valign==RS2::VAlignTop) {
            data.valign=RS2::VAlignBottom;
        } else if (data.valign==RS2::VAlignBottom) {
            data.valign=RS2::VAlignTop;
        }
    }
    update();
}



bool RS_Text::hasEndpointsWithinWindow(const RS_Vector& /*v1*/, const RS_Vector& /*v2*/) {
    return false;
}



/**
 * Implementations must stretch the given range of the entity 
 * by the given offset.
 */
void RS_Text::stretch(const RS_Vector& firstCorner,
                      const RS_Vector& secondCorner,
                      const RS_Vector& offset) {

    if (getMin().isInWindow(firstCorner, secondCorner) &&
            getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    }
}



RS_Variant RS_Text::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_TEXT_INSERTIONPOINT_X) {
        return getInsertionPoint().x;
    } else if (name==RS_TEXT_INSERTIONPOINT_Y) {
        return getInsertionPoint().y;
    } else if (name==RS_TEXT_HEIGHT) {
        return getHeight();
    } else if (name==RS_TEXT_VALIGN) {
        return vAlignToText(getVAlign());
    } else if (name==RS_TEXT_HALIGN) {
        return hAlignToText(getHAlign());
    } else if (name==RS_TEXT_LINESPACINGFACTOR) {
        return getLineSpacingFactor();
    } else if (name==RS_TEXT_TEXT) {
        return getText();
    } else if (name==RS_TEXT_STYLE) {
        return getStyle();
    } else if (name==RS_TEXT_ANGLE) {
        return RS_Math::rad2deg(getAngle());
    } else {
        return RS_EntityContainer::getProperty(name, def);
    }
}



RS_StringList RS_Text::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_EntityContainer::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << RS_TEXT_INSERTIONPOINT_X;
        ret << RS_TEXT_INSERTIONPOINT_Y;
        ret << RS_TEXT_HEIGHT;
        ret << RS_TEXT_VALIGN;
        ret << RS_TEXT_HALIGN;
        ret << RS_TEXT_LINESPACINGFACTOR;
        if (!data.text.contains('\n')) {
            ret << RS_TEXT_TEXT;
        }
        ret << RS_TEXT_STYLE;
        ret << RS_TEXT_ANGLE;
    }
    return ret;
}


void RS_Text::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_TEXT_INSERTIONPOINT_X) {
        data.insertionPoint.x = RS_Math::eval(value.toString());
    } else if (name==RS_TEXT_INSERTIONPOINT_Y) {
        data.insertionPoint.y = RS_Math::eval(value.toString());
    } else if (name==RS_TEXT_HEIGHT) {
        setHeight(RS_Math::eval(value.toString()));
    } else if (name==RS_TEXT_VALIGN) {
        setVAlign(textToVAlign(value.toString()));
    } else if (name==RS_TEXT_HALIGN) {
        setHAlign(textToHAlign(value.toString()));
    } else if (name==RS_TEXT_LINESPACINGFACTOR) {
        setLineSpacingFactor(RS_Math::eval(value.toString()));
    } else if (name==RS_TEXT_TEXT) {
        setText(value.toString());
    } else if (name==RS_TEXT_STYLE) {
        setStyle(value.toString());
    } else if (name==RS_TEXT_ANGLE) {
        setAngle(RS_Math::deg2rad(RS_Math::eval(value.toString())));
    } else {
        RS_EntityContainer::setProperty(name, value);
    }
    update();
}


/**
 * Limits the choices for the text style property.
 */
RS_PropertyAttributes RS_Text::getPropertyAttributes(const RS_String& propertyName) {
    RS_PropertyAttributes ret = 
        RS_EntityContainer::getPropertyAttributes(propertyName);

    if (propertyName==RS_TEXT_STYLE) {
        for (RS_Font* f=RS_FONTLIST->firstFont();
            f!=NULL;
            f=RS_FONTLIST->nextFont()) {
            
            ret.choices << f->getFileName();
        }
    }
    else if (propertyName==RS_TEXT_HALIGN) {
        ret.choices
            << hAlignToText(RS2::HAlignLeft)
            << hAlignToText(RS2::HAlignCenter)
            << hAlignToText(RS2::HAlignRight);
    }
    else if (propertyName==RS_TEXT_VALIGN) {
        ret.choices
            << vAlignToText(RS2::VAlignTop)
            << vAlignToText(RS2::VAlignMiddle)
            << vAlignToText(RS2::VAlignBottom);
    }

    return ret;
}
