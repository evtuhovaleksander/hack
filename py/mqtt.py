
GROUP_ID = 1
import json

#
# MQTT client.
#
import ibmiotf.device
import RPIO

PUBLISH_TOPIC=''

def connect(config):
    options = ibmiotf.device.ParseConfigFile(config)
    client = ibmiotf.device.Client(options)
    client.connect()
    client.commandCallback = on_message
    return client



def on_message(cmd):
    if cmd.command != 'button':
        return
    inpstr=cmd.command;
    parsed_string = json.loads(inpstr)
    fullID=parsed_string["lotID"]





def send_data(ident,status):
    #topic = PUBLISH_TOPIC
    #a=ident/4
    #b=ident%4
    #jsonID=a+'_'+b
    if(status==1):
        data='blocked'
    else:
        data='free'
    data='{"lotID": '+ident+',"status": "'+st+'"}'
    client.publishEvent(topic, 'json', data)

client = connect('device.cfg')

def main():
    while True:
        payload = receive_if_available()
        if payload:
            sid, data = payload
            if 0 <= sid <= 1:
                send_data(sid, data)
        else:
            time.sleep(0.1)

if __name__ == '__main__':
    main()