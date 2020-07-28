# /* ========================================
#  * FILE:   3HC_UI.py
#  * AUTHOR: Nick Jannuzzi/Caleb Cotter
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


#requirements for install: PyUSB 1.0.2, PyQt5 5.14.2
#additional libraries necessary: NMEA_0183.py, USB.py

from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from USB import * 
import webbrowser
import time
import NMEA_0183 as NMEA


class App(QMainWindow):
	#init function sets up the whole thing
	def __init__(self):
		super().__init__()
		self.title = "SlugSat 3-Axis HHC/SVS Control Interface Rev. 2.5"
		self.setWindowTitle(self.title)
		self.setGeometry(0,0,500,1000)
		self.table_widget = MyTableWidget(self)
		self.setCentralWidget(self.table_widget)
		self.show()


class MyTableWidget(QWidget):
    #init handles setting up the Qt widget class and all the necessary variables needed
	def __init__(self, parent):
		super(QWidget, self).__init__(parent)
		self.layout = QVBoxLayout(self)
		self.tabs = QTabWidget()
		self.tab1 = QWidget()
		self.tab2 = QWidget()
		self.tab3 = QWidget()


		#variables-----------------------------------------------
		self.x = 0
		self.y = 0
		self.z = 0
		self.test = 0

		#setpoint values
		self.XSET = 0.0
		self.YSET = 0.0
		self.ZSET = 0.0

		self.XCAL = 0
		self.YCAL = 0
		self.ZCAL = 0

		#magnetometer data
		self.XMAG = 0.0
		self.YMAG = 0.0
		self.ZMAG = 0.0

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
		#make this 1 to stop USB with changing mode, good for when you can't actually connect to the cage but want to see the UI
		self.disableUSB = 0
		self.mode = 0
		#this changes when system is changed from ARMED to IDLE
		#1 = AMRED, 0 = IDLE

		#Calibration status, 0 = uncalibrated, 1 = in process, 2 = dcalibrated
		self.calstatus = 0


		#end of variable declarations-----------------------------


		#initializes UI layout for each tab
		self.tab1UI()
		self.tab2UI()
		self.tab3UI()
		# self.tab4UI()



		#Qt runs application in a while loop inside main
		#timers allow periodic sampling without blocking the main loop
		#changing the interval of the time changes how often the linked function is called
		#all functions are called upon timeout of the set interval
		self.DataTimer = QTimer()
		self.DataTimer.setInterval(100)
		self.DataTimer.timeout.connect(self.pulldata)
		self.CalibrationTimer = QTimer()
		self.CalibrationTimer.setInterval(5000)
		self.CalibrationTimer.timeout.connect(self.doneCalibrating)
		#start timers for USB read/write if USB is enabled
		if(self.disableUSB == 0 and self.mode == 1):
			self.DataTimer.start()
		#define tabe names	
		self.tabs.addTab(self.tab1,"HHC")
		#tab 2 is a future work
		self.tabs.addTab(self.tab2,"Solar Vectors")
		self.tabs.addTab(self.tab3,"SOP and Useful Links")
        # Add tabs to widget
		self.layout.addWidget(self.tabs)
		self.setLayout(self.layout)


		# init USB handles setting up the USB connection class item
		if(self.disableUSB == 0):
			self.USB = USBprimary()
			#check if USb is connected
			if(self.USB.verify() == True):
				print("USB connection verified")






	#processes NMEA messages and sends data to its appropriate variable
	#NMEA protocol allows us to feed all messages into a single chain of conditionals for processing
	#this will update all internal readouts and variables
	def Process_NMEA(self,NMEA_list):
		#bad data pauses USB until reset occurs, then continues
		if(NMEA_list == None):
			print("Error Encountered: Bad Data. Resetting device; this will pause data feed briefly!")			
			self.USB.resetUSB()						
			return 
		#store list elements into variables for comparisons	
		ID = NMEA_list[0]
		Payload = NMEA_list[1]
		# print(ID)

		#basically a simple checker to route NMEA_0183 messages to their designated UI elements
		if(ID == "XCUR"):
			self.XCUR = float("%.4g" % float(Payload))
			self.XIreadout.setText(str(self.XCUR))

		if(ID == "YCUR"):
			self.YCUR = float("%.4g" % float(Payload))
			self.YIreadout.setText(str(self.YCUR))

		if(ID == "ZCUR"):	
			self.ZCUR = float("%.4g" % float(Payload))
			self.ZIreadout.setText(str(self.ZCUR))	

		if(ID == "XMAG"):
			self.XMAG = float("%.3g" % float(Payload))
			self.XMAGreadout.setText(str(self.XMAG))

		if(ID == "YMAG"):
			self.YMAG = float("%.3g" % float(Payload))
			self.YMAGreadout.setText(str(self.YMAG))

		if(ID == "ZMAG"):
			self.ZMAG = float("%.3g" % float(Payload))
			self.ZMAGreadout.setText(str(self.ZMAG))

		if(ID == "XSET"):
			self.XSET2 = float("%.3g" % float(Payload))
			#compares received setpoint with one set on UI end
			#overwrites UI one if PSoC one is different
			if(self.XSET2 != self.XSET):
				self.XSET = self.XSET2
				if(self.XSET == 0):
					self.XS.setText('X Setpoint (Current Value: %s)' % 0)
				else:					
					self.XS.setText('X Setpoint (Current Value: %s)' % self.XSET)


		if(ID == "YSET"):
			self.YSET2 = float("%.3g" % float(Payload))
			#compares received setpoint with one set on UI end
			#overwrites UI one if PSoC one is different
			if(self.YSET2 != self.YSET):
				self.YSET =self.YSET2
				if(self.YSET == 0):
					self.YS.setText('Y Setpoint (Current Value: %s)' % 0)
				else:	
					self.YS.setText('Y Setpoint (Current Value: %s)' % self.YSET)

		

		if(ID == "ZSET"):
			self.ZSET2 = float("%.3g" % float(Payload))
			#compares received setpoint with one set on UI end
			#overwrites UI one if PSoC one is different
			if(self.ZSET2 != self.ZSET):
				self.ZSET =self.ZSET2
				if(self.ZSET == 0):
					self.ZS.setText('Z Setpoint (Current Value: %s)' % 0)
				else:	
					self.ZS.setText('Z Setpoint (Current Value: %s)' % self.ZSET)



	#Calibration process to compensate for local magnetic field
	#Gathers a series of magnetic field readings on each axis, then averages them and stores internally as the offset
	def Calibration_Process(self, NMEA_list):
		ID = NMEA_list[0]
		Payload = NMEA_list[1]
		#X-Axis
		if(ID == "XMAG"):
			self.XCAL += float(Payload)
		#Y-Axis
		if(ID == "YMAG"):
			self.YCAL += float(Payload)
		#Z-Axis
		if(ID == "ZMAG"):
			self.ZCAL += float(Payload)

	#Switches the UI once calibration is done, allows arming of the 3HC 
	def doneCalibrating(self):
		self.CalLED.resize(80, 80)
		self.CalLED.setStyleSheet("border: 1px solid black; background-color: green; border-radius: 40px;")
		self.CalLED.setText('       Calibrated         ')
		self.calstatus = 2


	#Helper function to catch edge cases on inputs for the setpoints
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

	#updates current values displayed for setpoints	on the UI
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

		#invalid input popup		
		if(self.xflag or self.yflag or self.zflag):
			SPInvalid = QMessageBox()
			SPInvalid.setText('Invalid input(s) for one or more axis setpoints; valid range is +/-150+axis null offset with 3 decimal place maximum precision for each setpoint')
			self.xflag = False
			self.yflag = False
			self.zflag = False
			SPInvalid.exec_()

		#rounds the setpoint to 3 decimal places
		self.XSET -= round(self.XNOFF,3)
		self.YSET -= round(self.YNOFF,3)
		self.ZSET -= round(self.ZNOFF,3)

		#encodes NMEA messages, subtracting NULL offset for each axis		
		self.XSETE = NMEA.Encode('SETX',str(self.XSET))
		self.YSETE = NMEA.Encode('SETY',str(self.YSET))
		self.ZSETE = NMEA.Encode('SETZ',str(self.ZSET))	

		# #send USB messages across USB endpoint
		if(self.disableUSB == 0 and self.mode == 1):
			self.USB.writeUSB(self.XSETE)	
			self.USB.writeUSB(self.YSETE)	
			self.USB.writeUSB(self.ZSETE)		
	
	


	#pulls data from the firmware of the 3HC, passes into the processing function for decoding and routing
	def pulldata(self):
		#reads all data sequentially and processes
		if(self.disableUSB == 0 and self.mode == 1):
			self.USB.writeUSB(NMEA.Encode('READ',"XCUR"))
			CNMEA = NMEA.Decode(self.USB.readUSB())
			self.Process_NMEA(CNMEA)


			self.USB.writeUSB(NMEA.Encode('READ',"YCUR"))
			CNMEA2 = NMEA.Decode(self.USB.readUSB())
			self.Process_NMEA(CNMEA2)

			self.USB.writeUSB(NMEA.Encode('READ',"ZCUR"))
			CNMEA3 = NMEA.Decode(self.USB.readUSB())
			self.Process_NMEA(CNMEA3)

			self.USB.writeUSB(NMEA.Encode('READ',"XMAG"))
			NNMEA = NMEA.Decode(self.USB.readUSB())
			self.Process_NMEA(NNMEA)


			self.USB.writeUSB(NMEA.Encode('READ',"YMAG"))
			NNMEA2 = NMEA.Decode(self.USB.readUSB())
			self.Process_NMEA(NNMEA2)

			self.USB.writeUSB(NMEA.Encode('READ',"ZMAG"))
			NNMEA3 = NMEA.Decode(self.USB.readUSB())
			self.Process_NMEA(NNMEA3)






	#handles swapping from IDLE to ARMED and USB handling
	def modechange(self):
		#IDLE->ARMED
		if(self.mode == 0 and self.calstatus == 2):
			self.LED.resize(80, 80)
			self.LED.setStyleSheet("border: 1px solid black; background-color: green; border-radius: 40px;")
			self.LED.setText('       Armed         ')
			if(self.disableUSB == 0):
				#if we havent setup USB, do it here
				if(self.USB.verify() == False):
					self.USB = USBprimary()
					if(self.USB != None):
						self.USB.writeUSB(NMEA.Encode("ARMED","1"))

				elif(self.USB.verify() == True):
					self.USB.writeUSB(NMEA.Encode("ARMED","1"))
				self.DataTimer.start()
			self.mode = 1


		elif(self.mode == 1 and self.calstatus == 2):
			self.LED.resize(80, 80)
			self.LED.setStyleSheet("border: 1px solid black; background-color: red; border-radius: 40px;")
			self.LED.setText('        Idle         ')
			if(self.disableUSB == 0):
				self.USB.writeUSB(NMEA.Encode("IDLE","0"))
				self.DataTimer.stop()
			self.mode = 0


	#runs calibration scheme on 3HC
	def calibrate3HC(self):
		# if(self.calstatus != 0):
		self.XSETE = NMEA.Encode('SETX',0)
		self.YSETE = NMEA.Encode('SETY',0)
		self.ZSETE = NMEA.Encode('SETZ',0)	
		#clear setpoints
		self.USB.writeUSB(self.XSETE)	
		self.USB.writeUSB(self.YSETE)	
		self.USB.writeUSB(self.ZSETE)	
		#reset stored calibration values
		xcal = 0
		ycal = 0
		zcal = 0
		self.XCAL = 0
		self.YCAL = 0
		self.ZCAL = 0


		self.calstatus = 1
		#take 20 data samples for each axis of the magnetometer, then take the average on each axis
		for x in range(0,20):
			self.USB.writeUSB(NMEA.Encode('READ',"XMAG"))
			xcal = NMEA.Decode(self.USB.readUSB())
			self.Calibration_Process(xcal)

			self.USB.writeUSB(NMEA.Encode('READ',"YMAG"))
			ycal = NMEA.Decode(self.USB.readUSB())
			self.Calibration_Process(ycal)
			# print('Y' + str(ycal))

			self.USB.writeUSB(NMEA.Encode('READ',"ZMAG"))
			zcal = NMEA.Decode(self.USB.readUSB())
			self.Calibration_Process(zcal)

			time.sleep(0.03)
		#sleep gives better user interface experience	
		time.sleep(2)
		#finish taking average	
		self.XNOFF = self.XCAL/20
		self.YNOFF = self.YCAL/20
		self.ZNOFF = self.ZCAL/20
		self.XNULLreadout.setText('%f'% self.XNOFF)
		self.YNULLreadout.setText('%f'% self.YNOFF)
		self.ZNULLreadout.setText('%f'% self.ZNOFF)
		#adjusts setpoint input ranges to account for null offset
		self.XSPoint.setValidator(QDoubleValidator(-150.0+self.XNOFF, 150.0+self.XNOFF, 3))

		self.YSPoint.setValidator(QDoubleValidator(-150.0+self.YNOFF, 150.0+self.YNOFF, 3))

		self.ZSPoint.setValidator(QDoubleValidator(-150.0+self.ZNOFF, 150.0+self.ZNOFF, 3))

		#enable user to arm 3HC 
		self.calstatus = 2
		#update LED
		self.CalLED.resize(80, 80)
		self.CalLED.setStyleSheet("border: 1px solid black; background-color: green; border-radius: 40px;")
		self.CalLED.setText('       Calibrated            ')





	def tab4buttonpressed(self):
		webbrowser.open('https://docs.google.com/forms/d/e/1FAIpQLSfV1I_m_XHpySkWWFHY50TkHchBT7OENFbcBqAayC7Oqqf6HQ/formResponse')	


	def HHCbuttonpressed(self):
		webbrowser.open('https://en.wikipedia.org/wiki/Helmholtz_coil')


	def WMMbuttonpressed(self):
		webbrowser.open('https://www.ngdc.noaa.gov/geomag/WMM/')







	#Tab1: HHC control interface
	#graphical display for visual part of the UI
	def tab1UI(self):


		#setpoint label
		self.tab1.sp = QHBoxLayout()
		self.spLabel = QLabel('HHC Axis Setpoints (Microteslas)',self)
		self.spLabel.setFont(QFont('Arial', 20))
		self.tab1.sp.addWidget(self.spLabel)

		self.tab1.null = QHBoxLayout()
		self.NullLabel = QLabel('3HC NULL Offsets (Microteslas)',self)
		self.NullLabel.setFont(QFont('Arial', 20))
		self.tab1.null.addWidget(self.NullLabel)

		self.tab1.mag = QHBoxLayout()
		self.MagLabel = QLabel('3HC Magnetometer Readings (Microteslas)',self)
		self.MagLabel.setFont(QFont('Arial', 20))
		self.tab1.mag.addWidget(self.MagLabel)

		self.tab1.I = QHBoxLayout()
		self.ILabel = QLabel('Current Sensor Readouts (Amps)',self)
		self.ILabel.setFont(QFont('Arial', 20))
		self.tab1.I.addWidget(self.ILabel)


		#X Setpoint
		self.tab1.hlayout = QHBoxLayout()
		self.XSPoint =  QLineEdit(self)
		self.XSPoint.setValidator(QDoubleValidator(-150.0+self.XNOFF, 150.0+self.XNOFF, 3))
		self.tab1.hlayout.addWidget(self.XSPoint)
		self.XS = QLabel('X Setpoint (Current Value: %d)' % self.XSET,self)
		self.tab1.hlayout.addWidget(self.XS)
		self.tab1.hlayout.addStretch(1)


		#X NULL offset
		self.tab1.hlayoutN = QHBoxLayout()
		self.XNULLLabel = QLabel('X:',self)
		self.tab1.hlayoutN.addWidget(self.XNULLLabel)
		self.XNULLreadout = QLabel('%d'% 0,self)
		self.tab1.hlayoutN.addWidget(self.XNULLreadout)
		self.tab1.hlayoutN.addStretch(2)





		#X Magnetic field
		self.tab1.hlayout1b = QHBoxLayout()
		self.XMAGLabel = QLabel('X:',self)
		self.tab1.hlayout1b.addWidget(self.XMAGLabel)
		self.XMAGreadout = QLabel('%d'% 0,self)
		self.tab1.hlayout1b.addWidget(self.XMAGreadout)
		self.tab1.hlayout1b.addStretch(2)

		#X-axis current sensor
		self.tab1.hlayout1c = QHBoxLayout()
		self.XILabel = QLabel('X:',self)
		self.tab1.hlayout1c.addWidget(self.XILabel)
		self.XIreadout = QLabel('%d'% 0,self)
		self.tab1.hlayout1c.addWidget(self.XIreadout)
		self.tab1.hlayout1c.addStretch(2)

		#Y Setpoint
		self.tab1.hlayout2 = QHBoxLayout()
		self.YSPoint =  QLineEdit(self)
		self.YSPoint.setValidator(QDoubleValidator(-150.0+self.YNOFF, 150.0+self.YNOFF, 3))
		self.tab1.hlayout2.addWidget(self.YSPoint)
		self.YS = QLabel('Y Setpoint (Current Value: %d)' % self.YSET,self)
		self.tab1.hlayout2.addWidget(self.YS)
		self.tab1.hlayout2.addStretch(1)


		#Y NULL offset
		self.tab1.hlayoutN2 = QHBoxLayout()
		self.YNULLLabel = QLabel('Y:',self)
		self.tab1.hlayoutN2.addWidget(self.YNULLLabel)
		self.YNULLreadout = QLabel('%d'% 0,self)
		self.tab1.hlayoutN2.addWidget(self.YNULLreadout)
		self.tab1.hlayoutN2.addStretch(2)

		#Y Magnetic field
		self.tab1.hlayout2b = QHBoxLayout()
		self.YMAGLabel = QLabel('Y:',self)
		self.tab1.hlayout2b.addWidget(self.YMAGLabel)
		self.YMAGreadout = QLabel('%d'% 0,self)
		self.tab1.hlayout2b.addWidget(self.YMAGreadout)
		self.tab1.hlayout2b.addStretch(2)

		#Y-axis current readout
		self.tab1.hlayout2c = QHBoxLayout()
		self.YILabel = QLabel('Y:',self)
		self.tab1.hlayout2c.addWidget(self.YILabel)
		self.YIreadout = QLabel('%d'% 0,self)
		self.tab1.hlayout2c.addWidget(self.YIreadout)
		self.tab1.hlayout2c.addStretch(2)		


		#Z Setpoint
		self.tab1.hlayout3 = QHBoxLayout()
		self.ZSPoint =  QLineEdit(self)
		self.ZSPoint.setValidator(QDoubleValidator(-150.0+self.ZNOFF, 150.0+self.ZNOFF, 3))
		self.tab1.hlayout3.addWidget(self.ZSPoint)
		self.ZS = QLabel('Z Setpoint (Current Value: %d)' % self.ZSET,self)
		self.tab1.hlayout3.addWidget(self.ZS)
		self.tab1.hlayout3.addStretch(1)


		#Z NULL offsets
		self.tab1.hlayoutN3 = QHBoxLayout()
		self.ZNULLLabel = QLabel('Z:',self)
		self.tab1.hlayoutN3.addWidget(self.ZNULLLabel)
		self.ZNULLreadout = QLabel('%d'% 0,self)
		self.tab1.hlayoutN3.addWidget(self.ZNULLreadout)
		self.tab1.hlayoutN3.addStretch(2)


		#Z Magnetic field
		self.tab1.hlayout3b = QHBoxLayout()
		self.ZMAGLabel = QLabel('Z:',self)
		self.tab1.hlayout3b.addWidget(self.ZMAGLabel)
		self.ZMAGreadout = QLabel('%d'% 0,self)
		self.tab1.hlayout3b.addWidget(self.ZMAGreadout)
		self.tab1.hlayout3b.addStretch(2)

		#Z-axis Current readout
		self.tab1.hlayout3c = QHBoxLayout()
		self.ZILabel = QLabel('Z:',self)
		self.tab1.hlayout3c.addWidget(self.ZILabel)
		self.ZIreadout = QLabel('%d'% 0,self)
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
		self.creditlabel = QLabel('Created by Nick Jannuzzi for usage with the SlugSat 3-Axis HHC (April 2020)',self)
		self.creditlabel.setFont(QFont('Arial', 12))
		self.tab1.credits.addWidget(self.creditlabel)
		self.tab1.credits.addStretch(2)
		self.tab1.credits2 = QHBoxLayout()
		self.creditlabel2 = QLabel('Designed using PyQt5, a Python wrapper of the C++ Qt designer suite',self)
		self.creditlabel2.setFont(QFont('Arial', 12))
		self.tab1.credits2.addWidget(self.creditlabel2)
		self.tab1.credits2.addStretch(2)


		#3HC mode button
		self.tab1.mode = QHBoxLayout()
		self.mbut = QPushButton('3HC mode',self)
		self.calbut = QPushButton('Calibrate',self)

		self.LED = QLabel()
		
		if(self.mode == 1):
			#IDLE->ARMED
			self.LED.resize(80, 80)
			self.LED.setStyleSheet("border: 1px solid black; background-color: green; border-radius: 40px;")
			self.LED.setText('       Armed         ')
			self.stopUSB = 0
		else:
			#ARMED->IDLE
			self.LED.resize(80, 80)
			self.LED.setStyleSheet("border: 1px solid black; background-color: red; border-radius: 40px;")
			self.LED.setText('        Idle         ')
			self.stopUSB = 1



		self.CalLED = QLabel()	
		self.CalLED.resize(80, 80)
		self.CalLED.setStyleSheet("border: 1px solid black; background-color: red; border-radius: 40px;")
		self.CalLED.setText('       Not Calibrated        ')
			

	

		self.mbut.clicked.connect(self.modechange)
		self.calbut.clicked.connect(self.calibrate3HC)

		self.tab1.mode.addWidget(self.mbut)
		self.tab1.mode.addWidget(self.LED)
		self.tab1.mode.addWidget(self.calbut)
		self.tab1.mode.addWidget(self.CalLED)


		self.tab1.mode.addStretch(2)


		#vertically arranges hboxes
		self.tab1.vlayout = QVBoxLayout()

		#setpoints
		self.tab1.vlayout.addLayout(self.tab1.sp)
		self.tab1.vlayout.addLayout(self.tab1.hlayout)
		self.tab1.vlayout.addLayout(self.tab1.hlayout2)
		self.tab1.vlayout.addLayout(self.tab1.hlayout3)
		self.tab1.vlayout.addLayout(self.tab1.hlayout4)
		self.tab1.vlayout.addStretch(1)


		#null offsets
		self.tab1.vlayout.addLayout(self.tab1.null)
		self.tab1.vlayout.addLayout(self.tab1.hlayoutN)
		self.tab1.vlayout.addLayout(self.tab1.hlayoutN2)
		self.tab1.vlayout.addLayout(self.tab1.hlayoutN3)
		# self.tab1.vlayout.addLayout(self.tab1.hlayout4)
		self.tab1.vlayout.addStretch(1)

		#magnetic field
		self.tab1.vlayout.addLayout(self.tab1.mag)
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

		#mode button
		self.tab1.vlayout.addLayout(self.tab1.mode)
		self.tab1.vlayout.addStretch(1)

		#credits
		self.tab1.vlayout.addLayout(self.tab1.credits)
		self.tab1.vlayout.addLayout(self.tab1.credits2)
		self.tab1.vlayout.addStretch(1)
		self.tab1.setLayout(self.tab1.vlayout)


	#tab3: SVS control interface (pH)
	#this will need to be added once the solar vector simulator is complete and ready to be integrated with the 3HC
	#We had also discussed keeping them as separate systems, which would allow us to remove this part
	def tab2UI(self):
		pass


	#tab4: SOP/User Manual and Useful Links
	def tab3UI(self):
		self.tab3.vlayout = QVBoxLayout()
		self.SOPLabel = QLabel('Click the following link to view the SOP and User manual',self)
		self.SOPLabel.setAlignment(Qt.AlignCenter)
		self.button = QPushButton('HHC SOP and User Manual',self)

		self.HHCLabel = QLabel('Basic Info on Helmholtz Coils',self)
		self.HHCLabel.setAlignment(Qt.AlignCenter)
		self.HHCbutton = QPushButton('Helmholtz Coil info',self)

		self.WMMLabel = QLabel('World Magnetic Model Website',self)
		self.WMMLabel.setAlignment(Qt.AlignCenter)
		self.WMMbutton = QPushButton('World Magnetic Model (WMM)',self)
		self.button.clicked.connect(self.tab4buttonpressed)
		self.HHCbutton.clicked.connect(self.HHCbuttonpressed)
		self.WMMbutton.clicked.connect(self.WMMbuttonpressed)
		# self.USBbutton.clicked.connect(self.USBbuttonpressed)

		self.tab3.vlayout.addWidget(self.SOPLabel)
		self.tab3.vlayout.addWidget(self.button)
		self.tab3.vlayout.addWidget(self.HHCLabel)
		self.tab3.vlayout.addWidget(self.HHCbutton)
		self.tab3.vlayout.addWidget(self.WMMLabel)
		self.tab3.vlayout.addWidget(self.WMMbutton)
		# self.tab4.vlayout.addWidget(self.Label)
		# self.tab4.vlayout.addWidget(self.USBbutton)
		self.tab3.vlayout.addStretch(2)
		self.tab3.setLayout(self.tab3.vlayout)


#Actually runs the GUI (Main loop), don't put anything else in here
#Don't put anything else inside this while loop 
if __name__=='__main__':
	app = QApplication(sys.argv)
	mainWindow = App()
	mainWindow.show()
	sys.exit(app.exec_())
