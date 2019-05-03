import time
import nxppy
import paho.mqtt.client as mqtt

mifare = nxppy.Mifare()

mqttsender = mqtt.Client('clientid123')

def on_connect(client, userdata, flags, rc):
      print("Connected with result code "+str(rc))

def on_message(client, userdata, message):
        print("Received message '" + str(message.payload) + "' on topic '" + message.topic + "' with QoS " + str(message.qos))

def on_log(mqttc, obj, level, string):
        print(string)

mqttsender.on_log = on_log
mqttsender.on_message = on_message
mqttsender.on_connect = on_connect
mqttsender.connect("localhost", 1883)
mqttsender.subscribe("/nfc/reader1")

while True:
   try:
      uid = mifare.select()
      print(mqttsender.publish("/nfc/reader1", uid))
      print(uid)
   except nxppy.SelectError:
      pass

   time.sleep(1)