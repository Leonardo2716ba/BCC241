#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>   // Para time()
#include <chrono>

using namespace std;
using namespace std::chrono;


// Variável global para contar as podas
int num_pruned = 0;

// Função para ler os dados do arquivo
void read_knapsack_data(const string& file_path, int& max_weight, vector<int>& weights, vector<int>& values) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        exit(1);
    }

    // Primeira linha é o peso máximo da mochila
    file >> max_weight;

    // Lê os pesos e valores dos itens
    int weight, value;
    while (file >> weight >> value) {
        weights.push_back(weight);
        values.push_back(value);
    }

    file.close();
}

// Função para construir candidatos válidos
vector<int> construct_candidates(const vector<int>& s, int i, int total_weight, int m, const vector<int>& w, int n) {
    vector<int> candidates;

    // Sempre podemos escolher 0 (não incluir o item)
    candidates.push_back(0);

    // Só podemos escolher 1 se não ultrapassar a capacidade
    if (total_weight + w[i] <= m) {
        candidates.push_back(1);
    } else {
        num_pruned += (n - i); // Conta as execuções economizadas pela poda
    }

    return candidates;
}

// Função de backtracking
void knapsack_backtracking(int n, int i, vector<int>& s, int total_weight, int total_value, const vector<int>& w, const vector<int>& v, int m, vector<int>& best_solution, int& best_value) {
    // Se atingimos o nível final, processamos a solução
    if (i == n) {
        if (total_value > best_value) {
            best_value = total_value;
            best_solution = s; // Copia a melhor solução encontrada
        }
        return;
    }

    // Constrói candidatos válidos (0 ou 1) sem ultrapassar a capacidade
    vector<int> candidates = construct_candidates(s, i, total_weight, m, w, n);

    for (int candidate : candidates) {
        s[i] = candidate; // Atribui valor à variável
        int new_weight = total_weight + (candidate * w[i]);
        int new_value = total_value + (candidate * v[i]);

        // Chama a recursão para a próxima variável
        knapsack_backtracking(n, i + 1, s, new_weight, new_value, w, v, m, best_solution, best_value);

        s[i] = -1; // Limpa valor da solução para próxima iteração
    }
}

void write_to_csv(const string& csv_path, int max_weight, long long duration) {
    ofstream file(csv_path, ios::app); // Abre o arquivo em modo de append (adiciona ao final)
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo CSV." << endl;
        exit(1);
    }

    // Escreve max_weight e duration no arquivo CSV, separados por vírgula
    file << max_weight << "," << duration << endl;
    file.close();
}



// Função para rodar o backtracking com os dados lidos
int run_knapsack_backtracking(const string& file_path) {
    int max_weight;
    vector<int> weights, values;

    // Lê os dados do arquivo
    read_knapsack_data(file_path, max_weight, weights, values);

    int n = weights.size(); // Número de itens
    vector<int> s(n, -1); // Solução inicial (todos os itens não incluídos)
    vector<int> best_solution(n, -1); // Melhor solução encontrada
    int best_value = 0; // Melhor valor encontrado

    // Chama a função de backtracking
    knapsack_backtracking(n, 0, s, 0, 0, weights, values, max_weight, best_solution, best_value);

    // Exibe a melhor solução encontrada
    cout << "Melhor solução: ";
    for (int item : best_solution) {
        cout << item << " ";
    }
    cout << endl;

    cout << "Valor total da melhor solução: " << best_value << endl;
    cout << "Número de podas: " << num_pruned << endl;
    return max_weight;
}

int main() {

    string file_path = "/Users/guilhermecarolino/Documents/paa/knapsack_data_30_1_items.txt";
    auto inicio = high_resolution_clock::now();
    int maxweighttt = run_knapsack_backtracking(file_path);
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<nanoseconds>(fim - inicio);
    write_to_csv("xp_30i_wvariado.csv",maxweighttt, duracao.count());
    cout << "Tempo decorrido: " << duracao.count() << " nanossegundos" << endl;
    return 0;
}