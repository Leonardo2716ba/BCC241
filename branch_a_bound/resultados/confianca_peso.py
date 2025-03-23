import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import scipy.stats as stats

# Carregar o arquivo CSV
df = pd.read_csv("400.csv", header=None)

# Selecionar a segunda coluna como capacidade e a última como tempo
capacidades = df.iloc[:, 1]
tempos = df.iloc[:, -1]

# Calcular média e desvio padrão
media = np.mean(tempos)
desvio_padrao = np.std(tempos, ddof=1)

# Definir nível de confiança (95%)
confianca = 0.95
n = len(tempos)
erro_padrao = desvio_padrao / np.sqrt(n)

# Calcular intervalo de confiança
intervalo = stats.t.interval(confianca, df=n-1, loc=media, scale=erro_padrao)

print(f"Média dos tempos: {media:.2f} ns")
print(f"Intervalo de confiança ({confianca*100:.0f}%): {intervalo[0]:.2f} ns a {intervalo[1]:.2f} ns")

# Plotar o gráfico
plt.figure(figsize=(8, 6))
plt.hist(tempos, bins=20, alpha=0.7, label="Distribuição dos Tempos")
plt.axvline(intervalo[0], color='red', linestyle='dashed', linewidth=2, label="Limite Inferior")
plt.axvline(intervalo[1], color='green', linestyle='dashed', linewidth=2, label="Limite Superior")
plt.axvline(media, color='blue', linestyle='solid', linewidth=2, label="Média")
plt.xlabel("Tempo (ns)")
plt.ylabel("Frequência")
plt.legend()
plt.title("Intervalo de Confiança para os Tempos")
plt.grid()
plt.show()