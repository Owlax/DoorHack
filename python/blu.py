import asyncio
from bleak import BleakScanner
from bleak import BleakClient

SERVICE_UUID = "c5816eb9-773f-4478-aa3b-c466da9ec23d"
CHARACTEISTIC_UUID = "a290ac53-b59f-4254-b910-c5044e1942c8"
data_to_write = "AHHHH"

async def main():
    devices = await BleakScanner.discover(service_uuids=[SERVICE_UUID])
    for d in devices:
        print(d)
        print("Attempting to connect")
        async with BleakClient(d.address) as client:
            print("Connected!")
            # Connect to the BLE device
            try:
                # Get the specified characteristic
                services = client.services
                characteristic =services.get_characteristic(CHARACTEISTIC_UUID)

                # Convert the data to bytes before writing
                data_bytes = data_to_write.encode('utf-8')  # Assuming the data is a string

                # Write the data to the characteristic
                await client.write_gatt_char(characteristic.uuid, data_bytes)

                print(f"Data '{data_to_write}' written to characteristic {characteristic.uuid}")

            finally:
                # Disconnect from the BLE device even if an exception occurs
                await client.disconnect()
                print("Disconnecting")

asyncio.run(main())