#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip> // Para formatação de saída
#include "branch_a_bound.h" // Inclui o cabeçalho com as declarações

using namespace std;
using namespace std::chrono;

// Função para formatar a duração no formato minutos:segundos:milissegundos
string formatar_tempo(milliseconds ms) {
    // Converte milissegundos para minutos, segundos e milissegundos
    auto minutos = duration_cast<minutes>(ms);
    ms -= minutos;
    auto segundos = duration_cast<seconds>(ms);
    ms -= segundos;
    auto milissegundos = ms;

    // Formata a string no formato mm:ss:ms
    stringstream ss;
    ss << setfill('0') << setw(2) << minutos.count() << ":"
       << setfill('0') << setw(2) << segundos.count() << ":"
       << setfill('0') << setw(3) << milissegundos.count();
    return ss.str();
}

int main(int argc, char* argv[]) {
    // Verifica se o número de itens foi passado como argumento
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <numero_de_itens>" << endl;
        return 1; // Encerra o programa se o argumento não for fornecido
    }

    // Converte o argumento para inteiro
    int num_itens = stoi(argv[1]);

    string nome_arquivo = "../arquivos_texto/mochila.txt";

    // Ler o peso da mochila
    int peso_mochila = ler_peso_mochila(nome_arquivo);
    if (peso_mochila == -1) {
        return 1; // Encerra o programa se o arquivo não puder ser aberto
    }
    cout << "Peso da mochila: " << peso_mochila << endl;
    cout << " #### REALIZANDO LEITURA ####\n";
    // Ler os itens
    vector<Item> itens = ler_itens(nome_arquivo, num_itens);
    cout << "Total de itens lidos: " << itens.size() << endl;
    cout << " +++++ INICIADO +++++\n";
    // Medir o tempo de execução
    auto start = high_resolution_clock::now(); // Inicia a contagem do tempo

    // Resolver o problema da mochila
    int W = peso_mochila; // Capacidade da mochila
    int n = itens.size(); // Número de itens
    int maxProfit = knapsack(W, itens, n);

    auto stop = high_resolution_clock::now(); // Finaliza a contagem do tempo

    // Calcular a duração em milissegundos
    auto duration = duration_cast<milliseconds>(stop - start);

    // Formatar o tempo no formato mm:ss:ms
    string tempo_formatado = formatar_tempo(duration);

    // Exibir resultados
    cout << "Maximum possible profit = " << maxProfit << endl;
    cout << "Tempo de execucao: " << tempo_formatado << endl;

    // Salvar os dados em um arquivo CSV
    std::string nome = "resultados/" + std::to_string(num_itens) + ".csv"; // Concatena o nome do arquivo
    std::ofstream csv_file(nome, std::ios::app); // Abre o arquivo em modo de append
    if (!csv_file) {
        cerr << "Erro ao abrir o arquivo CSV!" << endl;
        return 1;
    }
    cout << "n_itens: " << num_itens      << endl <<
            "peso_m: "  << peso_mochila   << endl <<
            "maxProfit: " << maxProfit    << endl <<
            "tempo: "   << tempo_formatado  << endl;
    // Escreve os dados no arquivo CSV
    csv_file << num_itens << ","
             << peso_mochila << ","
             << maxProfit << ","
             << tempo_formatado << endl;

    csv_file.close(); // Fecha o arquivo

    cout << "Dados salvos em resultados.csv" << endl;

    return 0;
}
