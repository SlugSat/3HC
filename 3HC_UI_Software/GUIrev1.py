#created by Nick Jannuzzi 2/21/20
#testing PyQt Widget builder library


from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from USB import * 
#from USB.py import *






#Window Class
class MainWindow(QMainWindow):
	def __init__(self):
		super().__init__()
		self.initUI()
		self.x = 0
		self.y = 0
		self.z = 0
		#connect USB and verify connection
		#U = self.connectUSB()




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
		self.XSPoint.move(20, 60)
		self.XSPoint.resize(280,40)

		#sets X setpoint value
		self.XS = QLabel('X Setpoint',self)
		self.XS.move(300, 60)
		self.XS.resize(160,40)
		

		#enter Y setpoint value
		self.YSPoint =  QLineEdit(self)
		self.YSPoint.move(20, 140)
		self.YSPoint.resize(280,40)


		#set Y setpoint value
		self.YS = QLabel('Y Setpoint',self)
		self.YS.move(300, 140)
		self.YS.resize(160,40)

		#enter Z setpoint value
		self.ZSPoint =  QLineEdit(self)
		self.ZSPoint.move(20, 220)
		self.ZSPoint.resize(280,40)


		#sets Z setpoint
		self.ZS = QLabel('Z Setpoint',self)
		self.ZS.move(300, 220)
		self.ZS.resize(160,40)


		#sets X,Y,Z setpoints in one go
		self.XYZSPButton = QPushButton('Set X, Y, and Z Setpoints',self)
		self.XYZSPButton.move(40, 260)
		self.XYZSPButton.resize(240,40)
		self.XYZSPButton.clicked.connect(self.SetpointsEntered)


		#Calibration labels
		self.CalLabel = QLabel('Calibration',self)
		self.CalLabel.move(20,360)
		self.CalLabel.setFont(QFont('Arial', 20))
		self.NULLLabel = QLabel('Null Offsets',self)
		self.NULLLabel.move(20,380)

		self.XNULLLabel = QLabel('X',self)
		self.XNULLLabel.move(20,420)
		self.XNULLreadout = QLabel('%d'% 0,self)
		#self.setCentralWidget(self.XNULLreadout)
		self.XNULLreadout.move(60,420)

		self.YNULLLabel = QLabel('Y',self)
		self.YNULLLabel.move(120,420)
		self.YNULLreadout = QLabel('%d'% 0,self)
		#self.setCentralWidget(self.YNULLreadout)
		self.YNULLreadout.move(160,420)

		self.ZNULLLabel = QLabel('Z',self)
		self.ZNULLLabel.move(220,420)
		self.ZNULLreadout = QLabel('%d'% 0,self)
		#self.setCentralWidget(self.ZNULLreadout)
		self.ZNULLreadout.move(260,420)



		self.FieldVectorLabel = QLabel('Field Vector',self)
		self.FieldVectorLabel.move(600,20)
		self.FieldVectorLabel.resize(240,40)
		self.FieldVectorLabel.setFont(QFont('Arial', 20))


		self.CurrentReadoutLabel = QLabel('Current Readouts', self)
		self.CurrentReadoutLabel.move(600,500)
		self.CurrentReadoutLabel.resize(240,40)
		self.CurrentReadoutLabel.setFont(QFont('Arial', 20))



		self.Helpbutton = QPushButton('WTF is Going On?', self)
		self.Helpbutton.move(20,800)
		self.Helpbutton.resize(240,40)
		self.Helpbutton.clicked.connect(self.NeedHelp)


		#update timer
		self.NullTimer = QTimer()
		self.NullTimer.setInterval(100)
		self.NullTimer.timeout.connect(self.update)
		self.NullTimer.start()

	def SetpointsEntered(self):
		print("Set Setpoints Clicked")


	def NeedHelp(self):
		#print("User Needs Help")
		helpalert = QMessageBox()
		helpalert.setText('Fuck Off')
		helpalert.exec_()

	def connectUSB(self):
		item = USB()
		#USB_Write()
		if(item.verify() == True):
			return item
		else:	
			return False




	def update(self):
		#self.x = usbitem.read()
		self.x += 1
		self.y += 1
		self.z += 1
		#f = U.read()
		'''U = USB()
		#USB_Write()
		U.verify()
		booty = U.read()'''
		self.XNULLreadout.setText('%s' % self.x)
		self.YNULLreadout.setText('%s' % self.y)
		self.ZNULLreadout.setText('%s' % self.z)	
		#print('Tick Tock')



#Actually runs the GUI
if __name__=='__main__':


	
	#This must always come first
	app = QApplication(sys.argv)
	mainWindow = MainWindow()
	mainWindow.show()
	sys.exit(app.exec_())



