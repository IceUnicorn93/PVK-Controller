using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PVKK_Simulator.Classes
{
    public class MappingClass
    {
        public Control MappedControl { get; set; }
        public Type DataType { get; set; }
        public string PropertyName { get; set; }
        public string EnumName { get; set; }
        public Action<int> callAction { get; set; }

        public MappingClass(Control control, Type dataType, string propertyName, string enumName, Action<int> callAction)
        {
            MappedControl = control;
            DataType = dataType;
            PropertyName = propertyName;
            EnumName = enumName;
            this.callAction = callAction;
        }
    }
}
