import pandas as pd
import scipy.stats as stats
import numpy as np
import matplotlib.pyplot as plt

def calcular_intervalo_confianca(csv_path, confianca=0.95):
    # Carregar o arquivo CSV
    df = pd.read_csv(csv_path, header=None)
    
    # Criar um dicionário para armazenar os intervalos de confiança
    resultados = {}
    
    # Listas para o gráfico
    tamanhos_instancia = []
    medias = []
    margens_erro = []
    
    # Iterar sobre cada linha do arquivo
    for _, row in df.iterrows():
        tamanho_instancia = row.iloc[0]  # Primeiro item da linha
        tempos = row.iloc[1:].dropna().astype(float)  # Restante da linha
        
        # Calcular média e erro padrão
        media = np.mean(tempos)
        erro_padrao = stats.sem(tempos)
        
        # Calcular intervalo de confiança
        margem_erro = erro_padrao * stats.t.ppf((1 + confianca) / 2, len(tempos) - 1)
        intervalo = (media - margem_erro, media + margem_erro)
        
        # Armazenar no dicionário
        resultados[tamanho_instancia] = intervalo
        
        # Adicionar aos dados do gráfico
        tamanhos_instancia.append(tamanho_instancia)
        medias.append(media)
        margens_erro.append(margem_erro)
    
    # Gerar gráfico de linha
    plt.figure(figsize=(10, 6))
    plt.plot(tamanhos_instancia, medias, marker='o', linestyle='-', label='Média do Tempo')
    plt.fill_between(tamanhos_instancia, 
                     np.array(medias) - np.array(margens_erro), 
                     np.array(medias) + np.array(margens_erro), 
                     color='b', alpha=0.2, label='Intervalo de Confiança')
    
    # Adicionar rótulos aos pontos
    for i, tamanho in enumerate(tamanhos_instancia):
        plt.text(tamanho, medias[i], str(tamanho), fontsize=10, ha='right', va='bottom')
    
    plt.xlabel("Tamanho da Instância")
    plt.ylabel("Tempo de Execução (ns)")
    plt.title("Intervalo de Confiança para o Tempo de Execução")
    plt.legend()
    plt.grid()
    plt.savefig("intervalo_confianca.png")
    plt.show()
    
    return resultados

# Exemplo de uso
csv_path = "dados.csv"  # Substitua pelo caminho real do seu arquivo
intervalos = calcular_intervalo_confianca(csv_path)

# Exibir os resultados
for tamanho, intervalo in intervalos.items():
    print(f"Instância {tamanho}: Intervalo de confiança {intervalo}")
