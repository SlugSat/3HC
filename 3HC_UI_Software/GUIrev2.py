# /* ========================================
#  * FILE:   GUIrev2.py
#  * AUTHOR: Nick Jannuzzi
#  *
#  * CREATED ON March 30, 2020, 5:07 PM
#  *
#  * ========================================
#  *
#  * Copyright SLUGSAT, 2020
#  * All Rights Reserved
#  * UNPUBLISHED, LICENSED SOFTWARE.
#  *
#  * CONFIDENTIAL AND PROPRIETARY INFORMATION
#  * WHICH IS THE PROPERTY OF SLUGSAT.
#  *
#  * ========================================
# */



from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from USB import * 
import pyqtgraph as pg
import webbrowser
import NMEA_0183 as NMEA


class App(QMainWindow):
	def __init__(self):
		super().__init__()
		self.title = "SlugSat 3-Axis HHC/SVS Control Interface Rev. 2"
		self.setWindowTitle(self.title)
		self.setGeometry(0,0,500,1000)
		self.table_widget = MyTableWidget(self)
		self.setCentralWidget(self.table_widget)
		self.show()



		

class MyTableWidget(QWidget):
    
	def __init__(self, parent):
		super(QWidget, self).__init__(parent)
		self.layout = QVBoxLayout(self)
		self.tabs = QTabWidget()
		self.tab1 = QWidget()
		self.tab2 = QWidget()
		self.tab3 = QWidget()
		self.tab4 = QWidget()
		# self.tabs.resize(400,400)


		#variables-----------------------------------------------
		self.x = 0
		self.y = 0
		self.z = 0
		self.test = 0

		#setpoint values
		self.XSET = 0.0
		self.YSET = 0.0
		self.ZSET = 0.0

		#flags for invalid setpoint input
		self.xflag = False
		self.yflag = False
		self.zflag = False

		#null offsets
		self.XNOFF = 0
		self.YNOFF = 0
		self.ZNOFF = 0

		#Current Sensor
		self.XCUR = 0
		self.YCUR = 0
		self.ZCUR = 0


		#other
		self.test = 0


		#end of variable declarations-----------------------------


		#initializes tab UI for each tab
		self.tab1UI()
		self.tab2UI()
		self.tab3UI()
		self.tab4UI()


		#Qtimers for periodic value updating
		#update null offset timer
		self.NullTimer = QTimer()
		self.NullTimer.setInterval(10)
		self.NullTimer.timeout.connect(self.updatenulloffsets)
		


		#update current sensors' readings timer
		self.ITimer = QTimer()
		self.ITimer.setInterval(50)
		self.ITimer.timeout.connect(self.updateIread)
		



		self.USBTimer = QTimer()
		self.USBTimer.setInterval(10)
		self.USBTimer.timeout.connect(self.updateUSBParam)
		

		self.tabs.addTab(self.tab1,"HHC")
		self.tabs.addTab(self.tab2,"HHC Field Vector Graphs")
		self.tabs.addTab(self.tab3,"Solar Vectors")
		self.tabs.addTab(self.tab4,"SOP and Useful Links")
        # Add tabs to widget
		self.layout.addWidget(self.tabs)
		self.setLayout(self.layout)


		# init USB
		self.USB = USBprimary()
		#check if USb is connected
		if self.USB.verify() == True:
			print("USB connection verified")
		self.USB.writeUSB("\n")	
		self.NullTimer.start()
		self.ITimer.start()	
		self.USBTimer.start()


	#processes NMEA messages and sends data to its appropriate variable
	def Process_NMEA(self,NMEA_list):
		#store list elements into variables for comparisons
		ID = NMEA_list[0]
		Payload = NMEA_list[1]
		# if(ID == 'ERR'):
		# 	pass
		if(ID == "XCUR"):
			self.XCUR = Payload
			pass
		if(ID == "YCUR"):
			self.YCUR = Payload
			pass
		if(ID == "ZCUR"):	
			self.ZCUR = Payload
			pass
		if(ID == "XMAG"):
			pass
		if(ID == "YMAG"):
			pass
		if(ID == "ZMAG"):
			pass
		if(ID == "XSET"):
			pass
		if(ID == "YSET"):
			pass
		if(ID == "ZSET"):
			pass					



	def SetpointsEntered(self):
		#handles 0 case due to unexpected functionality enocuntered
		if(self.XSPoint.text() == ""):
			self.XSET = 0.0
		else:
		 	self.XSET = float(self.XSPoint.text())
		if(self.YSPoint.text() == ""): 
			self.YSET = 0.0
		else:
			self.YSET = float(self.YSPoint.text())
		if(self.ZSPoint.text() == ""):
			self.ZSET =0.0
		else:
			 self.ZSET = float(self.ZSPoint.text())
		self.UpdateSetpoints()	

#updates current values dispaly for setpoints	
	def UpdateSetpoints(self):
		#makes sure we have a valid input

		#X
		if(self.XSET and self.XSPoint.hasAcceptableInput()):
			self.XS.setText('X Setpoint (Current Value: %s)' % self.XSET)
		else:
			if(self.XSET == 0):
				self.XS.setText('X Setpoint (Current Value: %s)' % 0)
			else:	
				self.xflag = True
		#Y
		if(self.YSET and self.YSPoint.hasAcceptableInput()):	
			self.YS.setText('Y Setpoint (Current Value: %s)' % self.YSET)
		else:	
			if(self.YSET == 0):
				self.YS.setText('X Setpoint (Current Value: %s)' % 0)
			else:	
				self.yflag = True
		#Z
		if(self.ZSET and self.ZSPoint.hasAcceptableInput()):
			self.ZS.setText('Z Setpoint (Current Value: %s)' % self.ZSET)
		else:	
			if(self.ZSET == 0):
				self.ZS.setText('Z Setpoint (Current Value: %s)' % 0)
			else:	
				self.zflag = True

		#encodes NMEA messages
		self.XSETE = NMEA.Encode('SETX',str(self.XSET))
		self.YSETE = NMEA.Encode('SETY',str(self.YSET))
		self.ZSETE = NMEA.Encode('SETZ',str(self.ZSET))	


		# NMEA.Decode(self.XSETE)

		#send USB messages
		self.USB.writeUSB(self.XSETE)	
		self.USB.writeUSB(self.YSETE)	
		self.USB.writeUSB(self.ZSETE)			
		# pg.plot([self.XSP],[self.YSP])		
			#invalid input popup
		if(self.xflag or self.yflag or self.zflag):
			SPInvalid = QMessageBox()
			SPInvalid.setText('Invalid input(s) for one or more axis setpoints; valid range is -200 to 200 with 3 decimal place maximum precision for each setpoint')
			self.xflag = False
			self.yflag = False
			self.zflag = False
			SPInvalid.exec_()	


	def updateUSBParam(self):
		
		self.test = self.USB.readUSB()
		self.test = self.test.replace("\n","")
		# self.USB.writeUSB(str(self.XSP))	


	#updates the null offset values
	def updatenulloffsets(self):
		#a = self.GUIUSB.read()
		self.x += 1
		self.y += 1
		self.z += 1
		# self.test = self.USB.readUSB()
		# self.test = self.test.replace("\n","")
		self.XNULLreadout.setText('%s' % self.x)
		self.YNULLreadout.setText('%s' % self.y)
		self.ZNULLreadout.setText('%s' % self.z)	


	def updateIread(self):
		# self.XCUR += 1
		# self.YCUR += 1
		# self.ZCUR += 1
		self.USB.writeUSB(NMEA.Encode('READ',"XCUR"))
		CNMEA = NMEA.Decode(self.USB.readUSB())
		self.Process_NMEA(CNMEA)


		self.USB.writeUSB(NMEA.Encode('READ',"YCUR"))
		CNMEA2 = NMEA.Decode(self.USB.readUSB())
		self.Process_NMEA(CNMEA2)

		self.USB.writeUSB(NMEA.Encode('READ',"ZCUR"))
		CNMEA3 = NMEA.Decode(self.USB.readUSB())
		self.Process_NMEA(CNMEA3)
		# print(CNMEA)
		self.XIreadout.setText('%s Amps' % self.XCUR)
		self.YIreadout.setText('%s Amps' % self.YCUR)
		self.ZIreadout.setText('%s Amps' % self.ZCUR)	


	def exitpressed(self):
		self.USB.writeUSB("$RST,*FF\n")
		self.close()		

	def tab4buttonpressed(self):
		webbrowser.open('https://docs.google.com/forms/d/e/1FAIpQLSfV1I_m_XHpySkWWFHY50TkHchBT7OENFbcBqAayC7Oqqf6HQ/formResponse')	


	def HHCbuttonpressed(self):
		webbrowser.open('https://en.wikipedia.org/wiki/Helmholtz_coil')


	def WMMbuttonpressed(self):
		webbrowser.open('https://www.ngdc.noaa.gov/geomag/WMM/')


	# def USBbuttonpressed(self):		
	# 	# self.test = self.USB.read()
	# 	self.USB.writeUSB(str(self.x))
	# 	# l = len("hi")
	# 	print("Received: " + str(self.USB.readUSB())) # receive the echo





	#Tab1: HHC control interface
	def tab1UI(self):


		#setpoint label
		self.tab1.sp = QHBoxLayout()
		self.spLabel = QLabel('HHC Axis Setpoints',self)
		self.spLabel.setFont(QFont('Arial', 20))
		self.tab1.sp.addWidget(self.spLabel)



		self.tab1.no = QHBoxLayout()
		self.calLabel = QLabel('HHC Null Offsets',self)
		self.calLabel.setFont(QFont('Arial', 20))
		self.tab1.no.addWidget(self.calLabel)
		self.tab1.I = QHBoxLayout()
	
		self.ILabel = QLabel('Current Sensor Readouts',self)
		self.ILabel.setFont(QFont('Arial', 20))
		self.tab1.I.addWidget(self.ILabel)


		#X Setpoint
		self.tab1.hlayout = QHBoxLayout()
		self.XSPoint =  QLineEdit(self)
		self.XSPoint.setValidator(QDoubleValidator(-200.0, 200.0, 3))
		self.tab1.hlayout.addWidget(self.XSPoint)
		self.XS = QLabel('X Setpoint (Current Value: %d)' % self.XSET,self)
		self.tab1.hlayout.addWidget(self.XS)
		self.tab1.hlayout.addStretch(1)

		#X Null Offset
		self.tab1.hlayout1b = QHBoxLayout()
		self.XNULLLabel = QLabel('X:',self)
		self.tab1.hlayout1b.addWidget(self.XNULLLabel)
		self.XNULLreadout = QLabel('%d'% 0,self)
		self.tab1.hlayout1b.addWidget(self.XNULLreadout)
		self.tab1.hlayout1b.addStretch(2)

		#X-axis current sensor
		self.tab1.hlayout1c = QHBoxLayout()
		self.XILabel = QLabel('X:',self)
		self.tab1.hlayout1c.addWidget(self.XILabel)
		self.XIreadout = QLabel('%d Amps'% 0,self)
		self.tab1.hlayout1c.addWidget(self.XIreadout)
		self.tab1.hlayout1c.addStretch(2)

		#Y Setpoint
		self.tab1.hlayout2 = QHBoxLayout()
		self.YSPoint =  QLineEdit(self)
		self.YSPoint.setValidator(QDoubleValidator(-200.0, 200.0, 3))
		self.tab1.hlayout2.addWidget(self.YSPoint)
		self.YS = QLabel('Y Setpoint (Current Value: %d)' % self.YSET,self)
		self.tab1.hlayout2.addWidget(self.YS)
		self.tab1.hlayout2.addStretch(1)

		#Y Null Offset
		self.tab1.hlayout2b = QHBoxLayout()
		self.YNULLLabel = QLabel('Y:',self)
		self.tab1.hlayout2b.addWidget(self.YNULLLabel)
		self.YNULLreadout = QLabel('%d'% 0,self)
		self.tab1.hlayout2b.addWidget(self.YNULLreadout)
		self.tab1.hlayout2b.addStretch(2)

		#Y-axis current readout
		self.tab1.hlayout2c = QHBoxLayout()
		self.YILabel = QLabel('Y:',self)
		self.tab1.hlayout2c.addWidget(self.YILabel)
		self.YIreadout = QLabel('%d Amps'% 0,self)
		self.tab1.hlayout2c.addWidget(self.YIreadout)
		self.tab1.hlayout2c.addStretch(2)		


		#Z Setpoint
		self.tab1.hlayout3 = QHBoxLayout()
		self.ZSPoint =  QLineEdit(self)
		self.ZSPoint.setValidator(QDoubleValidator(-200.0, 200.0, 3))
		self.tab1.hlayout3.addWidget(self.ZSPoint)
		self.ZS = QLabel('Z Setpoint (Current Value: %d)' % self.ZSET,self)
		self.tab1.hlayout3.addWidget(self.ZS)
		self.tab1.hlayout3.addStretch(1)

		#Z Null Offset
		self.tab1.hlayout3b = QHBoxLayout()
		self.ZNULLLabel = QLabel('Z:',self)
		self.tab1.hlayout3b.addWidget(self.ZNULLLabel)
		self.ZNULLreadout = QLabel('%d'% 0,self)
		self.tab1.hlayout3b.addWidget(self.ZNULLreadout)
		self.tab1.hlayout3b.addStretch(2)

		#Z-axis Current readout
		self.tab1.hlayout3c = QHBoxLayout()
		self.ZILabel = QLabel('Z:',self)
		self.tab1.hlayout3c.addWidget(self.ZILabel)
		self.ZIreadout = QLabel('%d Amps'% 0,self)
		self.tab1.hlayout3c.addWidget(self.ZIreadout)
		self.tab1.hlayout3c.addStretch(2)	

		#X,Y,Z setpoint enter button
		self.tab1.hlayout4 = QHBoxLayout()
		self.XYZSPButton = QPushButton('Set X, Y, and Z Setpoints',self)
		self.XYZSPButton.clicked.connect(self.SetpointsEntered)
		self.tab1.hlayout4.addWidget(self.XYZSPButton)
		self.tab1.hlayout4.addStretch(2)
		

		#credits
		self.tab1.credits = QHBoxLayout()
		self.creditlabel = QLabel('Created by Nicholas Jannuzzi for usage with the SlugSat 3-Axis HHC (April 2020)',self)
		self.creditlabel.setFont(QFont('Arial', 12))
		self.tab1.credits.addWidget(self.creditlabel)
		self.tab1.credits.addStretch(2)
		self.tab1.credits2 = QHBoxLayout()
		self.creditlabel2 = QLabel('Designed using PyQt5, a Python wrapper of the C++ Qt designer suite',self)
		self.creditlabel2.setFont(QFont('Arial', 12))
		self.tab1.credits2.addWidget(self.creditlabel2)
		self.tab1.credits2.addStretch(2)


		#close app button
		self.tab1.close = QHBoxLayout()
		self.exitbutton = QPushButton('Click this to exit the UI',self)
		self.exitbutton.clicked.connect(self.exitpressed)
		self.tab1.close.addWidget(self.exitbutton)
		self.tab1.close.addStretch(2)


		#vertically arranges hboxes
		self.tab1.vlayout = QVBoxLayout()

		#setpoints
		self.tab1.vlayout.addLayout(self.tab1.sp)
		self.tab1.vlayout.addLayout(self.tab1.hlayout)
		self.tab1.vlayout.addLayout(self.tab1.hlayout2)
		self.tab1.vlayout.addLayout(self.tab1.hlayout3)
		self.tab1.vlayout.addLayout(self.tab1.hlayout4)
		self.tab1.vlayout.addStretch(1)


		#null
		self.tab1.vlayout.addLayout(self.tab1.no)
		self.tab1.vlayout.addLayout(self.tab1.hlayout1b)
		self.tab1.vlayout.addLayout(self.tab1.hlayout2b)
		self.tab1.vlayout.addLayout(self.tab1.hlayout3b)
		self.tab1.vlayout.addStretch(1)


		#current reading
		self.tab1.vlayout.addLayout(self.tab1.I)
		self.tab1.vlayout.addLayout(self.tab1.hlayout1c)
		self.tab1.vlayout.addLayout(self.tab1.hlayout2c)
		self.tab1.vlayout.addLayout(self.tab1.hlayout3c)
		self.tab1.vlayout.addStretch(1)

		#exit button
		self.tab1.vlayout.addLayout(self.tab1.close)
		self.tab1.vlayout.addStretch(1)

		#credits
		self.tab1.vlayout.addLayout(self.tab1.credits)
		self.tab1.vlayout.addLayout(self.tab1.credits2)
		self.tab1.vlayout.addStretch(1)





		

		self.tab1.setLayout(self.tab1.vlayout)



	#Tab 2: HHC plotting (pH)
	def tab2UI(self):
		pass
	


	


	#tab3: SVS control interface (pH)
	def tab3UI(self):
		pass


	#tab4: SOP/User Manual and Useful Links
	def tab4UI(self):
		self.tab4.vlayout = QVBoxLayout()

		self.SOPLabel = QLabel('Click the following link to view the SOP and User manual',self)
		self.SOPLabel.setAlignment(Qt.AlignCenter)
		self.button = QPushButton('HHC SOP and User Manual',self)

		self.HHCLabel = QLabel('Basic Info on Helmholtz Coils',self)
		self.HHCLabel.setAlignment(Qt.AlignCenter)
		self.HHCbutton = QPushButton('Helmholtz Coil info',self)

		self.WMMLabel = QLabel('World Magnetic Model Website',self)
		self.WMMLabel.setAlignment(Qt.AlignCenter)
		self.WMMbutton = QPushButton('World Magnetic Model (WMM)',self)


		# self.Label = QLabel('USB test',self)
		# self.Label.setAlignment(Qt.AlignCenter)
		# self.USBbutton = QPushButton('USB test',self)




		self.button.clicked.connect(self.tab4buttonpressed)
		self.HHCbutton.clicked.connect(self.HHCbuttonpressed)
		self.WMMbutton.clicked.connect(self.WMMbuttonpressed)
		# self.USBbutton.clicked.connect(self.USBbuttonpressed)







		self.tab4.vlayout.addWidget(self.SOPLabel)
		self.tab4.vlayout.addWidget(self.button)
		self.tab4.vlayout.addWidget(self.HHCLabel)
		self.tab4.vlayout.addWidget(self.HHCbutton)
		self.tab4.vlayout.addWidget(self.WMMLabel)
		self.tab4.vlayout.addWidget(self.WMMbutton)
		# self.tab4.vlayout.addWidget(self.Label)
		# self.tab4.vlayout.addWidget(self.USBbutton)
		self.tab4.vlayout.addStretch(2)
		self.tab4.setLayout(self.tab4.vlayout)



#Actually runs the GUI (Main loop)
if __name__=='__main__':
	app = QApplication(sys.argv)
	mainWindow = App()
	mainWindow.show()
	sys.exit(app.exec_())
