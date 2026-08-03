import json
import os
from datetime import datetime

class ReportGenerator:
    def __init__(self, filename: str):
        self.filename = filename
        self.metadata = {
            "timestamp": datetime.utcnow().isoformat(),
            "tool": "Hexis Framework",
            "version": "1.0.0"
        }
        self.chip_info = {}
        self.analysis = {}
        self.secrets = []

    def set_chip_info(self, chip):
        self.chip_info = chip
        
    def set_analysis(self, entropy: float, arch: str, endian: str, signatures: list):
        self.analysis = {
            "entropy": entropy,
            "architecture": arch,
            "endianness": endian,
            "signatures": signatures
        }
        
    def add_secret(self, secret_type: str, offset: int, description: str, matched_text: str):
        self.secrets.append({
            "type": secret_type,
            "offset": offset,
            "description": description,
            "matched_text": matched_text
        })
        
    def generate_json(self):
        out_file = f"{self.filename}.json"
        with open(out_file, "w") as f:
            json.dump({
                "metadata": self.metadata,
                "chip": self.chip_info,
                "firmware_analysis": self.analysis,
                "security_scan": self.secrets
            }, f, indent=4)
        return out_file

    def generate_markdown(self):
        out_file = f"{self.filename}.md"
        with open(out_file, "w") as f:
            f.write(f"# Hexis Security Report\n\n")
            f.write(f"**Generated:** {self.metadata['timestamp']}\n\n")
            
            f.write("## 1. Hardware Information\n")
            for k, v in self.chip_info.items():
                f.write(f"- **{k.capitalize()}:** {v}\n")
            f.write("\n")
            
            f.write("## 2. Firmware Analysis\n")
            f.write(f"- **Architecture:** {self.analysis.get('architecture', 'Unknown')}\n")
            f.write(f"- **Endianness:** {self.analysis.get('endianness', 'Unknown')}\n")
            f.write(f"- **Entropy:** {self.analysis.get('entropy', 0.0):.4f}\n\n")
            
            f.write("### Extracted Filesystems / Signatures\n")
            for sig in self.analysis.get("signatures", []):
                f.write(f"- `0x{sig['offset']:X}`: {sig['description']} ({sig['type']})\n")
            f.write("\n")
            
            f.write("## 3. Secret & Security Scan\n")
            if not self.secrets:
                f.write("*No vulnerabilities or secrets found.*\n")
            else:
                for sec in self.secrets:
                    f.write(f"- **[0x{sec['offset']:X}] {sec['description']}**\n")
                    f.write(f"  ```text\n  {sec['matched_text']}\n  ```\n")
                    
        return out_file
