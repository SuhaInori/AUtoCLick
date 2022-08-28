#include "AUtoClickM.h"
#include <QFileDialog>
#include <atltypes.h>

AUtoClickM::AUtoClickM(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //timer = new Timer::CTimer(AUtoClickM::KeyDown);
    time = 0;
    connect(ui.LiuLan, & QPushButton::clicked, this, & AUtoClickM::GetPath);//�������������¼�
    connect(ui.ReadCokFile, &QPushButton::clicked, this, &AUtoClickM::ReadCokFile);//�󶨶�ȡ�ļ���������¼�
    connect(ui.remove, &QPushButton::clicked, this, &AUtoClickM::Remove);//��ɾ����������¼�
    connect(ui.clear, &QPushButton::clicked, this, &AUtoClickM::Clear);//�������������¼�
    connect(ui.ClickTable, &QTableWidget::itemChanged, this, &AUtoClickM::ItemChangedC);//�󶨵����Ķ��¼�
    connect(ui.save, &QPushButton::clicked, this, &AUtoClickM::Save);//�󶨱��水������¼�
    isTimerEnable = false;
    clickList = true;
    //��ȡ�����ļ�
    file = new IniFile::IniW_R("./setSystem.ini");
    ReadIniFile();
#ifdef DebugL3
    qDebug() << "�ļ���ȡ ����;";
#endif
    updateMsg("�����ļ��ļ���ȡ!");
    //ȫ�ְ������
    ts = new std::thread(&AUtoClickM::Key_Capturer, this);
    ts->detach();
    updateMsg("ȫ�ְ������ ����");
#ifdef DebugL3
        qDebug() << "ȫ�ְ������ ����;";
#endif
        cok = NULL;
}

AUtoClickM::~AUtoClickM()                       
{
    SetIniFile();
    delete ts;
    delete file;
    delete cok;
    delete timer;
}

void AUtoClickM::updateMsg(std::string msg)//������Ϣ����
{
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() +" " + QString::fromStdString(msg) + "\r\n");
}

void AUtoClickM::ReadIniFile()//��ȡ�����ļ�
{
    if (!file->IsEmpty)
    {
        ui.AutoClickTimes->setValue(file->GetInt32("Main","AutoClickTimes"));
        ui.AutoClickSleepTime->setValue(file->GetInt32("Main", "AutoClickSleepTime"));
        ui.AutoClickTime1->setValue(file->GetInt32("Main", "AutoClickTime1"));

        ui.IsAutoClick->setChecked(file->GetBool("Main", "IsAutoClick"));
        ui.IsAutoClick_2->setChecked(file->GetBool("Main", "IsAutoClick_2"));
        ui.clickTimesE->setChecked(file->GetBool("Main", "clickTimesE"));

        FilePath = file->GetString("Main", "FilePath");
        ui.path->setText(QString::fromStdString(FilePath));
    }
    else {
        FilePath = "c:/";
    }
}

void AUtoClickM::SetIniFile()//���������ļ�
{
    file->SetValue("Main", "AutoClickTimes", std::to_string(ui.AutoClickTimes->value()));
    file->SetValue("Main", "AutoClickSleepTime", std::to_string(ui.AutoClickSleepTime->value()));
    file->SetValue("Main", "AutoClickTime1", std::to_string(ui.AutoClickTime1->value()));

    file->SetValue("Main", "IsAutoClick", std::to_string(ui.IsAutoClick->isChecked()));
    file->SetValue("Main", "IsAutoClick_2", std::to_string(ui.IsAutoClick_2->isChecked()));
    file->SetValue("Main", "clickTimesE", std::to_string(ui.clickTimesE->isChecked()));

    file->SetValue("Main", "FilePath", FilePath);

    file->AllWriteIni();
}

void AUtoClickM::KeyDown()//���ε��
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void AUtoClickM::Key_Capturer()//ȫ�ְ������
{
    bool flag = true;//������
    while (true)
    {
        if (KEY_DOWN(VK_SHIFT) && KEY_DOWN(VK_LBUTTON) && !ui.IsAutoClick->isChecked()) {
#ifdef DebugL3
            qDebug() << "��ʼ�����Զ����;";
#endif
            Timer::CTimer timer(AUtoClickM::KeyDown);
            timer.TimerStartForCount(ui.AutoClickSleepTime->value(), ui.AutoClickTimes->value());
            std::this_thread::sleep_for(std::chrono::milliseconds(ui.AutoClickSleepTime->value() * ui.AutoClickTimes->value()));
            /*for (int i = 0; i < ui.AutoClickTimes->value(); i++)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(ui.AutoClickSleepTime->value()));
            }*/
        }
        else if (KEY_DOWN(VK_SHIFT) && KEY_DOWN(VK_LBUTTON) && ui.IsAutoClick->isChecked())
        {
#ifdef DebugL3
            qDebug() << "��ʼ�����Զ����;";
#endif
            Timer::CTimer timer(AUtoClickM::KeyDown);
            timer.TimerStartForCount(ui.AutoClickSleepTime->value());
            while (true)
            {
                if (KEY_DOWN(VK_RBUTTON))
                {
                    timer.Stop();
#ifdef DebugL3
                    qDebug() << "ֹͣ�����Զ����;";
#endif
                    break;
                }
                //Sleep(ui.AutoClickSleepTime->value());
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        }
        else if (KEY_DOWN(VK_F7) && flag)
        {
            CPoint m_mouse;
            GetCursorPos(&m_mouse);
            if (ui.clickTimesE->isChecked())
            {
                int cltT = clock() - time;
                time = clock();
                if (cltT > 10000)
                {
                    cltT = 50;
                }
                CokFile::CokCC temp;
                temp.nub = cokVect.size() + 1;
                temp.xadd = m_mouse.x;
                temp.yadd = m_mouse.y;
                temp.sleepTime = cltT;
                cokVect.push_back(temp);
                //cok->AddCokCC(temp);
                clickList = false;
                ui.ClickTable->setRowCount(ui.ClickTable->rowCount() + 1);
                ui.ClickTable->setItem(temp.nub, 0, new QTableWidgetItem(QString::number(temp.nub)));
                ui.ClickTable->setItem(temp.nub, 1, new QTableWidgetItem(QString::number(temp.xadd)));
                ui.ClickTable->setItem(temp.nub, 2, new QTableWidgetItem(QString::number(temp.yadd)));
                ui.ClickTable->setItem(temp.nub, 3, new QTableWidgetItem(QString::number(temp.sleepTime)));
                ui.ClickTable->setItem(temp.nub, 4, new QTableWidgetItem(QString::fromStdString(temp.another)));
                clickList = true;
                //clock()
            }
            else
            {
                CokFile::CokCC temp;
                temp.nub = cokVect.size() + 1;
                temp.xadd = m_mouse.x;
                temp.yadd = m_mouse.y;
                temp.sleepTime = ui.AutoClickTime1->value();
                cokVect.push_back(temp);
                //cok->AddCokCC(temp);
                clickList = false;
                ui.ClickTable->setRowCount(ui.ClickTable->rowCount() + 1);
                ui.ClickTable->setItem(temp.nub, 0, new QTableWidgetItem(QString::number(temp.nub)));
                ui.ClickTable->setItem(temp.nub, 1, new QTableWidgetItem(QString::number(temp.xadd)));
                ui.ClickTable->setItem(temp.nub, 2, new QTableWidgetItem(QString::number(temp.yadd)));
                ui.ClickTable->setItem(temp.nub, 3, new QTableWidgetItem(QString::number(temp.sleepTime)));
                ui.ClickTable->setItem(temp.nub, 4, new QTableWidgetItem(QString::fromStdString(temp.another)));
                clickList = true;
            }
            flag = false;
        }
        else if (KEY_DOWN(VK_F8)) {
            
            do
            {
                StartAutoClick(cokVect);//���õ������
                if (KEY_DOWN(VK_F9))
                {
                    break;
                }
            }while (ui.IsAutoClick_2->isChecked());//���Ϊ����Ϊ������� ����ѭ��������
        }
           
        else if (!KEY_DOWN(VK_F7) && !flag)
            flag = true;
    }
}

void AUtoClickM::GetPath()//��ȡ��ȡ�ļ���·��
{
#ifdef INIDEBUG
    qDebug() << "����ļ���";
#endif
    FilePath = QFileDialog::getOpenFileName(this, tr("����ļ�"), QString::fromStdString(FilePath), //��ȡĿ¼
        tr("cok�ļ� (*.cok);;kok�ļ�(*.kok)")).toStdString();
    updateMsg("��ȡĿ¼:" + FilePath);
#ifdef INIDEBUG
    qDebug() << "·��Ϊ��" << filePath << " �ļ�����" << fileName;
#endif

    cok = new CokFile::CokFileR_W(FilePath);
    ui.path->setText(QString::fromStdString(FilePath));
    if (!cok->is_open())//�ļ���ʧ��
    {
#ifdef INIDEBUG
        qDebug() << file_Open_into->fileName() << "�ļ���ʧ�ܣ�";
#endif
        updateMsg("·������");
        MessageBox(NULL, TEXT("·������"), TEXT("����"), MB_OK);
        return;
    }
#ifdef INIDEBUG
    qDebug() << file_Open_into->fileName() << "�ļ��򿪳ɹ���";
#endif
    updateMsg("·�����سɹ�!");
}

void AUtoClickM::ReadCokFile()//����cok�ļ�
{
    if (cok==NULL) {//�ļ���������ʼ��
        cok = new CokFile::CokFileR_W(FilePath);
#ifdef DebugL3
        qDebug() << "��ʼ��ȡ�ļ� Ŀ¼:" + QString::fromStdString(FilePath);
#endif
        updateMsg("���ڴ�Ŀ¼�ļ�");
    }
       

    if (!cok->is_open())//�ж��Ƿ���ļ�
    {
        updateMsg("�ļ���ʧ��! �ļ�Ŀ¼��"+FilePath);
        return;
    }
#ifdef DebugL3
    qDebug() << "��ʼ��ȡ�ļ�;"<< cok->isEnable;
#endif
        cokVect.clear();
        try
        {
            cokVect = cok->GetCokFile();//��ȡ�����
        }
        catch (const std::exception&)
        {
            updateMsg("�����ļ�ʧ�ܣ� ����Ƿ�Ϊ��Ч�ļ���");
        }
    
#ifdef DebugL3
    qDebug() << "�ļ��ڵ���;"<< cokVect.size();
#endif
    clickList = false;
    for (std::vector<CokFile::CokCC>::iterator iter = cokVect.begin(); iter != cokVect.end(); ++iter)
    {
#ifdef DebugL3
        qDebug() << iter->nub << ',' << iter->xadd << ',' << iter->yadd << ',' << iter->sleepTime << ',' << QString::fromStdString(iter->another);
#endif
        if (iter->nub == 0) {
            updateMsg("�ļ����ܳ���0����������");
            continue;
        }
           
        //ui.ClickTable->setItem(i + 1, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui.ClickTable->setRowCount(ui.ClickTable->rowCount() + 1);
        ui.ClickTable->setItem(iter->nub, 0, new QTableWidgetItem(QString::number(iter->nub)));
        ui.ClickTable->setItem(iter->nub, 1, new QTableWidgetItem(QString::number(iter->xadd)));
        ui.ClickTable->setItem(iter->nub, 2, new QTableWidgetItem(QString::number(iter->yadd)));
        ui.ClickTable->setItem(iter->nub, 3, new QTableWidgetItem(QString::number(iter->sleepTime)));
        ui.ClickTable->setItem(iter->nub, 4, new QTableWidgetItem(QString::fromStdString(iter->another)));
    }
    clickList = true;
}

void AUtoClickM::Remove()//ɾ��һ��
{

    if (ui.ClickTable->rowCount()<=1)
        return;
    ui.ClickTable->setRowCount(ui.ClickTable->rowCount() - 1);
    cokVect.pop_back();
    updateMsg("ɾ�����һ�У�");
}

void AUtoClickM::Clear()//��������
{
    ui.ClickTable->setRowCount(1);
    cokVect.clear();
    updateMsg("����������");
}
void AUtoClickM::ItemChangedC()
{
    try
    {
        if (clickList)
        {
            updateMsg("���µ����");
#ifdef DebugL3
            qDebug() <<"���µ����" ;
#endif
            cokVect.clear();
            for (int i = 1; i < ui.ClickTable->rowCount(); i++)
            {
                CokFile::CokCC temp;
#ifdef DebugL3
                qDebug() << ui.ClickTable->item(i, 0)->text();
                qDebug() << ui.ClickTable->item(i, 1)->text();
                qDebug() << ui.ClickTable->item(i, 2)->text();
                qDebug() << ui.ClickTable->item(i, 3)->text();
                qDebug() << ui.ClickTable->item(i, 4)->text();

#endif
                temp.nub = ui.ClickTable->item(i, 0)->text().toInt();
                temp.xadd = ui.ClickTable->item(i, 1)->text().toInt();
                temp.yadd = ui.ClickTable->item(i, 2)->text().toInt();
                temp.sleepTime = ui.ClickTable->item(i, 3)->text().toInt();
                temp.another = ui.ClickTable->item(i, 4)->text().toStdString();
#ifdef DebugL3
                qDebug() << temp.nub << ',' << temp.xadd << ',' << temp.yadd << ',' << temp.sleepTime << ',' << QString::fromStdString(temp.another);
#endif
                cokVect.push_back(temp);
            }

        }
      
    }
    catch (const std::exception&)
    {
        updateMsg("���µ�����쳣��");
    }
   
}
void AUtoClickM::Save()//�����ļ�
{
    FilePath=QFileDialog::getSaveFileName(this, tr("�����ļ�"),  //�ຯ��QFileDiaLog:��ȡ�ļ�·��//getSaveFileName:��ȡ�����ļ�����
        QString::fromStdString(FilePath),
        "cok�ļ�(*.cok);;kok(*.kok)").toStdString();
    updateMsg("����Ŀ¼��"+ FilePath);
    if (cok == NULL) {//�ļ���������ʼ��
        cok = new CokFile::CokFileR_W(FilePath);
#ifdef DebugL3
        qDebug() << "��ʼ�����ļ� Ŀ¼:" + QString::fromStdString(FilePath);
#endif
        updateMsg("���ڴ�Ŀ¼�ļ�");
    }
    else
        cok->ReLoadPath(FilePath);//�ؼ���Ŀ¼
    if (!cok->is_open())//�ж��Ƿ����Ŀ¼
    {
        updateMsg("Ŀ¼��ʧ��! �ļ�Ŀ¼��" + FilePath);
        return;
    }
    try
    {
        cok->WriteNewFile(cokVect, FilePath);
    }
    catch (const std::exception&)
    {
        updateMsg("�ļ�����ʧ�ܣ�");
        MessageBox(NULL, TEXT("�ļ�����ʧ�ܣ�"), TEXT("����"), MB_OK);
    }
   
}
static int clickCount;

/// <summary>
/// ��ʼ���õ��
/// </summary>
/// <param name="cokVect">�����</param>
void AUtoClickM::StartAutoClick(std::vector<CokFile::CokCC>cokVect)
{
    if (cokVect.empty())//��������Ϊ���򷵻�
    {
        //updateMsg("��������ʧ�ܣ�");
        MessageBox(NULL, TEXT("��������ʧ�ܣ�"), TEXT("����"), MB_OK);
        return;
    }
    if (isTimerEnable)//������ ��ֹ�����еĹ����� �ٴο���
    {
#ifdef DebugL3
        qDebug() << "��������";
#endif
        return;
    }
    //updateMsg("�Զ�������ã�");

    clickCount = 0;
    if (!timer)//����ϴε�ʱ��
    {
        delete timer;
    }
    timer = new Timer::CTimer(cokVect, AUtoClickM::AutoClick);
     timer->SetEnAble(&isTimerEnable);//���ô�����
#ifdef DebugL3
     qDebug() << "��ʱ��ʹ�ܡ�";
     qDebug() << "vector size��"<< cokVect.size();
#endif
     //updateMsg("�����������");
    timer->TimerStartForModeEN(cokVect[1].sleepTime, cokVect.size());//������ʱ��
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
/// <summary>
/// ȫ�Զ���� 
/// </summary>
/// <param name="cokVect">�����������</param>
/// <param name="x">����ȥ��һ�ε���ļ��</param>
void AUtoClickM::AutoClick(std::vector<CokFile::CokCC> cokVect,unsigned int &x)
{
    SetCursorPos(cokVect[clickCount].xadd,
        cokVect[clickCount].yadd);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#ifdef DebugL3
    qDebug() << "�����"<< cokVect[clickCount].nub;
#endif
    x = cokVect[clickCount].sleepTime;
    clickCount++;
}


