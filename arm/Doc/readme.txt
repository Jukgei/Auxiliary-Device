时间：2019年4月22日11:01:11

1.本程序用于控制3-DOF机械臂。
2.控制方法，串口通信。波特率：115200；数据位：8；校验：无； 停止位：1
3.通信协议：

	一个标准数据包15个字节，少于15按照前面的数据来解读，不影响最后的结果，数据包分为命令数据包和工作数据包。
	命令数据包的包头：0xCC 0xCC; 工作数据包的包头为：0xDD 0xDD.
	（下面命令后面的省略号代表任意数据都不影响）
	初始化命令（矫正Yaw角）： 0xCC 0xCC 0xAA ...	（必须要先初始化，才能响应其他的命令）
	机械臂所有电机停转	   ： 0xCC 0xCC 0xBB ...
	工作模式			   ： 0xDD 0xDD [Yaw:4 byte float] [Pitch:4 byte float] [Roll: 4 byte float] [gripper: 1 byte uint8_t]
	（以上弃用）
	
	一个控制数据包30个字节，按以下方式组织：
			[0xDD] [0xDD] [2 byte angles] [2 byte times] [2 byte angles] [2 byte times] [2 byte angles] [2 byte times] [2 byte angles] [2 byte times] [2 byte angles] [2 byte times] 
			[1 byte gripper] [2 byte yaw gimbal] [2 byte pitch gimbal] [1 byte feedback data] [~0xDD] [~0xDD]
			共30个字节。
			feedback data:由一个字节八位组成。第1-5位为1时返回1-5号电机位置。第6位为云台yaw\pitch。第0位为无人机高度。
			(gripper: 0x11:loose ; 0x22:grasp; 0x33:stop)
			由于大小端问题：要先发第八位再发高八位，例：500=0x01F4 发送顺序是F4 01
			
	反馈数据格式：
			[0xFF-ID] [0xFF-ID] [uint16_t position] 反馈舵机位置
			[0xEE] [0xEE][uint16_t distance] 反馈高度
			[0xAA] [0xAA] [Yaw: 4 byte float] [Pitch: 4 byte float]将接收到的yaw数据原封不动地发到TX-2
			
	与gimbal云台通信的格式：
		发送：	[0xAA] [0xAA]: 请求yaw\pitch
				
		接收：	[0xAA] [0xAA] [Yaw: 4 byte float] [Pitch: 4 byte float]
		
	信号不可信表示值：（指在TX2上收到的数据）
		高度： 		0xFFFF （读到此数时，表示不可信）
		舵机位置：  >1000 时候不可信
		云台角度：  若有返回，必可信
		

4.中断分配：				 优先级				管脚
	usart3：接收串口数据		0			PB10 |	PB11
	usart6：接受imu数据		\				PC7					(暂时取消)
	can1：	3个电机通信		\			PD0 R	 |	PD1	D		(取消)
	TIM6：	系统时基			1
	TIM7:	超声波定时计数	\			(暂时取消)
	EXIT0:	超声波管脚		\			PE0->Echo | PE1-> Trig	(暂时取消)
	usart1: 机械臂舵机		3			PA9->TX   | PA10->RX
	usart2: 激光雷达测高		2			PA2->TX	  |	PA3->RX
	TIM3:	输出4路PWM 			无		CH1:PB4 | CH2:PB3 | CH3:PB0 | CH4:PB1		// PB0是夹子，PB4:yaw和PB3:pitch
	GPIO    控制夹子电机正反转 	无		PC2 | PC3	pwm_out: PB0
			舵机控制            无		yaw：PB4		pitch：PB3 
	UART4	与云台通信		4			
	
	
5.电机控制周期：100ms

6.调试软件，山外上位机
			命令1：DD DD F4 01 2C 01 E0 01 14 00 00 00 0A 00 5A 00 63 00 43 00 75 00 11 78 05 35 05 01 22 22 （用于测试）


7.文件夹里的.m文件用于显示期望值和输出值曲线，用于看图调节PID。