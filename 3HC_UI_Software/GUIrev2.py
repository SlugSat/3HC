#created by Nick Jannuzzi 2/21/20
#GUI layout for 3 axis HHC user interface
#created using PyQt5 to communicate with PSoC 5LP microcontroller


from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from USB import * 
import pyqtgraph as pg


#($string[0] == '-') ? '-'.ltrim(substr($string, 1), '0') : ltrim($string, '0')


class App(QMainWindow):
	def __init__(self):
		super().__init__()

		#test variables
		


		#dummy arrays
		self.arr1 = []
		self.arr2 = []


		#current sensor readings
		self.XI = 0
		self.YI = 0
		self.ZI = 0
		#self.initUI()
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
		# self.tabs.resize(400,400)
		self.x = 0
		self.y = 0
		self.z = 0


		#setpoint values
		self.XSP = 0.0
		self.YSP = 0.0
		self.ZSP = 0.0

		#flags for invalid setpoints
		self.xflag = False
		self.yflag = False
		self.zflag = False

		#null offsets
		self.XNOFF = 0
		self.YNOFF = 0
		self.ZNOFF = 0
		self.XI = 0
		self.YI = 0
		self.ZI = 0
		self.tab1UI()
		self.tab2UI()
		self.tab3UI()

		#update null offset timer
		self.NullTimer = QTimer()
		self.NullTimer.setInterval(100)
		self.NullTimer.timeout.connect(self.updatenulloffsets)
		self.NullTimer.start()


		#update current sensors' readings timer
		self.ITimer = QTimer()
		self.ITimer.setInterval(2000)
		self.ITimer.timeout.connect(self.updateIread)
		self.ITimer.start()		

		self.tabs.addTab(self.tab1,"HHC")
		self.tabs.addTab(self.tab2,"Solar Vectors")
		self.tabs.addTab(self.tab3,"SOP")
        # Add tabs to widget
		self.layout.addWidget(self.tabs)
		self.setLayout(self.layout)


	def SetpointsEntered(self):
		#handles 0 case due to unexpected functionality enocuntered
		if(self.XSPoint.text() == ""):
			self.XSP = 0.0
		else:
		 	self.XSP = float(self.XSPoint.text())
		if(self.YSPoint.text() == ""): 
			self.YSP = 0.0
		else:
			self.YSP = float(self.YSPoint.text())
		if(self.ZSPoint.text() == ""):
			self.ZSP =0.0
		else:
			 self.ZSP = float(self.ZSPoint.text())
		self.UpdateSetpoints()	

#updates current values dispaly for setpoints	
	def UpdateSetpoints(self):
		#makes sure we have a valid input
		#X
		if(self.XSP and self.XSPoint.hasAcceptableInput()):
			self.XS.setText('X Setpoint (Current Value: %s)' % self.XSP)
		else:
			if(self.XSP == 0):
				self.XS.setText('X Setpoint (Current Value: %s)' % 0)
			else:	
				self.xflag = True
		#Y
		if(self.YSP and self.YSPoint.hasAcceptableInput()):	
			self.YS.setText('Y Setpoint (Current Value: %s)' % self.YSP)
		else:	
			if(self.YSP == 0):
				self.YS.setText('X Setpoint (Current Value: %s)' % 0)
			else:	
				self.yflag = True
		#Z
		if(self.ZSP and self.ZSPoint.hasAcceptableInput()):
			self.ZS.setText('Z Setpoint (Current Value: %s)' % self.ZSP)
		else:	
			if(self.ZSP == 0):
				self.ZS.setText('Z Setpoint (Current Value: %s)' % 0)
			else:	
				self.zflag = True
		# pg.plot([self.XSP],[self.YSP])		
			#invalid input popup
		if(self.xflag or self.yflag or self.zflag):
			SPInvalid = QMessageBox()
			SPInvalid.setText('Invalid input(s) for one or more axis setpoints; valid range is -200 to 200 with 3 decimal place maximum precision for each setpoint')
			self.xflag = False
			self.yflag = False
			self.zflag = False
			SPInvalid.exec_()	


	#updates the null offset values
	def updatenulloffsets(self):
		#a = self.GUIUSB.read()
		self.x += 1
		self.y += 1
		self.z += 1
		self.XNULLreadout.setText('%s' % self.x)
		self.YNULLreadout.setText('%s' % self.y)
		self.ZNULLreadout.setText('%s' % self.z)	


	def updateIread(self):
		self.XI += 1
		self.YI += 1
		self.ZI += 1
		self.XIreadout.setText('%s Amps' % self.XI)
		self.YIreadout.setText('%s Amps' % self.YI)
		self.ZIreadout.setText('%s Amps' % self.ZI)	


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
		self.XS = QLabel('X Setpoint (Current Value: %d)' % self.XSP,self)
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
		self.YS = QLabel('Y Setpoint (Current Value: %d)' % self.YSP,self)
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
		self.ZS = QLabel('Z Setpoint (Current Value: %d)' % self.ZSP,self)
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

		#credits
		self.tab1.vlayout.addLayout(self.tab1.credits)
		self.tab1.vlayout.addLayout(self.tab1.credits2)
		self.tab1.vlayout.addStretch(1)





		

		self.tab1.setLayout(self.tab1.vlayout)



	def tab2UI(self):
		pass
	


	



	def tab3UI(self):
		pass





#Actually runs the GUI (Main loop)
if __name__=='__main__':
	app = QApplication(sys.argv)
	mainWindow = App()
	mainWindow.show()
	sys.exit(app.exec_())
