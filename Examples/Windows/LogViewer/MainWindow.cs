using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.IO;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace LogViewer
{
    public partial class MainWindow : Form
    {
        private static List<LogItem> loadedLog = new List<LogItem>();
        private List<LogItem> filteredLog = null;

        private int objectCounter = 0;
        private List<int> objectIds = new List<int>();
        private bool m_threadStopped = true;
        private UdpClient m_udpClient = null;
        private Thread m_thread = null;
        public const int LoggerPort = 4444;
        private bool filterIsEmpty = false;

        public delegate void UpdateRowCountDelegate(int rowCount);
        public delegate void AddNewObjectDelegate(int objectID);
        public MainWindow()
        {
            InitializeComponent();

            comboBox1.Items.Add(new ComboBoxItem((int)DebugLevel.Debug,    "Debug"));
            comboBox1.Items.Add(new ComboBoxItem((int)DebugLevel.Info, "Info"));
            comboBox1.Items.Add(new ComboBoxItem((int)DebugLevel.Warning, "Warning"));
            comboBox1.Items.Add(new ComboBoxItem((int)DebugLevel.Error, "Error"));
            comboBox1.Items.Add(new ComboBoxItem((int)DebugLevel.Exception, "Exception"));
            comboBox1.SelectedIndex = 0;
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {            
            if (openFileDialog1.ShowDialog() != DialogResult.OK) return;

            loadedLog.Clear();
            filteredLog = null;
            dataGridView1.RowCount = 0;
            dataGridView1.Refresh();

            objectCounter = 1;
            objectIds.Clear();
            FileOpen(openFileDialog1.FileName);

            UpdateObjectCombobox();
            ApplyLogFilter(DebugLevel.Debug, -1);
            comboBox2.SelectedIndex = 0;
        }

        private void FileOpen(string filename)
        {
            FileStream   fs = new FileStream(filename, FileMode.Open);
            StreamReader sr = new StreamReader(fs);

            string line;

            while (!sr.EndOfStream)
            {
                line = sr.ReadLine();
                if (line[0] != 'T') continue;
                ReadLogLine(line);
            }
        }

        void ReadLogLine(string line)
        {
            string[] part = line.Split('\t');

            CultureInfo  cultureClone = (CultureInfo)CultureInfo.InvariantCulture.Clone();
            cultureClone.NumberFormat.NumberDecimalSeparator = ".";
            cultureClone.NumberFormat.NumberGroupSeparator = "'";

            try
            {
                LogItem item = new LogItem();

                string time = part[0].Substring(1);
                string[] timepart = time.Split(':');

                int hour = Int32.Parse(timepart[0]);
                int min = Int32.Parse(timepart[1]);
                float f = float.Parse(timepart[2], cultureClone);
                int sec = (int)f;
                int msec = (int)((f - sec) * 1000);

                item.Time = new DateTime(1900, 1, 1, hour, min, sec, msec);
                item.ObjectInstance = Int32.Parse(part[1]);
                item.DebugLevel = (DebugLevel)Enum.Parse(typeof(DebugLevel), part[2]);
                item.Text = part[3];

                if (!objectIds.Contains(item.ObjectInstance))
                {
                    objectIds.Add(item.ObjectInstance);
                }
                loadedLog.Add(item);        
            } catch
            {
                //
            }
        }

        private void ApplyLogFilter(DebugLevel minDebugLevel, int objectInstanceID)
        {
            filteredLog = new List<LogItem>();
            filteredLog.Clear();
            foreach(LogItem item in loadedLog)
            {
                if (objectInstanceID != -1)
                {
                    if (item.ObjectInstance != objectInstanceID) continue;
                }
                if (item.DebugLevel>=minDebugLevel) filteredLog.Add(item);
            }
            dataGridView1.RowCount = filteredLog.Count;
            dataGridView1.Refresh();            
        }

        void UpdateObjectCombobox()
        {
            comboBox2.Items.Clear();
            comboBox2.Items.Add(new ComboBoxItem(-1, "<Any object>"));
            comboBox2.Items.Add(new ComboBoxItem(0, "NULL"));
            for (int i = 0; i < objectIds.Count; i++)
            {
                comboBox2.Items.Add(new ComboBoxItem(objectIds[i], String.Format("Object {0}", i + 1)));
            }
            comboBox2.SelectedIndex = 0;
        }

        private void dataGridView1_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            List<LogItem> dataSource = loadedLog;
            if (m_threadStopped)
            {
                if (filteredLog != null) dataSource = filteredLog;
            };

            if (e.RowIndex >= dataSource.Count)
            {
                return;
            }            
            switch(e.ColumnIndex)
            {
                case 0: e.Value = dataSource[e.RowIndex].Time.ToShortTimeString(); break;
                case 1: if (dataSource[e.RowIndex].ObjectInstance == 0)
                        {
                            e.Value = "NULL";
                        }
                        else
                        {
                            e.Value = "<Unknown>";
                            int id = dataSource[e.RowIndex].ObjectInstance;
                            if (id == 0)
                            {
                                e.Value = "NULL";
                            }
                            else
                            {
                                for (int i = 0; i < objectIds.Count; i++)
                                {
                                    if (objectIds[i] == id)
                                    {
                                        e.Value = String.Format("Object {0}", i + 1);
                                    }
                                }
                            }
                        }
                        break;

                case 2: e.Value = dataSource[e.RowIndex].DebugLevel.ToString(); break;
                case 3: e.Value = dataSource[e.RowIndex].Text; break;
            }
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBoxItem cbItem1 = comboBox1.SelectedItem as ComboBoxItem;
            ComboBoxItem cbItem2 = comboBox2.SelectedItem as ComboBoxItem;

            if (cbItem1 == null) return;
            if (cbItem2 == null) return;

            DebugLevel level = (DebugLevel)cbItem1.Value;
            int        objID = (int)cbItem2.Value;

            ApplyLogFilter(level, objID);
        }

        private void startListeningToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_threadStopped)
            {
                m_threadStopped = false;
                m_udpClient = new UdpClient(LoggerPort);
                startListeningToolStripMenuItem.Text = "Stop listening";

                loadedLog.Clear();
                filteredLog = null;
                dataGridView1.RowCount = 0;
                dataGridView1.Refresh();

                objectCounter = 1;
                objectIds.Clear();
                comboBox2.Items.Clear();
                comboBox2.Items.Add(new ComboBoxItem(-1, "<Any object>"));
                comboBox2.Items.Add(new ComboBoxItem(0, "NULL"));

                m_thread = new Thread(new ThreadStart(InternalThread));
                m_thread.Start();
                comboBox1.Enabled = false;
                comboBox2.Enabled = false;
            }
            else
            {
                m_threadStopped = true;
                m_udpClient.Close();

                while (m_threadStopped);
                m_threadStopped = true;

                startListeningToolStripMenuItem.Text = "Start listening";

                UpdateObjectCombobox();

                comboBox1.Enabled = true;
                comboBox2.Enabled = true;
            }            
        }

        private void InternalThread()
        {                        
            byte[]  datagram;
            long    offset = 0;

            string filename = "temp.log";

            FileStream fs = new FileStream(filename, FileMode.Create);
            fs.Close();

            fs = new FileStream(filename, FileMode.Append);

            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, LoggerPort);
            while (!m_threadStopped)
            {
                datagram = new byte[0];
                try
                {
                    datagram = m_udpClient.Receive(ref endPoint);
                } catch
                {

                }
                if (datagram.Length==0) continue;
                
                fs.Write(datagram, 0, datagram.Length);
                offset += datagram.Length;

                byte c = (byte)'\n';
                fs.WriteByte(c);

                string line = Encoding.UTF8.GetString(datagram);
                if (line.Length == 0) continue;
                if (line[0] != 'T') continue;

                ReadLogLine(line);

                this.Invoke(new UpdateRowCountDelegate(UpdateGridRowCount), new object[1] { loadedLog.Count });
            }
            fs.Close();

            m_threadStopped = false;
            m_udpClient.Close();
            m_udpClient = null;
        }

        void UpdateGridRowCount(int rowCount)
        {
            dataGridView1.RowCount = rowCount;
        }

        void AddNewObject(int objectID)
        {
            comboBox2.Items.Add(new ComboBoxItem(objectID, String.Format("Object {0}", objectCounter)));
        }

        private void uDPToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }
    }
}

