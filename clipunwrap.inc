clipunwrap ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    doclip | sed ':a;N;$!ba;s/\n/ /g;s/  */ /g'
}
