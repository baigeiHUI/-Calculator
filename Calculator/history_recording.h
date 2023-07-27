#ifndef HISTORY_RECORDING_H
#define HISTORY_RECORDING_H

#include <QWidget>


namespace Ui {
class History_Recording;
}

class History_Recording : public QWidget
{
public:
    Q_OBJECT

public:
    explicit History_Recording(QWidget *parent = nullptr);
    ~History_Recording();
    Ui::History_Recording *ui;
private slots:
    void on_CLC_Button_clicked();
};

#endif // HISTORY_RECORDING_H
