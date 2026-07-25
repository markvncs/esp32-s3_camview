import serial
import re
import time
import os
import base64

PORTA = 'COM7'
BAUD_RATE = 115200

FOLDER = 'taken_photos'

def main():

    os.makedirs(FOLDER, exist_ok=True)

    try:
        ser = serial.Serial(PORTA, BAUD_RATE, timeout=1)
        print(f"listening {PORTA}...")
        
        while True:
            linha = ser.readline()
            if not linha:
                continue

            try:
                texto = linha.decode('utf-8', errors='ignore').strip()
                if texto:
                    print(f"[ESP32 Log]: {texto}")
            except Exception:
                pass
            
            match = re.search(r'--IMG_START:(\d+)--', texto)
            if match:
                tamanho_imagem = int(match.group(1))
                print(f"\n[ alert ] downloading image {tamanho_imagem} bytes...")
                
                imagem_bytes = ser.read(tamanho_imagem)
                
                if len(imagem_bytes) == tamanho_imagem:
                    try:
                        jpeg_limpo = base64.b64decode(imagem_bytes)
                        
                        nome_arquivo = f"captura_{int(time.time())}.jpg"
                        caminho_completo = os.path.join(FOLDER, nome_arquivo)
                        
                        with open(caminho_completo, "wb") as f:
                            f.write(jpeg_limpo)
                            
                        print(f"[ success ] imaged saved in: {caminho_completo}\n")
                    except Exception as e:
                        print(f"[ error ] failed to decode image: {e}")
                else:
                    print(f"[ error ] missing bytes. received {len(imagem_bytes)} from {tamanho_imagem}")

    except serial.SerialException as e:
        print(f"error at serial port: {e}")
    except KeyboardInterrupt:
        print("\nleaving...")
        ser.close()

if __name__ == '__main__':
    main()