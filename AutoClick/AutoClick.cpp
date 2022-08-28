#include "AutoClick.h"
#include "ui_Widget.h"
#include <QFileDialog>
#include <winnt.rh>


AutoClick::AutoClick(QWidget *parent)
    : QWidget(parent), osuFileClick(false)
{
    setWindowIcon(QIcon("./icon.png"));
    ui.setupUi(this);
    ui.clickTime->setFocusPolicy(Qt::NoFocus);
    ui.AutoClickTime->setFocusPolicy(Qt::NoFocus);//�޷��������� ֻ�ܵ���ֵ
    //setReadOnly(true);
    //setStyleSheet("background:transparent;border:2px solid red;");
    //�ޱ߿�
    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);//����͸��
    // 
    // 
    // 
    //��������
    //connect(KeyCapturer::instance(), &KeyCapturer::getKey, [=](int key) {
    //        
    //        qDebug() << QString::number(key);

    //    });
    //connect(ui.startButton, &QPushButton::clicked, this, &AutoClick::Key_Capturer);
    connect(ui.ClrClick, &QPushButton::clicked, this, &AutoClick::clearCClick);
    connect(ui.DelClick, &QPushButton::clicked, this, &AutoClick::DelCClick);
    connect(ui.liuLan, &QPushButton::clicked, this, &AutoClick::getPatn);
    connect(ui.fileRead, &QPushButton::clicked,this, &AutoClick::readOsuFile);
    connect(ui.displayOsu, &QPushButton::clicked, this, &AutoClick::displayOsuFile);

    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "����������\n");
    file_Open_into = NULL;
    iniFile = new CMyINI("./set.ini");
    osuFile = NULL;
    //Text = "����\tX����\tY����\t\n";
   // ui.ClickTable->setText(Text);
    ts = new std::thread(&AutoClick::Key_Capturer,this);
    ts->detach();
    readIniFile();
  /*  ui.debugText->setText(ui.debugText)*/
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ʼ����ϣ�\n");
#ifdef INIDEBUG
    if (iniFile->startEmpty)
        qDebug() << "���ļ�";
    else
        qDebug() << "���ļ�";
#endif
   
}

AutoClick::~AutoClick()
{
    setIniValue();
    delete ts;
    delete file_Open_into;
    delete iniFile;
    delete osuFile;
}

void AutoClick::Key_Capturer()
{
    while (true)
    {
        if (KEY_DOWN(VK_SHIFT) && KEY_DOWN(VK_LBUTTON)&&(!ui.AutoClick->isChecked()))
        {
            //ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ʼ�����Զ������\n");

#ifdef INIDEBUG
            qDebug() << "��������-�����Զ����";
#endif
            for (int i = 0; i < ui.clickTime->value(); i++)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(ui.AutoClickTime->value());
            }
        }
        else if (KEY_DOWN(VK_SHIFT) && KEY_DOWN(VK_LBUTTON) && ui.AutoClick->isChecked())
        {
            if (ui.AutoClickTime->value())
            {
                //ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ʼ�����Զ������\n");
            }
            else
            {
                //ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ʼ�����Զ������\n");
            }

#ifdef INIDEBUG
            qDebug() << "��������-�Զ����";
#endif
            while (true)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                if (KEY_DOWN(VK_RBUTTON))//KEY_DOWN(VK_SHIFT)&& KEY_DOWN(80)
                {
                    //ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "�Զ����ȡ����\n");
#ifdef INIDEBUG
                    qDebug() << "��������-�Զ����ȡ��";
#endif
                    break;
                }
                Sleep(ui.AutoClickTime->value());
            }
        }
        else if (KEY_DOWN(VK_F7))
        {
           // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "¼���Զ������\n");
#ifdef INIDEBUG
            qDebug() << "��������-��ӵ��";
#endif
            int x, y ,z;
            z = ui.AutoClickTime1->value();
            cClick.add_Coordinates(x,y,z);
            int row = cClick.GetMaxIndex();
            ui.ClickTable->setRowCount(ui.ClickTable->rowCount()+1);
#ifdef INIDEBUG
            qDebug() << row;
#endif
            ui.ClickTable->setItem(row, 0, new QTableWidgetItem(QString::number(row)));
            ui.ClickTable->setItem(row, 1, new QTableWidgetItem(QString::number(x)));
            ui.ClickTable->setItem(row, 2, new QTableWidgetItem(QString::number(y)));
            ui.ClickTable->setItem(row, 3, new QTableWidgetItem(QString::number(ui.AutoClickTime1->value())));
#ifdef INIDEBUG
            qDebug() << "��������-��ӵ�����";
#endif
            Sleep(500);
        }
        else if (KEY_DOWN(VK_F8)) 
        {
            if (osuFileClick)
            {
#ifdef INIDEBUG
                qDebug() << "����osu���";
#endif
               // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ʼosu�Զ������\n");
                osuFile->startOsuFileAutoClick();
               // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "osu�Զ������ɣ�\n");
            }
            else
            {
#ifdef INIDEBUG
                qDebug() << "����¼�Ƶ��";
#endif
               // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ʼ¼�Ƶ����\n");
#ifdef INIDEBUG
                qDebug() << "����¼�Ƶ��1";
#endif 
                cClick.SetKeep(ui.continuityClick->isChecked());
                cClick.startCustomClick();
               // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "¼���Զ������ɣ�\n");
            }
           
        }
    }
}

void AutoClick::DelCClick()
{
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "¼���Զ����ɾ����\n");
    if (cClick.GetMaxIndex() == 0)
        return;
    cClick.delete_Coordinates();
    ui.ClickTable->setRowCount(ui.ClickTable->rowCount() -1);
}

void AutoClick::clearCClick()
{
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "�Զ����¼����գ�\n");
    cClick.clear_Coordinates();
    ui.ClickTable->setRowCount(1);
    iniFile->deleteForRoot("autoClick");
    Ui_Form f;
    f.setupUi(this);

}

void AutoClick::getPatn()
{
#ifdef INIDEBUG
    qDebug() << "����ļ���" ;
#endif
     fileName = QFileDialog::getOpenFileName(this,tr("����ļ�"), filePath, tr("osu�ļ� (*.osu)"));
     ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ȡ�ļ�·����"+fileName+"\n");
#ifdef INIDEBUG
    qDebug() << "·��Ϊ��"<< filePath<<" �ļ�����"<< fileName;
#endif
    file_Open_into = new QFile(fileName);

    if (!file_Open_into->open(QIODevice::ReadOnly))
    {
#ifdef INIDEBUG
        qDebug() << file_Open_into ->fileName() << "�ļ���ʧ�ܣ�";
#endif
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "·������\n");
        MessageBox(NULL, TEXT("·������"),TEXT("����"), MB_OK);
        return ;
    }
#ifdef INIDEBUG
        qDebug() << file_Open_into->fileName() << "�ļ��򿪳ɹ���";
#endif
    filePath = fileName;
    ui.path->setText(fileName);
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "·�����سɹ���\n");
}

void AutoClick::readIniFile()
{

    if (iniFile->startEmpty)
    {
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "�����ļ�д�룡\n");
#ifdef INIDEBUG
        qDebug() << "�ļ�д�룡=======";
#endif
        setIniValue();
    }
    else
    {
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ȡ�����ļ���\n");
#ifdef INIDEBUG
        qDebug() << "�ļ���ȡ��======";
#endif
        readIniValue();
    }
}

void AutoClick::setIniValue()
{

    iniFile->SetValue("main", "clickTime",std::to_string(ui.clickTime->value()));
    iniFile->SetValue("main", "AutoClick", std::to_string(ui.AutoClick->isChecked()));
    iniFile->SetValue("main", "AutoClickTime", std::to_string(ui.AutoClickTime->value()));
    iniFile->SetValue("main", "continuityClick", std::to_string(ui.continuityClick->isChecked()));
    iniFile->SetValue("main", "AutoClickTime1", std::to_string(ui.AutoClickTime1->value()));
    if (filePath ==NULL)
        iniFile->SetValue("main", "path", "C:/Users");
    else
    {

        iniFile->SetValue("main", "path", fileName.toStdString());
#ifdef INIDEBUG
        qDebug() << iniFile->GetValue("main","path").c_str();
        Sleep(500);
#endif
    }

    if (cClick.GetMaxIndex() <= 0)
        return;
    iniFile->SetValue("autoClick", "maxIndex", std::to_string(cClick.GetMaxIndex()));
    for (int i = 0; i < cClick.GetMaxIndex(); i++)
    {
        int temp[3];
        cClick.find_Coordinates(i, temp);
#ifdef INIDEBUG
        //cClick.find_Coordinates(cClick.GetMaxIndex() - 1, cdp);
        qDebug() << "x:" << temp[0] << "y:" << temp[1] << "t:" << temp[2];
        qDebug() << "maxIndex" << cClick.GetMaxIndex();
#endif
        iniFile->SetValue("autoClick", std::to_string(i) + 'x', std::to_string(temp[0]));
        iniFile->SetValue("autoClick", std::to_string(i) + 'y', std::to_string(temp[1]));
        iniFile->SetValue("autoClick", std::to_string(i) + 'd', std::to_string(temp[2]));
        //qDebug() << temp[2];
    }
    //Sleep(25000);
}

void AutoClick::readIniValue()
{

    ui.clickTime->setValue(std::stoi(iniFile->GetValue("main", "clickTime")));
    if (iniFile->GetValue("main", "AutoClick") == "1")
        ui.AutoClick->setChecked(true);
    else
        ui.AutoClick->setChecked(false);
    ui.AutoClickTime->setValue(std::stoi(iniFile->GetValue("main", "AutoClickTime")));
    if (iniFile->GetValue("main", "continuityClick") == "1")
        ui.continuityClick->setChecked(true);
    else
        ui.continuityClick->setChecked(false);
    ui.AutoClickTime1->setValue(std::stoi(iniFile->GetValue("main", "AutoClickTime1")));
    filePath = QString::fromStdString(iniFile->GetValue("main", "path"));
    if (filePath!="null key")
    {
        fileName = filePath;
    }
    ui.path->setText(filePath);

#ifdef INIDEBUG
    //printf("%s=========================", iniFile->GetValue("autoClick", "0"));
    //ui.path->setText(QString::fromStdString(iniFile->GetValue("autoClick", "0")));
    //qDebug() << QString::fromStdString(iniFile->GetValue("autoClick","0"));
#endif
    if (iniFile->GetValue("autoClick", "maxIndex") == "null root")
    {
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "�޵��¼�Ʊ��棡\n");
        return;
    }
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ȡ���¼���ļ���\n");
    for (int i = 0; i < std::stoi(iniFile->GetValue("autoClick", "maxIndex")); i++)
    {
        int cdp[3];
        cdp[0] = std::stoi(iniFile->GetValue("autoClick", std::to_string(i) + 'x'));
        cdp[1] = std::stoi(iniFile->GetValue("autoClick", std::to_string(i) + 'y'));
        cdp[2] = std::stoi(iniFile->GetValue("autoClick", std::to_string(i) + 'd'));
        ui.ClickTable->setRowCount(ui.ClickTable->rowCount() + 1);
        ui.ClickTable->setItem(i + 1, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui.ClickTable->setItem(i + 1, 1, new QTableWidgetItem(QString::number(cdp[0])));
        ui.ClickTable->setItem(i + 1, 2, new QTableWidgetItem(QString::number(cdp[1])));
        ui.ClickTable->setItem(i + 1, 3, new QTableWidgetItem(QString::number(cdp[2])));
#ifdef INIDEBUG
        //cClick.find_Coordinates(cClick.GetMaxIndex() - 1, cdp);
        qDebug() << "x:" << cdp[0] << "y:" << cdp[1] << "t:" << cdp[2];
#endif
        cClick.fileReadAdd(cdp[0], cdp[1], cdp[2]);
#ifdef INIDEBUG
        cClick.find_Coordinates(cClick.GetMaxIndex(), cdp);
        qDebug() << "===========x:" << cdp[0] << "y : " << cdp[1] << "t : " << cdp[2];
#endif
    }
}

void AutoClick::readOsuFile()
{
#ifdef INIDEBUG
    qDebug() << "��ȡ�ļ�";
#endif
    if (osuFile != NULL)
    {
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ȡ���ļ���\n");
#ifdef INIDEBUG
        qDebug() << "��ȡ�ļ�--�ļ��Ѿ�����";
#endif
        osuFile->reSetPath(fileName.toStdString());
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "���ļ���ȡ�ɹ���\n");
    }
    else
    {
#ifdef INIDEBUG
        qDebug() << "��ȡ�ļ�--���ļ���ȡ";
        qDebug() << fileName << " ת����" << fileName.toStdString().c_str();
#endif
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ȡosu�ļ���\n");
        osuFile = new osuFileAutoClick(fileName.toStdString());
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "osu�ļ���ȡ�ɹ���\n");
    }
       
}

void AutoClick::displayOsuFile()
{
    if (ui.displayOsu->text()=="��ʾ���±�")
    {
       
        if (osuFile==NULL)
        {
            ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "δ��ȡ�ļ��� ���ȶ�ȡosu�ļ�\n");
            MessageBox(NULL, TEXT("δ��ȡ�ļ�,���ȶ�ȡ�ļ���"), TEXT("����"), MB_OK);
            osuFileClick = false;
            return;
        }
        bool temp1 = !osuFile->windowsSet();
        if (temp1)
            MessageBox(NULL, TEXT("δ��osu,���ȴ�osu��"), TEXT("����"), MB_OK);
        ui.displayOsu->setText("��ʾ����¼��");
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ʾ��ɣ����f8��ʼ\n");
        //MessageBox(NULL, TEXT("��ʾ��ɣ� f8 �����Զ������"), TEXT("����"), MB_OK);
        ui.ClickTable->setRowCount(1);
        click_Node temp;
        for (int i = 0; i < osuFile->getMaxObjects(); i++)
        {
            temp=osuFile->getClick(i);
            ui.ClickTable->setRowCount(ui.ClickTable->rowCount() + 1);
            ui.ClickTable->setItem(i + 1, 0, new QTableWidgetItem(QString::number(i + 1)));
            ui.ClickTable->setItem(i + 1, 1, new QTableWidgetItem(QString::number(temp.x)));
            ui.ClickTable->setItem(i + 1, 2, new QTableWidgetItem(QString::number(temp.y)));
            ui.ClickTable->setItem(i + 1, 3, new QTableWidgetItem(QString::number(temp.time)));
        }
        osuFileClick = true;
       
    }
    else if (ui.displayOsu->text()== "��ʾ����¼��")
    {
        //MessageBox(NULL, TEXT("��ʾ��ɣ� f8 �����Զ������"), TEXT("����"), MB_OK);
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "��ʾ��ɣ� f8 �����Զ��������\n");
        ui.displayOsu->setText("��ʾ���±�");
        ui.ClickTable->setRowCount(1);
        for (int i = 0; i < cClick.GetMaxIndex(); i++)
        {
            int temp[3];
            cClick.find_Coordinates(i, temp);
            ui.ClickTable->setRowCount(ui.ClickTable->rowCount() + 1);
            ui.ClickTable->setItem(i + 1, 0, new QTableWidgetItem(QString::number(i + 1)));
            ui.ClickTable->setItem(i + 1, 1, new QTableWidgetItem(QString::number(temp[0])));
            ui.ClickTable->setItem(i + 1, 2, new QTableWidgetItem(QString::number(temp[1])));
            ui.ClickTable->setItem(i + 1, 3, new QTableWidgetItem(QString::number(temp[2])));
        }
        osuFileClick = false;
       
    }
}

