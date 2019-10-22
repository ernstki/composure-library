= ()
{
    author 'Kevin Ernst'
    about "do math with 'bc' "
    param "$\* - any valid 'bc' expr.; be sure to escape or quote shell metachars"
    example '= 7.68 \* 1024^3'
    group 'math'
    bc -l <<<"$*"
}
