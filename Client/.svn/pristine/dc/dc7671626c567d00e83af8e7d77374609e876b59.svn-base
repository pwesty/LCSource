using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace UVCapture
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    /// 
    enum eCALC_TYPE
    {
        eCALC_TYPE_ADD,
        eCALC_TYPE_MINUS,

        eCALC_TYPE_MAX
    };

    public partial class MainWindow : Window
    {
        int nState;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, TextChangedEventArgs e)
        {
            TextBox textBox = sender as TextBox;
            string strText = textBox.Text;

            char[] sep = { ',' };

            string[] StrSplit = strText.Split(sep);
            System.Console.WriteLine("{0} words in text:", StrSplit.Length);

            int split = StrSplit.Length;

            if (split != 4)
            {
                return;
            }

            string output = String.Format("x=\"{0:D}\" y=\"{1:D}\" w=\"{2:D}\" h=\"{3:D}\"",
                calcString(StrSplit[0]), calcString(StrSplit[1]),
                calcString(StrSplit[2]), calcString(StrSplit[3]));

            Clipboard.SetText(output);

            if (nState == 1)
                textBox.Clear();
        }

        private void textBox2_TextChanged(object sender, TextChangedEventArgs e)
        {
            TextBox textBox = sender as TextBox;
            string strText = textBox.Text;

            char[] sep = { ',' };

            string[] StrSplit = strText.Split(sep);
            System.Console.WriteLine("{0} words in text:", StrSplit.Length);

            int split = StrSplit.Length;

            if (split != 4)
            {
                return;
            }

            int nX = calcString(StrSplit[0]);
            int nY = calcString(StrSplit[1]);
            string output = String.Format("x=\"{0:D}\" y=\"{1:D}\" w=\"{2:D}\" h=\"{3:D}\"",
                calcString(StrSplit[0]), calcString(StrSplit[1]),
                calcString(StrSplit[2]) - nX, calcString(StrSplit[3]) - nY);

            Clipboard.SetText(output);

            if (nState == 1)
                textBox.Clear();
        }

        private void textBox3_TextChanged(object sender, TextChangedEventArgs e)
        {
            TextBox textBox = sender as TextBox;
            string strText = textBox.Text;

            char[] sep = { ',' };

            string[] StrSplit = strText.Split(sep);
            System.Console.WriteLine("{0} words in text:", StrSplit.Length);

            int split = StrSplit.Length;

            if (split != 4)
            {                
                return;
            }

            string output = String.Format("l=\"{0:D}\" t=\"{1:D}\" r=\"{2:D}\" b=\"{3:D}\"",
                calcString(StrSplit[0]), calcString(StrSplit[1]),
                calcString(StrSplit[2]), calcString(StrSplit[3]));

            Clipboard.SetText(output);

            if (nState == 1)
                textBox.Clear();
        }        

        class stCalc
        {
            public int num;
            public eCALC_TYPE calc_type;
        };

        private int calcString(string text)
        {
            if (text == "")
                return 0;

            int     i, j, cur = 0;
            char[] sep = { '+', '-' };
            string tmpStr;
            List<stCalc> list_num = new List<stCalc>();

            int nMaxString = text.Length;
            char[] StrArray = text.ToCharArray();

            for (i = 0; i < nMaxString; ++i )
            {
                for (j = 0; j < 2; ++j)
                {
                    if (StrArray[i] == sep[j])
                    {
                        stCalc tmp = new stCalc();
                        // array add
                        tmpStr = text.Substring(cur, (i - cur));
                        tmp.num = Convert.ToInt32(tmpStr);
                        cur = i + 1;
                        tmp.calc_type = (eCALC_TYPE)j;

                        list_num.Add(tmp);
                    }
                }
            }

            // 마지막 숫자 입력            
            tmpStr = text.Substring(cur, (i - cur));
            if (String.IsNullOrWhiteSpace(tmpStr) == false && 
                String.IsNullOrEmpty(tmpStr) == false)
            {
                stCalc last = new stCalc();
                last.num = Convert.ToInt32(tmpStr);
                last.calc_type = eCALC_TYPE.eCALC_TYPE_MAX;
                list_num.Add(last);
            }

            int prev_num = 0;
            eCALC_TYPE prev_type = eCALC_TYPE.eCALC_TYPE_MAX;

            foreach (stCalc calc in list_num)
            {
                if (prev_type == eCALC_TYPE.eCALC_TYPE_ADD)
                    prev_num += calc.num;
                else if (prev_type == eCALC_TYPE.eCALC_TYPE_MINUS)
                    prev_num -= calc.num;
                else //if (prev_num == 0)
                    prev_num = calc.num;
                    
                prev_type = calc.calc_type;
            }

            return prev_num;
        }

        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (Keyboard.Modifiers == ModifierKeys.Control && e.Key == Key.V)
                nState = 1;
            else
                nState = 0;

        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                TextBox textBox = sender as TextBox;
                textBox.Clear();
            }
        }

        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                TextBox textBox = sender as TextBox;
                textBox.Clear();
            }
        }

        private void textBox3_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                TextBox textBox = sender as TextBox;
                textBox.Clear();
            }
        }
    }
}
