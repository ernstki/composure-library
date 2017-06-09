myq ()
{
    author 'Kevin Ernst'
    about 'Query propriate dev/prod MySQL server (using credentials in ~/.my.cnf)'
    param '$1 - (optional) database name; default "bedintersect"'
    param '$2 - quoted string of SQL commands to execute'
    example 'mq hg19_data_summary_table "'
    group ''

    set -x
    myhost=
    db=bedintersect
    sql='show tables'

    if [[ $(hostname -s) =~ p[0-9]$ ]]; then
        myhost=bmirschp04my.chmcres.cchmc.org
    elif [[ $(hostname -s) =~ d[0-9]$ ]]; then
        myhost=bmirschd04my.chmcres.cchmc.org
    else
        echo "ACK! Must run this script on either a p1/d1 host" >&2
        return 1
    fi

    # If the first argument has any spaces in it, then assume the default table
    if [[ ! $1 =~ \  ]]; then
        db=$1
        shift
    fi

    # automatically append a semicolon
    sql=$1
    if [[ ! $sql =~ \;$ ]]; then
        sql="${sql};"
    fi

    mysql -h $myhost $db <<<"$sql"
    set +x
}

# vim: ft=sh
