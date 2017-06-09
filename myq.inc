myq ()
{
    author 'Kevin Ernst'
    about 'Query propriate dev/prod MySQL server (using credentials in ~/.my.cnf)'
    param '$1 - (optional) database name; default "bedintersect"'
    param '$2 - quoted string of SQL commands to execute'
    example 'mq bedintersect "select * from gene_aliases"'
    group 'dba'

    myhost=
    db=bedintersect
    sql='show tables;'

    if [[ $(hostname -s) =~ p[0-9]$ ]]; then
        myhost=bmirschp04my.chmcres.cchmc.org
    elif [[ $(hostname -s) =~ d[0-9]$ ]]; then
        myhost=bmirschd04my.chmcres.cchmc.org
    else
        echo "ACK! Must run this script on either a p1/d1 host" >&2
        return 1
    fi

    while (( $# )); do
        # If the first argument looks like a table name...
        if [[ ! $1 =~ [[:space:]] ]]; then
            db=$1
            shift
            continue
        fi

        sql=$1
        shift

        # automatically append a semicolon if missing
        if [[ ! $sql =~ \;$ ]]; then
            sql="${sql};"
        fi
    done

    mysql -h $myhost $db <<<"$sql"
}

# vim: ft=sh