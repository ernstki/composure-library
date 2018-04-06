myhosts ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    awk -F'client' '/[[]client:/{print substr($2, 0, length($2)-1)}' ~/.my.cnf
}
