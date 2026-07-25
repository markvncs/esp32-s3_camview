import serial
import re
import time
import os

PORTA = 'COM7'
BAUD_RATE = 115200

FOLDER = 'taken_photos'

def main():

    os.makedirs(FOLDER, exist_ok=True)

    try:
        ser = serial.Serial(PORTA, BAUD_RATE, timeout=1)
        print(f"Escutando a porta {PORTA}...")
        
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
                print(f"\n[ ALERTA ] Baixando imagem de {tamanho_imagem} bytes...")
                
                imagem_bytes = ser.read(tamanho_imagem)
                
                if len(imagem_bytes) == tamanho_imagem:
                    nome_arquivo = f"captura_{int(time.time())}.jpg"

                    caminho_completo = os.path.join(FOLDER, nome_arquivo)
                    
                    with open(caminho_completo, "wb") as f:
                        f.write(imagem_bytes)
                        
                    print(f"[ SUCESSO ] Imagem salva como: {nome_arquivo}\n")
                else:
                    print(f"[ ERRO ] Faltaram bytes. Recebidos {len(imagem_bytes)} de {tamanho_imagem}")

    except serial.SerialException as e:
        print(f"Erro na porta serial: {e}")
    except KeyboardInterrupt:
        print("\nSaindo...")
        ser.close()

if __name__ == '__main__':
    main()