/**
 * Demonstrates the insert entity class.
 *
 * Author: Andrew Mustun
 */
var doc;
var view;

/**
 * Starts the demo.
 */
function main() {
    doc = new Document;
    view = new View;

    inserts();

    view.redraw();
}

/**
 * Inserts demo
 */
function inserts() {
    var ins;
    ins = new Insert(doc,
        "misc/tux",        // library symbol name
        10.0, 5.0,         // position
        1.0,               // factor
        0.0);              // angle
    doc.addEntity(ins);
    
    var ins2;
    ins2 = new Insert(doc,
        "misc/tux",        // library symbol name
        50.0, 50.0,        // position
        4.0,               // factor
        Math.PI/6);        // angle
    doc.addEntity(ins2);
}
