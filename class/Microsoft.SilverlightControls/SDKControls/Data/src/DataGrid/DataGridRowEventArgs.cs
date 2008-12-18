﻿// (c) Copyright Microsoft Corporation.
// This source is subject to the Microsoft Public License (Ms-PL).
// Please see http://go.microsoft.com/fwlink/?LinkID=131993 for details.
// All other rights reserved.

namespace System.Windows.Controls
{
    public class DataGridRowEventArgs : EventArgs
    {
        public DataGridRowEventArgs(DataGridRow dataGridRow)
        {
            this.Row = dataGridRow;
        }

        public DataGridRow Row
        {
            get;
            private set;
        }
    }
}
