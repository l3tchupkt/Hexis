from hexis.core import Programmer

class ExamplePythonPlugin:
    def __init__(self):
        self.name = "Example Python Plugin"
        self.version = "1.0.0"
        self.author = "Letchu"

    def run(self, prog: Programmer):
        print(f"[{self.name}] Interfacing with programmer: {prog.driver_name}")
        # Custom logic goes here
        chip = prog.detect()
        print(f"[{self.name}] Plugin detected chip: {chip['model']}")

def register_plugin():
    return ExamplePythonPlugin()
