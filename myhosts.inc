myhosts ()
{
    author 'Kevin Ernst'
    about "For use with MySQL utilities' '--defaults-group-suffix' option"
    param 'n/a'
    example 'myhosts'
    group 'dba'
    awk -F'client' '/[[]client:/{print substr($2, 0, length($2)-1)}' ~/.my.cnf
}
