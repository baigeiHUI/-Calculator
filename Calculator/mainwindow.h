#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStack>
#include<QQueue>
#include<QMap>
#include"history_recording.h"
#include"ui_history_recording.h"
#include"QVector"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QVector<QString>Operator_Vector;
    QStack<QString>Operator_Stack;
    QStack<double>OperaNum_Stack;
    QMap<QString,int>Prior_Bound;
    History_Recording *hr=new History_Recording();
    int Prior;


public:
    MainWindow(QWidget *parent = nullptr);
    void Calculate();
    void Deal_Parly();
    int Get_Prior(QString s);
    ~MainWindow();
    void InitOpPrior();


private slots:



    void on_duoxaingshi_Button_clicked();

    void on_Tri_Button_clicked();

    void on_f_Func_Button_clicked();

    void on_History_Rec_clicked();

    void on_F_E_Button_clicked();

    void on_nd_2_clicked();

    void on_R_D_Exchange_Button_clicked();

    void on_Left_bra_Button_clicked();

    void on_Right_Bra_Button_clicked();

    void on_Button_7_clicked();

    void on_Button_8_clicked();

    void on_Button_9_clicked();

    void on_Button_4_clicked();

    void on_Button_5_clicked();

    void on_Button_6_clicked();

    void on_Button_1_clicked();

    void on_Button_2_clicked();

    void on_Button_3_clicked();

    void on_Button_0_clicked();

    void on_divButton_clicked();

    void on_Mul_Button_clicked();

    void on_Sub_Button_clicked();

    void on_Add_Button_clicked();

    void on_Point_Button_clicked();

    void on_Equal_Button_clicked();

    void on_Pi_Button_clicked();

    void on_Get_ModButton_clicked();

    void on_CLC_Button_clicked();

    void on_del_Button_clicked();

    void on_Sin_Button_clicked();

    void on_Cos_Button_clicked();

    void on_Tan_Button_clicked();

    void on_Sec_Button_clicked();

    void on_Csc_Button_clicked();

    void on_Cot_Button_clicked();

    void on_e_Button_clicked();

    void on_x2_Button_clicked();

    void on_sqrt_xButton_clicked();

    void on_x_fuyici_Button_clicked();

    void on_absx_Button_clicked();

    void on_mi_10_d_xciButton_clicked();

    void on_jiecheng_clicked();

    void on_xd_ycimi_Button_clicked();

    void on_x_10dxcimi_Button_clicked();

    void on_lnx_Button_clicked();

    void on_logx_Button_clicked();

    void on_Random_Button_clicked();

    void on_xiangshang_Button_clicked();

    void on_xiangxiaqu_Button_clicked();




    void on_symbol_exchange_Button_clicked();


private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
