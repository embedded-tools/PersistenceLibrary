using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace LogViewer
{
    public partial class PortDialog : Form
    {
        public PortDialog()
        {
            InitializeComponent();
        }

        private void PortDialog_Load(object sender, EventArgs e)
        {
            numericUpDown1.Value = Properties.Settings.Default.UdpPort;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Program.UdpPort = Decimal.ToInt32(numericUpDown1.Value);
            Properties.Settings.Default.UdpPort = Program.UdpPort;
            Properties.Settings.Default.Save();
            Close();
        }
    }
}
