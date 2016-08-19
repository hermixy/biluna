/**
 *  Author: Michael Mustun
 *  Status: works just fine.
 *  Read from a ASCII text file xy values.
 * 1. lower this window.
 * 2. Click on it (make active) so that the QCad 
 *    windows is shown.
 * 3. Hit 5F in this lowered windows.
 * 4. Nice watching!
 *
 */

function main() {

  var filename = "inputxy.txt";  // default file name

  var doc;       // current document
  var view;      // current document view
  var line;      // line to add
  var i;         // counter
  var factor;    // scaling factor
  var lx = -1.0;
  var ly = -1.0;
  var x;
  var y;
        
  var file;  

  var aLine;
  var bLine;
  
  filename = inputFileName();
  factor = inputData();

  doc = new Document;
  view = new View;

  try {
    file = new File( filename);
    file.open(1);

    var re = /(.*)[,|\t](.*)/;  // comma separated list 
    var num_x; // 1st point x
    var num_y; // 1st point y
    var num_bx;  // 2nd point x
    var num_by;  // 2nd point y
    var line;

    
    // --- line 1 and 2
    aLine = file.readLine();
    bLine = file.readLine();

    aLine.match(re);
    num_x = parseFloat( re.capturedTexts[1] );
    num_y = parseFloat( re.capturedTexts[2] );

    bLine.match(re);
    num_bx = parseFloat( re.capturedTexts[1] );
    num_by = parseFloat( re.capturedTexts[2] );
    
    lx = num_x;   
    ly = num_y;   
  
    x = num_bx;   
    y = num_by;   

    line = new Line(doc, lx*factor.x, ly*factor.y,
                          x*factor.x,  y*factor.y);
    doc.addEntity(line);

    // line 2 and 3 ...
    while  ( aLine = file.readLine() ) {
      //print("DEBUG: raw input data line: \"" + aLine + "\"");

      aLine.match(re);
      num_x = parseFloat( re.capturedTexts[1] );
      num_y = parseFloat( re.capturedTexts[2] );

      //print("DEBUG: x y: \'" + num_x + "\'   \'" + num_y + "\'");
      //print("DEBUG: bx by: \'" + num_bx + "\'   \'" + num_by + "\'");
    
      lx = num_bx;   
      ly = num_by;   
  
      x = num_x;   
      y = num_y;   

      line = new Line(doc, lx*factor.x, ly*factor.y,
                            x*factor.x,  y*factor.y);
      doc.addEntity(line);
      view.redraw();  // optional, see every step on screen
      
      num_bx = num_x;
      num_by = num_y;
    }

  }
  finally {
    file.close();
  }

  print("DEBUG: End of File reached.");     

  view.redraw();

  print("All done.");
}


/**
 * Presents a dialog to input file name.
 */
function inputFileName() {
    var dialog = new Dialog;
    dialog.caption = "Input filename";
    dialog.okButtonText = "OK";
    dialog.cancelButtonText = "Abort";

    var fn = new LineEdit;
    fn.label = "Filename: ";
    fn.text = "support_inputxy.txt";
    dialog.add(fn);

    if (dialog.exec()) {
        print("filename: " + fn.text);
    }

    var ret = fn.text;

    return ret;
}

/**
 * Presents a dialog to input an X and Y factor for the sine.
 */
function inputData() {
    var dialog = new Dialog;
    dialog.caption = "Factor for sine curve";
    dialog.okButtonText = "OK";
    dialog.cancelButtonText = "Abort";

    var xfact = new LineEdit;
    xfact.label = "X Factor: ";
    xfact.text = "100";
    dialog.add(xfact);

    var yfact = new LineEdit;
    yfact.label = "Y Factor: ";
    yfact.text = "100";
    dialog.add(yfact);

    if (dialog.exec()) {
        print("Factors: " + xfact.text + "/" + yfact.text);
    }

    var ret = new Vector(xfact.text, yfact.text);

    return ret;
}

// EOF.
