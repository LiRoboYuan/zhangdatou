# # 定时器用于实时刷新串口列表
# self.timer = QTimer(self)
# self.timer.timeout.connect(self.refresh_ports)
# self.timer.start(1000)  # 每1000毫秒（1秒）刷新一次
import sys
import time

import serial
import serial.tools.list_ports
import pyqtgraph as pg
import threading
import json

from PyQt5.QtCore import QThread, pyqtSignal
from PyQt5.QtWidgets import QApplication, QWidget, QCheckBox, QHBoxLayout, QVBoxLayout, QFileDialog, QComboBox, QLabel, QPushButton, QSlider,QTextEdit
from PyQt5.QtCore import Qt,QTimer, QTime

class MainWindow(QWidget):
    send_message_signal = pyqtSignal(bool)

    def __init__(self):
        super().__init__()

        self.motor_err = None
        self.satues = None
        self.pressure = 0
        self.array = []
        self.location_num = 0
        self.ports_all = 0
        self.ser = serial.Serial()
        self.folder_path = None
        # 初始化窗口
        self.setWindowTitle('串口配置')
        self.setGeometry(300, 300, 1200, 600)



        #主布局
        main_layout = QHBoxLayout ()
        lift_layout = QVBoxLayout()
        right_layout = QVBoxLayout()

        # 创建图形界面
        plot_layout = QHBoxLayout()
        pl_layout = QVBoxLayout()
        self.plot_widget = pg.PlotWidget()
        pl_layout.addWidget(self.plot_widget)
        pl_button_layout = QVBoxLayout()

        self.x_data_time = []  # 用于存储x轴时间数据
        self.y_data1 = []  # 第一条曲线的y数据
        self.y_data2 = []  # 第二条曲线的y数据，确保初始化为空列表
        # 创建两个 plot item 用于绘制两条曲线
        self.plot_item1 = self.plot_widget.plot(self.y_data1, self.x_data_time, pen='r', name="Curve 1")
        self.plot_item2 = self.plot_widget.plot(self.y_data2, self.x_data_time, pen='b', name="Curve 2")
        # 设置按钮之间的间距为5像素
        # 创建按钮
        self.clear_button = QPushButton('清除图形')
        self.refresh_button = QPushButton('刷新图形')
        self.choose_button = QPushButton('选择文件夹')
        # 创建勾选框
        self.checkbox = QCheckBox("是否保存文件", self)
        self.other_button = QPushButton('其他功能')
        # 设置按钮的固定大小（避免按钮大小不一致）
        self.clear_button.setFixedSize(100, 40)
        self.refresh_button.setFixedSize(100, 40)
        self.other_button.setFixedSize(100, 40)
        self.checkbox.setFixedSize(100, 40)
        self.choose_button.setFixedSize(100, 40)
        # 为按钮连接事件处理函数
        # self.clear_button.clicked.connect(self.clear_plot)
        # self.refresh_button.clicked.connect(self.refresh_plot)
        # self.other_button.clicked.connect(self.other_function)
        self.choose_button.clicked.connect(self.saveexcelfile)
        # 创建垂直布局
        pl_button_layout.addWidget(self.clear_button, alignment=Qt.AlignLeft)
        pl_button_layout.addWidget(self.refresh_button, alignment=Qt.AlignLeft)
        pl_button_layout.addWidget(self.choose_button, alignment=Qt.AlignLeft)
        pl_button_layout.addWidget(self.checkbox, alignment=Qt.AlignLeft)
        pl_button_layout.addWidget(self.other_button, alignment=Qt.AlignLeft)
        pl_button_layout.addStretch()
        self.time = 0
        #添加进布局
        # 让按钮占用一定的宽度并保持合理的布局
        plot_layout.addLayout(pl_button_layout)
        plot_layout.addLayout(pl_layout)
        # 创建布局
        butten_layout = QHBoxLayout()
        butten_layout.setAlignment(Qt.AlignLeft)
        self.port_label = QLabel('选择串口:')
        self.port_combo = QComboBox()
        self.populate_ports()  # 初始化串口列表
        # 连接下拉框弹出事件，每次弹出时刷新串口列表
        self.port_combo.currentIndexChanged.connect(self.on_port_change)
        butten_layout.addWidget(self.port_label, alignment=Qt.AlignBottom)
        butten_layout.addWidget(self.port_combo, alignment=Qt.AlignBottom)
        # 添加定时器，每隔1秒刷新一次串口列表
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.populate_ports)
        self.timer.start(1000)  # 每1000毫秒（1秒）刷新一次
        # 波特率选择下拉框
        self.baudrate_label = QLabel('选择波特率:')
        self.baudrate_combo = QComboBox()
        self.populate_baudrates()
        butten_layout.addWidget(self.baudrate_label, alignment=Qt.AlignBottom)
        butten_layout.addWidget(self.baudrate_combo, alignment=Qt.AlignBottom)
        # 确认按钮
        self.confirm_button = QPushButton('打开串口')
        self.confirm_button.setFixedSize(80, 25)
        self.confirm_button.clicked.connect(self.on_confirm)
        butten_layout.addWidget(self.confirm_button, alignment = Qt.AlignBottom)

        #设置电机控制滑块
        self.value_label = -5
        self.slider = QSlider(Qt.Horizontal)
        self.slider.setFixedWidth(300)  # 设置滑块宽度为200像素
        self.slider.setMinimum(-220)  # 设置最小值
        self.slider.setMaximum(-5)  # 设置最大值
        self.slider.setValue(-5)  # 设置初始值
        self.slider.valueChanged.connect(self.update_value)
        self.value_label = QLabel("当前位置: -5")

        dis_motor = QPushButton("失能电机")
        dis_motor.setFixedSize(100, 40)
        dis_motor.clicked.connect(lambda: self.counter_motor(1))
        en_motor = QPushButton("使能电机")
        en_motor.setFixedSize(100, 40)
        en_motor.clicked.connect(lambda: self.counter_motor(2))
        b0_layout = QHBoxLayout()
        # 设置按钮之间的间距（可选）
        b0_layout.setSpacing(10)
        # 将按钮添加到水平布局中
        b0_layout.addWidget(dis_motor)
        b0_layout.addWidget(en_motor)

        motor_init = QPushButton("电机归零")
        motor_init.setFixedSize(100, 40)
        motor_init.clicked.connect(lambda: self.counter_motor(3))
        motor_start = QPushButton("单次测试")
        motor_start.clicked.connect(self.json_print)
        motor_start.setFixedSize(100, 40)
        b1_layout = QHBoxLayout()
        b1_layout.setSpacing(10)
        b1_layout.addWidget(motor_init)
        b1_layout.addWidget(motor_start)
        # 将按钮布局添加到右侧布局中
        self.text_edit = QTextEdit(self)
        self.text_edit.setReadOnly(True)  # 设置为只读
        self.text_edit.setPlaceholderText("记录")  # 设置占位符文本
        self.text_edit.setFixedSize(320,300)

        b2_layout = QHBoxLayout()
        record_button = QPushButton("记录当前位置")
        record_button.setFixedSize(100, 100)
        record_button.clicked.connect(self.record_location)
        delete_button = QPushButton("删除")
        delete_button.setFixedSize(100, 100)
        delete_button.clicked.connect(self.delete_location)
        self.start_button = QPushButton("开始连续测试")
        self.start_button.setFixedSize(100, 100)
        self.start_button.clicked.connect(self.test_start)
        b2_layout.addWidget(record_button)
        b2_layout.addWidget(delete_button)
        b2_layout.addWidget(self.start_button)

        right_layout.addWidget (self.slider)
        right_layout.addWidget(self.value_label)
        right_layout.addLayout(b0_layout)
        right_layout.addLayout(b1_layout)
        right_layout.addWidget(self.text_edit)
        right_layout.addLayout(b2_layout)
        right_layout.addStretch()
        # 设置布局

        lift_layout.addLayout (plot_layout)
        lift_layout.addLayout (butten_layout)

        main_layout.addLayout(lift_layout)
        main_layout.addLayout(right_layout)
        self.setLayout(main_layout)
        # 线程相关
        self.thread_flag = False
        self.thread = threading.Thread(target=self.read_serialthread)
        self.thread.daemon = True
        self.thread.start()

        self.start_time = QTime.currentTime()  # 在初始化时记录起始时间
        self.dis_play_time = time.perf_counter()

    def update_value(self):
        # 获取滑块的当前值，并更新标签显示
        slider_value = int(self.slider.value())
        self.value_label.setText(f"当前位置: {slider_value}")
        send_data = bytearray([0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00])
        send_data[0] = 0xa5
        send_data[1] = 0x5a
        send_data[2] = 0x1a
        send_data[3] = 0x04

        send_data[4] = (-1*slider_value) & 0xFF
        send_data[5] = ((-1*slider_value) >> 8) & 0xFF  # 高字节
        send_data[6] = 0xa5
        send_data[7] = 0xa5
        if self.ser.is_open:
            self.ser.write(send_data)
            time.sleep(0.001)


    def test_start(self):
        if self.thread_flag:
            self.thread_flag = False
            #关闭线程

            self.start_button.setText("开始连续测试")
        else:
            if self.ser.is_open:
                self.thread_flag = True
                self.text_edit.append("测试运行中")

                self.start_button.setText("关闭任务")

                # 创建并启动串口线程

            else:
                self.text_edit.append("请先连接串口！！！")

    def record_location(self):
        if self.location_num >= len(self.array):
            # 如果当前行数不够，则增加新的行
            self.array.append([0] * 1)  # 每行有 1 列，初始值为 0（你可以根据需要调整列数）

        self.array[self.location_num][0] = int(self.slider.value())

        self.text_edit.clear()
        self.text_edit.append(f"已记录{self.location_num+1}个点位")

        self.location_num += 1
        if self.location_num > 0:
            for i in range(self.location_num):
                self.text_edit.append(f"第{i+1}个位置 {self.array[i][0]}")
                print(self.array[i][0])

    def delete_location(self):
        del self.array[-1]
        self.location_num -= 1
        self.text_edit.clear()
        self.text_edit.append(f"已记录{self.location_num}个点位")
        if self.location_num > 0:
            for i in range(self.location_num):
                self.text_edit.append(f"第{i+1}个位置 {self.array[i][0]}")
                print(self.array[i][0])

    def populate_ports(self):
        ports = self.get_available_ports()  # 获取当前串口列表（你需要实现这个函数）
        if self.ports_all != ports:
            self.port_combo.clear()  # 清空串口列表
            self.ports_all = ports
            self.port_combo.addItems(ports)  # 更新串口列表

    def on_port_change(self):
        """当选择串口时刷新列表"""
        # 在这里你可以处理选择的串口变化逻辑
        print(f"选择的串口: {self.port_combo.currentText()}")

    def get_available_ports(self):
        """获取当前可用的串口（这里根据操作系统可以调用相关接口）"""
        # 假设你使用的是 PySerial 获取串口列表
        import serial.tools.list_ports
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports]

    def populate_baudrates(self):
        """填充波特率选择下拉框"""
        baudrates = [9600, 115200, 460800, 921600]
        self.baudrate_combo.addItem("选择波特率")
        for baudrate in baudrates:
            self.baudrate_combo.addItem(str(baudrate))

    def display_two_data(self,presure,resis,err):
        # 获取当前时间与开始时间的时间差（以秒为单位）
        elapsed_time = self.start_time.elapsed() / 1000  # QTime.elapsed() 返回毫秒
        self.pressure = presure
        self.motor_err = err
        # 计算两条曲线的y值
        new_y1 = presure
        new_y2 = err

        # 添加新数据点到数据列表
        self.x_data_time.append(elapsed_time)  # 使用实际时间作为x轴
        self.y_data1.append(new_y1)  # 添加第一条曲线的y数据点
        self.y_data2.append(new_y2)  # 添加第二条曲线的y数据点
        MAX_DATA_POINTS = 1000
        if len(self.x_data_time) > MAX_DATA_POINTS:
            self.x_data_time = self.x_data_time[-MAX_DATA_POINTS:]
            self.y_data1 = self.y_data1[-MAX_DATA_POINTS:]
            self.y_data2 = self.y_data2[-MAX_DATA_POINTS:]
        # 更新图形
        # self.plot_item1.setData(self.y_data2, self.y_data1)
        # self.plot_item1.setData(self.x_data_time, self.y_data1)
        # 更新图形，只更新最近一个数据点

        self.plot_item1.setData(self.x_data_time, self.y_data1)
        if self.thread_flag == 1 and self.satues == 0:
            self.satues = 1
            print(self.satues)
        if self.satues == 1 and self.motor_err > 20:
            self.satues = 2
            print(self.satues)
        if self.satues == 2 and self.motor_err < 30:
            self.satues = 3

            print(self.satues)
        if self.satues == 4 and self.pressure > 100:
            self.satues = 5
            print(self.satues)
        # time.sleep(0.001)
        # self.plot_item2.setData(self.x_data_time, self.y_data2)


    def on_confirm(self):
        """点击确认按钮时的操作"""
        port = self.port_combo.currentText()
        baudrate = self.baudrate_combo.currentText()

        if port != "选择串口" and baudrate != "选择波特率":
            try:
                if self.ser.is_open:
                    self.confirm_button.setText("打开串口")
                    self.ser.close()  # 关闭串口连接
                    self.send_message_signal.emit(False)
                else:
                    self.ser = serial.Serial(port, baudrate=int(baudrate), timeout=1)
                    self.ser.set_buffer_size(rx_size=2048, tx_size=1024)  # 设置读取和写入缓冲区的大小
                    if self.ser.is_open:
                        self.confirm_button.setText("关闭串口")
                        #启动串口监视
                        self.monitoring_run = SerialThread(self.ser)
                        self.monitoring_run.two_signal.connect(self.display_two_data)


                        self.send_message_signal.connect(self.monitoring_run.update_loop)
                        # self.send_message_signal.emit(True)
                        self.monitoring_run.daemon = True
                        self.monitoring_run.start()
                    else:
                        self.confirm_button.setText("打开串口")
            except Exception as e:
                print(f"串口连接错误: {e}")
                # 更新按钮文字
                self.confirm_button.setText("打开串口")  # 设置按钮文字为 "连接错误"

        #     self.ser = serial.Serial(port, baudrate=int(baudrate), timeout=1)
        #     print(f"串口: {port}, 波特率: {baudrate}")
        else:
            self.start_button.setText("打开串口")
        #     print("请选择有效的串口和波特率!")

    def read_serialthread(self):
        test_num = 0
        self.satues = 0 #当前任务状态 0 初始化，1水平移动中，2水平移动到达一定区间,3水平移动完成，上下运动中,4,上下运动到一定区间，5，


        while 1 :

            # if self.satues == 4 and self.pressure < 10:
            #     self.satues = 0
                
            if self.thread_flag:
                if test_num == self.location_num:
                    self.thread_flag = False
                    self.start_button.setText("开始连续测试")

                    self.text_edit.append("测试结束")
                    test_num = 0
                    continue
                slider_value = self.array[test_num][0]
                if self.satues == 5 and self.pressure < 10:
                    test_num += 1
                    self.text_edit.append(f"测试进度{int(test_num / self.location_num * 100)}%")
                    self.satues = 1
                    print(self.satues)
                    time.sleep(1)
                self.value_label.setText(f"期望运行: {slider_value}")

                send_data = bytearray([0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
                send_data[0] = 0xa5
                send_data[1] = 0x5a
                send_data[2] = 0x1a
                send_data[3] = 0x04

                send_data[4] = (-1 * slider_value) & 0xFF
                send_data[5] = ((-1 * slider_value) >> 8) & 0xFF  # 高字节
                send_data[6] = 0xa5
                send_data[7] = 0xa5
                if self.satues == 3:
                    send_data[3] = 0x07
                    self.ser.write(send_data)
                    self.satues = 4
                    time.sleep(2)
                if self.ser.is_open and self.satues == 1:
                    self.ser.write(send_data)
                    time.sleep(2)
            time.sleep(0.001)
    def json_print(self):
        if self.ser.is_open:
            data = {
                "target_device_name": "test_3",
                "items": {
                    "CMD": {
                        "action": "get raw pressure",
                        "com": "5"
                    }
                }
            }

            parsed_data = str(data)
            parsed_data = parsed_data.encode()
            self.ser.write(parsed_data)
            # print(parsed_data)

    def counter_motor(self,flag):
        send_data = bytearray([0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
        send_data[0] = 0xa5
        send_data[1] = 0x5a
        send_data[2] = 0x1a

        if flag == 1:
            send_data[3] = 0x01
        elif flag == 2:
            send_data[3] = 0x02
        elif flag == 3:
            send_data[3] = 0x03
        elif flag == 5:
            send_data[3] = 0x05
        elif flag == 6:
            send_data[3] = 0x06
        elif flag == 7:
            send_data[3] = 0x07

        send_data[6] = 0xa5
        send_data[7] = 0xa5
        if self.ser.is_open:
            self.ser.write(send_data)

    def saveexcelfile(self):
        # 打开文件夹对话框，让用户选择保存新Excel文件的文件夹
        options = QFileDialog.Options()
        folderpath = QFileDialog.getExistingDirectory(self, "选择要保存的文件夹", "", options=options)

        if folderpath:  # 如果用户选择了文件夹
            self.folder_path = folderpath  # 保存文件夹路径

class SerialThread(QThread):    #接收串口数据
    two_signal = pyqtSignal(int,int,int)

    def __init__(self, serial_port,parent = None):
        super().__init__(parent)
        self.serial_port = serial_port  # 传入串口对象
        self.loop = True

    def run(self):
        port = self.serial_port
        while self.loop:
            if port.in_waiting:
                date = port.read(12)
                self.check_date(date)


    def update_loop(self,flag):
        self.loop = flag
        print("收到线程结束flag"+str(flag))

    def check_date(self,date):
        date_hand = date[0] | (date[1]<<8)
        tran_type = date[2]
        date_type = date[3]
        date_press = date[4]|(date[5]<<8)
        date_resis = date[6]|(date[7]<<8)
        motor_err  = date[8]|(date[9]<<8)
        if date_press >= 0x8000:
            date_press -= 0x10000

        date_check = date[10]|(date[11]<<8)
        # print("date_hand"+str(date_hand))
        # print("tran_type" + str(tran_type))
        # print("date_type" + str(date_type))

        # print("date_check" + str(date_check))
        if date_hand == 0xa55a:
            # print("date_press" + str(date_press))
            # print("date_resis" + str(date_resis))
            # print("motor_err" + str(motor_err))
            self.two_signal.emit(date_press,date_resis,motor_err)
        else:
            self.serial_port.flushInput()



if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())

