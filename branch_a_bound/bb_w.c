#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <cstdlib> // Para rand()
#include <ctime>   // Para time()
#include <filesystem> // Para criar diretórios

using namespace std;
using namespace std::chrono;

// Estrutura para representar um item
struct Item {
    int weight;
    int value;
};

// Estrutura para representar o problema da mochila
struct Problema {
    int capacidade; // Capacidade da mochila
    vector<Item> itens; // Lista de itens
};

// Estrutura para representar uma solução
struct Solucao {
    vector<int> selecionados; // Itens selecionados (1 = selecionado, 0 = não selecionado)
    int valor; // Valor total da solução
    int peso; // Peso total da solução
};

// Função para ler o problema do arquivo
Problema LeProblema(const string& nome_arquivo, int capacidade) {
    ifstream file(nome_arquivo);
    Problema p;
    srand(time(nullptr)); // Inicializa o gerador de números aleatórios

    // Define a capacidade da mochila passada como argumento
    p.capacidade = capacidade;

    // Lê os itens
    int peso, valor;
    while (file >> peso >> valor) {
        // Gera um número aleatório entre 0 e 99, ignorando o item se for menor que 80
        if (rand() % 100 >= 95) {
            p.itens.push_back({peso, valor});
        }
    }

    return p;
}

// Função gulosa para gerar uma solução inicial
Solucao Gulosa(Problema& p) {
    Solucao s;
    s.valor = 0;
    s.peso = 0;
    s.selecionados = vector<int>(p.itens.size(), 0);

    // Ordena os itens por valor/peso em ordem decrescente
    sort(p.itens.begin(), p.itens.end(), [](Item a, Item b) {
        return (float)a.value / a.weight > (float)b.value / b.weight;
    });

    // Seleciona os itens enquanto couberem na mochila
    for (size_t i = 0; i < p.itens.size(); ++i) {
        if (s.peso + p.itens[i].weight <= p.capacidade) {
            s.selecionados[i] = 1;
            s.peso += p.itens[i].weight;
            s.valor += p.itens[i].value;
        }
    }

    return s;
}

// Função para verificar se uma solução é completa
bool eCompleta(const Problema& p, const Solucao& s) {
    return s.selecionados.size() == p.itens.size();
}

// Função para verificar se uma solução é consistente
bool eConsistente(const Problema& p, const Solucao& s) {
    return s.peso <= p.capacidade;
}

// Função para verificar se uma solução é promissora
bool ePromissora(const Problema& p, const Solucao& s, const Solucao& melhor) {
    // Calcula o limite superior (bound) usando a heurística gulosa
    int valor_restante = 0;
    int peso_restante = p.capacidade - s.peso;

    for (size_t i = 0; i < p.itens.size(); ++i) {
        if (s.selecionados[i] == 0 && p.itens[i].weight <= peso_restante) {
            valor_restante += p.itens[i].value;
            peso_restante -= p.itens[i].weight;
        }
    }

    return (s.valor + valor_restante) > melhor.valor;
}

// Função de Branch and Bound
void BBOtimizacao(Solucao& s, int i, Problema& p, Solucao& melhor) {
    if (eCompleta(p, s)) {
        if (s.valor > melhor.valor) {
            melhor = s;
        }
    } else {
        // Tenta incluir o item i
        s.selecionados[i] = 1;
        s.peso += p.itens[i].weight;
        s.valor += p.itens[i].value;

        if (eConsistente(p, s) && ePromissora(p, s, melhor)) {
            BBOtimizacao(s, i + 1, p, melhor);
        }

        // Desfaz a inclusão do item i
        s.selecionados[i] = 0;
        s.peso -= p.itens[i].weight;
        s.valor -= p.itens[i].value;

        // Tenta excluir o item i
        if (ePromissora(p, s, melhor)) {
            BBOtimizacao(s, i + 1, p, melhor);
        }
    }
}

// Função para criar o diretório de resultados, se não existir
void criarDiretorioResultados() {
    if (!filesystem::exists("./resultados")) {
        filesystem::create_directory("./resultados");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <peso_mochila>" << endl;
        return 1;
    }

    // Define o número de itens fixo
    const int num_itens = 400; // Você pode alterar esse valor conforme necessário

    // Lê o peso da mochila passado como argumento
    int peso_mochila = stoi(argv[1]);
    string nome_arquivo = "../arquivos_texto/mochila.txt";

    // Ler o problema
    Problema p = LeProblema(nome_arquivo, peso_mochila);

    // Gerar solução inicial gulosa
    Solucao melhor = Gulosa(p);

    // Inicializar solução inicial
    Solucao inicial;
    inicial.selecionados = vector<int>(p.itens.size(), -1);
    inicial.valor = 0;
    inicial.peso = 0;

    // Medir o tempo de execução com alta precisão
    auto start = high_resolution_clock::now();

    // Executar Branch and Bound
    BBOtimizacao(inicial, 0, p, melhor);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    // Exibir resultados
    cout << "Maximum possible profit = " << melhor.valor << endl;
    cout << "Tempo de execucao: " << duration.count() << " ns" << endl;

    // Criar diretório de resultados, se não existir
    criarDiretorioResultados();

    // Salvar os resultados em um arquivo CSV
    string nome_csv = "./resultados/" + to_string(peso_mochila) + ".csv";
    ofstream csv_file(nome_csv, ios::app);
    if (!csv_file) {
        cerr << "Erro ao abrir o arquivo CSV!" << endl;
        return 1;
    }

    // Escreve os dados no arquivo CSV
    csv_file << num_itens << ","
             << p.capacidade << ","
             << melhor.valor << ","
             << duration.count() << endl;

    csv_file.close();

    cout << "Dados salvos em " << nome_csv << endl;

    return 0;
}