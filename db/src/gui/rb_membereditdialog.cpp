#include "rb_dialogfactory.h"
#include "rb_membereditdialog.h"
#include "ui_rb_membereditdialog.h"

#include "rb_memberdelegate.h"

RB_MemberEditDialog::RB_MemberEditDialog(QWidget *parent) :
    RB_Dialog(parent), ui(new Ui::RB_MemberEditDialog) {

    ui->setupUi(this);
}

RB_MemberEditDialog::~RB_MemberEditDialog() {
    delete ui;
}

RB_MemberEditDialog::showObjectMembers(RB_ObjectBase* obj,
                                      const QStringList& memberList) {
    ui->twMember->clear();
    mObject = obj;
    mMemberList = memberList;

    if (mMemberList.count() < 1 || !obj) {
        RB_DEBUG->error("RB_MemberEditDialog::setObjectMembers() ERROR");
        return;
    }

    int colCount = 2;
    int rowCount = memberList.count();

    ui->twMember->setColumnCount(colCount);
    ui->twMember->setRowCount(rowCount);
    ui->twMember->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    ui->twMember->setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));

    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem* item = new QTableWidgetItem(memberList.at(row));
        item->setBackgroundColor(QPalette::color(QPalette::Active,
                                                 QPalette::Button));
        ui->twMember->setItem(row, 0, item);
        item = new QTableWidgetItem(mObject->getValue(row).toString());
        ui->twMember->setItemDelegate(new RB_MemberDelegate(ui->twMember));
    }
}

void RB_MemberEditDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

void RB_MemberEditDialog::on_pbOk_clicked() {

}
