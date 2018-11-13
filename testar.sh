for file in testfiles/*; do
    filename="$(basename "$file")"
    
    filename_without_extension="${filename%.*}"
    echo FICHEIRO: $filename_without_extension

    ./tuktuk testfiles/$filename
    diff saida.valid testfiles_valid/$filename_without_extension.valid;
done