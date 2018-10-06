headers () 
{ 
    set -x
    author 'Kevin Ernst'
    about 'Prints the header row from a delimited file, one per line.'
    param '$1 - the filename to print the headers from'
    param '-F|-d|--delim - the delimiter (default: tab)'
    group 'text processing'

    local delim=$'\t'
    local filename

    while (( $# )); do
        case $1 in
            -d*|-F*|--delim*)
                # if the delimiter is stuck to the option switch, break it off
                if [[ $1 =~ -(d|F).+ || $1 =~ --delim=.+ ]]; then
                    delim=${1##-d}
                    delim=${1##-F}
                    delim=${1##--delim=}
                    continue
                fi

                # otherwise, the delimiter comes next
                shift
                
                if [[ -z $1 || -f $1 ]]; then
                    echo "ACK! Missing argument to delimiter (-d) option." >&2
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
        shift
    done

    awk -F "$delim" '
        NR==1 {
            for (i=1; i<=NF; i++) {
                gsub(/"/, "", $i)
                printf("%s\n", $i);
            }
        }' "$filename"
    set +x
}
