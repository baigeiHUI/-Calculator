#include "sub_window.h"
#include "ui_sub_window.h"
#include"QRegularExpression"
#include"QRegularExpressionMatch"
#include"QRegularExpressionMatchIterator"
#include<iterator>
#include"QString"

QString s_1=R"([+\-]*\S[1-9]*[0-9]*\.?[0-9]*\w*\^*[1-9]*[0-9]*)"; //将多项式拆成子项
QString s1=R"([+-]|[1-9][0-9]*\.?[0-9]*|\w|\^)";                //将子项拆分成单个字符
QString Variable_Name;
QRegularExpression re(s_1);
QRegularExpression re_1(s1);
Sub_Window::Sub_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sub_Window)
{
    ui->setupUi(this);
    setWindowTitle("Polyomial_Computing");
    ui->InputText_1->setAlignment(Qt::AlignRight|Qt::AlignBottom);
    ui->InputText_2->setAlignment(Qt::AlignRight|Qt::AlignBottom);
    ui->OutputText_1->setAlignment(Qt::AlignRight|Qt::AlignBottom);
    ui->OutputText_2->setAlignment(Qt::AlignRight|Qt::AlignBottom);
    ui->OutputText_1->setClearButtonEnabled(true);
    ui->OutputText_2->setClearButtonEnabled(true);
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

    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());
    ui->InputText_1->setClearButtonEnabled(true);
    ui->InputText_2->setClearButtonEnabled(true);
    QFont f0("楷体",16);
    hr->ui->textBrowser->setAlignment(Qt::AlignRight);
    hr->ui->textBrowser->setFont(f0);
    ui->History_Rec1->setStyleSheet(
        "border-image:url(D:/Qt_Pro_Main/Calculator/history_rec.png)");


}
void Sub_Window::Put_Cut_StringIntoQueue(QString qs){
     Operator_Queue.clear();
    QString tem;
    QRegularExpressionMatchIterator qem=re.globalMatch(qs);
    QRegularExpressionMatch match ;
    while(qem.hasNext()){
        match=qem.next();
        tem=match.captured(0);
        Operator_Queue.append(tem);                                       //input_1中分割好的字符进队列
    }
}

void Sub_Window::Put_Little_Piece_Into_Array(QString s0){
    InitStringArray();
    int i=0;
    QRegularExpressionMatchIterator qem=re_1.globalMatch(s0);
      QRegularExpressionMatch match ;
    while(qem.hasNext()){
        match=qem.next();
        Part_Deal[i++]=match.captured(0);
    }

}

void Sub_Window::InitStringArray(){
    for(int i=0;i<5;i++){
        Part_Deal[i]="#";
    }

}

void Sub_Window::Piece_Matched_IntoMap(QMap<int,double> &map0){

    QString s4=R"([a-zA-Z])";                                       //匹配变量名称
    QString s5=R"([+\-])";
    QRegularExpression qe_4(s4);
    QRegularExpression qe_5(s5);
    QString tem;
    int key;
    double value;
    while(!Operator_Queue.isEmpty()){
        tem=Operator_Queue.dequeue();
        Put_Little_Piece_Into_Array(tem);       //将每一块运算基本单元分割成操作数符号、操作数、变量、乘方号、幂次
        if(qe_5.match(Part_Deal[0]).hasMatch()) {     //如果有符号

            if(Part_Deal[0]=="+")                       //如果第0位是+号

            {
                if(qe_4.match(Part_Deal[1]).hasMatch()){    //如果第一位是变量
                    Variable_Name=Part_Deal[1];
                    value=1;
                    if(Part_Deal[2]=="#")
                        key=1;
                    else if(Part_Deal[2]=="^")
                        key=(Part_Deal[3].toInt());
                }



                else{                                       //第一位是数字
                    value=Part_Deal[1].toDouble();
                    if(Part_Deal[2]=="#")                      //第二位是"#"
                        key=0;
                    else{                                      //第二位是变量
                         Variable_Name=Part_Deal[2];
                        if(Part_Deal[3]=="#")
                            key=1;
                        else if(Part_Deal[3]=="^")
                            key=Part_Deal[4].toInt();
                      }

                  }



            }

            else if(Part_Deal[0]=="-")                 //如果第0位是负号
            {
                  if(qe_4.match(Part_Deal[1]).hasMatch()){    //如果第一位是变量
                       Variable_Name=Part_Deal[1];
                      value=-1;
                      if(Part_Deal[2]=="#")
                        key=1;
                      else if(Part_Deal[2]=="^")
                        key=(Part_Deal[3].toInt());
                  }



                  else{                                       //第一位是数字
                      value=-Part_Deal[1].toDouble();
                      if(Part_Deal[2]=="#")                      //第二位是"#"
                        key=0;
                      else{                                      //第二位是变量
                         Variable_Name=Part_Deal[2];
                        if(Part_Deal[3]=="#")
                            key=1;
                        else if(Part_Deal[3]=="^")
                            key=Part_Deal[4].toInt();
                      }

                  }


            }



        }
        else           //没有符号,起始运算单元
        {
            if(qe_4.match(Part_Deal[0]).hasMatch()){        //第0位是变量
                   Variable_Name=Part_Deal[0];
                  value=1;
                  if(Part_Deal[1]=="#")      //单元结束
                      key=1;
                  else if(Part_Deal[1]=="^")
                      key=Part_Deal[2].toInt();

            }
            else                                            //第0位是数字
            {
                  value=Part_Deal[0].toDouble();
                  if(Part_Deal[1]=="#")
                      key=0;
                  else{                                     //第一位是变量
                       Variable_Name=Part_Deal[1];
                      if(Part_Deal[2]=="#")
                        key=1;
                      else
                        key=Part_Deal[3].toInt();
                  }

            }



        }

        map0.insert(key,value);
    }
}


void Sub_Window::GetInput(){
    input_1.clear();
    input_2.clear();

    QString tem=ui->InputText_1->text();
    Put_Cut_StringIntoQueue(tem);
    Piece_Matched_IntoMap(input_1);
    tem=ui->InputText_2->text();
    Put_Cut_StringIntoQueue(tem);
    Piece_Matched_IntoMap(input_2);

    if(ui->Operator_Choosen->currentText()=="+")
      Add_Achieved();
    else if(ui->Operator_Choosen->currentText()=="-")
      Sub_Achieved();
    else if(ui->Operator_Choosen->currentText()=="*")
     Mul_Achieved();

    //此时input_1映射已经成为结果映射,除去其中value值为0的元素
    QList<int> zero_del=input_1.keys();
    int tes;
    for(int i=0;i<zero_del.size();i++){
     tes=zero_del[i];
     if(input_1.value(tes)==0)
        input_1.remove(tes);
    }
    //思考输出方式即可.
    ShowAnswer();
    ShowAnswer_Reverse();
    hr->ui->textBrowser->append(ui->InputText_1->text());
    hr->ui->textBrowser->insertPlainText(ui->Operator_Choosen->currentText());
    hr->ui->textBrowser->append(ui->InputText_2->text());
    hr->ui->textBrowser->insertPlainText("=");
    hr->ui->textBrowser->append(ui->OutputText_1->text());
    hr->ui->textBrowser->insertPlainText("or");
    hr->ui->textBrowser->append(ui->OutputText_2->text());
    hr->ui->textBrowser->append(" ");
    hr->ui->textBrowser->append(" ");










}

void Sub_Window::ShowAnswer(){
    QList<int>key_list=input_1.keys();
    QList<double>value_list=input_1.values();

    for(int i=0;i<key_list.size();i++){
     if(i==0)                                   //第一项
     {
        if(value_list[i]<0&&value_list[i]!=-1)
                  ui->OutputText_1->insert(QString::number(value_list[i]));
        else if(value_list[i]<0&&value_list[i]==-1)
                  ui->OutputText_1->insert("-");
        else if(value_list[i]>0&&value_list[i]!=1){
                  ui->OutputText_1->insert(QString::number(value_list[i]));
        }                                                              //系数搞定,轮到次幂


      }
 else{                                      //后续所有项
        if(value_list[i]<0&&value_list[i]!=-1)
                  ui->OutputText_1->insert(QString::number(value_list[i]));
        else if(value_list[i]<0&&value_list[i]==-1)
                  ui->OutputText_1->insert("-");
        else if(value_list[i]>0&&value_list[i]!=1){
                  ui->OutputText_1->insert("+");
                  ui->OutputText_1->insert(QString::number(value_list[i]));
        }
        else if(value_list[i]>0&&value_list[i]==1)
                  ui->OutputText_1->insert("+"); //系数搞定,该次幂了(0,1和其他)



     }

      if(key_list[i]==0){                        //幂次等于0,如果value值等于+-1,补一个1
        if(value_list[i]==1||value_list[i]==-1)
                  ui->OutputText_1->insert("1");


      }
      if(key_list[i]==1){                      //幂次等于1,插入一个x进去
        ui->OutputText_1->insert(Variable_Name);
      }
      else if(key_list[i]>1){                  //幂次大于1,插入一个x^,再插入幂次值
        ui->OutputText_1->insert(Variable_Name);
        ui->OutputText_1->insert("^");
        ui->OutputText_1->insert(QString::number(key_list[i]));
      }

    }

}

void Sub_Window::ShowAnswer_Reverse(){
    QList<int>key_list=input_1.keys();
    QList<double>value_list=input_1.values();
    for(int i=key_list.size()-1;i>=0;i--){
      if(i==key_list.size()-1)                                   //第一项
      {
        if(value_list[i]<0&&value_list[i]!=-1)
                  ui->OutputText_2->insert(QString::number(value_list[i]));
        else if(value_list[i]<0&&value_list[i]==-1)
                  ui->OutputText_2->insert("-");
        else if(value_list[i]>0&&value_list[i]!=1){
                  ui->OutputText_2->insert(QString::number(value_list[i]));
        }                                                              //系数搞定,轮到次幂


      }
      else{                                      //后续所有项
        if(value_list[i]<0&&value_list[i]!=-1)
                  ui->OutputText_2->insert(QString::number(value_list[i]));
        else if(value_list[i]<0&&value_list[i]==-1)
                  ui->OutputText_2->insert("-");
        else if(value_list[i]>0&&value_list[i]!=1){
                  ui->OutputText_2->insert("+");
                  ui->OutputText_2->insert(QString::number(value_list[i]));
        }
        else if(value_list[i]>0&&value_list[i]==1)
                  ui->OutputText_2->insert("+"); //系数搞定,该次幂了(0,1和其他)



      }

      if(key_list[i]==0){                        //幂次等于0,如果value值等于+-1,补一个1
        if(value_list[i]==1||value_list[i]==-1)
                  ui->OutputText_2->insert("1");


      }
      if(key_list[i]==1){                      //幂次等于1,插入一个x进去
        ui->OutputText_2->insert(Variable_Name);
      }
      else if(key_list[i]>1){                  //幂次大于1,插入一个x^,再插入幂次值
        ui->OutputText_2->insert(Variable_Name);
        ui->OutputText_2->insert("^");
        ui->OutputText_2->insert(QString::number(key_list[i]));
      }

    }
}


void Sub_Window::Add_Achieved(){
    int d_i;
    QList<int> Key_List=input_2.keys();
    for(int i=0;i<Key_List.size();i++){
     d_i=Key_List[i];
     if(input_1.contains(d_i))
            input_1.insert(d_i,input_1.value(d_i)+input_2.value(d_i));
     else
            input_1.insert(d_i,input_2.value(d_i));                  //新键值对储存在input_1中

     }
    }



void Sub_Window::Sub_Achieved(){
    int d_i;
    QList<int> Key_List=input_2.keys();
    for(int i=0;i<Key_List.size();i++){
     d_i=Key_List[i];
     if(input_1.contains(d_i))
            input_1.insert(d_i,input_1.value(d_i)-input_2.value(d_i));
     else
            input_1.insert(d_i,-input_2.value(d_i));              //新键值对储存在input_1中

    }

}
void Sub_Window::Mul_Achieved()
    {
    QList<int> Key_1_List=input_1.keys();
    QList<int> Key_2_List=input_2.keys();
    QMap<int,double> Tem_Map;
    int i,j,tem;

     for(j=0;j<Key_2_List.size();j++)
    {
     i=Key_2_List[j];
     Tem_Map.insert(Key_1_List[0]+Key_2_List[j],input_1.value(Key_1_List[0])*input_2.value(i)) ;
     }
     for(i=1;i<Key_1_List.size();i++)
        for(j=0;j<Key_2_List.size();j++)
     {
            tem=Key_1_List[i]+Key_2_List[j];
            if(Tem_Map.contains(tem))
                  Tem_Map.insert(tem,Tem_Map.value(tem)+input_1.value(Key_1_List[i])*input_2.value(Key_2_List[j]));
            else
                  Tem_Map.insert(tem,input_1.value(Key_1_List[i])*input_2.value(Key_2_List[j]));     //Mul函数的幂次和对应系数编译完成

     }

     input_1=Tem_Map;




}

Sub_Window::~Sub_Window()
{
    delete ui;
}

void Sub_Window::on_CLC_Button_clicked()
{
    ui->InputText_1->setText("");
    ui->InputText_2->setText("");
    ui->OutputText_1->setText("");
    ui->OutputText_2->setText("");
}


void Sub_Window::on_Compute_Button_clicked()
{
    GetInput();
}


void Sub_Window::on_History_Rec1_clicked()
{
    QPoint globalPos = this->mapToGlobal(QPoint(0, 0));//历史记录界面位置
    hr->move(globalPos.x()+760,globalPos.y()-30);
    hr->ui->textBrowser->setAlignment(Qt::AlignRight);
    if(hr->isHidden())
     hr->show();
}

