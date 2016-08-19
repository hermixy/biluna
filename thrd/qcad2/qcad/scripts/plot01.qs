/**
 * Author: Andrew Mustun
 * $Id: plot01.qs 2706 2005-09-11 11:30:47Z andrew $
 *
 * Plots a sine.
 */
function main() {
    var doc = new Document();
    var view = new View();

    var lx = -1.0;
    var ly = -1.0;
    var x;
    var y;

    for (x=0.0; x<2*Math.PI; x+=0.1) {
        print("x: " + x);
        y = Math.sin(x);

        print("y: " + y);
        if (lx>=0.0) {
            var line = new Line(doc, lx, ly, x, y);
            doc.addEntity(line);
        }
        lx = x;
        ly = y;
    }

    view.redraw();
}
