#include "history_recording.h"
#include "ui_history_recording.h"

History_Recording::History_Recording(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::History_Recording)
{
    ui->setupUi(this);
    setWindowTitle("History_Recording");
    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());
}

History_Recording::~History_Recording()
{
    delete ui;
}

void History_Recording::on_CLC_Button_clicked()
{
    ui->textBrowser->clear();
    ui->textBrowser->setAlignment(Qt::AlignRight);
}

