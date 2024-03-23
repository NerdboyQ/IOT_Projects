# Installing pybluez was an issue, but this command worked:
# pip install git+https://github.com/pybluez/pybluez.git#egg=pybluez
import bluetooth
import asyncio
import subprocess
from bleak import BleakScanner
from random import randrange

PASSKEY="1234"
BTADDR="00:14:03:06:12:84"
BTPORT=1

bt_devices = {
    "BLE" : [],
    "NonBLE" : []
}

async def ble_scan():
    nearby_devices = await BleakScanner.discover()
    # print(f"Found {len(nearby_devices)} ble devices:")
    for d in nearby_devices:
        if d.name is not None:
            print(f" {d.address} : {d.name}")
            bt_devices["BLE"].append({"name" : d.name, "bt_addr":d.address})

async def non_ble_scan():
    nearby_devices = bluetooth.discover_devices(lookup_names=True)
    # print(f"Found {len(nearby_devices)} non-ble devices:")
    for addr, name in nearby_devices:
        if name is not None:
            print(f" {addr} : {name}")
            bt_devices["NonBLE"].append({"name" : name, "bt_addr":addr})

def pair_nonBLE(addr:str):
    # kill any "bluetooth-agent" process that is already running
    subprocess.call("kill -9 `pidof bluetooth-agent`",shell=True)

    # Start a new "bluetooth-agent" process where XXXX is the passkey
    status = subprocess.call("bluetooth-agent " + PASSKEY + " &",shell=True)

    # Now, connect in the same way as always with PyBlueZ
    try:
        s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        try:
            s.connect((BTADDR,BTPORT))
            msg=[
                "00", #Servo ctrl
                "FF", #led intensity
                "FF", #led red
                "00", #led green
                "00", #led blue
            ] 

            servo_opts = [
                "0x00",
                "0x01"
            ]
            color_opts = [
                str(hex(randrange(0,255))),
                str(hex(randrange(0,255))),
                str(hex(randrange(0,255))),
            ]
            msg = [servo_opts[randrange(0,1)], "c8"]
            msg+= color_opts
            # reverse the list, so it's easier to 
            # read on the arduino side
            msg.reverse()
            for b in msg:
                # BT commands will be input as strings for user frontend simplification
                # and to ensure the bytes are read as bytes successfully on the arduino 
                # side, the strings are converted into 16bit integers and casted as bytes.
                # Sending the string objects causes errors in bt communication on the RX end.
                print(f"{bytes([int(b,16)])} : {str(hex(int(b, 16)))[2:]} : {int(b,16)}")
                s.send(bytes([int(b,16)]))
            
            s.close()
        
        except Exception as e:
            print(f"BT Connection Error: {e}")
    
    except bluetooth.btcommon.BluetoothError as err:
        # Error handler
        pass

async def run_BT_scan():
    await asyncio.gather(non_ble_scan(), ble_scan())
    for k in bt_devices:
        print(k,":",bt_devices[k])

    return bt_devices


#asyncio.run(ble_scan())
#asyncio.run(non_ble_scan())
#pair_nonBLE(BTADDR)