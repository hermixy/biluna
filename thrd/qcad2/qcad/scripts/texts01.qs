/**
 * Demonstrates the text entity class.
 *
 * Author: Andrew Mustun
 */
var doc;       // current document
var view;

/**
 * Starts the demo.
 */
function main() {
    doc = new Document;
    view = new View;

    texts();

    view.redraw();
}

/**
 * Text demo
 */
function texts() {
    var text;
    text = new Text(doc,
        "Hello World",     // text
        "standard",        // style
        10.0, 5.0,         // position
        5.0,               // height
        0.0,               // angle
        1.0,               // line spacing factor
        "Top",             // vertical alignment
        "Left");           // horizontal alignment
    doc.addEntity(text);
    
    var text2;
    text2 = new Text(doc,
        "Hello World 2",   // text
        "standard",        // style
        50.0, 50.0,        // position
        8.0,               // height
        Math.PI/6,         // angle
        1.0,               // line spacing factor
        "Middle",          // vertical alignment
        "Center");         // horizontal alignment
    doc.addEntity(text2);
}
