import typer
from rich.console import Console
from rich.table import Table
from .core import Programmer, HexisError

app = typer.Typer(help="Hexis Hardware Hacking Framework CLI", no_args_is_help=True)
console = Console()

@app.command()
def detect(driver: str = typer.Option("ch341a", help="Programmer driver to use")):
    """Detect the connected flash memory chip."""
    try:
        prog = Programmer(driver)
        prog.connect()
        chip = prog.detect()
        
        table = Table(title="Detected Chip Information")
        table.add_column("Property", style="cyan")
        table.add_column("Value", style="magenta")
        
        table.add_row("Manufacturer", chip['manufacturer'])
        table.add_row("Model", chip['model'])
        table.add_row("Capacity", f"{chip['capacity']} bytes")
        table.add_row("Sector Size", f"{chip['sector_size']} bytes")
        table.add_row("Page Size", f"{chip['page_size']} bytes")
        table.add_row("JEDEC ID", f"0x{chip['jedec_id']:X}")
        
        console.print(table)
    except HexisError as e:
        console.print(f"[bold red]Error:[/] {e}")

@app.command()
def read(filename: str, driver: str = typer.Option("ch341a", help="Programmer driver to use")):
    """Read the flash memory and save to a file."""
    try:
        prog = Programmer(driver)
        prog.connect()
        prog.detect()
        prog.read(filename)
        console.print(f"[bold green]Successfully read flash to {filename}[/]")
    except HexisError as e:
        console.print(f"[bold red]Error:[/] {e}")

@app.command()
def write(filename: str, driver: str = typer.Option("ch341a", help="Programmer driver to use")):
    """Write a file to the flash memory."""
    try:
        prog = Programmer(driver)
        prog.connect()
        prog.detect()
        prog.write(filename)
        console.print(f"[bold green]Successfully wrote {filename} to flash[/]")
    except HexisError as e:
        console.print(f"[bold red]Error:[/] {e}")

@app.command()
def erase(driver: str = typer.Option("ch341a", help="Programmer driver to use")):
    """Erase the entire flash memory."""
    try:
        prog = Programmer(driver)
        prog.connect()
        prog.detect()
        prog.erase()
        console.print("[bold green]Successfully erased flash memory.[/]")
    except HexisError as e:
        console.print(f"[bold red]Error:[/] {e}")

@app.command()
def verify(filename: str, driver: str = typer.Option("ch341a", help="Programmer driver to use")):
    """Verify flash memory against a file."""
    try:
        prog = Programmer(driver)
        prog.connect()
        prog.detect()
        if prog.verify(filename):
            console.print(f"[bold green]Verification successful. Flash matches {filename}.[/]")
        else:
            console.print(f"[bold red]Verification failed. Flash does not match {filename}.[/]")
    except HexisError as e:
        console.print(f"[bold red]Error:[/] {e}")

@app.command()
def backup(filename: str, driver: str = typer.Option("ch341a", help="Programmer driver to use")):
    """Backup firmware to a file."""
    # Backup is essentially a read operation for now. In the future it will create a project.
    try:
        prog = Programmer(driver)
        prog.connect()
        prog.detect()
        prog.read(filename)
        console.print(f"[bold green]Successfully backed up flash to {filename}[/]")
    except HexisError as e:
        console.print(f"[bold red]Error:[/] {e}")

@app.command()
def analyze(filename: str):
    """Analyze firmware file for architecture, entropy, and strings."""
    console.print("[yellow]Firmware analysis is planned for Phase 7.[/]")

@app.command()
def extract(filename: str):
    """Extract filesystems from firmware."""
    console.print("[yellow]Firmware extraction is planned for Phase 7.[/]")

@app.command()
def secrets(filename: str):
    """Scan firmware for secrets and keys."""
    console.print("[yellow]Secret scanning is planned for Phase 8.[/]")

if __name__ == "__main__":
    app()
