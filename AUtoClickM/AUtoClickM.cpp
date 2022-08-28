#include "AUtoClickM.h"
#include <QFileDialog>
#include <atltypes.h>

AUtoClickM::AUtoClickM(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //timer = new Timer::CTimer(AUtoClickM::KeyDown);
    time = 0;
    connect(ui.LiuLan, & QPushButton::clicked, this, & AUtoClickM::GetPath);//绑定浏览按键点击事件
    connect(ui.ReadCokFile, &QPushButton::clicked, this, &AUtoClickM::ReadCokFile);//绑定读取文件按键点击事件
    connect(ui.remove, &QPushButton::clicked, this, &AUtoClickM::Remove);//绑定删除按键点击事件
    connect(ui.clear, &QPushButton::clicked, this, &AUtoClickM::Clear);//绑定清除按键点击事件
    connect(ui.ClickTable, &QTableWidget::itemChanged, this, &AUtoClickM::ItemChangedC);//绑定点击表改动事件
    connect(ui.save, &QPushButton::clicked, this, &AUtoClickM::Save);//绑定保存按键点击事件
    isTimerEnable = false;
    clickList = true;
    //读取配置文件
    file = new IniFile::IniW_R("./setSystem.ini");
    ReadIniFile();
#ifdef DebugL3
    qDebug() << "文件读取 启动;";
#endif
    updateMsg("配置文件文件读取!");
    //全局按键检测
    ts = new std::thread(&AUtoClickM::Key_Capturer, this);
    ts->detach();
    updateMsg("全局按键检测 启动");
#ifdef DebugL3
        qDebug() << "全局按键检测 启动;";
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

void AUtoClickM::updateMsg(std::string msg)//更新消息队列
{
    ui.debugText->setMarkdown(ui.debugText->toMarkdown() + QTime::currentTime().toString() +" " + QString::fromStdString(msg) + "\r\n");
}

void AUtoClickM::ReadIniFile()//读取配置文件
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

void AUtoClickM::SetIniFile()//保存配置文件
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

void AUtoClickM::KeyDown()//单次点击
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void AUtoClickM::Key_Capturer()//全局按键监控
{
    bool flag = true;//按键锁
    while (true)
    {
        if (KEY_DOWN(VK_SHIFT) && KEY_DOWN(VK_LBUTTON) && !ui.IsAutoClick->isChecked()) {
#ifdef DebugL3
            qDebug() << "开始单次自动点击;";
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
            qDebug() << "开始连续自动点击;";
#endif
            Timer::CTimer timer(AUtoClickM::KeyDown);
            timer.TimerStartForCount(ui.AutoClickSleepTime->value());
            while (true)
            {
                if (KEY_DOWN(VK_RBUTTON))
                {
                    timer.Stop();
#ifdef DebugL3
                    qDebug() << "停止连续自动点击;";
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
                StartAutoClick(cokVect);//配置点击表点击
                if (KEY_DOWN(VK_F9))
                {
                    break;
                }
            }while (ui.IsAutoClick_2->isChecked());//如果为配置为连续点击 将会循环进入点击
        }
           
        else if (!KEY_DOWN(VK_F7) && !flag)
            flag = true;
    }
}

void AUtoClickM::GetPath()//获取读取文件的路径
{
#ifdef INIDEBUG
    qDebug() << "浏览文件！";
#endif
    FilePath = QFileDialog::getOpenFileName(this, tr("浏览文件"), QString::fromStdString(FilePath), //获取目录
        tr("cok文件 (*.cok);;kok文件(*.kok)")).toStdString();
    updateMsg("获取目录:" + FilePath);
#ifdef INIDEBUG
    qDebug() << "路径为：" << filePath << " 文件名：" << fileName;
#endif

    cok = new CokFile::CokFileR_W(FilePath);
    ui.path->setText(QString::fromStdString(FilePath));
    if (!cok->is_open())//文件打开失败
    {
#ifdef INIDEBUG
        qDebug() << file_Open_into->fileName() << "文件打开失败！";
#endif
        updateMsg("路径错误！");
        MessageBox(NULL, TEXT("路径错误！"), TEXT("错误"), MB_OK);
        return;
    }
#ifdef INIDEBUG
    qDebug() << file_Open_into->fileName() << "文件打开成功！";
#endif
    updateMsg("路径加载成功!");
}

void AUtoClickM::ReadCokFile()//加载cok文件
{
    if (cok==NULL) {//文件加载器初始化
        cok = new CokFile::CokFileR_W(FilePath);
#ifdef DebugL3
        qDebug() << "开始读取文件 目录:" + QString::fromStdString(FilePath);
#endif
        updateMsg("正在打开目录文件");
    }
       

    if (!cok->is_open())//判断是否打开文件
    {
        updateMsg("文件打开失败! 文件目录："+FilePath);
        return;
    }
#ifdef DebugL3
    qDebug() << "开始读取文件;"<< cok->isEnable;
#endif
        cokVect.clear();
        try
        {
            cokVect = cok->GetCokFile();//获取点击表
        }
        catch (const std::exception&)
        {
            updateMsg("加载文件失败！ 检查是否为有效文件！");
        }
    
#ifdef DebugL3
    qDebug() << "文件节点数;"<< cokVect.size();
#endif
    clickList = false;
    for (std::vector<CokFile::CokCC>::iterator iter = cokVect.begin(); iter != cokVect.end(); ++iter)
    {
#ifdef DebugL3
        qDebug() << iter->nub << ',' << iter->xadd << ',' << iter->yadd << ',' << iter->sleepTime << ',' << QString::fromStdString(iter->another);
#endif
        if (iter->nub == 0) {
            updateMsg("文件不能出现0索引！！！");
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

void AUtoClickM::Remove()//删除一行
{

    if (ui.ClickTable->rowCount()<=1)
        return;
    ui.ClickTable->setRowCount(ui.ClickTable->rowCount() - 1);
    cokVect.pop_back();
    updateMsg("删除最后一行！");
}

void AUtoClickM::Clear()//清除点击表
{
    ui.ClickTable->setRowCount(1);
    cokVect.clear();
    updateMsg("点击表清除！");
}
void AUtoClickM::ItemChangedC()
{
    try
    {
        if (clickList)
        {
            updateMsg("更新点击表！");
#ifdef DebugL3
            qDebug() <<"更新点击表！" ;
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
        updateMsg("更新点击表异常！");
    }
   
}
void AUtoClickM::Save()//保存文件
{
    FilePath=QFileDialog::getSaveFileName(this, tr("保存文件"),  //类函数QFileDiaLog:获取文件路径//getSaveFileName:获取保存文件名字
        QString::fromStdString(FilePath),
        "cok文件(*.cok);;kok(*.kok)").toStdString();
    updateMsg("保存目录："+ FilePath);
    if (cok == NULL) {//文件加载器初始化
        cok = new CokFile::CokFileR_W(FilePath);
#ifdef DebugL3
        qDebug() << "开始加载文件 目录:" + QString::fromStdString(FilePath);
#endif
        updateMsg("正在打开目录文件");
    }
    else
        cok->ReLoadPath(FilePath);//重加载目录
    if (!cok->is_open())//判断是否加载目录
    {
        updateMsg("目录打开失败! 文件目录：" + FilePath);
        return;
    }
    try
    {
        cok->WriteNewFile(cokVect, FilePath);
    }
    catch (const std::exception&)
    {
        updateMsg("文件保存失败！");
        MessageBox(NULL, TEXT("文件保存失败！"), TEXT("错误"), MB_OK);
    }
   
}
static int clickCount;

/// <summary>
/// 开始配置点击
/// </summary>
/// <param name="cokVect">点击表</param>
void AUtoClickM::StartAutoClick(std::vector<CokFile::CokCC>cokVect)
{
    if (cokVect.empty())//点击表如果为空则返回
    {
        //updateMsg("点击表加载失败！");
        MessageBox(NULL, TEXT("点击表加载失败！"), TEXT("错误"), MB_OK);
        return;
    }
    if (isTimerEnable)//触发锁 防止在运行的过程中 再次开启
    {
#ifdef DebugL3
        qDebug() << "触发锁。";
#endif
        return;
    }
    //updateMsg("自动点击配置！");

    clickCount = 0;
    if (!timer)//清除上次的时钟
    {
        delete timer;
    }
    timer = new Timer::CTimer(cokVect, AUtoClickM::AutoClick);
     timer->SetEnAble(&isTimerEnable);//设置触发锁
#ifdef DebugL3
     qDebug() << "定时器使能。";
     qDebug() << "vector size："<< cokVect.size();
#endif
     //updateMsg("启动点击器！");
    timer->TimerStartForModeEN(cokVect[1].sleepTime, cokVect.size());//启动定时器
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
/// <summary>
/// 全自动点击 
/// </summary>
/// <param name="cokVect">点击数据容器</param>
/// <param name="x">设置去下一次点击的间隔</param>
void AUtoClickM::AutoClick(std::vector<CokFile::CokCC> cokVect,unsigned int &x)
{
    SetCursorPos(cokVect[clickCount].xadd,
        cokVect[clickCount].yadd);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#ifdef DebugL3
    qDebug() << "点击："<< cokVect[clickCount].nub;
#endif
    x = cokVect[clickCount].sleepTime;
    clickCount++;
}


