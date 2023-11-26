# Installing pybluez was an issue, but this command worked:
# pip install git+https://github.com/pybluez/pybluez.git#egg=pybluez
import bluetooth
import asyncio
from bleak import BleakScanner

async def ble_scan():
    nearby_devices = await BleakScanner.discover()
    print(f"Found {len(nearby_devices)} ble devices:")
    for d in nearby_devices:
        print(f" {d}")

async def non_ble_scan():
    nearby_devices = bluetooth.discover_devices(lookup_names=True)
    print(f"Found {len(nearby_devices)} non-ble devices:")
    for addr, name in nearby_devices:
        print(f" {addr} : {name}")


asyncio.run(ble_scan())
asyncio.run(non_ble_scan())