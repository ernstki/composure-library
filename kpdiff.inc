# print Title, UserName, Password, and URL sorted by Title, tab delimited
# for KeePass XML dump given as '$1'
kptsv()
{
    (( TRACE )) && set -x
    trap "set +x" RETURN

    if [[ -z ${1:-} || -f ${1:-} ]]; then
        echo "ERROR: need a KeePass XML export file as first argument." >&2
        return 1
    fi
    xml sel -t \
        --var upper=ABCDEFGHIJKLMNOPQRSTUVWXYZ \
        --var lower=abcdefghijklmnopqrstuvwxyz \
        -m '//Entry' \
        -s A:T:- 'translate(String/Key[.="Title"]/../Value, $lower, $upper)' \
        -v $'concat(String/Key[.="Title"]/../Value, "\t",
                    String/Key[.="UserName"]/../Value, "\t",
                    String/Key[.="Password"]/../Value, "\t",
                    String/Key[.="URL"]/../Value)' \
        -n "$1"
} # kptsv

#export -f kptsv

# diff two KeePass XML dump files $1 and $2
kpdiff () 
{ 
    author 'Kevin Ernst'
    about 'show the differences between two KeePass XML exports'
    param '$1 - "left" KeePass XML export file'
    param '$2 - "right" KeePass XML export file'
    example 'kpdiff conflict.xml good.xml'
    group 'security'

    (( TRACE )) && set -x
    trap "set +x" RETURN

    if [[ -z ${1:-} || -z ${2:-} || $1 != *.xml || $2 != *.xml ]]; then 
        echo "ERROR: need two KeePass XML export files as arguments." >&2
        return 1
    fi

    sdiff --suppress-common-lines <(kptsv "$1") <(kptsv "$2")
} # kpdiff

# vim: ft=sh
