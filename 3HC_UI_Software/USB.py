import usb.core
import usb.util
import sys
#created by Nick Jannuzzi 3/3/20
		

#implements the USB class for allowing USB transfer
class USB:
	def __init__(self):
		self.OUT = 0
		self.IN = 0
	#find our device
		dev = usb.core.find(idVendor = 0x4b4, idProduct=0x80)
		#EPEEN = endpoints()
		#found?
		if dev is None:
			raise ValueError('Device not found')



		dev.set_configuration()
		cfg = dev.get_active_configuration()
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



	def read(self):
		OUTE = self.OUT
		rawdog = (OUTE.read(64).tostring())
		#print(rawdog.decode())
		print(type(rawdog))
		wrapped = rawdog.decode()
		print(type(wrapped))
		return wrapped
		#dataraw = (self.read(i).tostring())
		

		'''print(type(dataraw))
		data = dataraw.decode()
		print(data)
		print(type(data))
		return data'''


	def write(writing):	
		endpoints.IN.write(writing) # send it




	


