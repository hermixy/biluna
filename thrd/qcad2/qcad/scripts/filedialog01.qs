/**
 * Author: Andrew Mustun
 *
 * Simple demo of an open file dialog.
 */
function main() {
	var file = FileDialog.getOpenFileName("*.txt", "Blah");
	debug("Selected file is: " + file);
}

