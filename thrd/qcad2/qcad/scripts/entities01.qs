/**
 * Author: Andrew Mustun
 *
 * Demonstrates the entity classes.
 */
var doc;       // current document
var view;      // current document view

/**
 * Starts the demo.
 */
function main() {
    doc = new Document;
    view = new View;

    points();
    lines();
    circles();
    arcs();
    ellipses();
    
    view.zoomAuto();
}


/**
 * Circle demo.
 */
function circles() {
    var circle;
    var radius;
    for (radius = 40.0; radius>10.0; radius-=5) {
        circle = new Circle(doc,
                      250.0,190.0-radius,        // center
                      radius);                   // radius
        doc.addEntity(circle);
    }
}



/**
 * Arc demo.
 */
function arcs() {
    var angle, arc;
    var radius = 1;
    for (angle = 0.0; angle<Math.PI*3; angle+=Math.PI/16) {
        arc = new Arc(doc,
                      50.0,50.0,                 // center
                      radius,                    // radius
                      angle, angle+Math.PI/2,    // start- / end angle
                      false);                    // counter clock wise
        doc.addEntity(arc);
        radius+=1;
    }
}



/**
 * Line demo.
 */
function lines() {
    var line, v1, v2, v3;
    v1 = new Vector(150,90);
    v2 = new Vector(150,90);
    v3 = new Vector(150,90);
    v2.rotate(150,45, Math.PI*2/3);
    v3.rotate(150,45, Math.PI*2/3*2);

    for (var i=0; i<10; i++) {
        line = new Line(doc, v1, v2);
        doc.addEntity(line);
        line = new Line(doc, v2, v3);
        doc.addEntity(line);
        line = new Line(doc, v3, v1);
        doc.addEntity(line);

        v1.rotate(150,45, Math.PI/16);
        v2.rotate(150,45, Math.PI/16);
        v3.rotate(150,45, Math.PI/16);

        v1.scale(150,45, 0.758);
        v2.scale(150,45, 0.758);
        v3.scale(150,45, 0.758);
    }
}



/**
 * Point demo
 */
function points() {
    var p11 = new Vector(10, 135);
    var p12 = new Vector(90, 145);
    var p21 = new Vector(40, 165);
    var p22 = new Vector(60, 160);
    var point;

    for (var i=0; i<=10; i++) {
        var p1 = divide(p11, p12, 10, i);
        var p2 = divide(p21, p22, 10, i);

        print("p1: " + p1.toString());
        print("p2: " + p2.toString());
        for (var k=0; k<=10; k++) {
            var p = divide(p1, p2, 10, k);
            point = new Pnt(doc, p);
            doc.addEntity(point);
        }
    }
}



/**
 * Ellipse demo.
 */
function ellipses() {
    for (var i=0; i<=25; i+=5) {
        ellipse = new Ellipse(doc,
                      150.0,150.0,                // center
                      50.0-i,25.0,               // radius
                      0.0,                       // rotation
                      0,2*Math.PI,               // arc angles
                      false);                    // counter clock wise
        doc.addEntity(ellipse);
    }
}



/**
 * Helper function for points demo.
 */
function divide(p1, p2, steps, step) {
    var p = new Vector;
    p.x = p1.x + (p2.x - p1.x)/steps*step;
    p.y = p1.y + (p2.y - p1.y)/steps*step;
    return p;
}
