#Metam na pasta do tuktuk, os ficheiros .cities numa pasta testfiles, os walks do professor numa chamada testfiles_walks.
#Na primeira execução fazer chmod +x testar.sh
#Para correr fazer ./testar.sh

#Ele imprime FICHEIRO NOME_DO_FICHEIRO e de seguida imprime as diferenças entre o gerado e o do professor
#Apenas imprimir os nomes dos ficheiros é porque todos funcionam bem

for file in testfiles/*; do
    filename="$(basename "$file")"
    if [ ${file: -7} == ".cities" ]
    then

        filename_without_extension="${filename%.*}"
        echo FICHEIRO: $filename_without_extension

        ./tuktuk testfiles/$filename
        diff testfiles/$filename_without_extension.walks testfiles_valid/$filename_without_extension.walks;
    fi
done
