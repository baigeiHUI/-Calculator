#ifndef SUB_WINDOW_H
#define SUB_WINDOW_H

#include <QMainWindow>
#include<QMap>
#include"QQueue"
#include"history_recording.h"
#include"ui_history_recording.h"
namespace Ui {
class Sub_Window;
}

class Sub_Window : public QMainWindow
{
    Q_OBJECT

    QMap<int,double>input_1;
    QMap<int,double>input_2;
    QQueue<QString>Operator_Queue;
    QString Part_Deal[5];
    History_Recording *hr=new History_Recording;



public:
    void Piece_Matched_IntoMap(QMap<int, double> &map0);
    void Put_Little_Piece_Into_Array(QString s0);
    void Put_Cut_StringIntoQueue(QString s);
    void GetInput();
    void InitStringArray();
    void Add_Achieved();
    void Sub_Achieved();
    void Mul_Achieved();
    void ShowAnswer() ;
    void ShowAnswer_Reverse();
    explicit Sub_Window(QWidget *parent = nullptr);
    ~Sub_Window();


private slots:
    void on_CLC_Button_clicked();

    void on_Compute_Button_clicked();


    void on_History_Rec1_clicked();

private:
    Ui::Sub_Window *ui;
};

#endif // SUB_WINDOW_H
