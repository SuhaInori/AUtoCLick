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
    ui.AutoClickTime->setFocusPolicy(Qt::NoFocus);//无法键盘输入 只能调整值
    //setReadOnly(true);
    //setStyleSheet("background:transparent;border:2px solid red;");
    //无边框
    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);//背景透明
    // 
    // 
    // 
    //按键监听
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

    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "程序启动！\n");
    file_Open_into = NULL;
    iniFile = new CMyINI("./set.ini");
    osuFile = NULL;
    //Text = "计数\tX坐标\tY坐标\t\n";
   // ui.ClickTable->setText(Text);
    ts = new std::thread(&AutoClick::Key_Capturer,this);
    ts->detach();
    readIniFile();
  /*  ui.debugText->setText(ui.debugText)*/
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "初始化完毕！\n");
#ifdef INIDEBUG
    if (iniFile->startEmpty)
        qDebug() << "无文件";
    else
        qDebug() << "有文件";
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
            //ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "开始单次自动点击！\n");

#ifdef INIDEBUG
            qDebug() << "按键按下-单次自动点击";
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
                //ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "开始连续自动点击！\n");
            }
            else
            {
                //ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "开始单次自动点击！\n");
            }

#ifdef INIDEBUG
            qDebug() << "按键按下-自动点击";
#endif
            while (true)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                if (KEY_DOWN(VK_RBUTTON))//KEY_DOWN(VK_SHIFT)&& KEY_DOWN(80)
                {
                    //ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "自动点击取消！\n");
#ifdef INIDEBUG
                    qDebug() << "按键按下-自动点击取消";
#endif
                    break;
                }
                Sleep(ui.AutoClickTime->value());
            }
        }
        else if (KEY_DOWN(VK_F7))
        {
           // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "录制自动点击！\n");
#ifdef INIDEBUG
            qDebug() << "按键按下-添加点击";
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
            qDebug() << "按键按下-添加点击完成";
#endif
            Sleep(500);
        }
        else if (KEY_DOWN(VK_F8)) 
        {
            if (osuFileClick)
            {
#ifdef INIDEBUG
                qDebug() << "进入osu点击";
#endif
               // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "开始osu自动点击！\n");
                osuFile->startOsuFileAutoClick();
               // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "osu自动点击完成！\n");
            }
            else
            {
#ifdef INIDEBUG
                qDebug() << "进入录制点击";
#endif
               // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "开始录制点击！\n");
#ifdef INIDEBUG
                qDebug() << "进入录制点击1";
#endif 
                cClick.SetKeep(ui.continuityClick->isChecked());
                cClick.startCustomClick();
               // ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "录制自动点击完成！\n");
            }
           
        }
    }
}

void AutoClick::DelCClick()
{
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "录制自动点击删除！\n");
    if (cClick.GetMaxIndex() == 0)
        return;
    cClick.delete_Coordinates();
    ui.ClickTable->setRowCount(ui.ClickTable->rowCount() -1);
}

void AutoClick::clearCClick()
{
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "自动点击录制清空！\n");
    cClick.clear_Coordinates();
    ui.ClickTable->setRowCount(1);
    iniFile->deleteForRoot("autoClick");
    Ui_Form f;
    f.setupUi(this);

}

void AutoClick::getPatn()
{
#ifdef INIDEBUG
    qDebug() << "浏览文件！" ;
#endif
     fileName = QFileDialog::getOpenFileName(this,tr("浏览文件"), filePath, tr("osu文件 (*.osu)"));
     ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "获取文件路径："+fileName+"\n");
#ifdef INIDEBUG
    qDebug() << "路径为："<< filePath<<" 文件名："<< fileName;
#endif
    file_Open_into = new QFile(fileName);

    if (!file_Open_into->open(QIODevice::ReadOnly))
    {
#ifdef INIDEBUG
        qDebug() << file_Open_into ->fileName() << "文件打开失败！";
#endif
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "路径错误！\n");
        MessageBox(NULL, TEXT("路径错误！"),TEXT("错误"), MB_OK);
        return ;
    }
#ifdef INIDEBUG
        qDebug() << file_Open_into->fileName() << "文件打开成功！";
#endif
    filePath = fileName;
    ui.path->setText(fileName);
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "路径加载成功！\n");
}

void AutoClick::readIniFile()
{

    if (iniFile->startEmpty)
    {
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "配置文件写入！\n");
#ifdef INIDEBUG
        qDebug() << "文件写入！=======";
#endif
        setIniValue();
    }
    else
    {
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "读取配置文件！\n");
#ifdef INIDEBUG
        qDebug() << "文件读取！======";
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
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "无点击录制保存！\n");
        return;
    }
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "读取点击录制文件！\n");
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
    qDebug() << "读取文件";
#endif
    if (osuFile != NULL)
    {
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "读取新文件！\n");
#ifdef INIDEBUG
        qDebug() << "读取文件--文件已经存在";
#endif
        osuFile->reSetPath(fileName.toStdString());
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "新文件读取成功！\n");
    }
    else
    {
#ifdef INIDEBUG
        qDebug() << "读取文件--新文件读取";
        qDebug() << fileName << " 转换后：" << fileName.toStdString().c_str();
#endif
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "读取osu文件！\n");
        osuFile = new osuFileAutoClick(fileName.toStdString());
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "osu文件读取成功！\n");
    }
       
}

void AutoClick::displayOsuFile()
{
    if (ui.displayOsu->text()=="显示在下表")
    {
       
        if (osuFile==NULL)
        {
            ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "未读取文件！ 请先读取osu文件\n");
            MessageBox(NULL, TEXT("未读取文件,清先读取文件！"), TEXT("错误"), MB_OK);
            osuFileClick = false;
            return;
        }
        bool temp1 = !osuFile->windowsSet();
        if (temp1)
            MessageBox(NULL, TEXT("未打开osu,清先打开osu！"), TEXT("错误"), MB_OK);
        ui.displayOsu->setText("显示按键录制");
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "显示完成！点击f8开始\n");
        //MessageBox(NULL, TEXT("显示完成！ f8 可以自动点击！"), TEXT("错误"), MB_OK);
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
    else if (ui.displayOsu->text()== "显示按键录制")
    {
        //MessageBox(NULL, TEXT("显示完成！ f8 可以自动点击！"), TEXT("错误"), MB_OK);
        ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() + "显示完成！ f8 可以自动点击！！\n");
        ui.displayOsu->setText("显示在下表");
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

