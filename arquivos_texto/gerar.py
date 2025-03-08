import random

def gerar_arquivo():
    # Nome do arquivo
    nome_arquivo = f"mochila.txt"
    
    # Peso da mochila
    peso_mochila = 100
    
    # Abre o arquivo para escrita
    with open(nome_arquivo, 'w') as arquivo:
        # Escreve o peso da mochila na primeira linha
        arquivo.write(f"{peso_mochila}\n")
        
        # Gera 100*n itens
        for _ in range(409600):
            peso = random.randint(1, 30)
            valor = random.randint(1, 100)
            arquivo.write(f"{peso}    {valor}\n")

# Gera arquivos para n variando de 1 a 15
gerar_arquivo()
print("Arquivos gerados com sucesso!")
