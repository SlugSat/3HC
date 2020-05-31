#created by Nick Jannuzzi 2/21/20
#GUI layout for 3 axis HHC user interface
#created using PyQt5 to communicate with PSoC 5LP microcontroller


from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from USB import * 
import pyqtgraph as pg


#($string[0] == '-') ? '-'.ltrim(substr($string, 1), '0') : ltrim($string, '0')


#Window Class
class MainWindow(QMainWindow):
	def __init__(self):
		super().__init__()

		#test variables
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


		#dummy arrays
		self.arr1 = []
		self.arr2 = []


		#current sensor readings
		self.XI = 0
		self.YI = 0
		self.ZI = 0

		#connect our USB device(PSoC)
		#self.GUIUSB = USB()
		#self.GUIUSB.verify()
		#initialize layout of UI
		self.initUI()
		#pg.plot([self.x],[self.y])		




	






	def enableDarkMode(self):
		app.setStyle('WindowsVista')
		#Sets the color palette of the GUI
		dark_palette = QPalette()
		dark_palette.setColor(QPalette.Window, QColor(53, 53, 53))
		dark_palette.setColor(QPalette.WindowText, Qt.white)
		dark_palette.setColor(QPalette.Base, QColor(25, 25, 25))
		dark_palette.setColor(QPalette.AlternateBase, QColor(53, 53, 53))
		dark_palette.setColor(QPalette.ToolTipBase, Qt.white)
		dark_palette.setColor(QPalette.ToolTipText, Qt.white)
		dark_palette.setColor(QPalette.Text, Qt.white)
		dark_palette.setColor(QPalette.Button, QColor(53, 53, 53))
		dark_palette.setColor(QPalette.ButtonText, Qt.black)
		dark_palette.setColor(QPalette.BrightText, Qt.red)
		dark_palette.setColor(QPalette.Link, QColor(42, 130, 218))
		dark_palette.setColor(QPalette.Highlight, QColor(42, 130, 218))
		dark_palette.setColor(QPalette.HighlightedText, Qt.black)
		qApp.setPalette(dark_palette)
		qApp.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }")


#UI layout
	def initUI(self):

		#GUI style
		self.enableDarkMode()



		self.setWindowTitle('3-Axis HHC Control Terminal')
		self.setGeometry(100,100,4000,1400)	



		#setpoint label
		self.SPointLabel = QLabel('HHC Axis Setpoints',self)
		self.SPointLabel.move(20,20)
		self.SPointLabel.resize(280,40)
		self.SPointLabel.setFont(QFont('Arial', 20))



		#enter X setpoint value
		self.XSPoint =  QLineEdit(self)
		self.XSPoint.setValidator(QDoubleValidator(-200.0, 200.0, 3))
		self.XSPoint.move(20, 60)
		self.XSPoint.resize(280,40)

		#sets X setpoint value
		self.XS = QLabel('X Setpoint (Current Value: %d)' % self.XSP,self)
		self.XS.move(300, 60)
		self.XS.resize(250,40)
		

		#enter Y setpoint value
		self.YSPoint =  QLineEdit(self)
		self.YSPoint.setValidator(QDoubleValidator(-200.0, 200.0, 3))
		self.YSPoint.move(20, 140)
		self.YSPoint.resize(280,40)


		#set Y setpoint value
		self.YS = QLabel('Y Setpoint (Current Value: %d)' % self.YSP,self)
		self.YS.move(300, 140)
		self.YS.resize(250,40)

		#enter Z setpoint value
		self.ZSPoint =  QLineEdit(self)
		self.ZSPoint.setValidator(QDoubleValidator(-200.0, 200.0, 3))
		self.ZSPoint.move(20, 220)
		self.ZSPoint.resize(280,40)


		#sets Z setpoint
		self.ZS = QLabel('Z Setpoint (Current Value: %d)' % self.ZSP,self)
		self.ZS.move(300, 220)
		self.ZS.resize(250,40)



		#sets X,Y,Z setpoints in one go
		self.XYZSPButton = QPushButton('Set X, Y, and Z Setpoints',self)
		self.XYZSPButton.move(40, 260)
		self.XYZSPButton.resize(240,40)
		self.XYZSPButton.clicked.connect(self.SetpointsEntered)


		#Calibration labels (Null Offsets)
		self.CalLabel = QLabel('Calibration',self)
		self.CalLabel.move(20,360)
		self.CalLabel.setFont(QFont('Arial', 20))
		self.NULLLabel = QLabel('Null Offsets',self)
		self.NULLLabel.move(20,380)

		self.XNULLLabel = QLabel('X',self)
		self.XNULLLabel.move(20,420)
		self.XNULLreadout = QLabel('%d'% 0,self)
		self.XNULLreadout.move(60,420)

		self.YNULLLabel = QLabel('Y',self)
		self.YNULLLabel.move(120,420)
		self.YNULLreadout = QLabel('%d'% 0,self)
		self.YNULLreadout.move(160,420)

		self.ZNULLLabel = QLabel('Z',self)
		self.ZNULLLabel.move(220,420)
		self.ZNULLreadout = QLabel('%d'% 0,self)
		self.ZNULLreadout.move(260,420)


		#field vector graph labels
		self.FieldVectorLabel = QLabel('Field Vector',self)
		self.FieldVectorLabel.move(600,20)
		self.FieldVectorLabel.resize(240,40)
		self.FieldVectorLabel.setFont(QFont('Arial', 20))

		#current sensor readout labels
		self.CurrentReadoutLabel = QLabel('Current Readouts', self)
		self.CurrentReadoutLabel.move(600,500)
		self.CurrentReadoutLabel.resize(240,40)
		self.CurrentReadoutLabel.setFont(QFont('Arial', 20))


		self.XILabel = QLabel('X',self)
		self.XILabel.move(600,550)
		self.XIreadout = QLabel('%d'% 0,self)
		self.XIreadout.move(650,550)

		self.YILabel = QLabel('Y',self)
		self.YILabel.move(700,550)
		self.YIreadout = QLabel('%d'% 0,self)
		self.YIreadout.move(750,550)

		self.ZILabel = QLabel('Z',self)
		self.ZILabel.move(800,550)
		self.ZIreadout = QLabel('%d'% 0,self)
		self.ZIreadout.move(850,550)


		#help button
		self.Helpbutton = QPushButton('WTF is Going On?', self)
		self.Helpbutton.move(20,800)
		self.Helpbutton.resize(240,40)
		self.Helpbutton.clicked.connect(self.NeedHelp)


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
	#pulls setpoints from text entries and updates setpoint display
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
		pg.plot([self.XSP],[self.YSP])		
			#invalid input popup
		if(self.xflag or self.yflag or self.zflag):
			SPInvalid = QMessageBox()
			SPInvalid.setText('Invalid input(s) for one or more axis setpoints; valid range is -200 to 200 with 3 decimal place maximum precision for each setpoint')
			self.xflag = False
			self.yflag = False
			self.zflag = False
			SPInvalid.exec_()	
	


	#user pressed help button, bring up HHC manual in new window
	def NeedHelp(self):
		#print("User Needs Help")
		helpalert = QMessageBox()
		helpalert.setText('Help!')
		helpalert.exec_()




	#connects and verifies successful USB connection established
	def connectUSB(self):
		USBitem = USB()
		#USB_Write()
		if(USBitem.verify() == True):
			return USBitem
		else:	
			return False



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
		self.XIreadout.setText('%s' % self.XI)
		self.YIreadout.setText('%s' % self.YI)
		self.ZIreadout.setText('%s' % self.ZI)	


#Actually runs the GUI (Main loop)
if __name__=='__main__':
	app = QApplication(sys.argv)
	mainWindow = MainWindow()
	mainWindow.show()
	sys.exit(app.exec_())



