#ifndef BRANCH_A_BOUND_H
#define BRANCH_A_BOUND_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm> // Para std::sort
#include <cstdlib> // Para rand() e srand()
#include <ctime>   // Para time()
using namespace std;

// Structure for Item which store weight and corresponding value of Item
struct Item {
    int weight;
    int value;
};

// Node structure to store information of decision tree
struct Node {
    int level, profit, bound;
    float weight;
};

// Comparison function to sort Item according to val/weight ratio
bool cmp(Item a, Item b) {
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2;
}

// Returns bound of profit in subtree rooted with u.
int bound(Node u, int n, int W, vector<Item>& arr) {
    if (u.weight >= W)
        return 0;

    int profit_bound = u.profit;
    int j = u.level + 1;
    int totweight = u.weight;

    while ((j < n) && (totweight + arr[j].weight <= W)) {
        totweight += arr[j].weight;
        profit_bound += arr[j].value;
        j++;
    }

    if (j < n)
        profit_bound += (W - totweight) * arr[j].value / arr[j].weight;

    return profit_bound;
}

// Returns maximum profit we can get with capacity W
int knapsack(int W, vector<Item>& arr, int n) {
    sort(arr.begin(), arr.end(), cmp);

    queue<Node> Q;
    Node u, v;

    u.level = -1;
    u.profit = u.weight = 0;
    Q.push(u);

    int maxProfit = 0;
    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        if (u.level == -1)
            v.level = 0;

        if (u.level == n - 1)
            continue;

        v.level = u.level + 1;

        v.weight = u.weight + arr[v.level].weight;
        v.profit = u.profit + arr[v.level].value;

        if (v.weight <= W && v.profit > maxProfit)
            maxProfit = v.profit;

        v.bound = bound(v, n, W, arr);
        if (v.bound > maxProfit)
            Q.push(v);

        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, n, W, arr);
        if (v.bound > maxProfit)
            Q.push(v);
    }

    return maxProfit;
}

int ler_peso_mochila(const string& nome_arquivo) {
    ifstream arquivo(nome_arquivo);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return -1;
    }
    int peso_mochila;
    arquivo >> peso_mochila;
    return peso_mochila;
}

vector<Item> ler_itens(const string& nome_arquivo, int num_itens) {
    ifstream arquivo(nome_arquivo);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return {};
    }

    int peso_mochila;
    arquivo >> peso_mochila; // Ignora a primeira linha

    vector<Item> itens;
    Item item;
    int count = 0;

    // Inicializa a semente do gerador de números aleatórios
    srand(time(0));

    while (arquivo >> item.weight >> item.value && count < num_itens) {
        // Sorteia um número entre 0 e 100
        int sorteio = rand() % 101;

        // Se o número for maior que 80, ignora o item
        if (sorteio > 70) {
            continue; // Passa para o próximo item
        }

        // Adiciona o item ao vetor
        itens.push_back(item);
        count++;
    }

    return itens;
}

#endif