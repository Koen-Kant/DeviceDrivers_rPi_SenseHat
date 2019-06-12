from sense_hat import SenseHat

sense = SenseHat()

print("test")

for x in range(8):
	for y in range(8):
		sense.set_pixel(x,y,(0,0,0))

for x in range(4):
	for y in range(4):
		print("This is LED: %s:%s" % (x,y))
		# The color formati is Red, Green, Blue
		# Pixel 0/0 is top right, as seen with the I/O facing downwards
		sense.set_pixel(x+2, y+2, (125,  125, 125))

sense.set_rotation(270)
#sense.show_message("You DUMB GIT");

print("Temp: %s:[18.3-26.7 C]\nPress: %s[979-1027 mB]\nHumid: %s[60PC]" % (round(sense.get_temperature()), round(sense.get_pressure()), round(sense.get_humidity())))

# Yaw: topside, Pitchh: Long side, Roll: Shortside
pitch, roll, yaw = sense.get_orientation().values()
print("pitch=%s, roll=%s, yaw=%s" % (pitch,yaw,roll))

# Z: Hieght wise, X: Shortside Y: Long side
x,y,z = sense.get_accelerometer_raw().values()
print("x: %s\ny: %s\nz: %s\n" % (round(x),round(y),round(z)))
