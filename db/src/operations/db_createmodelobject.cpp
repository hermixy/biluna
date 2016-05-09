/*****************************************************************
 * $Id: db_createmodelobject.h 0001 2015-12-28 09:13:00Z rutger $
 * Created: Dec 28, 2015 10:14:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Database (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_createmodelobject.h"
#include "db_dialogfactory.h"


DB_CreateModelObject::DB_CreateModelObject() {

}

bool DB_CreateModelObject::execute(RB_ObjectBase* memberList) {
    mHeaderContent = "";
    mSourceContent = "";

    createHeaderContent();
    createSourceContent(memberList);
    return writeFiles();
}

void DB_CreateModelObject::setBaseObject(const QString& baseObjName) {
    mBaseObjectName = baseObjName;
}

void DB_CreateModelObject::setPerspective(const QString& perspCode) {
    mPerspectiveCode = perspCode;
}

void DB_CreateModelObject::setObjectName(const QString& objName) {
    mObjectName = objName;
}

void DB_CreateModelObject::setDescription(const QString& description) {
    mDescription = description;
}

void DB_CreateModelObject::setFilePath(const QString& filePath) {
    mFilePath = filePath;
}

void DB_CreateModelObject::createHeaderContent() {
    setFileHeader(mHeaderContent, "h");
    mHeaderContent += "\n";
    QString objNameUpperPlusH = mPerspectiveCode.toUpper() + "_"
            + mObjectName.toUpper() + "_H";
    mHeaderContent += "#ifndef " + objNameUpperPlusH + "\n";
    mHeaderContent += "#define " + objNameUpperPlusH + "\n";
    mHeaderContent += "\n";
    mHeaderContent += "#include \"" + mBaseObjectName.toLower() + ".h\"\n";
    mHeaderContent += "\n";
    mHeaderContent += "/**\n";
    mHeaderContent += " * " + mDescription + "\n";
    mHeaderContent += " */\n";
    QString objCamelCase = mPerspectiveCode.toUpper() + "_" + mObjectName;
    mHeaderContent += "class DB_EXPORT " + objCamelCase + " : public "
            + mBaseObjectName + " {\n";
    mHeaderContent += "\n";
    mHeaderContent += "public:\n";
    mHeaderContent +=
            "    " + objCamelCase
            + "(const QString& id = \"\", RB_ObjectBase* p = NULL,\n"
              "               const QString& n = \"" + objCamelCase
            + "\",\n               RB_ObjectFactory* f = NULL);\n";
    mHeaderContent += "    " + objCamelCase + "(" + objCamelCase + "* obj);\n";
    mHeaderContent += "    virtual ~" + objCamelCase + "();\n";
    mHeaderContent += "\n";
    mHeaderContent += "private:\n";
    mHeaderContent += "    void createMembers();\n";
    mHeaderContent += "\n";
    mHeaderContent += "};\n";
    mHeaderContent += "\n";
    mHeaderContent += "#endif /*" + objNameUpperPlusH + "*/\n";
}

void DB_CreateModelObject::createSourceContent(RB_ObjectBase* memberList) {
    setFileHeader(mSourceContent, "cpp");
    mSourceContent += "\n";
    mSourceContent += "#include \"" + mPerspectiveCode.toLower()
            + "_" + mObjectName.toLower() + ".h\"\n";
    mSourceContent += "\n";
    QString objCamelCase = mPerspectiveCode.toUpper() + "_" + mObjectName;
    mSourceContent += objCamelCase + "::" + objCamelCase + "(\n"
            "                   const QString& id, RB_ObjectBase* p,\n"
            "                   const QString& n, RB_ObjectFactory* f)\n"
            "					: " + mBaseObjectName + " (id, p, n, f) {\n"
            "	createMembers();\n"
            "}\n";
    mSourceContent += "\n";
    mSourceContent += objCamelCase + "::" + objCamelCase  + "(" + objCamelCase
        + "* obj) : \n                   " + mBaseObjectName + "(obj) {\n"
        "	createMembers();\n"
        "	*this = *obj;\n"
        "}\n";
    mSourceContent += "\n";
    mSourceContent += objCamelCase + "::~" + objCamelCase  + "() {\n"
        "	// clean up children done in RB_ObjectBase and RB_ObjectContainer\n"
        "}\n";
    mSourceContent += "\n";
    mSourceContent += "/**\n";
    mSourceContent += " * Create members:\n";

    RB_ObjectIterator* iter = memberList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        QString memberName = obj->getValue("member").toString();
        formatMemberName(memberName);
        QString description = obj->getValue("description").toString();
        mSourceContent += " * \\li " + memberName + " " + description + "\n";
    }

    // NOTE: iter is reused below, delete is further below
    mSourceContent += " */\n";


    mSourceContent += "void " + objCamelCase + "::createMembers() {\n";

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        QString memberName = obj->getValue("member").toString();
        formatMemberName(memberName);

        mSourceContent += "    addMember(\""
                + memberName + "\", \""
                + obj->getValue("unit").toString() + "\", ";
        QString memberType = "RB2::" + obj->getValue("type").toString();

        if (obj->getValue("default").toString().isEmpty()) {
            if (memberType.contains("MemberChar")
                    || memberType.contains("MemberString")) {
                mSourceContent += "\"\", ";
            } else if (memberType.contains("MemberDate")) {
                mSourceContent += "\"1970-01-01\", ";
            } else {
                mSourceContent += "0, ";
            }
        } else {
            if (memberType.contains("MemberChar")
                    || memberType.contains("MemberString")
                    || memberType.contains("MemberDate")) {
                mSourceContent += "\"" + obj->getValue("default").toString()
                        + "\", ";
            } else {
                mSourceContent += obj->getValue("default").toString() + ", ";
            }
        }

        mSourceContent += memberType + ");\n";
    }

    delete iter;

    mSourceContent += "}\n";
}

void DB_CreateModelObject::setFileHeader(QString& content,
                                         const QString& fileExtension) {
    content +=
    "/*****************************************************************\n"
    " * $Id: " + mPerspectiveCode.toLower() + "_" + mObjectName.toLower()
            + "." + fileExtension + " 0001 "
            + QDateTime::currentDateTime().toString(Qt::ISODate)
            + " rutger $\n"
    " *\n"
    " * Copyright (C) " + QString::number(QDate::currentDate().year())
            + " Red-Bag. All rights reserved.\n"
    " * This file is part of the Biluna " + mPerspectiveCode.toUpper()
            + " project.\n"
    " *\n"
    " * See http://www.biluna.com for further details.\n"
              " *****************************************************************/\n";
}

void DB_CreateModelObject::formatMemberName(QString& memberName) {
    memberName = memberName.simplified();
    memberName = memberName.remove(" ");
    memberName = memberName.toLower();
}

bool DB_CreateModelObject::writeFiles() {
    QString fullFileName = mFilePath + "/" + mPerspectiveCode.toLower() + "_"
            + mObjectName.toLower();
    QFile headerFile(fullFileName + ".h");
    if (!headerFile.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }

    QTextStream outHeader(&headerFile);
    outHeader << mHeaderContent;
    // If QTextStream operates on a string, flush does nothing.
    // outHeader.flush();

    QFile sourceFile(fullFileName + ".cpp");
    if (!sourceFile.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }

    QTextStream outSource(&sourceFile);
    outSource << mSourceContent;

    return true;
}

