import os
from .bindings import lib, ffi

class HexisError(Exception):
    pass

class Programmer:
    """
    Python wrapper for the Hexis Programmer API.
    """
    def __init__(self, driver_name: str = "ch341a"):
        self._driver = lib.hexis_driver_get(driver_name.encode('utf-8'))
        if not self._driver:
            raise HexisError(f"Driver '{driver_name}' not found or not registered.")
            
        self._ctx_ptr = ffi.new("hexis_device_context_t **")
        self._connected = False
        self.chip_info = None

    def connect(self):
        if self._connected:
            return
        res = self._driver.connect(self._ctx_ptr)
        if res != 0:
            raise HexisError(f"Failed to connect to hardware programmer (Error {res})")
        self._connected = True

    def disconnect(self):
        if self._connected:
            self._driver.disconnect(self._ctx_ptr[0])
            self._connected = False

    def detect(self):
        if not self._connected:
            raise HexisError("Programmer not connected.")
        chip = ffi.new("hexis_chip_info_t *")
        res = self._driver.probe(self._ctx_ptr[0], chip)
        if res != 0:
            raise HexisError("Failed to probe chip.")
            
        self.chip_info = {
            "manufacturer": ffi.string(chip.manufacturer).decode('utf-8'),
            "model": ffi.string(chip.model).decode('utf-8'),
            "capacity": int(chip.capacity_bytes),
            "sector_size": int(chip.sector_size),
            "page_size": int(chip.page_size),
            "jedec_id": int(chip.jedec_id)
        }
        return self.chip_info

    def read(self, filename: str):
        if not self._connected or not self.chip_info:
            raise HexisError("Programmer not connected or chip not detected.")
            
        capacity = self.chip_info["capacity"]
        buffer = ffi.new(f"uint8_t[{capacity}]")
        
        print(f"Reading {capacity} bytes from flash...")
        res = self._driver.read(self._ctx_ptr[0], 0, buffer, capacity)
        if res != 0:
            raise HexisError("Failed to read flash memory.")
            
        with open(filename, "wb") as f:
            f.write(bytes(buffer))
        print(f"Read successful. Saved to {filename}")

    def write(self, filename: str):
        if not self._connected or not self.chip_info:
            raise HexisError("Programmer not connected or chip not detected.")
            
        capacity = self.chip_info["capacity"]
        if not os.path.exists(filename):
            raise FileNotFoundError(f"{filename} does not exist.")
            
        with open(filename, "rb") as f:
            data = f.read()
            
        if len(data) > capacity:
            raise HexisError("File is larger than flash capacity.")
            
        buffer = ffi.new(f"uint8_t[{len(data)}]", data)
        print(f"Writing {len(data)} bytes to flash...")
        res = self._driver.write(self._ctx_ptr[0], 0, buffer, len(data))
        if res != 0:
            raise HexisError("Failed to write flash memory.")
        print("Write successful.")
        
    def erase(self):
        if not self._connected or not self.chip_info:
            raise HexisError("Programmer not connected or chip not detected.")
        capacity = self.chip_info["capacity"]
        print(f"Erasing {capacity} bytes of flash...")
        res = self._driver.erase(self._ctx_ptr[0], 0, capacity)
        if res != 0:
            raise HexisError("Failed to erase flash memory.")
            
    def verify(self, filename: str) -> bool:
        if not self._connected or not self.chip_info:
            raise HexisError("Programmer not connected or chip not detected.")
        if not os.path.exists(filename):
            raise FileNotFoundError(f"{filename} does not exist.")
            
        capacity = self.chip_info["capacity"]
        with open(filename, "rb") as f:
            data = f.read()
        if len(data) > capacity:
            raise HexisError("File is larger than flash capacity.")
            
        buffer = ffi.new(f"uint8_t[{len(data)}]", data)
        print(f"Verifying {len(data)} bytes of flash...")
        res = self._driver.verify(self._ctx_ptr[0], 0, buffer, len(data))
        if res == 0:
            return True
        return False
        
    def __del__(self):
        self.disconnect()
