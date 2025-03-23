import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import scipy.stats as stats

# Carregar o arquivo CSV
df = pd.read_csv("dados.csv", header=None)

# Inicializar listas para armazenar dados
tamanhos = []
medias = []
intervalos_confianca = []

# Iterar sobre cada linha do DataFrame
for index, row in df.iterrows():
    tamanho = row[0]  # Tamanho da instância (primeira coluna)
    tempos = row[1:].dropna()  # Tempos de execução (ignorando NaNs)
    
    # Calcular a média e o intervalo de confiança (95% de confiança)
    media = np.mean(tempos)
    intervalo_confianca = stats.t.interval(0.95, len(tempos)-1, loc=media, scale=stats.sem(tempos))
    
    # Armazenar os resultados
    tamanhos.append(tamanho)
    medias.append(media)
    intervalos_confianca.append(intervalo_confianca)

# Converter para arrays numpy
tamanhos = np.array(tamanhos)
medias = np.array(medias)
intervalos_confianca = np.array(intervalos_confianca)

# Calcular os erros para o gráfico (diferença entre a média e o limite inferior do intervalo)
erros = medias - intervalos_confianca[:, 0]

# Plotar o gráfico
plt.figure(figsize=(8, 6))
plt.errorbar(tamanhos, medias, yerr=erros, fmt='o-', capsize=5, label='Tempo de Execução')
plt.xlabel('Tamanho da Instância')
plt.ylabel('Tempo de Execução (ns)')
plt.title('Tempo de Execução vs Tamanho da Instância com Intervalo de Confiança')
plt.legend()
plt.grid(True)
plt.show()
