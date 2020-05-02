import usb.core
import usb.util
import sys
import array
#created by Nick Jannuzzi 3/3/20
		

#implements the USB class for allowing USB transfer (Primary USB connection for usage with HHC and STS)
class USBprimary:
	def __init__(self):
		self.OUT = None
		self.IN = None
		self.dev = None
	#find our device
		self.dev = usb.core.find(idVendor = 0x4b4, idProduct=0x80)
		#check is we found a device
		if self.dev is None:
			print("USB Device error: Device 1 not found: It appears the HHC is not connected or turned on. Please plug the power supply in or turn it on if it is already plugged in. ")
			sys.exit(1)
			
		self.dev.set_configuration()
		cfg = self.dev.get_active_configuration()
		intf = cfg[(0,0)]
		epOut = usb.util.find_descriptor(
			intf, 
			custom_match = \
			lambda e: \
				usb.util.endpoint_direction(e.bEndpointAddress) == \
		        usb.util.ENDPOINT_OUT)	
		self.OUT = epOut	

		epIn = usb.util.find_descriptor(
			intf,
			custom_match= \
			lambda e: \
				usb.util.endpoint_direction(e.bEndpointAddress) == \
				usb.util.ENDPOINT_IN)
		self.IN = epIn	
		#check endpoints, return true if both are found
		

	#verifies that we connected because and __init__ method cannot return something 	
	def verify(self):

		if self.IN is not None and self.OUT is not None:
			return True
		else: 
			return False	


	#read IN endpoint from device and processes it so we 
	def readUSB(self):
		wrapped = self.IN.read(64,1000).tobytes().decode()
		print("Received:" +str(wrapped))
		return wrapped
		


	def writeUSB(self,writing):	
		print("Sent: "+str(writing))
		self.OUT.write(writing,1000) # send it

	#terminate USB	
	def EndUSB(self):
		self.In = None
		self.Out = None

#setup for a secondary USB device, to be used exclusively by the Solar Vector Simulation Module of the HHC
class USBsecondary:
	def __init__(self):
		self.OUT = None
		self.IN = None
		self.dev = None
	#find our device
		self.dev = usb.core.find(idVendor = 0x4b4, idProduct=0x80)
		#check is we found a device
		if self.dev is None:
			print("USB Device error: Device 2 not found: It appears the HHC is not connected or turned on. Please plug the power supply in or turn it on if it is already plugged in. ")
			sys.exit(1)
			
		self.dev.set_configuration()
		cfg = self.dev.get_active_configuration()
		intf = cfg[(0,0)]
		epOut = usb.util.find_descriptor(
			intf, 
			custom_match = \
			lambda e: \
				usb.util.endpoint_direction(e.bEndpointAddress) == \
		        usb.util.ENDPOINT_OUT)	
		self.OUT = epOut	

		epIn = usb.util.find_descriptor(
			intf,
			custom_match= \
			lambda e: \
				usb.util.endpoint_direction(e.bEndpointAddress) == \
				usb.util.ENDPOINT_IN)
		self.IN = epIn	
		#check endpoints, return true if both are found
		

	#verifies that we connected because and __init__ method cannot return something 	
	def verify(self):

		if self.IN is not None and self.OUT is not None:
			return True

	#read IN endpoint from device and processes it so we 
	def readUSB(self):
		wrapped = self.IN.read(64, 10000).tobytes().decode()
		# print(wrapped)
		return wrapped
		


	def writeUSB(self,writing):	
		# print("Sent: "+str(writing))
		self.OUT.write(writing) # send it




	


