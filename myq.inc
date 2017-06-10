myq ()
{
    author 'Kevin Ernst'
    about 'Query propriate dev/prod MySQL server (using credentials in ~/.my.cnf)'
    param '$1 - (optional) database host; default "bmirsch{p|d}04my"'
    param '$2 - (optional) database name; default "bedintersect"'
    param '$3 - quoted string of SQL commands to execute'
    example 'mq bedintersect "select * from gene_aliases"'
    group 'dba'

    myhost=
    db=bedintersect
    sql='show tables;'

    # Default to the BMI MySQL server on the same side of the research bubble
    # as the host you're logged in to
    if [[ $(hostname -s) =~ p[0-9]$ ]]; then
        myhost=bmirschp04my.chmcres.cchmc.org
    elif [[ $(hostname -s) =~ d[0-9]$ ]]; then
        myhost=bmirschd04my.chmcres.cchmc.org
    fi

    while (( $# )); do
        # If the argument looks like a hostname...
        if [[ $1 =~ ^bmi || $1 = "localhost" ]]; then
            myhost=$1
            shift; continue
        fi
            
        # If the argument looks like a database name...
        if [[ ! $1 =~ [[:space:]] ]]; then
            db=$1
            shift; continue
        fi

        # automatically append a semicolon to SQL if missing
        sql=$1
        [[ $sql =~ \;$ ]] || sql="${sql};"
        shift
    done

    shorthost=${myhost%%.*}

    # If a group exists like '[client:hostname]' in ~/.my.cnf, then use all the
    # parameters defined there, including hostname. Refer to this section of
    # the MySQL manual for details:
    # https://dev.mysql.com/doc/refman/5.7/en/option-file-options.html
    if grep -qE "^[[:space:]]+\[client:${shorthost}" ~/.my.cnf; then
        mysql --defaults-group-suffix=:$shorthost $db <<<"$sql"
    else
        echo "WARNING: No group for '$shorthost' found in ~/.my.cnf" >&2
        echo "         Will use default credentials from '[client]' section" >&2
        mysql -h $myhost $db <<<"$sql"
    fi
}

# vim: ft=sh
