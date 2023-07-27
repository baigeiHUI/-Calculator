#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"sub_window.h"
#include"history_recording.h"
#include"ui_history_recording.h"
#include"QtMath"
#include"QRegularExpression"
#include"QRegularExpressionMatch"
#include"QRegularExpressionMatchIterator"
#include"iterator"
#include"QDebug"
#include"QVector"


//需要匹配的式子与操作数:
  QString s=R"(floor\(|\~\(|[+\-*\/()=%]|ceil\(|log\(|logbase\(|ln\(|\^\(|yroot\(|fact\(|abs\(|cuberoot\(|sqrt\(|sqr\(|cube\(|sin\(|cos\(|tan\(|sec\(|csc\(|cot\(|[1-9][0-9]*\.?[0-9]*|0|Π|\be\b)";//用于入队列
  QString qs_1=R"(floor\(|\~\(|[+\-*\/()=%]|ceil\(|log\(|logbase\(|ln\(|\^\(|yroot\(|fact\(|abs\(|cuberoot\(|sqrt\(|sqr\(|cube\(|sin\(|cos\(|tan\(|sec\(|csc\(|cot\()";//用于入运算符栈
  QString qs_2=R"([1-9][0-9]*\.?[0-9]*|0|Π|\be\b)"; //用于入操作数栈
  QString qs_4=R"(\w*\()";                 //括号匹配


//括号匹配(仅匹配左小括号!在计算前先匹配下)


   MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Calculator");

    //固定窗口大小
    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());
     setStyleSheet(     //设置按钮样式
        "QPushButton {"
        "border-radius: 12px;"
        "border: 1px solid #000000;"
        "color: #000000;"
        "}"

        "QPushButton:hover{"
        "border-radius: 12px;"
        "border: 1px solid #000000; "
         "background-color: qlineargradient(spread:pad,  x1:0, x2:0, y1:0, y2:1,"
        " stop: 0 rgba(200,220,240,255),"
        " stop: 0.504 rgba(173,216,230,255),"
        " stop: 0.505 rgba(171,215,225,255),"
        "stop: 1 rgba(200,220,240,255));"
        "}"

        "QPushButton:pressed{"
        " border-radius: 12px;"
        "background-color: qlineargradient(spread:pad,  x1:0, x2:0, y1:0, y2:1,"
        " stop: 0 rgba(200,220,240,255),"
        " stop: 0.504 rgba(230,230,230,255),"
        " stop: 0.505 rgba(230,230,230,255),"
        "stop: 1 rgba(200,220,225,255));"
        " border: 1px solid #000000;  "

        "}"


        );

    //此代码修改了扩展窗口的背景色,使得其可以对初始页面进行覆盖

    QPalette p;
      ui->Tri_Show_GUI->setAutoFillBackground(true);
    p.setBrush(ui->Tri_Show_GUI->backgroundRole(),QBrush(QColor(255,255,255)));
    ui->Tri_Show_GUI->setHidden(true);
    ui->Tri_Show_GUI->setPalette(p);

    ui->f_Func_GUI->setAutoFillBackground(true);
    p.setBrush(ui->f_Func_GUI->backgroundRole(),QBrush(QColor(255,255,255)));
    ui->f_Func_GUI->setHidden(true);
    ui->f_Func_GUI->setPalette(p);
    //历史记录设置背景
   ui->History_Rec->setStyleSheet(
         "border-image:url(D:/Qt_Pro_Main/Calculator/history_rec.png)");
    ui->shizi_TextArea->setAlignment(Qt::AlignRight);
   QFont font =QFont("楷体",16);
   ui->shizi_TextArea->setFont(font);
   ui->jieguo_TextArea->setAlignment(Qt::AlignRight|Qt::AlignBottom);
    hr->ui->textBrowser->setAlignment(Qt::AlignRight);
   hr->ui->textBrowser->setFont(font);


    InitOpPrior();




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Calculate(){
    Operator_Vector.swap(*new QVector<QString>);
      Operator_Stack.clear();
    Operator_Stack.push("#");
    OperaNum_Stack.clear();
    QRegularExpression re(s);
    QRegularExpression re_1(qs_2);
    QRegularExpression re_4(qs_4);
    QString tem;
    tem=ui->shizi_TextArea->toPlainText();
    QRegularExpressionMatchIterator qem=re.globalMatch(tem);   //将操作数与运算符分隔开,入队列
    QRegularExpressionMatch match ;
    while(qem.hasNext()){
        match=qem.next();
        tem=match.captured(0);
        Operator_Vector.append(tem);

    }



    //开始处理数据!!!!!!
    re.setPattern(qs_1);
    QVector<QString>::iterator it=Operator_Vector.begin();

    while(it!=Operator_Vector.end()){      //

        if(ui->jieguo_TextArea->text()=="Error")
            return;
        tem=*it;
        if(re_1.match(tem).hasMatch())   //如果数字匹配
            {
              if(tem=="e")
                OperaNum_Stack.push(2.71828182845904523536);
              else if(tem=="Π")
                 OperaNum_Stack.push(3.14159265358979323846);
              else
                OperaNum_Stack.push(tem.toDouble());
            }
        else if(re.match(tem).hasMatch()){  //如果运算符匹配

            if(tem==")"){                   //dealpart到一个运算符包含"("为止.
                while(!re_4.match(Operator_Stack.top()).hasMatch()){
                    Deal_Parly();
                }
                Deal_Parly();


            }
            else if(re_4.match(tem).hasMatch()){
                Operator_Stack.push(tem);
            }

            else if(tem=="="){              //匹配到等号
                while(!Operator_Stack.isEmpty())   //当运算符没有匹配到最后
                   Deal_Parly();
            }
            else                            //基本运算符
            {
                if(tem=="-"){               //如果是减号
                                                     //如果是第一项是减号,什么也不做,下面考虑了,此处考虑减号位于括号后!且存在*(it-1)
                   if(it==Operator_Vector.begin()){
                       OperaNum_Stack.push(0);
                   }
                   else if(re_4.match(*(it-1)).hasMatch())//减号前一项带括号了,减号要当作负号对待!且该减号是一定可以入栈的!
                       OperaNum_Stack.push(0);       //在操作数栈中压一个0


                }

                QString qs_top=Operator_Stack.top();
                if(re_4.match(qs_top).hasMatch())
                   Operator_Stack.push(tem);
                else if(Get_Prior(tem)>Get_Prior(qs_top))
                   Operator_Stack.push(tem);
                else{
                   Deal_Parly();
                   Operator_Stack.push(tem);
                }

           }

        }
            it++;
}

 if(ui->CLC_Button->text()=="C")
        tem=QString::number(OperaNum_Stack.top());
 else
          tem=QString::number(OperaNum_Stack.top(),'e');

        ui->jieguo_TextArea->setText(tem);

 if(ui->jieguo_TextArea->text()!="Error"){
        hr->ui->textBrowser->insertPlainText(ui->shizi_TextArea->toPlainText());
        hr->ui->textBrowser->insertPlainText(tem);
        hr->ui->textBrowser->append(" ");
        hr->ui->textBrowser->append(" ");
 }

}

//获取阶乘

int get_fact(int n)
{
    if(n <= 1)
    {
        return 1;
    }
    else
    {
        return n * get_fact(n-1);
    }
}


void MainWindow::Deal_Parly(){
    //二元运算符
    QString tem=Operator_Stack.top();
    Operator_Stack.pop();
    double num2=OperaNum_Stack.top();
    OperaNum_Stack.pop();
    double tem_num;
    int p=Prior_Bound.value(tem);
 //如果是二元运算符

    if(p==2){


    double num1=OperaNum_Stack.top();
    OperaNum_Stack.pop();

    if(tem=="+")
    { tem_num=num1+num2;OperaNum_Stack.push(tem_num);}
    if(tem=="-"){
           tem_num=num1-num2;OperaNum_Stack.push(tem_num);
    }

     if(tem=="*"){
     tem_num=num1*num2;OperaNum_Stack.push(tem_num);}
     if(tem=="/"){
     if(num2==0){
                ui->jieguo_TextArea->setText("Error");
                return;
     }
    tem_num=(num1/num2);OperaNum_Stack.push(tem_num);}
     if(tem=="%"){
    if(num2==0){
                ui->jieguo_TextArea->setText("Error");
                return;
    }
    tem_num=(num1-qFloor(num1/num2)*num2);OperaNum_Stack.push(tem_num);}
     if(tem=="logbase("){
    if(num2<=0||num2==1||num1<=0){
                ui->jieguo_TextArea->setText("Error");
                return;
    }
    tem_num=qLn(num1)/qLn(num2);OperaNum_Stack.push(tem_num);}
   if(tem=="yroot("){
    if(num1-qFloor((num1/2))*2==0&&num2<0){
                ui->jieguo_TextArea->setText("Error");
                return;
    }
    if(num1!=0){
                tem_num=pow(num2,1/num1);OperaNum_Stack.push(tem_num);}
    else
        OperaNum_Stack.push(1);
    }
     if(tem=="^("){
    tem_num=pow(num1,num2);OperaNum_Stack.push(tem_num);}
     }
 // 6-23*(13%2)*9-ln(e^(5))
 //如果是一元运算符
    if(p==1){

     if(tem=="floor("){
         tem_num=qFloor(num2);OperaNum_Stack.push(tem_num);}
     if(tem=="ceil(")
        {
        tem_num=qCeil(num2);OperaNum_Stack.push(tem_num);
        }

     if(tem=="log(")
     {
        if(num2<=0){
                ui->jieguo_TextArea->setText("Error");
                return;
        }
     tem_num=log10(num2);OperaNum_Stack.push(tem_num);
     }
     if(tem=="~("){
    tem_num=-num2;OperaNum_Stack.push(tem_num);
     }

     if(tem=="ln(")
     {
    if(num2<=0){
                ui->jieguo_TextArea->setText("Error");
                return;
    }
    tem_num=qLn(num2);OperaNum_Stack.push(tem_num);
     }
     if(tem=="fact(")
     {
    tem_num=get_fact(num2);OperaNum_Stack.push(tem_num);
     }
     if(tem=="abs(")
     {
    tem_num=qAbs(num2);OperaNum_Stack.push(tem_num);
     }
     if(tem=="cuberoot(")
     {
    tem_num=pow(num2,1/3);OperaNum_Stack.push(tem_num);
     }
     if(tem=="sqrt(")
     {
    tem_num=qSqrt(num2);OperaNum_Stack.push(tem_num);
     }
     if(tem=="sqr(")
     {
    tem_num=pow(num2,2);OperaNum_Stack.push(tem_num);
     }
     if(tem=="cube(")
     {
    tem_num=pow(num2,3);OperaNum_Stack.push(tem_num);
     }
     if(tem=="sin(")
     {
    if(ui->R_D_Exchange_Button->text()=="DEG")
             num2= qDegreesToRadians(num2);
    tem_num=qSin(num2);OperaNum_Stack.push(tem_num);



     }
     if(tem=="cos(")
     {
    if(ui->R_D_Exchange_Button->text()=="DEG")
             num2= qDegreesToRadians(num2);
    tem_num=qCos(num2);OperaNum_Stack.push(tem_num);


     }
     if(tem=="tan(")
     {
    if(ui->R_D_Exchange_Button->text()=="DEG")
             num2= qDegreesToRadians(num2);
    tem_num=qTan(num2);OperaNum_Stack.push(tem_num);
     }
     if(tem=="sec(")
     {
    if(ui->R_D_Exchange_Button->text()=="DEG")
             num2= qDegreesToRadians(num2);
    tem_num=qAcos(num2);OperaNum_Stack.push(tem_num);
     }
     if(tem=="csc(")
     {
    if(ui->R_D_Exchange_Button->text()=="DEG")
             num2= qDegreesToRadians(num2);
    tem_num=qAsin(num2);OperaNum_Stack.push(tem_num);
     }
     if(tem=="cot(")
     {
    if(ui->R_D_Exchange_Button->text()=="DEG")
             num2= qDegreesToRadians(num2);
    tem_num=qAtan(num2);OperaNum_Stack.push(tem_num);
     }
    }
    if(p==0){
//     if(tem=="#")
//    Operator_Stack.push(tem);
     OperaNum_Stack.push(num2);
    }
}
//floor\(|[+\-*\/()=%]|ceil\(|log\(|logbase\(|ln\(|\^\(|yroot\(|fact\(|abs\(|cuberoot\(|
//sqrt\(|sqr\(|cube\(|sin\(|cos\(|tan\(|sec\(|csc\(|cot\(|[1-9][0-9]*\.?[0-9]*|0|Π|\be\b

int MainWindow::Get_Prior(QString s){
if(s=="*"||s=="/"||s=="%")
     return 4;
else if(s=="+"||s=="-")
     return 3;
else if(s=="#")
     return 2;
else
     return 0;

}

void MainWindow::InitOpPrior(){

    Prior=0;
    Prior_Bound.insert("(",Prior);
    Prior_Bound.insert("#",Prior);
    Prior=1;                //一元表达式
    Prior_Bound.insert("floor(",Prior);
    Prior_Bound.insert("ceil(",Prior);
    Prior_Bound.insert("log(",Prior);
    Prior_Bound.insert("ln(",Prior);
    Prior_Bound.insert("fact(",Prior);
    Prior_Bound.insert("abs(",Prior);
    Prior_Bound.insert("cuberoot(",Prior);
    Prior_Bound.insert("~(",Prior);
    Prior_Bound.insert("sqrt(",Prior);
    Prior_Bound.insert("sqr(",Prior);
    Prior_Bound.insert("cube(",Prior);
    Prior_Bound.insert("sin(",Prior);
    Prior_Bound.insert("cos(",Prior);
    Prior_Bound.insert("tan(",Prior);
    Prior_Bound.insert("sec(",Prior);
    Prior_Bound.insert("csc(",Prior);
    Prior_Bound.insert("cot(",Prior);
    Prior=2;            //二元表达式
    Prior_Bound.insert("logbase(",Prior);
    Prior_Bound.insert("^(",Prior);
    Prior_Bound.insert("yroot(",Prior);
    Prior_Bound.insert("+",Prior);
    Prior_Bound.insert("-",Prior);
    Prior_Bound.insert("*",Prior);
    Prior_Bound.insert("/",Prior);
    Prior_Bound.insert("%",Prior);

}

void MainWindow::on_duoxaingshi_Button_clicked()
{
    Sub_Window *sw=new Sub_Window();
    sw->show();

}


void MainWindow::on_Tri_Button_clicked()
{
    if(ui->Tri_Show_GUI->isHidden())
        ui->Tri_Show_GUI->show();
    else
        ui->Tri_Show_GUI->hide();
}


void MainWindow::on_f_Func_Button_clicked()
{
    if(ui->f_Func_GUI->isHidden())
        ui->f_Func_GUI->show();
    else
        ui->f_Func_GUI->hide();
}


void MainWindow::on_History_Rec_clicked()
{

    QPoint globalPos = this->mapToGlobal(QPoint(0, 0));//历史记录界面位置
    hr->move(globalPos.x()+513,globalPos.y()-30);
    hr->ui->textBrowser->setAlignment(Qt::AlignRight);
    if(hr->isHidden())
    hr->show();

}





void MainWindow::on_F_E_Button_clicked()
{
    if(ui->CLC_Button->text()=="C")
    ui->CLC_Button->setText("CE");
    else
    ui->CLC_Button->setText("C");


}

//2nd改变界面
void MainWindow::on_nd_2_clicked()
{
   if(ui->x2_Button->text()=="x²")
      ui->x2_Button->setText("x³");
   else
        ui->x2_Button->setText("x²");


   if(ui->sqrt_xButton->text()=="²√x")
      ui->sqrt_xButton->setText("³√x");
   else
      ui->sqrt_xButton->setText("²√x");

   if(ui->xd_ycimi_Button->text()=="x^y")
      ui->xd_ycimi_Button->setText("y√x");
   else
      ui->xd_ycimi_Button->setText("x^y");

   if(ui->x_10dxcimi_Button->text()=="10^x")
      ui->x_10dxcimi_Button->setText("2^x");
   else
      ui->x_10dxcimi_Button->setText("10^x");

   if(ui->logx_Button->text()=="log")
      ui->logx_Button->setText("log_y(x)");
   else
      ui->logx_Button->setText("log");

   if(ui->lnx_Button->text()=="ln")
      ui->lnx_Button->setText("e^x");
   else
      ui->lnx_Button->setText("ln");

}


void MainWindow::on_R_D_Exchange_Button_clicked()
{
   if(ui->R_D_Exchange_Button->text()=="RAD")
      ui->R_D_Exchange_Button->setText("DEG");
   else
      ui->R_D_Exchange_Button->setText("RAD");
}


void MainWindow::on_Left_bra_Button_clicked()
{
   ui->shizi_TextArea->insertPlainText("(");
}


void MainWindow::on_Right_Bra_Button_clicked()
{
   ui->shizi_TextArea->insertPlainText(")");
}


void MainWindow::on_Button_7_clicked()
{
     ui->shizi_TextArea->insertPlainText("7");
}


void MainWindow::on_Button_8_clicked()
{
     ui->shizi_TextArea->insertPlainText("8");
}


void MainWindow::on_Button_9_clicked()
{
     ui->shizi_TextArea->insertPlainText("9");
}


void MainWindow::on_Button_4_clicked()
{
     ui->shizi_TextArea->insertPlainText("4");
}


void MainWindow::on_Button_5_clicked()
{
     ui->shizi_TextArea->insertPlainText("5");
}


void MainWindow::on_Button_6_clicked()
{
     ui->shizi_TextArea->insertPlainText("6");
}


void MainWindow::on_Button_1_clicked()
{
     ui->shizi_TextArea->insertPlainText("1");
}


void MainWindow::on_Button_2_clicked()
{
     ui->shizi_TextArea->insertPlainText("2");
}


void MainWindow::on_Button_3_clicked()
{
     ui->shizi_TextArea->insertPlainText("3");
}


void MainWindow::on_Button_0_clicked()
{
     ui->shizi_TextArea->insertPlainText("0");
}


void MainWindow::on_divButton_clicked()
{
     ui->shizi_TextArea->insertPlainText("/");
}


void MainWindow::on_Mul_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("*");
}


void MainWindow::on_Sub_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("-");
}


void MainWindow::on_Add_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("+");
}


void MainWindow::on_Point_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText(".");
}


void MainWindow::on_Equal_Button_clicked()
{
     ui->shizi_TextArea->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
     ui->shizi_TextArea->insertPlainText("=");

            Calculate();
     ui->shizi_TextArea->setReadOnly(true);
     }


void MainWindow::on_Pi_Button_clicked()
{

     ui->shizi_TextArea->insertPlainText("Π");
}


void MainWindow::on_Get_ModButton_clicked()
{
     ui->shizi_TextArea->insertPlainText("%");
}


void MainWindow::on_CLC_Button_clicked()
{
     ui->shizi_TextArea->setText("");
      ui->shizi_TextArea->setAlignment(Qt::AlignRight);
     ui->jieguo_TextArea->setText("0");
     ui->shizi_TextArea->setReadOnly(false);


}


void MainWindow::on_del_Button_clicked()
{
     ui->shizi_TextArea->textCursor().deletePreviousChar();
}


void MainWindow::on_Sin_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("sin()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);

}


void MainWindow::on_Cos_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("cos()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_Tan_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("tan()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_Sec_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("sec()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_Csc_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("csc()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_Cot_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("cot()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_e_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("e");

}


void MainWindow::on_x2_Button_clicked()
{
     if(ui->x2_Button->text()=="x²")
      ui->shizi_TextArea->insertPlainText("sqr()");
     else
      ui->shizi_TextArea->insertPlainText("cube()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_sqrt_xButton_clicked()
{
     if(ui->sqrt_xButton->text()=="²√x")
      ui->shizi_TextArea->insertPlainText("sqrt()");
     else
      ui->shizi_TextArea->insertPlainText("cuberoot()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_x_fuyici_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("1/()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}



void MainWindow::on_absx_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("abs()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_mi_10_d_xciButton_clicked()
{
     ui->shizi_TextArea->insertPlainText("*10^()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_jiecheng_clicked()
{
     ui->shizi_TextArea->insertPlainText("fact()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_xd_ycimi_Button_clicked()
{
     if(ui->xd_ycimi_Button->text()=="x^y")
      ui->shizi_TextArea->insertPlainText("^()");
     else
      ui->shizi_TextArea->insertPlainText("yroot()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_x_10dxcimi_Button_clicked()
{
     if(ui->x_10dxcimi_Button->text()=="10^x")
      ui->shizi_TextArea->insertPlainText("10^()");
     else
      ui->shizi_TextArea->insertPlainText("2^()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_lnx_Button_clicked()
{
     if(ui->lnx_Button->text()=="ln")
      ui->shizi_TextArea->insertPlainText("ln()");
     else
      ui->shizi_TextArea->insertPlainText("e^()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);

}


void MainWindow::on_logx_Button_clicked()
{
     if(ui->logx_Button->text()=="log")
      ui->shizi_TextArea->insertPlainText("log()");
     else
      ui->shizi_TextArea->insertPlainText("logbase()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_Random_Button_clicked()
{

     double m1=(double)(rand()%100)/100;
     QString s=QString::number(m1);
     ui->shizi_TextArea->insertPlainText(s);
}


void MainWindow::on_xiangshang_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("ceil()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}


void MainWindow::on_xiangxiaqu_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("floor()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}




void MainWindow::on_symbol_exchange_Button_clicked()
{
     ui->shizi_TextArea->insertPlainText("~()");
     QTextCursor tmpCursor = ui->shizi_TextArea->textCursor();
     int present_Position=ui->shizi_TextArea->textCursor().position();
     present_Position-=1;
     tmpCursor.setPosition(present_Position);
     ui->shizi_TextArea->setTextCursor(tmpCursor);
}

