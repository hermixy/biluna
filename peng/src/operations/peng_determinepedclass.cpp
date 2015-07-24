/*****************************************************************
 * $Id: peng_determinepedclass.cpp 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_determinepedclass.h"


/**
 * Constructor
 */
PENG_DeterminePedClass::PENG_DeterminePedClass() {
    // nothing
}

/**
 * Destructor
 */
PENG_DeterminePedClass::~PENG_DeterminePedClass() {
    // nothing
}

/**
 * Execute operation, asses PED classification
 * @param type apparatus type, Piping, Equipment,
 * Pressure_Accessory, Safety_Accessory or Generator
 * @param assess type AssessDn or AssessVolume in case of Piping always AssessDn
 * @param fluidState Gas, Vapour, Two_Phase, Gaseous or Liquid
 * @param fluidGroup Group1 or Group2
 * @param maxPress maximum allowable pressure in barg
 * @param maxTemp maximum allowable temperature in Celsius
 * @param maxVol maximum volume in litres
 * @param nomSize nominal diameter in mm
 * @param addInfo additional information, (none), Bottle_Breathing_Eqmt,
 * Portable_Extinguisher or Pressure_Cooker
 * @param unstableGas Unstable or Stable
 */
void PENG_DeterminePedClass::execute(const RB_String& type,
                                   const RB_String& assess,
                                   const RB_String& fluidState,
                                   const RB_String& fluidGroup,
                                   double maxPress,
                                   double maxTemp,
                                   double maxVol,
                                   double nomSize,
                                   const RB_String& addInfo,
                                   const RB_String& unstableGas) {
    // Set input parameters, this assessment is copied from the PHP internet page
    RB_String assessment = assess; // volume in litres or pipe diameter DN
    if (type == "Piping") {
		assessment = "AssessDn";
    }
	
    RB_String group = fluidGroup;
	RB_String state = "(none)";
    RB_String test = fluidState;
	
    if (test == "Gaseous" || test == "Gas" || test == "Vapour" || test == "Two_Phase") {
		state = "Gaseous";	
	} else if (test == "Liquid") {
		state = "Liquid";	
	}
	
    RB_String unstable = "(none)";
    if (state == "Gaseous") {
        if (unstableGas == "Unstable") {
            unstable = "Unstable_Gas";
        } else if (unstableGas == "Stable") {
            unstable = "Stable_Gas";
        }
    }

    // Determine the applicable mTable/chart based on
    // - type of equipment
    // - fluid group
    // - fluid state
    mTable = 0;
    mCategory = "";
    mNote = "";
	 
	if (type == "Equipment" || (type == "Pressure_Accessory" && assessment == "AssessVolume") 
							|| (type == "Safety_Accessory" && assessment == "AssessVolume") ) {
		if (state == "Gaseous" && group == "Group1") {
            mTable = 1;
		} else if (state == "Gaseous" && group == "Group2") {
            mTable = 2;
		} else if (state == "Liquid" && group == "Group1") {
            mTable = 3;
		} else if (state == "Liquid" && group == "Group2") {
            mTable = 4;
		}
	} else if (type == "Generator") {
        mTable = 5;
	} else if (type == "Piping" || (type == "Pressure_Accessory" && assessment == "AssessDn") 
							|| (type == "Safety_Accessory" && assessment == "AssessDn") ) {
		if (state == "Gaseous" && group == "Group1") {
            mTable = 6;
		} else if (state == "Gaseous" && group == "Group2") {
            mTable = 7;
		} else if (state == "Liquid" && group == "Group1") {
            mTable = 8;
		} else if (state == "Liquid" && group == "Group2") {
            mTable = 9;
		}
	}

    // Determine the category based on mTables
    // - split the x-axis in different areas, in that part
    //   determine the category based on the y position
    double ps = maxPress;
    double ts = maxTemp;
    double v = maxVol;
	int dn = nomSize;
    mPsv = ps * v;
    mPsdn = ps * dn;
	
    setCategory(ps, v, dn);

	
    // exceptions to mTables
    if (mTable == 1 && (mCategory == "I" || mCategory == "II") && unstable == "Unstable_Gas") {
        mCategory = "III";
        mNote = "Exception";
    } else if (mTable == 2 && (addInfo == "Bottle_Breathing_Eqmt" || addInfo == "Portable_Extinguisher")
                            && (mCategory == "I" || mCategory == "II")) {
        mCategory = "III";
        mNote = "Exception";
    } else if (mTable == 4 && (mCategory == "I" || mCategory == "II") && ts <= 110) {
        mNote = "Exception";
    } else if (mTable == 5 && addInfo == "Pressure_Cooker") {
        mNote = "Exception";
    } else if (mTable == 5 && ts <= 110 && ps > 0.5 && addInfo != "Pressure_Cooker") {
        mCategory = "NA";
    } else if (mTable == 6 && (mCategory == "I" || mCategory == "II") && unstable == "Unstable_Gas") {
        mCategory = "III";
        mNote = "Exception";
    } else if (mTable == 7 && ts > 350 && mCategory == "II") {
        mCategory = "III";
        mNote = "Exception";
	}
	
	if (type == "Safety_Accessory") {
        mCategory = "IV";
        mNote = "Safety_Accessory";
	}

    setModuleAndException();
}

/**
 * Set category
 * @param ps maximum pressure [barg]
 * @param v maximum volume [l]
 * @param dn nominal size [mm]
 */
void PENG_DeterminePedClass::setCategory(double ps, double v, int dn) {
    if (mTable == 1) {
        if (v <= 1) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (ps <= 200) {
                mCategory = "SEP"; return;
            } else if (ps <= 1000) {
                mCategory = "III"; return;
            } else { // ps > 1000
                mCategory = "IV"; return;
            }
        } else if (v <= 50) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 25) {
                mCategory = "SEP"; return;
            } else if (mPsv <= 50) {
                mCategory = "I"; return;
            } else if (mPsv <= 200) {
                mCategory = "II"; return;
            } else if (mPsv <= 1000) {
                mCategory = "III"; return;
            } else { // mPsv > 1000
                mCategory = "IV"; return;
            }
        } else if (v <= 100) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 50) {
                mCategory = "I"; return;
            } else if (mPsv <= 200) {
                mCategory = "II"; return;
            } else if (mPsv <= 1000) {
                mCategory = "III"; return;
            } else { // mPsv > 1000
                mCategory = "IV"; return;
            }
        } else if (v <= 400) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 200) {
                mCategory = "II"; return;
            } else if (mPsv <= 1000) {
                mCategory = "III"; return;
            } else { // mPsv > 1000
                mCategory = "IV"; return;
            }
        } else if (v <= 2000) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 1000) {
                mCategory = "III"; return;
            } else { // mPsv > 1000
                mCategory = "IV"; return;
            }
        } else { // v > 2000
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else { // mPsv > 1000
                mCategory = "IV"; return;
            }
        }
    } else if (mTable == 2) {
        if (v <= 1) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (ps <= 1000) {
                mCategory = "SEP"; return;
            } else if (ps <= 3000) {
                mCategory = "III"; return;
            } else { // ps > 3000
                mCategory = "IV"; return;
            }
        } else if (v <= 100) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 50) {
                mCategory = "SEP"; return;
            } else if (mPsv <= 200) {
                mCategory = "I"; return;
            } else if (mPsv <= 1000) {
                mCategory = "II"; return;
            } else if (mPsv <= 3000) {
                mCategory = "III"; return;
            } else { // mPsv > 3000
                mCategory = "IV"; return;
            }
        } else if (v <= 400) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 200) {
                mCategory = "I"; return;
            } else if (mPsv <= 1000) {
                mCategory = "II"; return;
            } else if (mPsv <= 3000) {
                mCategory = "III"; return;
            } else { // mPsv > 3000
                mCategory = "IV"; return;
            }
        } else if (v <= 750) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 1000) {
                mCategory = "II"; return;
            } else if (mPsv <= 3000) {
                mCategory = "III"; return;
            } else { // mPsv > 3000
                mCategory = "IV"; return;
            }
        } else if (v <= 2000) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 1000) {
                mCategory = "II"; return;
            } else if (ps <= 4) {
                mCategory = "III"; return;
            } else { // ps > 4
                mCategory = "IV"; return;
            }
        } else { // v > 2000
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (ps <= 4) {
                mCategory = "III"; return;
            } else { // ps > 4
                mCategory = "IV"; return;
            }
        }
    } else if (mTable == 3) {
        if (v <= 1) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (ps <= 500) {
                mCategory = "SEP"; return;
            } else { // ps > 500
                mCategory = "II"; return;
            }
        } else if (v <= 20) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 200) {
                mCategory = "SEP"; return;
            } else if (ps <= 500) {
                mCategory = "II"; return;
            } else { // ps > 500
                mCategory = "III"; return;
            }
        } else if (v <= 400) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 200) {
                mCategory = "SEP"; return;
            } else if (ps <= 10) {
                mCategory = "I"; return;
            } else if (ps <= 500) {
                mCategory = "II"; return;
            } else { // ps > 500
                mCategory = "III"; return;
            }
        } else { // v > 400
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (ps <= 10) {
                mCategory = "I"; return;
            } else if (ps <= 500) {
                mCategory = "II"; return;
            } else { // ps > 500
                mCategory = "III"; return;
            }
        }
    } else if (mTable == 4) {
        if (v <= 10) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (ps <= 1000) {
                mCategory = "SEP"; return;
            } else { // ps > 1000
                mCategory = "I"; return;
            }
        } else if (v <= 20) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 10000) {
                mCategory = "SEP"; return;
            } else { // mPsv > 10000
                mCategory = "II"; return;
            }
        } else if (v <= 1000) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 10000) {
                mCategory = "SEP"; return;
            } else if (ps <= 500) {
                mCategory = "I"; return;
            } else { // ps > 500
                mCategory = "II"; return;
            }
        } else { // v > 1000
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (ps <= 10) {
                mCategory = "SEP"; return;
            } else if (ps <= 500) {
                mCategory = "I"; return;
            } else { // ps > 500
                mCategory = "II"; return;
            }
        }
    } else if (mTable == 5) {
        if (v <= 2) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else { // ps > 0.5
                mCategory = "SEP"; return;
            }
        } else if (v <= 6.25) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 50) {
                mCategory = "I"; return;
            } else if (ps <= 32) {
                mCategory = "II"; return;
            } else { // ps > 32
                mCategory = "IV"; return;
            }
        } else if (v <= 93.75) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 50) {
                mCategory = "I"; return;
            } else if (mPsv <= 200) {
                mCategory = "II"; return;
            } else if (ps <= 32) {
                mCategory = "III"; return;
            } else { // ps > 32
                mCategory = "IV"; return;
            }
        } else if (v <= 100) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 50) {
                mCategory = "I"; return;
            } else if (mPsv <= 200) {
                mCategory = "II"; return;
            } else if (mPsv <= 3000) {
                mCategory = "III"; return;
            } else { // mPsv > 3000
                mCategory = "IV"; return;
            }
        } else if (v <= 400) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 200) {
                mCategory = "II"; return;
            } else if (mPsv <= 3000) {
                mCategory = "III"; return;
            } else { // mPsv > 3000
                mCategory = "IV"; return;
            }
        } else if (v <= 1000) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsv <= 3000) {
                mCategory = "III"; return;
            } else { // mPsv > 3000
                mCategory = "IV"; return;
            }
        } else { // v > 1000
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else { // ps > 0.5
                mCategory = "IV"; return;
            }
        }
    } else if (mTable == 6) {
        if (dn <= 25) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else { // ps > 0.5
                mCategory = "SEP"; return;
            }
        } else if (dn <= 100) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 1000) {
                mCategory = "I"; return;
            } else { // mPsdn > 1000
                mCategory = "II"; return;
            }
        } else if (dn <= 350) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 3500) {
                mCategory = "II"; return;
            } else { // mPsdn > 3500
                mCategory = "III"; return;
            }
        } else { // dn > 350
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else { // ps > 0.5
                mCategory = "III"; return;
            }
        }
    } else if (mTable == 7) {
        if (dn <= 32) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else { // ps > 0.5
                mCategory = "SEP"; return;
            }
        } else if (dn <= 100) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 1000) {
                mCategory = "SEP"; return;
            } else { // mPsdn > 1000
                mCategory = "I"; return;
            }
        } else if (dn <= 250) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 1000) {
                mCategory = "SEP"; return;
            } else if (mPsdn <= 3500) {
                mCategory = "I"; return;
            } else { // mPsdn > 3500
                mCategory = "II"; return;
            }
        } else if (dn <= 2000) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 1000) {
                mCategory = "SEP"; return;
            } else if (mPsdn <= 3500) {
                mCategory = "I"; return;
            } else if (mPsdn <= 5000) {
                mCategory = "II"; return;
            } else { // mPsdn > 5000
                mCategory = "III"; return;
            }
        } else if (dn <= 7000) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 3500) {
                mCategory = "I"; return;
            } else if (mPsdn <= 5000) {
                mCategory = "II"; return;
            } else { // mPsdn > 5000
                mCategory = "III"; return;
            }
        } else if (dn <= 10000) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 5000) {
                mCategory = "II"; return;
            } else { // mPsdn > 5000
                mCategory = "III"; return;
            }
        } else { // dn > 10000
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else { // ps > 0.5
                mCategory = "III"; return;
            }
        }
    } else if (mTable == 8) {
        if (dn <= 25) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else { // ps > 0.5
                mCategory = "SEP"; return;
            }
        } else if (dn <= 200) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 2000) {
                mCategory = "SEP"; return;
            } else if (ps <= 500) {
                mCategory = "II"; return;
            } else { // ps > 500
                mCategory = "III"; return;
            }
        } else if (dn <= 4000) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 2000) {
                mCategory = "SEP"; return;
            } else if (ps <= 10) {
                mCategory = "I"; return;
            } else if (ps <= 500) {
                mCategory = "II"; return;
            } else { // ps > 500
                mCategory = "III"; return;
            }
        } else { // dn > 4000
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (ps <= 10) {
                mCategory = "I"; return;
            } else if (ps <= 500) {
                mCategory = "II"; return;
            } else { // ps > 500
                mCategory = "III"; return;
            }
        }
    } else if (mTable == 9) {
        if (dn <= 200) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else { // ps > 0.5
                mCategory = "SEP"; return;
            }
        } else if (dn <= 500) {
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (mPsdn <= 5000) {
                mCategory = "SEP"; return;
            } else if (ps <= 500) {
                mCategory = "I"; return;
            } else { // ps > 500
                mCategory = "II"; return;
            }
        } else { // dn > 500
            if (ps <= 0.5) {
                mCategory = "NA"; return;
            } else if (ps <= 10) {
                mCategory = "SEP"; return;
            } else if (ps <= 500) {
                mCategory = "I"; return;
            } else { // ps > 500
                mCategory = "II"; return;
            }
        }
    }
}

void PENG_DeterminePedClass::setModuleAndException() {
    if (mCategory == "NA") {
        mModules = "PED not applicable";
    } else if (mCategory == "SEP") {
        mModules = "Sound (good) Engineering Practice (SEP)";
    } else if (mCategory == "I") {
        mModules = "A";
    } else if (mCategory == "II") {
        mModules = "A1, D1, E1";
    } else if (mCategory == "III") {
        mModules = "B1+D, B1+F, B+E, B+C1, H";
    } else if (mCategory == "IV") {
        mModules = "B+D, B+F, G, H1";
	}
	
    if (mNote == "Exception") {
        if (mTable == 1) {
            mNote = "exception to Table 1:<br> Exceptionally, vessels intended to contain an unstable gas and falling within categories I or II on the basis of Table 1 <br> \
                must be classified in Category III.";
        } else if (mTable == 2) {
            mNote = "exception to Table 2:<br> \
                Exceptionally, pormTable extinguishers and bottles for breathing equipment must be classified at least in Category III.";
        /*} else if (mTable == 3) {
            RB_DEBUG->print("&nbsp;&nbsp;&nbsp;and exception to Table 3");*/
        } else if (mTable == 4) {
            mNote = "exception to Table 4:<br> \
				Exceptionally, assemblies intended for generating warm water as referred to in Article 3, Section 2.3, must be subject <br> \
				either to an EC design examination (Module B1) with respect to their conformity with the essential requirements referred <br> \
				to in Sections 2.10, 2.11, 3.4, 5 (a) and 5 (d) of Annex I, or to full quality assurance (Module H).";
        } else if (mTable == 5) {
            mNote = "exception to Table 5: <br> \
				Exceptionally, the design of pressure-cookers must be subject to a conformity assessment procedure equivalent to at least <br> \
                one of the Category III modules.";
        } else if (mTable == 6) {
            mNote = "exception to Table 6:<br> \
                Exceptionally, piping intended for unstable gases and falling within categories I or II on the basis of Table 6 must be <br> \
                classified in Category III.";
        } else if (mTable == 7) {
            mNote = "exception to Table 7:<br> \
                Exceptionally, all piping containing fluids at a temperature greater than 350 C and falling within Category II <br> \
                on the basis of Table 7 must be classified in Category III.";
        /*} else if (mTable == 8) {
            RB_DEBUG->print("&nbsp;&nbsp;&nbsp; and exception to mTable 8:");
        } else if (output->mTable == 9) {
            RB_DEBUG->print("&nbsp;&nbsp;&nbsp; and exception to mTable 9:");*/
		}
    } else if (mNote == "Safety_Accessory") {
        mNote = "exception: The safety accessories defined in Article 1, <br> \
            Section 2.1.3, and referred to in Article 3, Section 1.4, are classified in Category IV. However, by way of exception, safety<br> \
            accessories manufactured for specific equipment may be classified in the same Category as the equipment they protect.";
	}
}	

