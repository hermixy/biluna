/****************************************************************************
** $Id: gv_font.h 4861 2007-03-07 15:29:43Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
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
** not clear to you. Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_FONT_H
#define GV_FONT_H

#include <iostream>

#include "gv_blocklist.h"
#include <QString>
#include <QStringList>


/**
 * Class for representing a font. This is implemented as a GV_Graphic
 * with a name (the font name) and several blocks, one for each letter
 * in the font.
 *
 * @author Andrew Mustun
 */
class GV_Font {
public:
    GV_Font(const QString& name, bool owner=true);
    //GV_Font(const char* name);
    ~GV_Font();

    /** @return the fileName of this font. */
    QString getFileName() const {
        return fileName;
    }
    
    /** @return the encoding of this font. */
    QString getEncoding() const {
        return encoding;
    }
    
    /** @return the alternative names of this font. */
    const QStringList& getNames() const {
        return names;
    }
    
    /** @return the author(s) of this font. */
    const QStringList& getAuthors() const {
        return authors;
    }

    /** @return Default letter spacing for this font */
    double getLetterSpacing() {
        return letterSpacing;
    }

    /** @return Default word spacing for this font */
    double getWordSpacing() {
        return wordSpacing;
    }

    /** @return Default line spacing factor for this font */
    double getLineSpacingFactor() {
        return lineSpacingFactor;
    }

    bool loadFont();

    // Wrappers for block list (letters) functions
    GV_BlockList* getLetterList() {
        return &letterList;
    }
    GV_Block* findLetter(const QString& name) {
        return letterList.find(name);
    }
    int countLetters() {
        return letterList.count();
    }
    GV_Block* letterAt(int i) {
        return letterList.at(i);
    }

    friend class GV_FontList;

private:
    //! block list (letters)
    GV_BlockList letterList;

    //! Font file name
    QString fileName;
    
    //! Font encoding (see docu for QTextCodec)
    QString encoding;

    //! Font names
    QStringList names;
    
    //! Authors
    QStringList authors;

    //! Is this font currently loaded into memory?
    bool loaded;

    //! Default letter spacing for this font
    double letterSpacing;

    //! Default word spacing for this font
    double wordSpacing;

    //! Default line spacing factor for this font
    double lineSpacingFactor;
};

#endif

