using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;



namespace 连点器
{
    public partial class Form1 : Form
    {
        public const int KEYEVENTF_KEYUP = 2;
        private bool DownMI=false;
        private Thread thread=null;
        private delegate void KeyD();
        //按键检测
        [DllImport("User32")]
        public static extern short GetAsyncKeyState(int vKey);

        [DllImport("user32.dll", EntryPoint = "keybd_event", SetLastError = true)]
        //bvk 按键的虚拟键值，如回车键为 vk_return, tab 键为 vk_tab（其他具体的参见附录）
        //bScan 扫描码，一般不用设置，用 0 代替就行；
        //dwFlags 选项标志，如果为 keydown 则置 0 即可，如果为 keyup 则设成 "KEYEVENTF_KEYUP"；
        //dwExtraInfo 一般也是置 0 即可。
        public static extern void keybd_event(Keys bVk, byte bScan, uint dwFlags, uint dwExtraInfo);
        [DllImport("user32.dll", EntryPoint = "mouse_event", SetLastError = true)]
        public static extern void mouse_event(uint dwFlags, uint dx, uint dy, uint dwData, uint dwExtraInfo);



        public Form1()
        {
            InitializeComponent();
            thread = new Thread(setAdd);
        }

        public  void setAdd()
        {
            //this.Invoke()//安全调用
            KeyD keyD = new KeyD(kEYdOWN);    //对线程进行托管
            keyD();
        }
        private  void kEYdOWN()
        {
            while (true)
            {
                KeyDownFun();
            }
          
        }

        private  void KeyDownFun()
        {
            for (int x = 0; x < Convert.ToInt32(comboBox1.Text); x++)
            {
                mouse_event(2, 0, 0, 0,0);
                mouse_event(4, 0, 0, 0,0);
                Thread.Sleep(5);
            }
        }


        private void timer1_Tick(object sender, EventArgs e)
        {
            if ((GetAsyncKeyState(16) != 0) && (GetAsyncKeyState(1) != 0) && !DownMI)
            {
                thread = new Thread(kEYdOWN);
                thread.Start();
                DownMI = true;
            }
            else
                DownMI = false;

        }

        private void button1_Click(object sender, EventArgs e)
        {
            thread.Start();
            //timer1.Start();
            //if (timer1.Enabled)
            //    MessageBox.Show("开始运行");
            //else
            //    MessageBox.Show("开始失败");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            thread.Abort();
            //timer1.Stop();
            //if (timer1.Enabled)
            //    MessageBox.Show("关闭失败");
            //else
            //    MessageBox.Show("关闭成功");
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
