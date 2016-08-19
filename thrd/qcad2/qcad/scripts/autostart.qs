//
// autostart.qs
//
// 1. Place this file into a directory location where QCad can find it
//
//   Replace <USERNAME> with your user login name on your system.
//
//   On a Windows XP system this will be:
//     C:/Documents and Settings/<USERNAME>/.qcad/autostart.qs
//
//   On a Unix/Linux system this will be:
//     /home/<USERNAME>/.qcad/autostart.qs
//
// 2. Restart QCad and check if it loads your autostart.qs file
//   You should see in the command line something like this:
//   Running script 'C:/Documents and Settings/<USERNAME>/.qcad/autostart.qs'
//
// 3. Once this works, place all neede files in your autostart directory
//    For example demo01.qs and demo01.png.
//
// 4. Optionally add customized icons for your script
//    If your sctipt is demo01.qs then the icon will be demo01.png.
//    Just a normal PNG file, 16x16 pixels in size, 72 dpi.
//
// 5. Press the button
//    You should see something like this in the command line:
// 
//    Running script 'C:/Documents and Settings/<USERNAME>/.qcad/demo01.qs'
//
//
// NOTE: You can also load and run this script directly within the
//       scripting IDE for testing purpose.
//
// For more information about this file see:
// http://www.ribbonsoft.com/qcad/manual_scripting/contents.html
//

function main() {

  // -- add a toolbar with some script buttons
  var tbScripts = new ToolBar("Scripts Tool Bar");
  tbScripts.addScriptButton("demo01.qs");   // -- icon demo01.png
  tbScripts.addScriptButton("entities01.qs");
  tbScripts.addScriptButton("layer01.qs");
  tbScripts.addScriptButton("math01.qs");
  tbScripts.addScriptButton("plot01.qs");
  tbScripts.addScriptButton("spiral.qs");

  // -- add a toolbar with some block buttons
  var tbBlocks = new ToolBar("Blocks Tool Bar");
  tbBlocks.addBlockButton("misc/t-part.dxf");
  tbBlocks.addBlockButton("misc/screw.dxf");

}

