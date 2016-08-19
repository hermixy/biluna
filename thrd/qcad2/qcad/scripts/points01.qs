/**
 * Demonstrates the entity classes.
 * $Id: entities01.qs 934 2005-02-16 22:22:56Z andrew $
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

    points();

    view.redraw();
}

/**
 * Dot demo
 */
function points() {
    var p11 = new Vector(20, 100);
    var p12 = new Vector(90, 110);
    var p21 = new Vector(40, 130);
    var p22 = new Vector(60, 125);
    var point;
    point = new Pnt(doc, p11);
    doc.addEntity(point);
    print("p11: " + p11.x + "/" + p11.y)
}
