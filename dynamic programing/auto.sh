#!/bin/bash
#./auto.sh 100 200 400 800 1600 3200 6400 12800 25600 51200
#./auto.sh 102400 204800 409600 819200 1638400 3276800
# Verifica se pelo menos um argumento foi passado
if [ $# -eq 0 ]; then
    echo "Uso: $0 <num_itens_1> <num_itens_2> ... <num_itens_n>"
    exit 1
fi

# Itera sobre cada valor passado como argumento
for num_itens in "$@"; do
    echo "Executando teste com $num_itens itens..."
    
    # Executa o loop 20 vezes para cada valor de num_itens
    for i in {1..20}; do
        ./main $num_itens  # Passa o número de itens como argumento para o programa
        sleep 1
        swapoff -av ; swapon -av  # Reinicia a swap
    done
done

