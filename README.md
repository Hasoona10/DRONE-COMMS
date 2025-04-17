# 🛰️ SkyLink – Wireless Drone Controller

A professional ground control station application for controlling custom-built drones using the Adafruit Feather M0 RFM69 wireless module.

## 🌟 Features

- **Real-time Control**: Send directional commands to your drone using WASD or arrow keys
- **Professional UI**: Modern dark theme interface with status indicators
- **Command Logging**: Track all sent commands in real-time
- **Serial Port Management**: Easy connection to your Feather M0 RFM69
- **Status Monitoring**: Visual connection status and battery level indicators
- **Expandable**: Ready for future telemetry and additional control features

## 🎮 Control Scheme

- `W` or `↑` - Move Forward
- `A` or `←` - Move Left
- `S` or `↓` - Move Backward
- `D` or `→` - Move Right
- `Space` - Stop/Emergency Stop

## 🛠️ Hardware Requirements

### Ground Station (Laptop)
- Computer running macOS, Windows, or Linux
- USB port for serial communication
- Adafruit Feather M0 RFM69 module
- USB cable for connecting the Feather

### Drone
- Adafruit Feather M0 RFM69 (for receiving commands)
- MPU6050 (6-axis gyroscope/accelerometer)
- 2× DRV8833 Motor Drivers
- 4× Coreless Motors
- 4× Propellers
- 3.7V 1200mAh Li-Po Battery
- Custom 3D-printed frame
- Wiring and connectors

## 💻 Software Requirements

- Python 3.8 or higher
- PyQt6
- pyserial

## 🚀 Installation

1. Clone this repository:
```bash
git clone https://github.com/yourusername/skylink.git
cd skylink
```

2. Install the required dependencies:
```bash
pip install -r requirements.txt
```

3. Run the application:
```bash
python drone_controller.py
```

## 🔌 Usage

1. Connect your Adafruit Feather M0 RFM69 to your computer via USB
2. Launch the SkyLink application
3. Select the appropriate serial port from the dropdown menu
4. Click "Connect" to establish communication with the Feather
5. Use WASD or arrow keys to control the drone
6. Monitor the command log for sent instructions
7. Check the status indicator for connection status

## 🔧 Troubleshooting

### Common Issues

1. **Port Not Found**
   - Ensure the Feather is properly connected
   - Try refreshing the port list
   - Check if the correct drivers are installed

2. **Connection Failed**
   - Verify the correct baud rate (9600)
   - Check if the port is already in use
   - Ensure proper permissions for serial access

3. **Commands Not Working**
   - Verify the connection status
   - Check the command log for errors
   - Ensure the drone's receiver is powered on

## 🔮 Future Enhancements

- [ ] Real-time telemetry display
- [ ] Battery level monitoring
- [ ] Flight data recording
- [ ] Custom control mapping
- [ ] Emergency stop button
- [ ] Flight mode selection
- [ ] Altitude control
- [ ] Camera feed integration


**Hasan** - Computer Science Student

## 🙏 Acknowledgments

- Adafruit for the Feather M0 RFM69 module
- PyQt6 team for the GUI framework
- The open-source drone community

---

