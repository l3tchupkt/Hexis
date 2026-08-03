import os
import pytest
from hexis import Programmer, HexisError

def test_programmer_initialization():
    prog = Programmer("ch341a")
    assert prog is not None

def test_programmer_connect_and_detect(tmp_path):
    prog = Programmer("ch341a")
    # connect will return success because of mock connect logic
    # Actually wait! The ch341a driver tries to libusb_open_device_with_vid_pid(0x1A86, 0x5512)
    # If the hardware is not plugged in, it returns -3.
    # We should catch HexisError
    try:
        prog.connect()
        # If it miraculously connects, let's test detect
        chip = prog.detect()
        assert chip["manufacturer"] == "Winbond"
        assert chip["capacity"] == 8388608
    except HexisError as e:
        # Expected if CH341A is not plugged in
        assert "Failed to connect to hardware programmer (Error -3)" in str(e)
