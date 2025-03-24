#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

struct Problema {
    int capacidade;
    vector<int> pesos;
    vector<int> valores;
};

// Função para ler o problema do arquivo
Problema LeProblema(const string& nome_arquivo, int num_itens) {
    ifstream file(nome_arquivo);
    Problema p;
    srand(time(nullptr)); // Inicializa o gerador de números aleatórios

    // Lê a capacidade da mochila
    file >> p.capacidade;

    // Lê os itens
    int peso, valor;
    for (int i = 0; i < num_itens;) {
        file >> peso >> valor;

        // Aceita apenas 70% dos itens aleatoriamente
        if (rand() % 100 >= 70) {
            p.pesos.push_back(peso);
            p.valores.push_back(valor);
            i++;
        }
    }

    return p;
}

// Retorna o valor máximo que pode ser colocado na mochila de capacidade W
int DPknapsack(int W, vector<int>& val, vector<int>& wt) {
    int n = wt.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1));

    // Construção da tabela dp[][] (Programação Dinâmica)
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= W; j++) {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else {
                int pick = 0;
                if (wt[i - 1] <= j)
                    pick = val[i - 1] + dp[i - 1][j - wt[i - 1]];

                int notPick = dp[i - 1][j];

                dp[i][j] = max(pick, notPick);
            }
        }
    }
    return dp[n][W];
}

// Criação do diretório de resultados, se necessário
void criarDiretorioResultados() {
    system("mkdir -p resultados");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <numero_de_itens>" << endl;
        return 1;
    }

    int num_itens = stoi(argv[1]);
    string nome_arquivo = "../arquivos_texto/mochila.txt";

    // Ler o problema
    Problema p = LeProblema(nome_arquivo, num_itens);

    // Medir tempo de execução
    auto start = high_resolution_clock::now();
    
    // Executar o algoritmo de Programação Dinâmica
    int melhor_valor = DPknapsack(p.capacidade, p.valores, p.pesos);
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    // Exibir resultados
    cout << "Maximum possible profit = " << melhor_valor << endl;
    cout << "Tempo de execucao: " << duration.count() << " ns" << endl;

    // Criar diretório de resultados
    criarDiretorioResultados();

    // Salvar os resultados em CSV
    //string nome_csv = "./resultados/" + to_string(num_itens) + ".csv";
    string nome_csv = "./resultados/dpfw.csv";
    ofstream csv_file(nome_csv, ios::app);
    if (!csv_file) {
        cerr << "Erro ao abrir o arquivo CSV!" << endl;
        return 1;
    }

    csv_file << num_itens << ","
             << p.capacidade << ","
             << melhor_valor << ","
             << duration.count() << endl;
    csv_file.close();

    cout << "Dados salvos em " << nome_csv << endl;

    return 0;
}
