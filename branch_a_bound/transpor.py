import pandas as pd
import argparse

def transpor_csv(entrada, saida):
    # Carregar o arquivo CSV
    df = pd.read_csv(entrada, header=None)
    
    # Transpor os dados
    df_transposto = df.T
    
    # Salvar o novo CSV
    df_transposto.to_csv(saida, index=False, header=False)
    
    print(f"Arquivo transposto salvo em: {saida}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Transpor um arquivo CSV.")
    parser.add_argument("entrada", help="Caminho do arquivo CSV de entrada.")
    parser.add_argument("saida", help="Caminho do arquivo CSV de saída.")
    
    args = parser.parse_args()
    transpor_csv(args.entrada, args.saida)

