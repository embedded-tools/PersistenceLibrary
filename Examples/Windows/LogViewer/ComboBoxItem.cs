using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;


namespace LogViewer
{
    public class ComboBoxItem
    {
        public int Value;
        public string Text;

        public ComboBoxItem(int id, string displayValue)
        {
            this.Value = id;
            this.Text = displayValue;
        }

        public override string ToString()
        {
            return Text;
        }

        public static bool Select(ComboBox cbbox, int itemID)
        {
            for(int i = 0; i<cbbox.Items.Count; i++)
            {
                ComboBoxItem item = cbbox.Items[i] as ComboBoxItem;
                if (item.Value == itemID)
                {
                    cbbox.SelectedIndex = i;
                    return true;
                }
            }            
            return false;
        }
    }
}
