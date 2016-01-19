#include "rb_membereditdialog.h"

#include "db_dialogfactory.h"
#include "rb_memberdelegate.h"
#include "ui_rb_membereditdialog.h"


RB_MemberEditDialog::RB_MemberEditDialog(QWidget *parent) :
    RB_Dialog(parent), ui(new Ui::RB_MemberEditDialog) {

    ui->setupUi(this);
}

RB_MemberEditDialog::~RB_MemberEditDialog() {
    delete ui;
}

void RB_MemberEditDialog::showObjectMembers(RB_ObjectBase* obj,
                                            const QStringList& memberList) {
    ui->twMember->clear();
    mObject = obj;
    mMemberList = memberList;

    if (mMemberList.count() < 1 || !obj) {
        RB_DEBUG->error("RB_MemberEditDialog::setObjectMembers() ERROR");
        return;
    }

    int colCount = 2;
    int rowCount = obj->memberCount();

    ui->twMember->setColumnCount(colCount);
    ui->twMember->setRowCount(rowCount);
    ui->twMember->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    ui->twMember->setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));

    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem* item;
        RB_ObjectMember* mem = mObject->getMember(row);
        QString memberName = mem->getName();
        item = new QTableWidgetItem(memberName);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        QPalette palette = QGuiApplication::palette();
        item->setBackgroundColor(palette.color(QPalette::Active,
                                               QPalette::Button));
        ui->twMember->setItem(row, 0, item);

        item = new QTableWidgetItem(mem->getValue().toString());
        RB_MemberDelegate* del = new RB_MemberDelegate(ui->twMember);
        del->setObject(obj);
        ui->twMember->setItemDelegate(del);
        ui->twMember->setItem(row, 1, item);

        if (!mMemberList.contains(memberName)) {
            ui->twMember->hideRow(row);
        }
    }
}

void RB_MemberEditDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

void RB_MemberEditDialog::on_pbOk_clicked() {
    int valueCol = 1;
    int rowCount = ui->twMember->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem* item = ui->twMember->item(row, valueCol);
        RB_ObjectMember* mem = mObject->getMember(row);

        if (!mem) {
            RB_DEBUG->error("RB_MemberEditDialog::on_pbOk_clicked()");
            return;
        }

        if (mMemberList.contains(mem->getName())) {
            if (mem->getType() == RB2::MemberDouble) {
                double value = item->text().toDouble();
                mem->setValue(value);
            } else if (mem->getType() == RB2::MemberInteger) {
                int value = item->text().toInt();
                mem->setValue(value);
            } else {
                mem->setValue(item->text());
            }
        }
    }

    RB_Dialog::accept();
}

void RB_MemberEditDialog::on_pbCancel_clicked() {
    RB_Dialog::reject();
}
