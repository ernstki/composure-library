spl () 
{ 
    author 'Kevin Ernst'
    about 'Splits input string or file (or stdin) on common delimiters'
    param '-d <delim>: delimiter list as string (defaults: [ :,\t])'
    example 'echo \$PATH | spl | sort | uniq -d'
    group 'util'

    local file
    local stringx
    local delim=' :,\t'

    while (( $# )); do
        if [[ $1 =~ ^--?d* ]]; then
            shift
            delim=$1
        elif [ -f "$1" ]; then
            file=$1
        else
            string=$1
        fi
        shift
    done

    if [ -n "$string" ]; then
        tr "$delim" '\n' <<<"$string" | sed '/^[    ]*$/d'
    else
        cat "$file" | tr "$delim" '\n'  | sed '/^[    ]*$/d'
    fi

}
