headers () 
{ 
    author 'Kevin Ernst'
    about 'Prints the header row from a delimited file, one per line.'
    param '$1 - the filename to print the headers from'
    param '-F|-d|--delim - the delimiter (default: tab)'
    group 'text processing'

    local delim=$'\t'
    local filename

    while (( $# )); do
        case $1 in
            -d|-F|--delim)
                shift;
                if (( ${#1} != 1 )); then
                    echo "ACK! Delimiter must be a one-character string." >&2
                    return 1
                fi
                delim=$1
                ;;
            *)
                filename=$1
                if [[ ! -f $filename ]]; then
                    echo "ACK! File '$filename does not exist." >&2
                    return 1
                fi
                ;;
        esac
        shift;
    done

    awk -F "$delim" '
        NR==1 {
            for (i=1; i<=NF; i++) {
                gsub(/"/, "", $i)
                printf("%s\n", $i);
            }
        }' "$filename"
}
