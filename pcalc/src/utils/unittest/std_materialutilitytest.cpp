#include "std_materialutilitytest.h"

#include "pcalc_objectfactory.h"
#include "std_materialutility.h"

STD_MaterialUtilityTest::STD_MaterialUtilityTest()
            : Biluna::Calc::RB_UnitTest() {
    mTestUuid = "{01234567-0123-0123-0123-012345678901}"; // 8-4-4-4-12
}

STD_MaterialUtilityTest::~STD_MaterialUtilityTest() {
    // nothing
}

void STD_MaterialUtilityTest::exec() {
    setupTarget();

    get_Rp02Test();
    get_Rp10Test();
    get_RmMinTest();

    delete STD_MATERIALUTILITY;
}

void STD_MaterialUtilityTest::setupTarget() {
    RB_ObjectBase* mat =
            STD_MATERIALUTILITY->mMaterialList->newObject(mTestUuid);
    mat->setValue("mname", "testmaterial");

    RB_ObjectContainer* rp02List = mat->getContainer("STD_Rp02List");
    RB_ObjectBase* rp02 = rp02List->newObject("");
    rp02->setValue("temperature", -10.0);
    rp02->setValue("rp02", -5.0);
    rp02 = rp02List->newObject("");
    rp02->setValue("temperature", 10.0);
    rp02->setValue("rp02", 100.0);
    rp02 = rp02List->newObject("");
    rp02->setValue("temperature", 20.0);
    rp02->setValue("rp02", 50.0);

    RB_ObjectContainer* rp10List = mat->getContainer("STD_Rp10List");
    RB_ObjectBase* rp10 = rp10List->newObject("");
    rp10->setValue("temperature", -10.0);
    rp10->setValue("rp10", -5.0);
    rp10 = rp10List->newObject("");
    rp10->setValue("temperature", 10.0);
    rp10->setValue("rp10", 100.0);
    rp10 = rp10List->newObject("");
    rp10->setValue("temperature", 20.0);
    rp10->setValue("rp10", 50.0);

    RB_ObjectContainer* rmminList = mat->getContainer("STD_RmMinList");
    RB_ObjectBase* rmmin = rmminList->newObject("");
    rmmin->setValue("temperature", -10.0);
    rmmin->setValue("rmmin", -5.0);
    rmmin = rmminList->newObject("");
    rmmin->setValue("temperature", 10.0);
    rmmin->setValue("rmmin", 100.0);
    rmmin = rmminList->newObject("");
    rmmin->setValue("temperature", 20.0);
    rmmin->setValue("rmmin", 50.0);
}

void STD_MaterialUtilityTest::get_Rp02Test() {
    QString matIdx = mTestUuid + "testmaterial";
    STD_MATERIALUTILITY->setCurrentMaterial(matIdx);

    double designTemp = -11.0; // out of bound
    double value = STD_MATERIALUTILITY->get_Rp02(designTemp);
    PR->addDetail("STD 901","Rp02", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_Rp02Test()",
             -5.0, value);

    designTemp = 0.0;
    value = STD_MATERIALUTILITY->get_Rp02(designTemp);
    PR->addDetail("STD 901","Rp02", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_Rp02Test()",
             47.5, value);

    designTemp = 15.0;
    value = STD_MATERIALUTILITY->get_Rp02(designTemp);
    PR->addDetail("STD 901","Rp02", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_Rp02Test()",
             75.0, value);

    designTemp = 35.0; // out of bound
    value = STD_MATERIALUTILITY->get_Rp02(designTemp);
    PR->addDetail("STD 901","Rp02", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_Rp02Test()",
             50.0, value);
}

void STD_MaterialUtilityTest::get_Rp10Test() {
    QString matIdx = mTestUuid + "testmaterial";
    STD_MATERIALUTILITY->setCurrentMaterial(matIdx);

    double designTemp = -11.0; // out of bound
    double value = STD_MATERIALUTILITY->get_Rp10(designTemp);
    PR->addDetail("STD 901","Rp10", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_Rp10Test()",
             -5.0, value);

    designTemp = 0.0;
    value = STD_MATERIALUTILITY->get_Rp10(designTemp);
    PR->addDetail("STD 901","Rp10", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_Rp10Test()",
             47.5, value);

    designTemp = 15.0;
    value = STD_MATERIALUTILITY->get_Rp10(designTemp);
    PR->addDetail("STD 901","Rp10", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_Rp10Test()",
             75.0, value);

    designTemp = 35.0; // out of bound
    value = STD_MATERIALUTILITY->get_Rp10(designTemp);
    PR->addDetail("STD 901","Rp10", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_Rp10Test()",
             50.0, value);
}

void STD_MaterialUtilityTest::get_RmMinTest() {
    QString matIdx = mTestUuid + "testmaterial";
    STD_MATERIALUTILITY->setCurrentMaterial(matIdx);

    double designTemp = -11.0; // out of bound
    double value = STD_MATERIALUTILITY->get_RmMin(designTemp);
    PR->addDetail("STD 901","RmMin", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_RmMinTest()",
             -5.0, value);

    designTemp = 0.0;
    value = STD_MATERIALUTILITY->get_RmMin(designTemp);
    PR->addDetail("STD 901","RmMin", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_RmMinTest()",
             47.5, value);

    designTemp = 15.0;
    value = STD_MATERIALUTILITY->get_RmMin(designTemp);
    PR->addDetail("STD 901","RmMin", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_RmMinTest()",
             75.0, value);

    designTemp = 35.0; // out of bound
    value = STD_MATERIALUTILITY->get_RmMin(designTemp);
    PR->addDetail("STD 901","RmMin", "from table",
                  value, "MPa", "table value");
    areEqual(PR->getLastOutput(),
             "STD_MaterialUtilityTest::get_RmMinTest()",
             50.0, value);
}
