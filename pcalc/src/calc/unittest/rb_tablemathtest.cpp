#include "rb_tablemathtest.h"
#include "rb_objectatomic.h"
NAMESPACE_BILUNA_CALC


RB_TableMathTest::RB_TableMathTest() : RB_UnitTest() {
    target = NULL;
}

RB_TableMathTest::~RB_TableMathTest() {
    delete target;
    target = NULL;
}

void RB_TableMathTest::exec() {
    getBilinearValueTest();
    getLinearValueTest();
    updateCornerObjectsTest();
    getThreeCornerValueTest();
    getTwoCornerValueTest();
    getOneCornerValueTest();
}

void RB_TableMathTest::setupTarget() {
    if (!target) {
        target = new RB_TableMath();
    }
}

/**
 * @brief Values based on http://en.wikipedia.org/wiki/Bilinear_interpolation
 */
void RB_TableMathTest::getBilinearValueTest() {
    setupTarget();
    double res = target->getBilinearValue(14.5, 20.2,
                                          14.0, 20.0, 91.0,
                                          15.0, 20.0, 210,
                                          14.0, 21.0, 162.0,
                                          15.0, 21.0, 95.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 146.1, res);
    res = target->getBilinearValue(1.0, 0.0,
                                   -1.0, 3.0, 1.0,
                                   3.0, 2.0, 2.0,
                                   -2.0, -1.0, 5.0,
                                   2.0, -2.0, 6.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 4.0, res);
    res = target->getBilinearValue(-1.0, 0.0,
                                   -1.0, 3.0, 2.0,
                                   -1.0, 3.0, 2.0,
                                   -1.0, -1.0, 5.0,
                                   -1.0, -1.0, 5.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 4.25, res);
    res = target->getBilinearValue(1.0, 0.0,
                                   -1.0, 0.0, 1.0,
                                   3.0, 0.0, 6.0,
                                   -1.0, 0.0, 1.0,
                                   3.0, 0.0, 6.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 3.5, res);
    res = target->getBilinearValue(1.0, 0.0,
                                   1.0, 0.0, 1.3,
                                   1.0, 0.0, 1.3,
                                   1.0, 0.0, 1.3,
                                   1.0, 0.0, 1.3);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 1.3, res);
}

void RB_TableMathTest::getLinearValueTest() {
    setupTarget();
    double res = target->getLinearValue(1.25,
                                        0.1, -7.0,
                                        3.3, 18.1);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getLinearValueTest()", 2.0203125, res);
    res = target->getLinearValue(1.25,
                                 1.25, -7.0,
                                 1.25, 18.1);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getLinearValueTest()", 5.55, res);
}

void RB_TableMathTest::updateCornerObjectsTest() {
    target->mXfield = "x";
    target->mYfield = "y";
    target->mZfield = "z";

    target->mXvalue = 0.5;
    target->mYvalue = 0.5;

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    double test = 1.0101;

    // Top left
    RB_ObjectBase* obj1 = new RB_ObjectAtomic("", nullptr, "");
    obj1->addMember("x", "-", -1.5);
    obj1->addMember("y", "-", 1.5);
    obj1->addMember("z", "-", test);

    target->updateCornerObjects(obj1);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Top right
    obj1->setValue("x", 1.5);
    obj1->setValue("y", 1.5);

    target->updateCornerObjects(obj1);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Bottom left
    obj1->setValue("x", -1.5);
    obj1->setValue("y", -1.5);

    target->updateCornerObjects(obj1);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Bottom right
    obj1->setValue("x", 1.5);
    obj1->setValue("y", -1.5);

    target->updateCornerObjects(obj1);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // 'on the spot'
    obj1->setValue("x", 0.5);
    obj1->setValue("y", 0.5);

    target->updateCornerObjects(obj1);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    double test2 = 2.0202;

    // Top left insert
    obj1->setValue("x", -1.5);
    obj1->setValue("y", 1.5);
    RB_ObjectBase* obj2 = new RB_ObjectAtomic("", nullptr, "");
    obj2->addMember("x", "-", -1.0);
    obj2->addMember("y", "-", 1.0);
    obj2->addMember("z", "-", test2);

    target->updateCornerObjects(obj1);
    target->updateCornerObjects(obj2);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test2,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Top right insert
    obj1->setValue("x", 1.5);
    obj1->setValue("y", 1.5);
    obj2->setValue("x", 1.0);
    obj2->setValue("y", 1.0);

    target->updateCornerObjects(obj1);
    target->updateCornerObjects(obj2);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test2,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Bottom left insert
    obj1->setValue("x", -1.5);
    obj1->setValue("y", -1.5);
    obj2->setValue("x", -1.0);
    obj2->setValue("y", -1.0);

    target->updateCornerObjects(obj1);
    target->updateCornerObjects(obj2);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test2,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Bottom right insert
    obj1->setValue("x", 1.5);
    obj1->setValue("y", -1.5);
    obj2->setValue("x", 1.0);
    obj2->setValue("y", -1.0);

    target->updateCornerObjects(obj1);
    target->updateCornerObjects(obj2);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test2,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Bottom right insert with equal x value 1
    obj1->setValue("x", 1.5);
    obj1->setValue("y", -1.5);
    obj2->setValue("x", 1.5);
    obj2->setValue("y", -1.0);

    target->updateCornerObjects(obj1);
    target->updateCornerObjects(obj2);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test2,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Bottom right insert with equal x value 2 (reverse order of obj1 and obj2)
    obj1->setValue("x", 1.5);
    obj1->setValue("y", -1.5);
    obj2->setValue("x", 1.5);
    obj2->setValue("y", -1.0);

    target->updateCornerObjects(obj2);
    target->updateCornerObjects(obj1); // reverse order
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", test2,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Top left not valid z value
    obj1->setValue("x", -1.5);
    obj1->setValue("y", 1.5);
    target->mExtraField = "z";
    target->mExtraValue = 9.0;

    target->updateCornerObjects(obj1);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopLeft ? target->mTopLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mTopRight ? target->mTopRight->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomLeft ? target->mBottomLeft->getValue("z").toDouble() : 0.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::updateCornerObjectsTest()", 0.0,
             target->mBottomRight ? target->mBottomRight->getValue("z").toDouble() : 0.0);

    delete obj1;
    delete obj2;
}

void RB_TableMathTest::getThreeCornerValueTest() {
    target->mXfield = "x";
    target->mYfield = "y";
    target->mZfield = "z";

    target->mXvalue = 0.5;
    target->mYvalue = 0.5;

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Top right
    RB_ObjectBase* objTR = new RB_ObjectAtomic("", nullptr, "");
    objTR->addMember("x", "-", 1.4);
    objTR->addMember("y", "-", 1.5);
    objTR->addMember("z", "-", 2.0);
    target->mTopRight = objTR;
    // Bottom left
    RB_ObjectBase* objBL = new RB_ObjectAtomic("", nullptr, "");
    objBL->addMember("x", "-", -1.6);
    objBL->addMember("y", "-", -1.7);
    objBL->addMember("z", "-", 2.0);
    target->mBottomLeft = objBL;
    // Bottom right
    RB_ObjectBase* objBR = new RB_ObjectAtomic("", nullptr, "");
    objBR->addMember("x", "-", 1.8);
    objBR->addMember("y", "-", -1.9);
    objBR->addMember("z", "-", 2.0);
    target->mBottomRight = objBR;

    double res = target->getThreeCornerValue();
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::getThreeCornerValueTest()", 2.0, res);
    delete objTR;
    delete objBL;
    delete objBR;
}

void RB_TableMathTest::getTwoCornerValueTest() {
    target->mXfield = "x";
    target->mYfield = "y";
    target->mZfield = "z";

    target->mXvalue = 0.0;
    target->mYvalue = 1.0;

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    // Top right
    RB_ObjectBase* objTR = new RB_ObjectAtomic("", nullptr, "");
    objTR->addMember("x", "-", 3.0);
    objTR->addMember("y", "-", 1.0);
    objTR->addMember("z", "-", 2.0);
    target->mTopRight = objTR;
    // Bottom Left
    RB_ObjectBase* objBL = new RB_ObjectAtomic("", nullptr, "");
    objBL->addMember("x", "-", 0.0);
    objBL->addMember("y", "-", 0.0);
    objBL->addMember("z", "-", 1.0);
    target->mBottomRight = objBL;

    double res = target->getTwoCornerValue();
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::getTwoCornerValueTest()", 1.25, res);
    delete objTR;
    delete objBL;
}

void RB_TableMathTest::getOneCornerValueTest() {
    target->mXfield = "x";
    target->mYfield = "y";
    target->mZfield = "z";

    target->mXvalue = 0.5;
    target->mYvalue = 0.5;

    target->mTopLeft = nullptr;
    target->mTopRight = nullptr;
    target->mBottomLeft = nullptr;
    target->mBottomRight = nullptr;

    RB_ObjectBase* obj = new RB_ObjectAtomic("", nullptr, "");
    obj->addMember("x", "-", -3.3);
    obj->addMember("y", "-", 1.34);
    obj->addMember("z", "-", 2.13);
    target->mTopRight = obj;

    double res = target->getOneCornerValue();
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::getOneCornerValueTest()", 2.13, res);
    delete obj;
}

END_NAMESPACE_BILUNA_CALC
