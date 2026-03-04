using PVKK_Simulator.Classes;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PVKK_Simulator
{
    public partial class Form1 : Form
    {
        private byte[] dataToSend = new byte[143];

        private readonly List<MappingClass> controlMappings = new List<MappingClass>();
        private readonly Dictionary<ControllerPart, (int from, int to)> ledMapping = new Dictionary<ControllerPart, (int from, int to)>();
        private readonly Dictionary<ControllerPart, Color> ledColorMapping = new Dictionary<ControllerPart, Color>();

        public Form1()
        {
            InitializeComponent();
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            controlMappings.Clear();

            //Buttons
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_Clear, typeof(bool), "Checked", ControllerPart.UPPER_BTN_Clear.ToString(), null));
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_KWG, typeof(bool), "Checked", ControllerPart.UPPER_BTN_KWG.ToString(), null));
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_PGM, typeof(bool), "Checked", ControllerPart.UPPER_BTN_PGM.ToString(), null));
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_HE, typeof(bool), "Checked", ControllerPart.UPPER_BTN_HE.ToString(), null));
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_Laser, typeof(bool), "Checked", ControllerPart.UPPER_BTN_LASER.ToString(), null));

            controlMappings.Add(new MappingClass(cbx_Upper_Btn_Load, typeof(bool), "Checked", ControllerPart.UPPER_BTN_Loaded.ToString(), SelectedAmmo));

            controlMappings.Add(new MappingClass(cbx_Upper_Btn_Prime1, typeof(bool), "Checked", ControllerPart.UPPER_BTN_PrimeStart1.ToString(), PrimeInProgress));
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_Prime2, typeof(bool), "Checked", ControllerPart.UPPER_BTN_PrimeStart2.ToString(), PrimeInProgress));
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_Prime3, typeof(bool), "Checked", ControllerPart.UPPER_BTN_PrimeStart3.ToString(), PrimeInProgress));

            controlMappings.Add(new MappingClass(cbx_Upper_Btn_ClearTarget1, typeof(bool), "Checked", ControllerPart.UPPER_BTN_TargetClear1.ToString(), null));
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_ClearTarget2, typeof(bool), "Checked", ControllerPart.UPPER_BTN_TargetClear2.ToString(), null));
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_ClearTarget3, typeof(bool), "Checked", ControllerPart.UPPER_BTN_TargetClear3.ToString(), null));
            controlMappings.Add(new MappingClass(cbx_Upper_Btn_ClearTarget4, typeof(bool), "Checked", ControllerPart.UPPER_BTN_TargetClear4.ToString(), null));

            controlMappings.Add(new MappingClass(cbx_Upper_Btn_Chamber, typeof(bool), "Checked", ControllerPart.UPPER_BTN_Chamber.ToString(), null));

            //LED Indicators
            controlMappings.Add(new MappingClass(pb_Upper_LED_Heat, typeof(int), "Value", ControllerPart.UPPER_LED_Heat.ToString(), null));

            controlMappings.Add(new MappingClass(btn_Upper_Ammo_Clear, typeof(string), "Text", ControllerPart.UPPER_LED_Clear.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_Ammo_KWG, typeof(string), "Text", ControllerPart.UPPER_LED_KWG.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_Ammo_PGM, typeof(string), "Text", ControllerPart.UPPER_LED_PGM.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_Ammo_HE, typeof(string), "Text", ControllerPart.UPPER_LED_HE.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_Ammo_Laser, typeof(string), "Text", ControllerPart.UPPER_LED_LASER.ToString(), null));

            controlMappings.Add(new MappingClass(btn_Upper_Ammo_Loaded, typeof(string), "Text", ControllerPart.UPPER_LED_Loaded.ToString(), null));

            controlMappings.Add(new MappingClass(btn_Upper_Prime_Ready1, typeof(string), "Text", ControllerPart.UPPER_LED_PrimeReady1.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_Prime_Ready2, typeof(string), "Text", ControllerPart.UPPER_LED_PrimeReady2.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_Prime_Ready3, typeof(string), "Text", ControllerPart.UPPER_LED_PrimeReady3.ToString(), null));
            controlMappings.Add(new MappingClass(pb_Upper_PrimeProgress1, typeof(int), "Value", ControllerPart.UPPER_LED_PrimeInProgress1.ToString(), null));
            controlMappings.Add(new MappingClass(pb_Upper_PrimeProgress2, typeof(int), "Value", ControllerPart.UPPER_LED_PrimeInProgress2.ToString(), null));
            controlMappings.Add(new MappingClass(pb_Upper_PrimeProgress3, typeof(int), "Value", ControllerPart.UPPER_LED_PrimeInProgress3.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_Prime_Done1, typeof(string), "Text", ControllerPart.Upper_LED_PrimeDone1.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_Prime_Done2, typeof(string), "Text", ControllerPart.Upper_LED_PrimeDone2.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_Prime_Done3, typeof(string), "Text", ControllerPart.Upper_LED_PrimeDone3.ToString(), null));

            controlMappings.Add(new MappingClass(btn_Upper_TargetLocked1, typeof(string), "Text", ControllerPart.UPPER_LED_Target1.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_TargetLocked2, typeof(string), "Text", ControllerPart.UPPER_LED_Target2.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_TargetLocked3, typeof(string), "Text", ControllerPart.UPPER_LED_Target3.ToString(), null));
            controlMappings.Add(new MappingClass(btn_Upper_TargetLocked4, typeof(string), "Text", ControllerPart.UPPER_LED_Target4.ToString(), null));

            controlMappings.Add(new MappingClass(btn_Upper_ChamberReady, typeof(string), "Text", ControllerPart.UPPER_LED_Chamber.ToString(), null));

            ledMapping.Clear();
            ledMapping.Add(ControllerPart.UPPER_LED_Loaded, (0, 0));
            ledMapping.Add(ControllerPart.UPPER_LED_LASER, (1, 1));
            ledMapping.Add(ControllerPart.UPPER_LED_HE, (2, 2));
            ledMapping.Add(ControllerPart.UPPER_LED_Clear, (3, 4));
            ledMapping.Add(ControllerPart.UPPER_LED_Heat, (5, 14));
            ledMapping.Add(ControllerPart.UPPER_LED_KWG, (15, 15));
            ledMapping.Add(ControllerPart.UPPER_LED_PGM, (16, 16));
            ledMapping.Add(ControllerPart.UPPER_LED_PrimeReady1, (17, 17));
            ledMapping.Add(ControllerPart.UPPER_LED_PrimeReady2, (18, 18));
            ledMapping.Add(ControllerPart.UPPER_LED_PrimeReady3, (19, 19));
            ledMapping.Add(ControllerPart.UPPER_LED_PrimeInProgress3, (20, 52));
            ledMapping.Add(ControllerPart.UPPER_LED_PrimeInProgress2, (85, 53));
            ledMapping.Add(ControllerPart.UPPER_LED_PrimeInProgress1, (86, 118));
            ledMapping.Add(ControllerPart.Upper_LED_PrimeDone1, (119, 119));
            ledMapping.Add(ControllerPart.Upper_LED_PrimeDone2, (120, 120));
            ledMapping.Add(ControllerPart.Upper_LED_PrimeDone3, (121, 121));
            ledMapping.Add(ControllerPart.UPPER_LED_Target4, (122, 122));
            ledMapping.Add(ControllerPart.UPPER_LED_Target3, (123, 123));
            ledMapping.Add(ControllerPart.UPPER_LED_Target2, (124, 124));
            ledMapping.Add(ControllerPart.UPPER_LED_Target1, (125, 125));
            ledMapping.Add(ControllerPart.UPPER_LED_Chamber, (126, 126));
            ledMapping.Add(ControllerPart.UPPER_LED_Energy, (136, 127));

            ledColorMapping.Clear();
            ledColorMapping.Add(ControllerPart.UPPER_LED_Loaded, Color.FromArgb(126, 250, 239));
            ledColorMapping.Add(ControllerPart.UPPER_LED_LASER, Color.FromArgb(163, 226, 203));
            ledColorMapping.Add(ControllerPart.UPPER_LED_HE, Color.FromArgb(163, 226, 203));
            ledColorMapping.Add(ControllerPart.UPPER_LED_Clear, Color.FromArgb(255, 246, 162));
            ledColorMapping.Add(ControllerPart.UPPER_LED_Heat, Color.FromArgb(255, 169, 57));
            ledColorMapping.Add(ControllerPart.UPPER_LED_KWG, Color.FromArgb(163, 226, 203));
            ledColorMapping.Add(ControllerPart.UPPER_LED_PGM, Color.FromArgb(163, 226, 203));
            ledColorMapping.Add(ControllerPart.UPPER_LED_PrimeReady1, Color.FromArgb(127, 250, 240));
            ledColorMapping.Add(ControllerPart.UPPER_LED_PrimeReady2, Color.FromArgb(127, 250, 240));
            ledColorMapping.Add(ControllerPart.UPPER_LED_PrimeReady3, Color.FromArgb(127, 250, 240));
            ledColorMapping.Add(ControllerPart.UPPER_LED_PrimeInProgress3, Color.FromArgb(252, 194, 72));
            ledColorMapping.Add(ControllerPart.UPPER_LED_PrimeInProgress2, Color.FromArgb(252, 194, 72));
            ledColorMapping.Add(ControllerPart.UPPER_LED_PrimeInProgress1, Color.FromArgb(252, 194, 72));
            ledColorMapping.Add(ControllerPart.Upper_LED_PrimeDone1, Color.FromArgb(145, 250, 247));
            ledColorMapping.Add(ControllerPart.Upper_LED_PrimeDone2, Color.FromArgb(145, 250, 247));
            ledColorMapping.Add(ControllerPart.Upper_LED_PrimeDone3, Color.FromArgb(145, 250, 247));
            ledColorMapping.Add(ControllerPart.UPPER_LED_Target4, Color.FromArgb(168, 249, 243));
            ledColorMapping.Add(ControllerPart.UPPER_LED_Target3, Color.FromArgb(168, 249, 243));
            ledColorMapping.Add(ControllerPart.UPPER_LED_Target2, Color.FromArgb(168, 249, 243));
            ledColorMapping.Add(ControllerPart.UPPER_LED_Target1, Color.FromArgb(168, 249, 243));
            ledColorMapping.Add(ControllerPart.UPPER_LED_Chamber, Color.FromArgb(252, 246, 54));

            for (int i = 0; i < dataToSend.Length; i++)
                dataToSend[i] = 0;
            dataToSend[137] = 10; //Brightness
            dataToSend[138] = 0;
            dataToSend[139] = 20;
            dataToSend[140] = 20;
            dataToSend[141] = 20;
            dataToSend[142] = 20;

            var t = new Task(async () =>
            {
                while (true)
                {
                    TCP_Connector.SendData(dataToSend,  (data) =>
                    {
                        var receiveT = new Task(() =>
                        {
                            switch (data[0])
                            {
                                case 1:
                                    HandleUI(ControllerPart.UPPER_BTN_Clear.ToString(), 1);
                                    break;
                                case 2:
                                    HandleUI(ControllerPart.UPPER_BTN_HE.ToString(), 1);
                                    break;
                                case 3:
                                    HandleUI(ControllerPart.UPPER_BTN_LASER.ToString(), 1);
                                    break;
                                case 4:
                                    HandleUI(ControllerPart.UPPER_BTN_PGM.ToString(), 1);
                                    break;
                                case 5:
                                    HandleUI(ControllerPart.UPPER_BTN_KWG.ToString(), 1);
                                    break;
                                case 0:
                                     HandleUI(ControllerPart.UPPER_BTN_Clear.ToString(), 0);
                                     HandleUI(ControllerPart.UPPER_BTN_HE.ToString(), 0);
                                     HandleUI(ControllerPart.UPPER_BTN_LASER.ToString(), 0);
                                     HandleUI(ControllerPart.UPPER_BTN_PGM.ToString(), 0);
                                     HandleUI(ControllerPart.UPPER_BTN_KWG.ToString(), 0);
                                    break;
                                default:
                                    break;
                            }
                            HandleUI(ControllerPart.UPPER_BTN_Loaded.ToString(), data[1]);
                            HandleUI(ControllerPart.UPPER_BTN_PrimeStart1.ToString(), data[2]);
                            HandleUI(ControllerPart.UPPER_BTN_PrimeStart2.ToString(), data[3]);
                            HandleUI(ControllerPart.UPPER_BTN_PrimeStart3.ToString(), data[4]);
                            HandleUI(ControllerPart.UPPER_BTN_TargetClear1.ToString(), data[5]);
                            HandleUI(ControllerPart.UPPER_BTN_TargetClear2.ToString(), data[6]);
                            HandleUI(ControllerPart.UPPER_BTN_TargetClear3.ToString(), data[7]);
                            HandleUI(ControllerPart.UPPER_BTN_TargetClear4.ToString(), data[8]);
                            HandleUI(ControllerPart.UPPER_BTN_Chamber.ToString(), data[9]);
                        });
                        receiveT.Start();
                        
                    });
                    await Task.Delay(16); 
                }
            });
            t.Start();

            //List<int> values = new List<int>();
            //for (int i = 0; i < ledMapping.Values.Max(n => n.to) + 1; i++)
            //{
            //    var controlerPart = ledMapping.Single(kvp => kvp.Value.from <= i && kvp.Value.to >= i).Key;
            //    var color = ledColorMapping[controlerPart];
            //    var str = "00000000" + Convert.ToString(color.R, 2).PadLeft(8, '0') + Convert.ToString(color.G, 2).PadLeft(8, '0') + Convert.ToString(color.B, 2).PadLeft(8, '0');
            //    values.Add(Convert.ToInt32(str, 2));
            //}
            //var data = string.Join("," + Environment.NewLine, values); 
        }

        private int MapValue(int val, int isMinimum, int isMaximum, int targetMinimum, int targetMaximum)
        {
            return (val - isMinimum) * (targetMaximum - targetMinimum) / (isMaximum - isMinimum) + targetMinimum;
        }

        private void HandleUI(string controllerPart, byte information)
        {
            var mapping = controlMappings.Single(kvp => kvp.EnumName == controllerPart);
            if (mapping != null)
            {
                var control = mapping.MappedControl;
                var datatype = mapping.DataType;
                var propertyName = mapping.PropertyName;

                if (datatype == typeof(bool))
                {
                    bool boolValue = information != 0;
                    control.Invoke(new Action(() => control.GetType().GetProperty(propertyName).SetValue(control, boolValue)));

                    //wenn das letzte Zeichen eine Zahl ist, dann speichere sie als wert
                    var lastChar = controllerPart[controllerPart.Length - 1];
                    var number = lastChar - '0';
                    if (mapping.callAction == PrimeInProgress) // Progressbar and Target Lists
                    {
                        if(number >= 0 && number <= 9 && information != 0)
                        { 
                            mapping.callAction?.Invoke(number); 
                        }
                    }
                    else
                    {
                        mapping.callAction?.Invoke(information);
                    }
                }
                else if (datatype == typeof(int))
                {
                    int intValue = information;
                    control.GetType().GetProperty(propertyName).SetValue(control, intValue);
                }
                else if (datatype == typeof(string))
                {
                    string intValue = information.ToString();
                    control.GetType().GetProperty(propertyName).SetValue(control, intValue);
                }
            }
        }

        private void PrimeInProgress(int primeNumber)
        {
            PrimeInProgress(primeNumber, 20, true);
        }

        bool[] primeInProgressFlags = new bool[3] { false, false, false };
        private void PrimeInProgress(int primeNumber, int speed, bool turnOffPrimeReady)
        {
            if ((primeInProgressFlags[primeNumber - 1])) return; // If already in progress, ignore subsequent calls until it's done
            primeInProgressFlags[primeNumber - 1] = true;
            ProgressBar pb = null;

            switch (primeNumber)
            {
                case 1:
                    pb = pb_Upper_PrimeProgress1;
                    break;
                case 2:
                    pb = pb_Upper_PrimeProgress2;
                    break;
                case 3:
                    pb = pb_Upper_PrimeProgress3;
                    break;
            }

            if (turnOffPrimeReady)
            {
                var toTurnOff = "Upper_LED_PrimeDone";
                var convertedEnum = (ControllerPart)Enum.Parse(typeof(ControllerPart), toTurnOff + primeNumber.ToString());
                var ledMap = ledMapping[convertedEnum];
                SetByteInDataArrayByRange(ledMap.from, ledMap.to, 1);
            }

            var map = controlMappings.Single(ctlr => ctlr.MappedControl.Name == pb.Name);
            var color = ledColorMapping[(ControllerPart)Enum.Parse(typeof(ControllerPart), map.EnumName)];
            var size = ledMapping[(ControllerPart)Enum.Parse(typeof(ControllerPart), map.EnumName)];

            var numberOfLeds = size.to - size.from + 1;

            SetByteInDataArrayByRange(size.from, size.to, 0);

            Task.Delay(300).GetAwaiter().GetResult();
            Task.Run(async () =>
            {
                for (int i = 0; i <= 100; i++)
                {
                    await Task.Delay(speed);
                    var mappedValue = MapValue(i, 0, 100, size.from, size.to);
                    SetByteInDataArrayByRange(size.from, mappedValue, 1);
                    pb.Invoke(new Action(() => pb.Value = i));
                }

                SetByteInDataArrayByRange(size.from, size.to, 0);
                pb.Invoke(new Action(() => pb.Value = 0));

                if (turnOffPrimeReady)
                {
                    var ledMap1 = ledMapping[(ControllerPart)Enum.Parse(typeof(ControllerPart), "UPPER_LED_PrimeReady" + primeNumber.ToString())];
                    SetByteInDataArrayByRange(ledMap1.from, ledMap1.to, 0);
                    var ledMap2 = ledMapping[(ControllerPart)Enum.Parse(typeof(ControllerPart), "Upper_LED_PrimeDone" + primeNumber.ToString())];
                    SetByteInDataArrayByRange(ledMap2.from, ledMap2.to, 2);

                    var ledMap3 = ledMapping[ControllerPart.UPPER_LED_Chamber];
                    SetByteInDataArrayByRange(ledMap3.from, ledMap3.to, CheckForChamberReady() ? (byte)1 : (byte)0);
                }

                primeInProgressFlags[primeNumber - 1] = false;
            });
        }

        private void SelectedAmmo(int information)
        {
            void SetPrimeToReady(int info)
            {
                btn_Upper_Prime_Ready1.Invoke(new Action(() => btn_Upper_Prime_Ready1.Text = $"{info}"));
                btn_Upper_Prime_Ready2.Invoke(new Action(() => btn_Upper_Prime_Ready2.Text = $"{info}"));
                btn_Upper_Prime_Ready3.Invoke(new Action(() => btn_Upper_Prime_Ready3.Text = $"{info}"));

                SetByteInDataArrayByRange(info == 1 ? ledMapping[ControllerPart.UPPER_LED_PrimeReady1].from : ledMapping[ControllerPart.UPPER_LED_PrimeReady1].to,
                                         info == 1 ? ledMapping[ControllerPart.UPPER_LED_PrimeReady1].to : ledMapping[ControllerPart.UPPER_LED_PrimeReady1].from, 
                                         (byte)info);
                SetByteInDataArrayByRange(info == 1 ? ledMapping[ControllerPart.UPPER_LED_PrimeReady2].from : ledMapping[ControllerPart.UPPER_LED_PrimeReady2].to,
                                         info == 1 ? ledMapping[ControllerPart.UPPER_LED_PrimeReady2].to : ledMapping[ControllerPart.UPPER_LED_PrimeReady2].from, 
                                         (byte)info);
                SetByteInDataArrayByRange(info == 1 ? ledMapping[ControllerPart.UPPER_LED_PrimeReady3].from : ledMapping[ControllerPart.UPPER_LED_PrimeReady3].to,
                                            info == 1 ? ledMapping[ControllerPart.UPPER_LED_PrimeReady3].to : ledMapping[ControllerPart.UPPER_LED_PrimeReady3].from, 
                                            (byte)info);

                if(info == 1)
                {
                    for (int i = 1; i <= 3; i++)
                        PrimeInProgress(i, 1, false);
                }

            }

            if(cbx_Upper_Btn_KWG.Checked && cbx_Upper_Btn_Load.Checked)
            { 
                cbx_Upper_AmmoSelect.Invoke(new Action( () => cbx_Upper_AmmoSelect.SelectedIndex = cbx_Upper_AmmoSelect.Items.IndexOf("KWG")));
                SetPrimeToReady(1);
            }
            else if(cbx_Upper_Btn_PGM.Checked && cbx_Upper_Btn_Load.Checked)
            { 
                cbx_Upper_AmmoSelect.Invoke(new Action(() => cbx_Upper_AmmoSelect.SelectedIndex = cbx_Upper_AmmoSelect.Items.IndexOf("PGM")));
                SetPrimeToReady(1);
            }
            else if(cbx_Upper_Btn_HE.Checked && cbx_Upper_Btn_Load.Checked)
            { 
                cbx_Upper_AmmoSelect.Invoke(new Action(() => cbx_Upper_AmmoSelect.SelectedIndex = cbx_Upper_AmmoSelect.Items.IndexOf("HE")));
                SetPrimeToReady(1);
            }
            else if(cbx_Upper_Btn_Laser.Checked && cbx_Upper_Btn_Load.Checked)
            { 
                cbx_Upper_AmmoSelect.Invoke(new Action(() => cbx_Upper_AmmoSelect.SelectedIndex = cbx_Upper_AmmoSelect.Items.IndexOf("Laser")));
                SetPrimeToReady(1);
            }
            else if(cbx_Upper_Btn_Clear.Checked && cbx_Upper_Btn_Load.Checked)
            { 
                cbx_Upper_AmmoSelect.Invoke(new Action(() => cbx_Upper_AmmoSelect.SelectedIndex = cbx_Upper_AmmoSelect.Items.IndexOf("")));
                SetPrimeToReady(0);
            }
        }

        private void SetByteInDataArrayByRange(int from, int to, byte value)
        {
            if (from > to)
            {
                var temp = from;
                from = to;
                to = temp;
            }
            for (int i = from; i <= to; i++)
            {
                dataToSend[i] = value;
            }
        }

        private bool CheckForChamberReady()
        {
            var Prime1Done = ledMapping[ControllerPart.Upper_LED_PrimeDone1];
            var Prime2Done = ledMapping[ControllerPart.Upper_LED_PrimeDone2];
            var Prime3Done = ledMapping[ControllerPart.Upper_LED_PrimeDone3];

            
            for (int i = Prime1Done.from; i <= Prime1Done.to; i++)  if (dataToSend[i] < 2)  return false;
            for (int i = Prime2Done.from; i <= Prime2Done.to; i++) if (dataToSend[i] < 2)  return false;
            for (int i = Prime3Done.from; i <= Prime3Done.to; i++) if (dataToSend[i] < 2) return false;

            return true;
        }

        private void btn_Heat_Cooldown_Click(object sender, EventArgs e)
        {
            pb_Upper_LED_Heat.Value = 100;

            var size = ledMapping[ControllerPart.UPPER_LED_Heat];
            SetByteInDataArrayByRange(size.from, size.to, 1);

            Task.Delay(300).GetAwaiter().GetResult();
            Task.Run(async () =>
            {
                for (int i = 100; i >= 0; i--)
                {
                    await Task.Delay(100);
                    SetByteInDataArrayByRange(size.from, size.to, 0);
                    var mappedValue = MapValue(i, 0, 100, size.from, size.to);
                    SetByteInDataArrayByRange(size.from, mappedValue, 1);
                    pb_Upper_LED_Heat.Invoke(new Action(() => pb_Upper_LED_Heat.Value = i));
                }
                SetByteInDataArrayByRange(size.from, size.to, 0);
            });
        }

        private void btn_Upper_LED_Toogle_Feedback(object sender, EventArgs e)
        {
            var btn = sender as Button;
            var mapping = controlMappings.Single(kvp => kvp.MappedControl.Name == btn.Name);
            var ledMap = ledMapping[(ControllerPart)Enum.Parse(typeof(ControllerPart), mapping.EnumName)];
            var info = btn.Text == "" | btn.Text == "0" ? (byte)1 : (byte)0;

            HandleUI(mapping.EnumName, info);

            if (btn.Text == "0")
            {
                btn.BackColor = SystemColors.Control;
                SetByteInDataArrayByRange(ledMap.from, ledMap.to, 0);
            }
            else
            {
                btn.BackColor = Color.LimeGreen;
                SetByteInDataArrayByRange(ledMap.from, ledMap.to, 1);
            }
        }

        private void nud_Upper_ValueChanged(object sender, EventArgs e)
        {
            var tag = Convert.ToInt32((sender as NumericUpDown).Tag.ToString());
            dataToSend[tag] = Convert.ToByte((sender as NumericUpDown).Value);
        }

        private void cbx_Upper_AmmoSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            dataToSend[138] = Convert.ToByte(cbx_Upper_AmmoSelect.SelectedIndex);
        }

        private void btn_Upper_Energy_Click(object sender, EventArgs e)
        {
            pb_Upper_Energy.Value = 100;

            var size = ledMapping[ControllerPart.UPPER_LED_Energy];
            SetByteInDataArrayByRange(size.from, size.to, 1);

            Task.Delay(300).GetAwaiter().GetResult();
            Task.Run(async () =>
            {
                for (int i = 100; i >= 0; i--)
                {
                    await Task.Delay(500);
                    SetByteInDataArrayByRange(size.from, size.to, 0);
                    var mappedValue = MapValue(i, 0, 100, size.from, size.to);
                    SetByteInDataArrayByRange(size.from, mappedValue, 1);
                    pb_Upper_Energy.Invoke(new Action(() => pb_Upper_Energy.Value = i));
                }
                SetByteInDataArrayByRange(size.from, size.to, 0);
            });
        }
    }
}
