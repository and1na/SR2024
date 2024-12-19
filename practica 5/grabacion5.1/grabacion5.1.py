
import serial
import time
import re
from halo import Halo
from tqdm import tqdm

"""
    grabacion5.1 - Grupo L2-A5

    Alvaro Garcia Fernandez - UO289295
    Daniel Andina Pailos - UO287791
    Enol Rodriguez Hevia - UO287935
"""

RECORD_MODE = 0
RUN_MODE = 1

class Instruction:
    def __init__(self, action: str, value: int|None = None) -> None:
        self.action = action
        self.value = value

    def __str__(self) -> str:
        if self.value is None: return f"{self.action}"
        return f"{self.action} {self.value}"

    def multiply(self, multiplier: float) -> "Instruction":
        if self.value is not None:
            return Instruction(self.action, int(self.value * multiplier))
        return Instruction(self.action)

class Arduino:
    instructions = {
        "stop": "FINISH",
        "instruction": r"(ADELANTE|ATRAS|PARAR) (\d+)",
    }
    def __init__(self, arduino: serial.Serial) -> None:
        self.arduino = arduino
        # self.__actions = []
        self.__actions = [
            Instruction("ADELANTE",  10),
            Instruction("ADELANTE",  10),
            Instruction("ADELANTE",  10),
            Instruction("FINISH")
        ]

    @Halo(text="Grabando instrucciones",
          spinner={ "interval": 80, "frames": [ "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏" ] })
    def record(self) -> None:
        self.arduino.reset_input_buffer()
        self.arduino.write(f"{RECORD_MODE}\n".encode("ascii"))
        finished = False
        while not finished:
            if self.arduino.in_waiting == 0: continue
            input = self.arduino.readline()
            if input == b'': continue

            input = input.decode("utf-8").strip()
            input = input.split("\r\n")

            for i in input:
                if i == self.instructions["stop"]:
                    finished = True
                    self.__actions.append(Instruction(i))
                    break
                match = re.match(self.instructions["instruction"], i)
                if not match: continue
                if i == self.instructions["stop"]: finished = True
                instruction = match.group(1)
                value = int(match.group(2))
                self.__actions.append(Instruction(instruction, value))

    def play(self, multiplier: float = 1) -> None:
        self.arduino.write(f"{RUN_MODE}\n".encode("ascii"))
        time.sleep(0.2)
        self.arduino.write(f"{len(self.__actions)}\n".encode("ascii"))
        for i in tqdm(list(map(lambda x: x.multiply(multiplier), self.__actions)), desc="Enviando instrucciones al arduino"):
            self.arduino.write(f"{str(i)}\n".encode("ascii"))
            # tqdm.write(f"[+] {str(i)}")
            time.sleep(0.1)

    def clear_actions(self) -> None:
        self.__actions.clear()

def main() -> None:
    com = x if (x:=input("Por favor, ingrese el puerto COM del arduino (por defecto COM4): ")) else "COM4"
    baudrate = x if (x:=input("Por favor, ingrese el baudrate del arduino (por defecto 9600): ")) else 9600

    arduino = Arduino(serial.Serial(port=com, baudrate=int(baudrate), timeout=0.1))
    while True:
        print("Acciones disponibles: ")
        print("\t1. Grabar (g)")
        print("\t2. Reproducir (r)")
        print("\t3. Salir (q)")
        value = None
        while value not in ["g", "r", "q"]:
            value = input("Que accion deseas realizar?: ")

        if value == "q":
            exit(0)

        if value == "g":
            arduino.clear_actions()
            arduino.record()

        if value == "r":
            multiplier = float(x if (x:=input("Por favor, ingrese el multiplicador de tiempo (por defecto 1): ")) else 1)
            arduino.play(multiplier)

if __name__ == "__main__":
    main()