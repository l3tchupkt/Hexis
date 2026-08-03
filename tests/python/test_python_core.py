import os
import pytest
from hexis import Programmer, HexisError
from hexis.cli import app
from typer.testing import CliRunner

runner = CliRunner()

def test_programmer_initialization():
    prog = Programmer("ch341a")
    assert prog is not None

def test_programmer_connect_error():
    try:
        prog = Programmer("missing_driver")
    except HexisError:
        pass

from unittest.mock import patch

@patch("hexis.cli.Programmer")
def test_cli_detect_command(mock_prog):
    mock_instance = mock_prog.return_value
    mock_instance.detect.return_value = {
        "manufacturer": "Winbond",
        "model": "W25Q64",
        "capacity": 8388608,
        "sector_size": 4096,
        "page_size": 256,
        "jedec_id": 0xEF4017
    }
    result = runner.invoke(app, ["detect"])
    assert result.exit_code == 0
    assert "Winbond" in result.stdout

@patch("hexis.cli.Programmer")
def test_cli_read_command(mock_prog, tmp_path):
    mock_instance = mock_prog.return_value
    mock_instance.detect.return_value = {"capacity": 8388608}
    out_file = tmp_path / "dump.bin"
    result = runner.invoke(app, ["read", str(out_file)])
    assert result.exit_code == 0

@patch("hexis.cli.Programmer")
def test_cli_write_command(mock_prog, tmp_path):
    mock_instance = mock_prog.return_value
    mock_instance.detect.return_value = {"capacity": 8388608}
    in_file = tmp_path / "fw.bin"
    in_file.write_bytes(b"A" * 1024)
    result = runner.invoke(app, ["write", str(in_file)])
    assert result.exit_code == 0
